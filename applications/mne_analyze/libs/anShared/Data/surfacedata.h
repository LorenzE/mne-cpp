//=============================================================================================================
/**
* @file     surfacedata.h
* @author   Lars Debor <lars.debor@tu-ilmenaul.de>;
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
* @brief     SurfaceData class declaration.
*
*/

#ifndef ANSHAREDLIB_SURFACEDATA_H
#define ANSHAREDLIB_SURFACEDATA_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "abstractdata.h"
#include "../anshared_global.h"
#include <fs/surface.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================

#include <Eigen/Core>


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
* This is a wrapper class for Surface.
*
* @brief This is a wrapper class for Surface.
*/
class ANSHAREDSHARED_EXPORT SurfaceData : public AbstractData
{

public:
    typedef QSharedPointer<SurfaceData> SPtr;            /**< Shared pointer type for SurfaceData. */
    typedef QSharedPointer<const SurfaceData> ConstSPtr; /**< Const shared pointer type for SurfaceData. */

    //=========================================================================================================
    /**
    * Default Constructor.
    */
    SurfaceData();

    //=========================================================================================================
    /**
    * Construts the surface by reading it of the given file.
    *
    * @param[in] p_sFile    Surface file name with path
    */
    explicit SurfaceData(const QString& p_sFile);

    //=========================================================================================================
    /**
    * Construts the surface by reading it of the given file.
    *
    * @param[in] subject_id         Name of subject
    * @param[in] hemi               Which hemisphere to load {0 -> lh, 1 -> rh}
    * @param[in] surf               Name of the surface to load (eg. inflated, orig ...)
    * @param[in] subjects_dir       Subjects directory
    */
    explicit SurfaceData(const QString &subject_id, qint32 hemi, const QString &surf, const QString &subjects_dir);

    //=========================================================================================================
    /**
    * Default Destructor.
    */
    ~SurfaceData() = default;

    void initiSettings() override;

    //=========================================================================================================
    /**
    * Returns whether SurfaceData is empty.
    *
    * @return true if is empty, false otherwise
    */
    inline bool isEmpty() const;

    //=========================================================================================================
    /**
    * Coordinates of vertices (rr)
    *
    * @return coordinates of vertices
    */
    inline const Eigen::MatrixX3f& vertices() const;

    Eigen::Vector3f vertexAt(int idx) const;

    //=========================================================================================================
    /**
    * Normalized surface normals for each vertex
    *
    * @return surface normals
    */
    inline const Eigen::MatrixX3f& normals() const;

    Eigen::Vector3f normalAt(int idx) const;

    //=========================================================================================================
    /**
    * The triangle descriptions
    *
    * @return triangle descriptions
    */
    inline const Eigen::MatrixX3i& tris() const;

protected:

private:

    FSLIB::Surface      m_surface;

    //TODO SETTINGS: hemi, surfname, path, offset, filename

};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

bool SurfaceData::isEmpty() const
{
    return m_surface.isEmpty();
}


//*************************************************************************************************************

const Eigen::MatrixX3f &SurfaceData::vertices() const
{
    return m_surface.rr();
}


//*************************************************************************************************************

const Eigen::MatrixX3f &SurfaceData::normals() const
{
    return m_surface.nn();
}


//*************************************************************************************************************

const Eigen::MatrixX3i &SurfaceData::tris() const
{
    return m_surface.tris();
}


//*************************************************************************************************************


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_SURFACEDATA_H
