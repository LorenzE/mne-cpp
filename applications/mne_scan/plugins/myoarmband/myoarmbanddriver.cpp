//=============================================================================================================
/**
* @file     myoarmbanddriver.cpp
* @author   Jeremias Baez Carballo <jere.baez91@gmail.com>
* @version  1.0
* @date     January, 2017
*
* @section  LICENSE
*
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
* @brief    MyoArmbandDriver class definition.
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
#include <QDebug>

//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MYOARMBANDPLUGIN;
using namespace IOBUFFER;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

MyoArmbandDriver::MyoArmbandDriver(MyoRealTimeProducer* pMyoProducer)
:m_pMyoRealTimeProducer(pMyoProducer)
,m_bInitDeviceSuccess(false)
,m_uiNumberOfChannels(8)
,m_uiSamplingFrequency(1024)
,m_uiSamplesPerBlock(1)
,hub("com.example.hello-myo")
,collector()
{
}

//*************************************************************************************************************


MyoArmbandDriver::~MyoArmbandDriver()
{

}

//*************************************************************************************************************


bool MyoArmbandDriver::initDevice()
{
    // First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
    // publishing your application. The Hub provides access to one or more Myos.
//    myo::Hub hub("com.example.hello-myo");

    std::cout << "Attempting to find a Myo..." << std::endl;

    // Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
    // immediately.
    // waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
    // if that fails, the function will return a null pointer.
    myo::Myo* myo = hub.waitForMyo(10000);

    // If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
    if (!myo) {
        throw std::runtime_error("Unable to find a Myo!");
        m_bInitDeviceSuccess = false;
        return false;
    }

    // We've found a Myo.
    std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

    // Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
//    DataCollector collector;

    // Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
    // Hub::run() to send events to all registered device listeners.
    hub.addListener(&collector);

    m_bInitDeviceSuccess = true;
    return true;
}

//*************************************************************************************************************


void MyoArmbandDriver::updateDevice(unsigned int eventLoopDuration)
{
    if (m_bInitDeviceSuccess)
    {
        hub.run(eventLoopDuration);
    }
    else
    {
        std::cout << "Plugin MyoArmband - ERROR - updateDevice() - driver must be initialized first!"<< std::endl;
    }

}

//*************************************************************************************************************


bool MyoArmbandDriver::getSampleMatrixValue(IOBUFFER::MatrixXf& sampleMatrix)
{
    float dValueRoll=0;

    dValueRoll = collector.getRoll();
    //Check if device was initialised and connected correctly
    if(!m_bInitDeviceSuccess)
    {
        std::cout << "Plugin MyoArmband - ERROR - getSampleMatrixValue() - Cannot start to get samples from device because device was not initialised correctly" << std::endl;
        return false;
    }

    sampleMatrix.setZero(); //Clear matrix - set all elements to zero
    sampleMatrix.setConstant(dValueRoll);
    std::cout << "Plugin MyoArmband - DEBUG- getSampleMatrixValue() - SampleMatrix Updated! "<< dValueRoll <<std::endl;

    return true;
}

//*************************************************************************************************************


bool MyoArmbandDriver::uninitDevice()
{
    return true;
}

//*************************************************************************************************************

//=============================================================================================================
// DeviceListener inhereted
//=============================================================================================================

DataCollector::DataCollector()
:onArm(false)
,isUnlocked(false)
,roll_w(0)
,pitch_w(0)
,yaw_w(0)
,currentPose()
{

}


//*************************************************************************************************************

void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
    // We've lost a Myo.
    // Let's clean up some leftover state.
    roll_w= 0;
    pitch_w = 0;
    yaw_w = 0;
    onArm = false;
    isUnlocked = false;
}


//*************************************************************************************************************

void DataCollector::onOrientationData(myo::Myo *myo, uint64_t timestamp, const myo::Quaternion<float> &quat)
{
    using std::atan2;
    using std::asin;
    using std::sqrt;
    using std::max;
    using std::min;

    // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
    roll_w = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                       1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
    pitch_w = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
    yaw_w = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

}


//*************************************************************************************************************

void DataCollector::onPose(myo::Myo *myo, uint64_t timestamp, myo::Pose pose)
{
    currentPose = pose;

    if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
        // Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
        // Myo becoming locked.
        myo->unlock(myo::Myo::unlockHold);

        // Notify the Myo that the pose has resulted in an action, in this case changing
        // the text on the screen. The Myo will vibrate.
        myo->notifyUserAction();
    } else {
        // Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
        // are being performed, but lock after inactivity.
        myo->unlock(myo::Myo::unlockTimed);
    }
}


//*************************************************************************************************************

void DataCollector::onArmSync(myo::Myo *myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
{
    onArm = true;
    whichArm = arm;
}


//*************************************************************************************************************

void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
{
    onArm = false;
}


//*************************************************************************************************************

void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = true;
}


//*************************************************************************************************************

void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = false;
}

//*************************************************************************************************************

float DataCollector::getRoll()
{
    return roll_w;
}
