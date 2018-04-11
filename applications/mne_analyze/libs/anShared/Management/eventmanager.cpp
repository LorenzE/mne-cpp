//=============================================================================================================
/**
* @file     eventmanager.cpp
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
* @brief    Implementation of the EventManager class
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "eventmanager.h"
#include "communicator.h"

#include <iostream>

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

void EventManager::addCommunicator(Communicator* commu)
{
    const QVector<Event::EVENT_TYPE>& subscriptions = commu->getSubscriptions();
    foreach(Event::EVENT_TYPE etype, subscriptions)
    {
        m_routingTable.insert(etype, commu);
    }
}

//*************************************************************************************************************

void EventManager::issueEvent(Event e)
{
    const QList<Communicator*> subscribers = m_routingTable.values(e.getType());
    foreach(Communicator* commu, subscribers)
    {
        // avoid self-messaging
        if (commu->getID() != e.getSender()->getID())
        {
            // notify communicator about event
            emit commu->receivedEvent(e);
        }
    }
}

//*************************************************************************************************************

void EventManager::addSubscriptions(Communicator* commu, QVector<Event::EVENT_TYPE> newsubs)
{
    foreach(Event::EVENT_TYPE etype, newsubs)
    {
        m_routingTable.insert(etype, commu);
    }
}

//*************************************************************************************************************

void EventManager::updateSubscriptions(Communicator* commu, QVector<Event::EVENT_TYPE> subs)
{
    // remove old subscriptions from EventManager routing table
    EventManager::removeCommunicator(commu);
    // add new key-value-pairs into map
    foreach(Event::EVENT_TYPE etype, subs)
    {
        m_routingTable.insert(etype, commu);
    }
}

//*************************************************************************************************************

void EventManager::removeCommunicator(Communicator* commu)
{
    foreach(Event::EVENT_TYPE etype, commu->getSubscriptions())
    {
        int removed = m_routingTable.remove(etype, commu);
        // consistency check:
        if (removed != 1)
        {
            std::cerr << "EventManager: WARNING ! Found " << removed << " entries instead of 1 for event type ";
            std::cerr << etype << " and communicator ID " << commu->getID() << std::endl;
        }
    }
}

//*************************************************************************************************************
//=============================================================================================================
// DEFINE STATIC MEMBERS
//=============================================================================================================

QMultiMap<Event::EVENT_TYPE, Communicator*> EventManager::m_routingTable;
