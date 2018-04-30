//=============================================================================================================
/**
* @file     surfacesettings.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     March, 2018
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
* @brief     SurfaceSettings class declaration.
*
*/

#ifndef ANSHAREDLIB_SURFACESETTINGS_H
#define ANSHAREDLIB_SURFACESETTINGS_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "datasettings.h"
#include "../anshared_global.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================

#include <Eigen/Core>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace FSLIB {
    class Surface;
}


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
* This class is a wrapper class for the settings of a Surface.
*
* @brief This class stores the settings of a Surface.
*/
class ANSHAREDSHARED_EXPORT SurfaceSettings : public DataSettings
{

public:
    typedef QSharedPointer<SurfaceSettings> SPtr;            /**< Shared pointer type for SurfaceSettings. */
    typedef QSharedPointer<const SurfaceSettings> ConstSPtr; /**< Const shared pointer type for SurfaceSettings. */

    //=========================================================================================================
    /**
    * Deleted default constructor
    */
    SurfaceSettings() = delete;

    //=========================================================================================================
    /**
    * Constructs a SurfaceSettings object.
    *
    * @param[in] pSurface    The surface that this object wraps.
    */
    SurfaceSettings(FSLIB::Surface* pSurface);

    //=========================================================================================================
    /**
    * Returns the hemisphere id (0 = lh; 1 = rh)
    *
    * @return hemisphere id
    */
    qint32 getHemi() const;

    //=========================================================================================================
    /**
    * Returns the loaded surface type (eg. inflated, orig ...).
    *
    * @return the surface type
    */
    QString getSurfaceType() const;

    //=========================================================================================================
    /**
    * Returns the vector offset.
    *
    * @return the offset vector
    */
    Eigen::Vector3f getOffset() const;

    //=========================================================================================================
    /**
    * Returns the path to surf directory.
    *
    * @return the path to surf directory
    */
    QString getFilePath() const;

    //=========================================================================================================
    /**
    * Retuns the surf file name.
    *
    * @return the surf file name
    */
    QString getFileName() const;

    //=========================================================================================================
    /**
    * Sets a new file path for the surface.
    *
    * @param[in] sPath  The new file path
    */
    void setFilePath(const QString &sPath);

    //=========================================================================================================
    /**
    * Sets the new offset.
    *
    * @param[in] vOffset    The new offset
    */
    void setOffset(const Eigen::Vector3f &vOffset);

protected:

private:
    FSLIB::Surface*     m_pSurface;

signals:



};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_SURFACESETTINGS_H
