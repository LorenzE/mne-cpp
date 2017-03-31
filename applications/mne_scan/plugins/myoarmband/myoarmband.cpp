//=============================================================================================================
/**
* @file     myoarmband.cpp
* @author   Jeremias Báez Carballo <jere.baez91@gmail.com>;
* @version  1.0
* @date     November, 2016
* @section  LICENSE
*
* Copyright (C) 2013, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Contains the implementation of the MyoArmband class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "myoarmband.h"
#include "FormFiles/myoarmbandgui.h"
#include "myorealtimeproducer.h"

#include <iostream>
#include <fiff/fiff_info.h>
#include <scMeas/newrealtimemultisamplearray.h>
#include <generics/circularmatrixbuffer.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtCore/QtPlugin>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtWidgets>
#include <QVector>
#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MYOARMBANDPLUGIN;
using namespace SCSHAREDLIB;
using namespace FIFFLIB;
using namespace IOBUFFER;
using namespace SCMEASLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

//Constructor and Destructor
MyoArmband::MyoArmband()
: m_iNumberOfChannels(1)
, m_iSamplesPerBlock(10)
, m_pRawMatrixBuffer(QSharedPointer<RawMatrixBuffer>(new RawMatrixBuffer(8, m_iNumberOfChannels, m_iSamplesPerBlock)))
, m_pMyoRealTimeProducer(QSharedPointer<MyoRealTimeProducer>(new MyoRealTimeProducer(this, m_pRawMatrixBuffer)))
, m_iDownsamplingFactor(1)
, m_bIsRunning(false)
, m_fSamplingRate(250.0)
, m_pFiffInfo(QSharedPointer<FiffInfo>(new FiffInfo()))
{

}


//*************************************************************************************************************

MyoArmband::~MyoArmband()
{
    //If the program is closed while the sampling is in process
    if(this->isRunning())
        this->stop();
}


//*************************************************************************************************************

QSharedPointer<IPlugin> MyoArmband::clone() const
{
    QSharedPointer<MyoArmband> pMyoArmbandClone(new MyoArmband);
    return pMyoArmbandClone;
}


//*************************************************************************************************************

void MyoArmband::init()
{
    m_pRTMSA_MYO_ROLL_new = PluginOutputData<NewRealTimeMultiSampleArray>::create(this, "Myo", "Myo armband output data");

    QStringList slDisplayFlags;
    slDisplayFlags << "view" << "scaling" << "colors";

    m_pRTMSA_MYO_ROLL_new->data()->setDisplayFlags(slDisplayFlags);

    m_outputConnectors.append(m_pRTMSA_MYO_ROLL_new);
}


//*************************************************************************************************************

void MyoArmband::unload()
{

}


//*************************************************************************************************************

bool MyoArmband::start()
{
    // Start the myo producer
    m_pMyoRealTimeProducer->start();

    if(m_pMyoRealTimeProducer->isRunning())
    {
        m_bIsRunning = true;
        QThread::start();
        return true;
    }

    return false;
}


//*************************************************************************************************************

bool MyoArmband::stop()
{
    // Stop threads
    m_pMyoRealTimeProducer->stop();

    //Wait until this thread is stopped
    m_bIsRunning = false;

    //In case the semaphore blocks the thread -> Release the QSemaphore and let it exit from the pop function (acquire statement)
    m_pRawMatrixBuffer->releaseFromPop();
    m_pRawMatrixBuffer->clear();

    return true;
}


//*************************************************************************************************************

void MyoArmband::run()
{
    setUpFiffInfo();
    initRTMSA();

    MatrixXf matValue;

    while(m_bIsRunning)
    {
        if(m_pMyoRealTimeProducer->isRunning())
        {
            matValue = m_pRawMatrixBuffer->pop();

            m_pRTMSA_MYO_ROLL_new->data()->setValue(matValue.cast<double>());
        }
    }
}


//*************************************************************************************************************

IPlugin::PluginType MyoArmband::getType()const
{
    return _ISensor;
}


//*************************************************************************************************************

QString MyoArmband::getName()const
{
    return "MyoArmband Plugin";
}


//*************************************************************************************************************

QWidget* MyoArmband::setupWidget()
{
    QWidget* setupWidget = new MyoArmbandGui();
    return setupWidget;
}


//*************************************************************************************************************

void MyoArmband::initRTMSA()
{
    //Init roll channel
    m_pRTMSA_MYO_ROLL_new->data()->initFromFiffInfo(m_pFiffInfo);
    m_pRTMSA_MYO_ROLL_new->data()->setMultiArraySize(10);
    m_pRTMSA_MYO_ROLL_new->data()->setSamplingRate(m_pFiffInfo->sfreq);
    m_pRTMSA_MYO_ROLL_new->data()->setVisibility(true);
}


//*************************************************************************************************************

void MyoArmband::setUpFiffInfo()
{
      //Clear old fiff info data
      m_pFiffInfo->clear();

      //Set number of channels, sampling frequency and high/-lowpass
      m_pFiffInfo->nchan = m_iNumberOfChannels;
      m_pFiffInfo->sfreq = m_fSamplingRate;

      //Set up the channel info
      QStringList QSLChNames;
      m_pFiffInfo->chs.clear();

      FiffChInfo fChInfo;

      //Roll channel
      fChInfo.ch_name = "Roll";
      fChInfo.kind = FIFFV_MISC_CH;
      QSLChNames << fChInfo.ch_name;
      m_pFiffInfo->chs.append(fChInfo);

      //TODO: Do the same for magnetometer, gyro sensor

      //Set channel names in fiff_info_base
      m_pFiffInfo->ch_names = QSLChNames;
}
