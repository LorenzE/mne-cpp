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

#include "myo/myo.hpp"
#include <generics/circularbuffer.h>

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
// DEFINE NAMESPACE MyoArmbandPlugin
//=============================================================================================================

namespace MyoArmbandPlugin {


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace IOBUFFER;

//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class MyoArmband;

//=============================================================================================================
/**
* DECLARE CLASS MyoRealTimeProducer
*
* @brief The MyoRealTimeProducer class handles the comunication with the MyoArmband and is intended to
* provide the data once a sample rate is given.
*/

class MyoRealTimeProducer : public QThread, public myo::DeviceListener
{

public:
    typedef QSharedPointer<MyoRealTimeProducer> SPtr;            /**< Shared pointer type for myorealtimeproducer. */
    typedef QSharedPointer<const MyoRealTimeProducer> ConstSPtr; /**< Const shared pointer type for myorealtimeproducer. */

    //=========================================================================================================
    /**
    * Constructs a myorealtimeproducer object.
    *
    * @param [in] pointer to the corresponding MyoArmband class
    * @param [in] bufferRoll a pointer to the buffer to which this class should save the roll data
    */
    MyoRealTimeProducer(MyoArmband* myo, dBuffer::SPtr& bufferRoll);

    /**
    * Destructor.
    */
    ~MyoRealTimeProducer();

    /**
    * Stops MyoRealTimeProducer by stopping the producer's thread.
    */
    void stop();
protected:
    /**
    * The starting point for the thread. After calling start(), the newly created thread calls this function.
    * Returning from this method will end the execution of the thread.
    * Pure virtual method inherited by QThread.
    */
    virtual void run();

private:
    MyoArmband*             m_pMyoArmband;      /**< Holds a pointer to corresponding MyoArmband class.*/
    dBuffer::SPtr           m_pdBuffer_Roll;    /**< Holds a pointer to the buffer where the Roll data should be written to.*/
    bool                    m_bIsRunning;       /**< Holds whether MyoRealTimeProducer is running.*/
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

//*************************************************************************************************************
//=============================================================================================================
// Device listener class - Check connection of myo armband
//=============================================================================================================
//class DataCollector : public myo::DeviceListener {
//public:
//    DataCollector()
//    : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
//    {
//    }

//    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
//    void onUnpair(myo::Myo* myo, uint64_t timestamp)
//    {
//        // We've lost a Myo.
//        // Let's clean up some leftover state.
//        roll_w = 0;
//        pitch_w = 0;
//        yaw_w = 0;
//        onArm = false;
//        isUnlocked = false;
//    }

//    // onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
//    // as a unit quaternion.
//    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
//    {
//        using std::atan2;
//        using std::asin;
//        using std::sqrt;
//        using std::max;
//        using std::min;

//        // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
//        float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
//                           1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
//        float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
//        float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
//                        1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

//        // Convert the floating point angles in radians to a scale from 0 to 18.
//        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
//        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
//        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
//    }

//    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
//    // making a fist, or not making a fist anymore.
//    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
//    {
//        currentPose = pose;

//        if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
//            // Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
//            // Myo becoming locked.
//            myo->unlock(myo::Myo::unlockHold);

//            // Notify the Myo that the pose has resulted in an action, in this case changing
//            // the text on the screen. The Myo will vibrate.
//            myo->notifyUserAction();
//        } else {
//            // Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
//            // are being performed, but lock after inactivity.
//            myo->unlock(myo::Myo::unlockTimed);
//        }
//    }

//    // onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
//    // arm. This lets Myo know which arm it's on and which way it's facing.
//    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
//                   myo::WarmupState warmupState)
//    {
//        onArm = true;
//        whichArm = arm;
//    }

//    // onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
//    // it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
//    // when Myo is moved around on the arm.
//    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
//    {
//        onArm = false;
//    }

//    // onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
//    void onUnlock(myo::Myo* myo, uint64_t timestamp)
//    {
//        isUnlocked = true;
//    }

//    // onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
//    void onLock(myo::Myo* myo, uint64_t timestamp)
//    {
//        isUnlocked = false;
//    }

//    // These values are set by onArmSync() and onArmUnsync() above.
//    bool onArm;
//    myo::Arm whichArm;

//    // This is set by onUnlocked() and onLocked() above.
//    bool isUnlocked;

//    // These values are set by onOrientationData() and onPose() above.
//    int roll_w, pitch_w, yaw_w;
//    myo::Pose currentPose;
//};

} // namespace NAMESPACE

#endif // NAMESPACE_MYOREALTIMEPRODUCER_H
