//=============================================================================================================
/**
* @file     surfacedata.cpp
* @author   Lars Debor <lars.debor@tu-ilmenaul.de>;
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
* @brief    SurfaceData class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "surfacedata.h"
#include <fs/surface.h>
#include "surfacesettings.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;
using namespace FSLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

SurfaceData::SurfaceData(const QString &p_sFile)
: m_surface(Surface(p_sFile))
{

}


//*************************************************************************************************************

SurfaceData::SurfaceData(const QString &subject_id, qint32 hemi, const QString &surf, const QString &subjects_dir)
: m_surface(Surface(subject_id, hemi, surf, subjects_dir))
{

}


//*************************************************************************************************************

void SurfaceData::initiSettings()
{
    m_pSettings = QSharedPointer<SurfaceSettings>::create(&m_surface);
}


//*************************************************************************************************************

Vector3f SurfaceData::vertexAt(int idx) const
{
    Eigen::Vector3f vector;
    vector[0] = m_surface.rr()(idx, 0);
    vector[1] = m_surface.rr()(idx, 1);
    vector[2] = m_surface.rr()(idx, 2);

    return vector;
}


//*************************************************************************************************************

Vector3f SurfaceData::normalAt(int idx) const
{
    Eigen::Vector3f vector;
    vector[0] = m_surface.nn()(idx, 0);
    vector[1] = m_surface.nn()(idx, 1);
    vector[2] = m_surface.nn()(idx, 2);

    return vector;
}


//*************************************************************************************************************

Vector3i SurfaceData::triAt(int idx) const
{
    Eigen::Vector3i vector;
    vector[0] = m_surface.tris()(idx, 0);
    vector[1] = m_surface.tris()(idx, 1);
    vector[2] = m_surface.tris()(idx, 2);

    return vector;
}


//*************************************************************************************************************

float SurfaceData::curvAt(int idx) const
{
    return m_surface.curv()[idx];
}


//*************************************************************************************************************
