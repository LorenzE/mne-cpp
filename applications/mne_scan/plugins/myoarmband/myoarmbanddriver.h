//=============================================================================================================
/**
* @file     myoarmbanddriver.h
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
* @brief     MyoArmbandDriver class declaration.
*
*/

#ifndef MYOARMBANDDRIVER_MYOARMBANDDRIVER_H
#define MYOARMBANDDRIVER_MYOARMBANDDRIVER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "myo/myo.hpp"
#include <generics/circularmatrixbuffer.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QtMath>
#include <QVector>


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


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE MYOARMBANDPLUGIN
//=============================================================================================================

namespace MYOARMBANDPLUGIN {

//*************************************************************************************************************
//=============================================================================================================
//  DATA COLLECTOR CLASS
//=============================================================================================================

class DataCollector : public myo::DeviceListener {
public:
    /**
    *   Constructor device listener. Recieves the events from Myo devices.
    */
    DataCollector();

    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
    void onUnpair(myo::Myo* myo, uint64_t timestamp);

    // onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
    // as a unit quaternion.
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);

    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
    // making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);

    // onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
    // arm. This lets Myo know which arm it's on and which way it's facing.
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState);

    // onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
    // it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
    // when Myo is moved around on the arm.
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp);

    // onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
    void onUnlock(myo::Myo* myo, uint64_t timestamp);

    // onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
    void onLock(myo::Myo* myo, uint64_t timestamp);

    float DataCollector::getRoll();
private:

    // These values are set by onArmSync() and onArmUnsync() above.
    bool onArm;
    myo::Arm whichArm;

    // This is set by onUnlocked() and onLocked() above.
    bool isUnlocked;

    // These values are set by onOrientationData() and onPose() above.
    float roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;

};


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class MyoRealTimeProducer;

//=============================================================================================================
/**
* Myo Armband Driver
*
* @brief The MyoArmbandDriver class provides the real time data adquisition of the Thalmic Labs Myo
* Armband device
*/

class MyoArmbandDriver
{
friend class DataCollector;

public:
    typedef QSharedPointer<MyoArmbandDriver> SPtr;            /**< Shared pointer type for MyoArmbandDriver. */
    typedef QSharedPointer<const MyoArmbandDriver> ConstSPtr; /**< Const shared pointer type for MyoArmbandDriver. */

    //=========================================================================================================
    /**
    * Constructs a MyoArmbandDriver object.
    */
    MyoArmbandDriver(MyoRealTimeProducer* pMyoProducer);

    //=========================================================================================================
    /**
    * Destroys MyoArmbandDriver object.
    */
    ~MyoArmbandDriver();

    //=========================================================================================================
    /**
    * Get sample from the device in form of a mtrix.
    * @param [in] MatrixXf the block sample values in form of a matrix.
    * @param [out] bool returns true if sample was successfully written to the input variable, false otherwise.
    */
    bool getSampleMatrixValue(IOBUFFER::MatrixXf& sampleMatrix);

    //=========================================================================================================
    /**
     * @brief initDevice
     * Initializes the device.
     * @return true if the device was succesfully initializated, false if not.
     */
    bool initDevice();

    //=========================================================================================================
    /**
     * @brief updateDevice
     * @param eventLoopDuration is the duration the event loop waits to catch the different events.
     * @return void
     */
    void updateDevice(unsigned int eventLoopDuration);

    //=========================================================================================================
    /**
    * Uninitialise device.
    * @param [out] bool returns true if device was successfully uninitialised, false otherwise.
    */
    bool uninitDevice();

protected:

private:
    myo::Hub hub;
    DataCollector collector;

    MyoRealTimeProducer*    m_pMyoRealTimeProducer;    /**< A pointer to the corresponding MyoRealTimeProducer */
    bool                    m_bInitDeviceSuccess;
    uint                    m_uiNumberOfChannels;
    uint                    m_uiSamplingFrequency;
    uint                    m_uiSamplesPerBlock;            /**< The samples per block defined by the user via the GUI.*/

};



} // namespace MYOARMBANDDRIVER

#endif // MYOARMBANDDRIVER_MYOARMBANDDRIVER_H

