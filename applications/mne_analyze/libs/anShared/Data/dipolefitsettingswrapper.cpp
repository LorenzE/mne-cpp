//=============================================================================================================
/**
* @file     dipolefitsettingswrapper.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     June, 2018
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
* @brief    DipoleFitSettingsWrapper class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dipolefitsettingswrapper.h"
#include <inverse/dipoleFit/dipole_fit_settings.h>


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
using namespace INVERSELIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

DipoleFitSettingsWrapper::DipoleFitSettingsWrapper()
    : m_pSettings(new DipoleFitSettings())
{

}


//*************************************************************************************************************

DipoleFitSettingsWrapper::DipoleFitSettingsWrapper(int *argc, char **argv)
    : m_pSettings(new DipoleFitSettings(argc, argv))
{

}


//*************************************************************************************************************

DipoleFitSettingsWrapper::~DipoleFitSettingsWrapper()
{
    delete m_pSettings;
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::checkIntegrity()
{
    m_pSettings->checkIntegrity();
}


//*************************************************************************************************************

INVERSELIB::DipoleFitSettings *DipoleFitSettingsWrapper::getSettings()
{
    return m_pSettings;
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setMeasurementFilePath(const QString &sPath)
{
    m_pSettings->measname = sPath;
    emit measurementFilePathChanged(sPath);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setIsRaw(bool value)
{
    m_pSettings->is_raw = value;
    emit isRawChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setSetNum(int value)
{
    m_pSettings->setno = value;
    emit setNumChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setIncludeMeg(bool value)
{
    m_pSettings->include_meg = value;
    emit includeMegChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setIncludeEeg(bool value)
{
    m_pSettings->include_eeg = value;
    emit includeEegChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setTMax(double value)
{
    m_pSettings->tmax = value;
    emit tMaxChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setTMin(double value)
{
    m_pSettings->tmin = value;
    emit tMinChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setBMax(double value)
{
    m_pSettings->bmax = value;
    emit bMaxChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setBMin(double value)
{
    m_pSettings->bmin = value;
    emit bMinChanged(value);
}


//*************************************************************************************************************

void DipoleFitSettingsWrapper::setDipPath(const QString &sDipName)
{
    m_pSettings->dipname = sDipName;
    emit dipPathChanged(sDipName);
}


//*************************************************************************************************************
