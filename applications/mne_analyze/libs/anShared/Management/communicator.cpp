//=============================================================================================================
/**
* @file     communicator.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     April, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Implementation of the Communicator class
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "communicator.h"
#include "eventmanager.h"
#include "../Interfaces/IExtension.h"

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;

//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

Communicator::Communicator(QVector<Event::EVENT_TYPE> subs)
    : m_ID(nextID()),
      m_EventSubscriptions(subs)
{
    EventManager::addCommunicator(this);
}

//*************************************************************************************************************

Communicator::Communicator(IExtension* extension)
    : Communicator(extension->getEventSubscriptions())
{
    QObject::connect(this,
                     SIGNAL(receivedEvent(Event)),
                     extension,
                     SLOT(handleEvent(Event)),
                     Qt::DirectConnection);
}

//*************************************************************************************************************

Communicator::~Communicator()
{
    EventManager::removeCommunicator(this);
}

//*************************************************************************************************************

void Communicator::publishEvent(Event::EVENT_TYPE etype, QVariant data) const
{
    // simply pass on to the EventManager
    EventManager::issueEvent(Event(etype, this, data));
}

//*************************************************************************************************************

void Communicator::updateSubscriptions(QVector<Event::EVENT_TYPE> subs)
{
    // update routing table of event manager
    EventManager::updateSubscriptions(this, subs);
    // update own subscription list: This HAS to be done after the EventManager::updateSubscriptions,
    // since the latter uses the old list in order to keep execution time low
    m_EventSubscriptions.clear();
    m_EventSubscriptions.append(subs);
}

//*************************************************************************************************************

void Communicator::addSubscriptions(QVector<Event::EVENT_TYPE> newsubs)
{
    m_EventSubscriptions.append(newsubs);
    // add new subscriptions to routing table of event manager
    EventManager::addSubscriptions(this, newsubs);
}

//*************************************************************************************************************

void Communicator::addSubscriptions(Event::EVENT_TYPE newsub)
{
    // convenience function, simply wrap in vector
    QVector<Event::EVENT_TYPE> temp;
    temp.push_back(newsub);
    addSubscriptions(temp);
}

//*************************************************************************************************************

void Communicator::manualDisconnect(void)
{
    // simply delegate to EventManager
    EventManager::removeCommunicator(this);
}

//*************************************************************************************************************
//=============================================================================================================
// DEFINE STATIC MEMBERS
//=============================================================================================================

Communicator::CommunicatorID Communicator::m_IDCounter;
