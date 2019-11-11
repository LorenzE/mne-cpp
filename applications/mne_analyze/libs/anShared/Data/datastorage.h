//=============================================================================================================
/**
* @file     datastorage.h
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
* @brief     DataStorage class declaration.
*
*/

#ifndef ANSHAREDLIB_DATASTORAGE_H
#define ANSHAREDLIB_DATASTORAGE_H


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
#include <QHash>
#include <QString>


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

class AbstractData;

//=============================================================================================================
/**
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class ANSHAREDSHARED_EXPORT DataStorage
{

public:
    typedef QSharedPointer<DataStorage> SPtr;            /**< Shared pointer type for DataStorage. */
    typedef QSharedPointer<const DataStorage> ConstSPtr; /**< Const shared pointer type for DataStorage. */

    //=========================================================================================================
    /**
    * Public enum for all available data types. Naming convention: Namespace_Classname
    */
    enum DATA_TYPE
    {
        FSLIB_SURFACE
    };

    //=========================================================================================================
    /**
    * Constructs a DataStorage object.
    */
    DataStorage();

    //=========================================================================================================
    /**
    * @brief isLoaded Returns whether or not a certain object is already loaded in the DataStorage
    * @param filename Name of the respective file
    * @param dtype Type of the object
    * @return True if the file in question has already been loaded, false otherwise
    */
    bool isLoaded(QString filename, DATA_TYPE dtype);

    //=========================================================================================================
    /**
    * @brief loadObjectFile This method loads the specified object into the DataStorage.
    * @param filename Name of the respective file
    * @param dtype Type of the object
    * @param blocking If this flag is true, the object in question is loaded in the calling thread. A different
    *                 thread will be used otherwise
    */
    void loadObjectFile(QString filename, DATA_TYPE dtype, bool blocking = false);

    //=========================================================================================================
    /**
    * @brief getLoadedObjects Returns a vector of all loaded objects that have type dtype
    * @param dtype Type of the object
    * @return Vector of all loaded objects that have type dtype
    */
    QVector<QSharedPointer<AbstractData> > getLoadedObjects(DATA_TYPE dtype);

protected:

private:

    //TODO File names as keys
    QHash<QString, QSharedPointer<AbstractData> >        m_data;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_DATASTORAGE_H
