//=============================================================================================================
/**
* @file     datasettings.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     March, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief     DataSettings class declaration.
*
*/

#ifndef ANSHAREDLIB_DATASETTINGS_H
#define ANSHAREDLIB_DATASETTINGS_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE ANSHAREDLIB
//=============================================================================================================

namespace ANSHAREDLIB {


//*************************************************************************************************************
//=============================================================================================================
// ANSHAREDLIB FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class ANSHAREDSHARED_EXPORT DataSettings
{

public:
    typedef QSharedPointer<DataSettings> SPtr;            /**< Shared pointer type for DataSettings. */
    typedef QSharedPointer<const DataSettings> ConstSPtr; /**< Const shared pointer type for DataSettings. */

    //=========================================================================================================
    /**
    * Constructs a DataSettings object.
    */
    DataSettings();

    //=========================================================================================================
    /**
    * Destructor.
    */
    virtual ~DataSettings();

protected:

private:



};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_DATASETTINGS_H
