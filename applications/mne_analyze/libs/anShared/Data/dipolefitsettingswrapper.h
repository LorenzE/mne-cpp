//=============================================================================================================
/**
* @file     dipolefitsettingswrapper.h
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
* @brief     DipoleFitSettingsWrapper class declaration.
*
*/

#ifndef ANSHAREDLIB_DIPOLEFITSETTINGSWRAPPER_H
#define ANSHAREDLIB_DIPOLEFITSETTINGSWRAPPER_H


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
#include <QObject>
#include <QString>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace INVERSELIB {
    class DipoleFitSettings;
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
* This class is a wrapper for the INVERSELIB::DipoleFitSettings class.
* It provides signals and slots for changing data.
*
* @brief This class is a wrapper for the INVERSELIB::DipoleFitSettings class.
*/
class ANSHAREDSHARED_EXPORT DipoleFitSettingsWrapper : public QObject
{
    Q_OBJECT

public:
    typedef QSharedPointer<DipoleFitSettingsWrapper> SPtr;            /**< Shared pointer type for DipoleFitSettingsWrapper. */
    typedef QSharedPointer<const DipoleFitSettingsWrapper> ConstSPtr; /**< Const shared pointer type for DipoleFitSettingsWrapper. */

    //=========================================================================================================
    /**
    * Constructs a DipoleFitSettingsWrapper object.
    */
    DipoleFitSettingsWrapper();

    //=========================================================================================================
    /**
    * Constructs Dipole Fit Settings
    *
    * @param [in] argc (argument count) is an integer that indicates how many arguments were entered on the command line when the program was started.
    * @param [in] argv (argument vector) is an array of pointers to arrays of character objects. The array objects are null-terminated strings, representing the arguments that were entered on the command line when the program was started.
    */
    explicit DipoleFitSettingsWrapper(int *argc,char **argv);

    //=========================================================================================================
    /**
    * Default destructor.
    */
    ~DipoleFitSettingsWrapper();

    //=========================================================================================================
    /**
    * Check whether Dipole Fit Settings are correctly set.
    */
    void checkIntegrity();

    //=========================================================================================================
    /**
    * Returns a pointer to the wapped object.
    */
    INVERSELIB::DipoleFitSettings *getSettings();

    //TODO Docu
    void setMeasurementFilePath(const QString &sPath);
    void setIsRaw(bool value);
    void setSetNum(int value);
    void setIncludeMeg(bool value);
    void setIncludeEeg(bool value);
    void setTMax(double value);
    void setTMin(double value);
    void setBMax(double value);
    void setBMin(double value);
    void setDipPath(const QString &sDipName);

signals:
    //TODO Docu
    void measurementFilePathChanged(const QString &path);
    void isRawChanged(bool value);
    void setNumChanged(int setNum);
    void includeMegChanged(bool value);
    void includeEegChanged(bool value);
    void tMaxChanged(double value);
    void tMinChanged(double value);
    void bMaxChanged(double value);
    void bMinChanged(double value);
    void dipPathChanged(const QString &sDipName);

private:

    INVERSELIB::DipoleFitSettings*    m_pSettings;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_DIPOLEFITSETTINGSWRAPPER_H
