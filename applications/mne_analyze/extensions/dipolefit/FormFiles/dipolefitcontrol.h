//=============================================================================================================
/**
* @file     dipolefitcontrol.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017 Christoph Dinh, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the declaration of the DipoleFitControl class.
*
*/

#ifndef DIPOLEFITCONTROL_H
#define DIPOLEFITCONTROL_H

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QWidget>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Ui {
class DipoleFitControl;
}

//=============================================================================================================
/**
* This class allows the connection between GUI and DipoleFit extension.
*
* @brief This class allows the connection between GUI and DipoleFit extension.
*/
class DipoleFitControl : public QWidget
{
    Q_OBJECT

public:

    //=========================================================================================================
    /**
    * Constructs a DipoleFitCotrol.
    */
    explicit DipoleFitControl(QWidget *parent = 0);

    //=========================================================================================================
    /**
    * Destructs a DipoleFitControl.
    */
    ~DipoleFitControl();

    //=========================================================================================================
    /**
    * Add a new model name to the select active model menu.
    * The new Model is also selected.
    */
    void addModel(const QString &modelName);

    //=========================================================================================================
    /**
    * Removes model name from the select active model menu.
    */
    void removeModel(const QString &modelName);

    //=========================================================================================================
    /**
    * Sets a new value for measurement path.
    */
    void setMeasFilePath(const QString &path);

    //=========================================================================================================
    /**
    * Sets a new value for use raw.
    */
    void setUseRaw(bool value);

    //=========================================================================================================
    /**
    * Sets a new value for set number.
    */
    void setSetNumber(int number);

    //=========================================================================================================
    /**
    * Sets a new value include MEG.
    */
    void setIncludeMeg(bool checked);

    //=========================================================================================================
    /**
    * Sets a new value for include EEG.
    */
    void setIncludeEeg(bool checked);

    //=========================================================================================================
    /**
    * Sets a new value tMax.
    */
    void setTMax(double value);

    //=========================================================================================================
    /**
    * Sets a new value tMin.
    */
    void setTMin(double value);

    //=========================================================================================================
    /**
    * Sets a new value bMax.
    */
    void setBMax(double value);

    //=========================================================================================================
    /**
    * Sets a new value bMin.
    */
    void setBMin(double value);

    //=========================================================================================================
    /**
    * Returns the fit name.
    */
    QString getFitName();

    //=========================================================================================================
    /**
    * Returns the the use raw state.
    */
    bool getUseRaw();

    //=========================================================================================================
    /**
    * Returns the the set number value.
    */
    int getSetNum();

    //=========================================================================================================
    /**
    * Returns the the include MEG state.
    */
    bool getIncludeMegState();

    //=========================================================================================================
    /**
    * Returns the the include EEG state.
    */
    bool getIncludeEegState();

    //=========================================================================================================
    /**
    * Returns the the tMax value.
    */
    double getTMax();

    //=========================================================================================================
    /**
    * Returns the the tMin value.
    */
    double getTMin();

    //=========================================================================================================
    /**
    * Returns the the bMax value.
    */
    double getBMax();

    //=========================================================================================================
    /**
    * Returns the the bMin value.
    */
    double getBMin();

signals:

    //=========================================================================================================
    /**
    * Signal is emitted when the calculate fit button was pressed.
    */
    void fitButtonClicked();

    //=========================================================================================================
    /**
    * Signal is emitted when the browse button was pressed.
    */
    void browseButtonClicked();

    //=========================================================================================================
    /**
    * Signal is emitted when the a new active model was selected.
    */
    void activeModelSelected(const QString &modelName);

    //=========================================================================================================
    /**
    * Signal is emitted when the use raw check state was changed.
    */
    void useRawCheckStateChanged(bool checkState);

    //=========================================================================================================
    /**
    * Signal is emitted when the set number was changed.
    */
    void setNumChanged(int setNum);

    //=========================================================================================================
    /**
    * Signal is emitted when the include MEG state was changed.
    */
    void includeMegChanged(bool checkState);

    //=========================================================================================================
    /**
    * Signal is emitted when the include EEG state was changed.
    */
    void includeEegChanged(bool checkState);

    //=========================================================================================================
    /**
    * Signal is emitted when the tMax value was changed.
    */
    void tMaxChanged(double value);

    //=========================================================================================================
    /**
    * Signal is emitted when the tMin value was changed.
    */
    void tMinChanged(double value);

    //=========================================================================================================
    /**
    * Signal is emitted when the bMax value was changed.
    */
    void bMaxChanged(double value);

    //=========================================================================================================
    /**
    * Signal is emitted when the bMin value was changed.
    */
    void bMinChanged(double value);

private slots:
    //=========================================================================================================
    /**
    * Called when calculating button was pressed.
    */
    void on_m_qPushButtonFit_released();

    //=========================================================================================================
    /**
    * Called when browse button was pressed.
    */
    void on_m_qPushButtonBrowseMeasurementPath_released();

    //=========================================================================================================
    /**
    * Called when a model is selected form the drop down menu.
    */
    void on_m_qComboBoxFitSelector_currentIndexChanged(const QString &text);

    //=========================================================================================================
    /**
    * Called when use raw state changed.
    */
    void on_m_qCheckBoxUseRaw_stateChanged(int state);

    //=========================================================================================================
    /**
    * Called when the set number value changed.
    */
    void on_m_qSpinBoxSetNumber_valueChanged(int value);

    //=========================================================================================================
    /**
    * Called when use include MEG state changed.
    */
    void on_m_qCheckBoxIncludeMeg_stateChanged(int state);

    //=========================================================================================================
    /**
    * Called when use include EEG state changed.
    */
    void on_m_qCheckBoxIncludeEeg_stateChanged(int state);

    //=========================================================================================================
    /**
    * Called when the tMax value changed.
    */
    void on_m_qDoubleSpinBoxTMax_valueChanged(double value);

    //=========================================================================================================
    /**
    * Called when the tMin value changed.
    */
    void on_m_qDoubleSpinBoxTMin_valueChanged(double value);

    //=========================================================================================================
    /**
    * Called when the bMax value changed.
    */
    void on_m_qDoubleSpinBoxBMax_valueChanged(double value);

    //=========================================================================================================
    /**
    * Called when the bMin value changed.
    */
    void on_m_qDoubleSpinBoxBMin_valueChanged(double value);

private:

    Ui::DipoleFitControl *ui;       /**< Pointer to the UI of this class. */
};

#endif // DIPOLEFITCONTROL_H
