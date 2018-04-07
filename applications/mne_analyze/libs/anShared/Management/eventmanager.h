//=============================================================================================================
/**
* @file     eventmanager.h
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
* @brief    EventManager class declaration.
*
*/

#ifndef ANSHAREDLIB_EVENT_MANAGER_H
#define ANSHAREDLIB_EVENT_MANAGER_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"
#include "event.h"

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QPointer>
#include <QMultiMap>

//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE ANSHAREDLIB
//=============================================================================================================

namespace ANSHAREDLIB
{


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class IExtension;

//=========================================================================================================
/**
* DECLARE CLASS EventManager
*
* @brief EventManager for Event communication
*/
class ANSHAREDSHARED_EXPORT EventManager
{
private:
    static QMultiMap<Event::EVENT_TYPE, IExtension*> m_routingTable;

public:
    typedef QSharedPointer<EventManager> SPtr;            /**< Shared pointer type for GeometryInfo. */
    typedef QSharedPointer<const EventManager> ConstSPtr; /**< Const shared pointer type for GeometryInfo. */

    //=========================================================================================================
    /**
    * deleted default constructor (static class).
    */
    EventManager() = delete;

    //=========================================================================================================
    /**
     * @brief publishEvent Communicate an event to all entities that have registered for the respective event type
     *
     * @param e The event to publish
     */
    static void publishEvent(Event* e);

    //=========================================================================================================
    /**
     * @brief addExtension Adds an Extension to the communication manager, meaning that all Events that the
     * Extensions has subscribed for will be passed to it.
     *
     * @param extension The Extension to add to the Event routing system.
     */
    static void addExtension(IExtension* extension);
};

} // namespace

#endif // ANSHAREDLIB_EVENT_MANAGER_H
