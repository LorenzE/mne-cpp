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

#include <QSharedPointer>
#include <QPointer>

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


//=========================================================================================================
/**
* DECLARE CLASS Communicator
*
* @brief Communicator class for Event communication
*/
class ANSHAREDSHARED_EXPORT Communicator
{
public:
    typedef long CommunicatorID;

private:
    static CommunicatorID communicatorID;

    CommunicatorID m_ID;

    Communicator(CommunicatorID ID);

public:
    typedef QSharedPointer<Communicator> SPtr;            /**< Shared pointer type for GeometryInfo. */
    typedef QSharedPointer<const Communicator> ConstSPtr; /**< Const shared pointer type for GeometryInfo. */

    //=========================================================================================================
    /**
    * deleted default constructor (use factory method)
    */
    Communicator() = delete;

    //=========================================================================================================
    /**
    * Creates and returns a new Communicator object with a unique ID
    *
    * @return A new Communicator object.
    */
    static Communicator* requestCommunicator(void);

    //=========================================================================================================
    /**
    * Called by EventManager whenever an event needs to be handled
    *
    * @param e The event that needs to be handled
    */
    void notify(Event* e);
};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline Communicator* Communicator::requestCommunicator()
{
    return new Communicator(communicatorID++);
}

} // namespace

#endif // ANSHAREDLIB_COMMUNICATOR_H
