//=============================================================================================================
/**
* @file     myorealtimeproducer.h
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
* @brief     myorealtimeproducer class declaration.
*
*/

#ifndef NAMESPACE_MYOREALTIMEPRODUCER_H
#define NAMESPACE_MYOREALTIMEPRODUCER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <generics/circularmatrixbuffer.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QtMath>
#include <QThread>


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
// DEFINE NAMESPACE MYOARMBANDPLUGIN
//=============================================================================================================

namespace MYOARMBANDPLUGIN {


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class MyoArmband;
class MyoArmbandDriver;


//=============================================================================================================
/**
* DECLARE CLASS MyoRealTimeProducer
*
* @brief The MyoRealTimeProducer class handles the comunication with the MyoArmband and is intended to
* provide the data once a sample rate is given.
*/
class MyoRealTimeProducer : public QThread
{

public:
    typedef QSharedPointer<MyoRealTimeProducer> SPtr;            /**< Shared pointer type for myorealtimeproducer. */
    typedef QSharedPointer<const MyoRealTimeProducer> ConstSPtr; /**< Const shared pointer type for myorealtimeproducer. */
    /**
    * Constructs a myorealtimeproducer object.
    *
    * @param [in] pointer to the corresponding MyoArmband class
    * @param [in] bufferRoll a pointer to the buffer to which this class should save the roll data
    */
    MyoRealTimeProducer(MyoArmband* pMyoPlugin, QSharedPointer<IOBUFFER::RawMatrixBuffer> pMatBuff);

    /**
    * Destructor.
    */
    ~MyoRealTimeProducer();

    //=============================================================================================================
    // QThread inhereted
    //=============================================================================================================
    /**
    * Stops MyoRealTimeProducer by stopping the producer's thread.
    */
    void stop();

    /**
    * Starts the Myo Armband Producer, by starting the thread and initialising wich data are we going to lift up
    * from the Myo Armband Driver class.
    * @param [in]
    */
    virtual void start();


protected:
    //=============================================================================================================
    // QThread inhereted
    //=============================================================================================================
    /**
    * The starting point for the thread. After calling start(), the newly created thread calls this function.
    * Returning from this method will end the execution of the thread.
    * Pure virtual method inherited by QThread.
    */
    virtual void run();

private:
    MyoArmband*                                 m_pMyoArmband;      /**< Holds a pointer to corresponding MyoArmband plugin class.*/
    QSharedPointer<MyoArmbandDriver>            m_pMyoArmbandDriver;     /**< Pointer to the MYO data producer*/

    QSharedPointer<IOBUFFER::RawMatrixBuffer>   m_pMatBuffer;       /**< Holds a pointer to the buffer where the Roll data should be written to.*/

    bool                                        m_bIsRunning;       /**< Holds whether MyoRealTimeProducer is running.*/
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

} // namespace NAMESPACE

#endif // NAMESPACE_MYOREALTIMEPRODUCER_H

