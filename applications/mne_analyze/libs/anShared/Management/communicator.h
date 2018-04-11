//=============================================================================================================
/**
* @file     communicator.h
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
* @brief    Communicator class declaration.
*
*/

#ifndef ANSHAREDLIB_COMMUNICATOR_H
#define ANSHAREDLIB_COMMUNICATOR_H

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

#include <QObject>
#include <QSharedPointer>
#include <QPointer>
#include <QVector>

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
* DECLARE CLASS Communicator
*
* @brief Communicator class for Event communication
*/
class ANSHAREDSHARED_EXPORT Communicator : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<Communicator> SPtr;            /**< Shared pointer type for GeometryInfo. */
    typedef QSharedPointer<const Communicator> ConstSPtr; /**< Const shared pointer type for GeometryInfo. */

    typedef long CommunicatorID;                        /**< Typedef for CommunicatorID. */

    //=========================================================================================================
    /**
     * @brief Communicator constructs a Communicator object that emits a signal (receivedEvent) when one of
     *        the passed list of events happens. A further QtConnect IS necessary (See implementation of
     *        second constructor for more details. Qt::DirectConnection is recommended.
     * @param subs The list of relevant events.
     */
    Communicator(QVector<Event::EVENT_TYPE> subs = QVector<Event::EVENT_TYPE>());

    //=========================================================================================================
    /**
     * @brief Communicator constructs a Communicator object that is connected to the Extensions' handleEvent
     *        method via a Qt::DirectConnection.
     * @param extension The Extensions to connect to.
     */
    Communicator(IExtension* extension);

    //=========================================================================================================

    /**
     * @brief Destructs the communicator and disconnects it from the EventManager
     */
    ~Communicator();

    //=========================================================================================================

    /**
     * @brief publishEvent Sends an Event of type etype into the event system
     * @param etype Type of the event to be published
     * @param data Potential data to be attached to the event
     */
    void publishEvent(Event::EVENT_TYPE etype, QVariant data = QVariant()) const;

    //=========================================================================================================

    /**
     * @brief updateSubscriptions Overwrites the Communicators subscriptions. Attention: old subscriptions will
     *        be deleted! See updateSubscriptions.
     * @param subs The new list of Event types to be notified about
     */
    void updateSubscriptions(QVector<Event::EVENT_TYPE> subs);

    //=========================================================================================================

    /**
     * @brief addSubscriptions Adds the provided list of Event types to the preexisting list.
     * @param newsubs List of new subscriptions.
     */
    void addSubscriptions(QVector<Event::EVENT_TYPE> newsubs);

    //=========================================================================================================

    /**
     * @brief addSubscriptions Convenience overload, see addSubscriptions
     * @param newsub
     */
    void addSubscriptions(Event::EVENT_TYPE newsub);

    //=========================================================================================================

    /**
     * @brief manualDisconnect Manually disconnects a Communicator from the Event system.
     */
    void manualDisconnect(void);

    //=========================================================================================================

    /**
     * @brief getSubscriptions Getter for list of subscriptions
     * @return List of subscriptions
     */
    inline QVector<Event::EVENT_TYPE> getSubscriptions(void) const;

    //=========================================================================================================

    /**
     * @brief getID Getter for internal ID
     * @return Internal ID
     */
    inline CommunicatorID getID(void) const;

    //=========================================================================================================

private:
    static CommunicatorID m_IDCounter;                  /**< ID-Counter for Communicator instances. */
    inline static CommunicatorID nextID();              /**< Simply increments the counter and returns it. */

    CommunicatorID m_ID;                                /**< Communicator ID. */
    QVector<Event::EVENT_TYPE> m_EventSubscriptions;    /**< All event types that the Communicator receives*/


    //=========================================================================================================

signals: 
    /**
    * @brief Called by EventManager whenever an event needs to be handled. This must be connected to some other
    *        function for actual usage.
    *
    * @param e The event that was received
    */
    void receivedEvent(Event e);

};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline Communicator::CommunicatorID Communicator::nextID()
{
    return ++m_IDCounter;
}

//=============================================================================================================

inline QVector<Event::EVENT_TYPE> Communicator::getSubscriptions(void) const
{
    return m_EventSubscriptions;
}

//=============================================================================================================

inline Communicator::CommunicatorID Communicator::getID(void) const
{
    return m_ID;
}

} // namespace

#endif // ANSHAREDLIB_COMMUNICATOR_H
