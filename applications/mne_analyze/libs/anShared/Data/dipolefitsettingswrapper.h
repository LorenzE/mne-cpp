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

    //=========================================================================================================
    /**
    * This is called whenever the measurement file path should be changed. It saves the passed path and emits
    * the measurementFilePathChanged signal.
    *
    * @param sPath The new path.
    */
    void setMeasurementFilePath(const QString &sPath);

    //=========================================================================================================
    /**
    * This is called whenever the usage of raw data should be changed. It saves the setting and emits the
    * isRawChanged signal.
    *
    * @param bValue Whether or not raw data should be used for the respective dipole fit
    */
    void setIsRaw(bool bValue);

    //=========================================================================================================
    /**
    * This is called whenever the set number should be changed. It saves the new value and emits the
    * setNumChanged signal.
    *
    * @param iValue The new value for set number.
    */
    void setSetNum(int iValue);

    //=========================================================================================================
    /**
    * This is called whenever the usage of MEG data should be changed. It saves the new setting and emits the
    * includeMegChanged signal.
    *
    * @param bValue Whether or not MEG data should be included.
    */
    void setIncludeMeg(bool bValue);

    //=========================================================================================================
    /**
    * This is called whenever the usage of EEG data should be changed. It saves the new setting and emits the
    * includeEegChanged signal.
    *
    * @param bValue Whether or not EEG data should be included.
    */
    void setIncludeEeg(bool bValue);

    //=========================================================================================================
    /**
    * This is called whenever the T max value should be changed. It saves the new value and emits the
    * tMaxChanged signal
    *
    * @param dValue The new value for T max.
    */
    void setTMax(double dValue);

    //=========================================================================================================
    /**
    * This is called whenever the T min value should be changed. It saves the new value and emits the
    * tMinChanged signal
    *
    * @param dValue The new value for T min.
    */
    void setTMin(double dValue);

    //=========================================================================================================
    /**
    * This is called whenever the B max value should be changed. It saves the new value and emits the
    * bMaxChanged signal
    *
    * @param dValue The new value for B max.
    */
    void setBMax(double dValue);

    //=========================================================================================================
    /**
    * This is called whenever the B min value should be changed. It saves the new value and emits the
    * bMinChanged signal
    *
    * @param dValue The new value for B min.
    */
    void setBMin(double dValue);

    //=========================================================================================================
    /**
    * This is called whenever the name of the dipole should be changed. It saves the new name and emits the
    * dipPathChanged signal.
    *
    * @param sDipName The new name of the dipole fit
    */
    void setDipPath(const QString &sDipName);

signals:
    //=========================================================================================================
    /**
    * This is emitted whenever the measurement file path has changed.
    *
    * @param sPath New measurement file path.
    */
    void measurementFilePathChanged(const QString &sPath);

    //=========================================================================================================
    /**
    * This is emitted whenever the usage of raw data has changed.
    *
    * @param bValue New value for usage of raw data.
    */
    void isRawChanged(bool bValue);

    //=========================================================================================================
    /**
    * This is emitted whenever the number of the set has changed.
    *
    * @param iSetNum New value for the set number.
    */
    void setNumChanged(int iSetNum);

    //=========================================================================================================
    /**
    * This emitted whenever the usage of MEG data has changed.
    *
    * @param bValue New value for MEG data usage.
    */
    void includeMegChanged(bool bValue);

    //=========================================================================================================
    /**
    * This emitted whenever the usage of EEG data has changed.
    *
    * @param bValue New value for EEG data usage.
    */
    void includeEegChanged(bool bValue);

    //=========================================================================================================
    /**
    * This is emitted whenever the value for T max has changed.
    *
    * @param dValue New value for T max.
    */
    void tMaxChanged(double dValue);

    //=========================================================================================================
    /**
    * This is emitted whenever the value for T min has changed.
    *
    * @param dValue New value for T min.
    */
    void tMinChanged(double dValue);

    //=========================================================================================================
    /**
    * This is emitted whenever the value for B max has changed.
    *
    * @param dValue New value for B max.
    */
    void bMaxChanged(double dValue);

    //=========================================================================================================
    /**
    * This is emitted whenever the value for B min has changed.
    *
    * @param dValue The new value for B min.
    */
    void bMinChanged(double dValue);

    //=========================================================================================================
    /**
    * This is emitted whenever the dipole fit's name has changed.
    *
    * @param sDipName New name of the fit.
    */
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
