//=============================================================================================================
/**
* @file     myorealtimeproducer.cpp
* @author   Jeremias Baez Carballo <jere.baez91@gmail.com>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     Month, Year
*
* @section  LICENSE
*
* Copyright (C) Year, Jeremias Baez Carballo and Matti Hamalainen. All rights reserved.
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
* @brief    myorealtimeproducer class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "myoarmbanddriver.h"
#include "myorealtimeproducer.h"
#include "myoarmband.h"

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MYOARMBANDPLUGIN;
using namespace IOBUFFER;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

MyoRealTimeProducer::MyoRealTimeProducer(MyoArmband* pMyoPlugin, QSharedPointer<RawMatrixBuffer> pMatBuff)
: m_pMyoArmband(pMyoPlugin)
, m_pMyoArmbandDriver(new MyoArmbandDriver (this))
, m_pMatBuffer(pMatBuff)
, m_bIsRunning(false)
{
}


//*************************************************************************************************************

MyoRealTimeProducer::~MyoRealTimeProducer()
{

}


//*************************************************************************************************************

void MyoRealTimeProducer::start()
{
    if(m_pMyoArmbandDriver->initDevice())
    {
        m_bIsRunning = true;
        QThread::start();
    }
    else
        m_bIsRunning=false;
}


//*************************************************************************************************************

void MyoRealTimeProducer::stop()
{
    m_bIsRunning = false;
    QThread::wait();
}


//*************************************************************************************************************

void MyoRealTimeProducer::run()
{
    unsigned int uiSamplePeriod = (unsigned int) (1000000.0/(m_pMyoArmband->m_fSamplingRate));
//    int uiCounter_Roll = 0;
    m_bIsRunning = true;
//    double value_Roll;

    MatrixXf mat(1,10);

    while(m_bIsRunning)
    {

        m_pMyoArmbandDriver->updateDevice(1000/20);
//        usleep(uiSamplePeriod);

        //Get data from myo armband drive
//        double dValueRoll = -180.0 + (rand() % (int)(180.0 + 180.0 + 1));
//        mat.setConstant(dValueRoll);

        m_pMyoArmbandDriver->getSampleMatrixValue(mat);
        m_pMatBuffer->push(&mat);
    }
}

//*************************************************************************************************************
