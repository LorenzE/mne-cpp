//=============================================================================================================
/**
* @file     myoarmband.h
* @author   Jeremias Báez Carballo <jere.baez91@gmail.com>;
* @version  1.0
* @date     November, 2016
*
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
* @brief    Contains the declaration of the MyoArmband class.
*
*/

#ifndef MYOARMBAND_H
#define MYOARMBAND_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "myoarmband_global.h"
#include "FormFiles/myoarmbandgui.h"
#include "myo/myo.hpp"

#include <scShared/Interfaces/ISensor.h>
#include <generics/circularmatrixbuffer.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// GENERIC INCLUDES
//=============================================================================================================

#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace FIFFLIB {
    class FiffInfo;
}

namespace SCMEASLIB {
    class NewRealTimeMultiSampleArray;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE MYOARMBANDPLUGIN
//=============================================================================================================

namespace MYOARMBANDPLUGIN
{


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* DECLARE CLASS MyoRealTimeProducer
*
* @brief The MyoRealTimeProducer class handles the comunication with the MyoArmband and is intended to
* provide the data once a sample rate is given.
*/
class MYOARMBANDSHARED_EXPORT MyoArmband : public SCSHAREDLIB::ISensor
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "scsharedlib/1.0" FILE "myoarmband.json") //New Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
    // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
    Q_INTERFACES(SCSHAREDLIB::ISensor)

    friend class MyoRealTimeProducer;
    friend class MyoArmbandGui;

public:
    //=========================================================================================================
    /**
    * Constructs MyoArmband.
    */
    MyoArmband();

    //=========================================================================================================
    /**
    * Destroy MyoArmband.
    */
    virtual ~MyoArmband();

    //=========================================================================================================
    /**
    * ISensorfunctions
    * clone - Clone the plugin
    * init - Initialize input and output connectors.
    * unload - Called when plugind is detached. Can be used to save settings.
    * start -
    * stop -
    * getType - Returns type of the class
    * getName - Returns name of eht class
    * setupWidget -
    */
    virtual QSharedPointer<SCSHAREDLIB::IPlugin> clone() const;
    virtual void init();
    virtual void unload();
    virtual bool start();
    virtual bool stop();
    virtual SCSHAREDLIB::IPlugin::PluginType getType() const;
    virtual QString getName() const;
//    virtual inline bool multiInstanceAllowed() const;
    virtual QWidget* setupWidget();

protected:
    virtual void run();

    //=========================================================================================================
    /**
    * Initialise the MyoArmband RTMSA.
    */
    void initRTMSA();

    //=========================================================================================================
    /**
    * Sets up the fiff info with the current data chosen by the user. Note: Only works for ANT Neuro Waveguard Duke caps.
    */
    void setUpFiffInfo();

private:    
    bool            m_bIsRunning;                       /**< Whether main thread is running */

    float           m_fSamplingRate;                    /**< the sampling rate.*/
    int             m_iDownsamplingFactor;              /**< the down sampling factor.*/
    int             m_iNumberOfChannels;                /**< The number of channels defined by the user via the GUI.*/
    int             m_iSamplesPerBlock;                 /**< The samples per block defined by the user via the GUI.*/

    QSharedPointer<IOBUFFER::RawMatrixBuffer>       m_pRawMatrixBuffer;         /**< Holds incoming raw data. */
    QSharedPointer<MyoRealTimeProducer>             m_pMyoRealTimeProducer;     /**< Pointer to the MYO data producer*/

    QSharedPointer<FIFFLIB::FiffInfo>               m_pFiffInfo;                /**< Fiff measurement info.*/

    QSharedPointer<SCSHAREDLIB::PluginOutputData<SCMEASLIB::NewRealTimeMultiSampleArray> >  m_pRTMSA_MYO_ROLL_new;       /**< The RealTimeSampleArray to provide the channel MYO Roll.*/

};

#endif // MYOARMBAND_H

}//NAMESPACE
