//=============================================================================================================
/**
* @file     surfacesetdata.h
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
* @brief     SurfaceSetData class declaration.
*
*/

#ifndef ANSHAREDLIB_SURFACESETDATA_H
#define ANSHAREDLIB_SURFACESETDATA_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "abstractdata.h"
#include "../anshared_global.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QMap>


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

class SurfaceData;

//=============================================================================================================
/**
* This is a wrapper class for SurfaceSet.
*
* @brief This is a wrapper class for SurfaceSet.
*/
class ANSHAREDSHARED_EXPORT SurfaceSetData : public AbstractData
{

public:
    typedef QSharedPointer<SurfaceSetData> SPtr;            /**< Shared pointer type for SurfaceSetData. */
    typedef QSharedPointer<const SurfaceSetData> ConstSPtr; /**< Const shared pointer type for SurfaceSetData. */

    //=========================================================================================================
    /**
    * Constructs a SurfaceSetData object.
    */
    SurfaceSetData();

    //=========================================================================================================
    /**
    * Construts the surface set by reading it of the given files.
    *
    * @param[in] subject_id         Name of subject
    * @param[in] hemi               Which hemisphere to load {0 -> lh, 1 -> rh, 2 -> both}
    * @param[in] surf               Name of the surface to load (eg. inflated, orig ...)
    * @param[in] subjects_dir       Subjects directory
    */
    explicit SurfaceSetData(const QString &subject_id, qint32 hemi, const QString &surf, const QString &subjects_dir);

protected:

private:

    QMap<qint32, SurfaceData>       m_surfaceData;

signals:


    void newSurfaceLoaded();

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_SURFACESETDATA_H
