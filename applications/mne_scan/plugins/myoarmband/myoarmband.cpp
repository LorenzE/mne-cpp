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
#include <iostream>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtCore/QtPlugin>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <QDebug>

//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MyoArmbandPlugin;
using namespace SCSHAREDLIB;

//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

//Constructor and Destructor
MyoArmband::MyoArmband()
{}

MyoArmband::~MyoArmband()
{
    //If the program is closed while the sampling is in process
    if(this->isRunning())
        this->stop();
}

QSharedPointer<IPlugin> MyoArmband::clone() const
{
    QSharedPointer<MyoArmband> pMyoArmbandClone(new MyoArmband);
    return pMyoArmbandClone;
}

void MyoArmband::init()
{
}

void MyoArmband::unload(){}

bool MyoArmband::start()
{
    return true;
}

bool MyoArmband::stop()
{
    return true;
}

void MyoArmband::run()
{

}

IPlugin::PluginType MyoArmband::getType()const
{
    return _ISensor;
}

QString MyoArmband::getName()const
{
    return "MyoArmband Plugin";
}

QWidget* MyoArmband::setupWidget()
{
    QWidget* setupWidget = new MyoArmbandGui();
    return setupWidget;
}


