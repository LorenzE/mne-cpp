//=============================================================================================================
/**
* @file     dipolefitcontrol.cpp
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
* @brief    Definition of the DipoleFitControl class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dipolefitcontrol.h"
#include "ui_dipolefitcontrol.h"


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

DipoleFitControl::DipoleFitControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DipoleFitControl)
{
    ui->setupUi(this);
}


//*************************************************************************************************************

DipoleFitControl::~DipoleFitControl()
{
    delete ui;
}


//*************************************************************************************************************

void DipoleFitControl::addModel(const QString &modelName)
{
    ui->m_qComboBoxFitSelector->addItem(modelName);
    qint32 index = ui->m_qComboBoxFitSelector->findText(modelName);
    ui->m_qComboBoxFitSelector->setCurrentIndex(index);
}


//*************************************************************************************************************

void DipoleFitControl::removeModel(const QString &modelName)
{
    qint32 index = ui->m_qComboBoxFitSelector->findText(modelName);
    ui->m_qComboBoxFitSelector->removeItem(index);
}


//*************************************************************************************************************

void DipoleFitControl::setMeasFilePath(const QString &path)
{
    ui->m_qLineEditSTCFile->setText(path);
}


//*************************************************************************************************************

void DipoleFitControl::setUseRaw(bool value)
{
    ui->m_qCheckBoxUseRaw->setChecked(value);
}


//*************************************************************************************************************

void DipoleFitControl::setSetNumber(int number)
{
    ui->m_qSpinBoxSetNumber->setValue(number);
}


//*************************************************************************************************************

void DipoleFitControl::setIncludeMeg(bool checked)
{
    ui->m_qCheckBoxIncludeMeg->setChecked(checked);
}


//*************************************************************************************************************

void DipoleFitControl::setIncludeEeg(bool checked)
{
    ui->m_qCheckBoxIncludeEeg->setChecked(checked);
}


//*************************************************************************************************************

void DipoleFitControl::setTMax(double value)
{
    ui->m_qDoubleSpinBoxTMax->setValue(value);
}


//*************************************************************************************************************

void DipoleFitControl::setTMin(double value)
{
    ui->m_qDoubleSpinBoxTMin->setValue(value);
}


//*************************************************************************************************************

void DipoleFitControl::setBMax(double value)
{
    ui->m_qDoubleSpinBoxBMax->setValue(value);
}


//*************************************************************************************************************

void DipoleFitControl::setBMin(double value)
{
    ui->m_qDoubleSpinBoxBMin->setValue(value);
}


//*************************************************************************************************************

QString DipoleFitControl::getFitName()
{
    return ui->m_qLineEditFitName->text();
}


//*************************************************************************************************************

bool DipoleFitControl::getUseRaw()
{
    Qt::CheckState checkState = ui->m_qCheckBoxUseRaw->checkState();
    bool bState = false;
    if(checkState == Qt::CheckState::Checked) {
        bState = true;
    }

    return bState;
}


//*************************************************************************************************************

int DipoleFitControl::getSetNum()
{
    return ui->m_qSpinBoxSetNumber->value();
}


//*************************************************************************************************************

bool DipoleFitControl::getIncludeMegState()
{
     Qt::CheckState checkState = ui->m_qCheckBoxIncludeMeg->checkState();
     bool bState = false;
     if(checkState == Qt::CheckState::Checked) {
         bState = true;
     }

     return bState;
}


//*************************************************************************************************************

bool DipoleFitControl::getIncludeEegState()
{
    Qt::CheckState checkState = ui->m_qCheckBoxIncludeEeg->checkState();
    bool bState = false;
    if(checkState == Qt::CheckState::Checked) {
        bState = true;
    }

    return bState;
}


//*************************************************************************************************************

double DipoleFitControl::getTMax()
{
    return ui->m_qDoubleSpinBoxTMax->value();
}


//*************************************************************************************************************

double DipoleFitControl::getTMin()
{
    return ui->m_qDoubleSpinBoxTMin->value();
}


//*************************************************************************************************************

double DipoleFitControl::getBMax()
{
    return ui->m_qDoubleSpinBoxBMax->value();
}


//*************************************************************************************************************

double DipoleFitControl::getBMin()
{
    return ui->m_qDoubleSpinBoxBMin->value();
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qPushButtonFit_released()
{
    emit fitButtonClicked();
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qPushButtonBrowseMeasurementPath_released()
{
    emit browseButtonClicked();
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qComboBoxFitSelector_currentIndexChanged(const QString &text)
{
    emit activeModelSelected(text);
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qCheckBoxUseRaw_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked) {
        emit useRawCheckStateChanged(true);
    }
    else if(state == Qt::CheckState::Unchecked) {
        emit useRawCheckStateChanged(false);
    }
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qSpinBoxSetNumber_valueChanged(int value)
{
    emit setNumChanged(value);
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qCheckBoxIncludeMeg_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked) {
        emit includeMegChanged(true);
    }
    else if(state == Qt::CheckState::Unchecked) {
        emit includeMegChanged(false);
    }
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qCheckBoxIncludeEeg_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked) {
        emit includeEegChanged(true);
    }
    else if(state == Qt::CheckState::Unchecked) {
        emit includeEegChanged(false);
    }
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qDoubleSpinBoxTMax_valueChanged(double value)
{
    emit tMaxChanged(value);
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qDoubleSpinBoxTMin_valueChanged(double value)
{
    emit tMinChanged(value);
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qDoubleSpinBoxBMax_valueChanged(double value)
{
    emit bMaxChanged(value);
}


//*************************************************************************************************************

void DipoleFitControl::on_m_qDoubleSpinBoxBMin_valueChanged(double value)
{
    emit bMinChanged(value);
}


//*************************************************************************************************************

void DipoleFitControl::on_loadDipoleButton_released()
{
    emit loadFitFile();
}


//*************************************************************************************************************

void DipoleFitControl::on_saveDipoleFitButton_released()
{
    emit saveFitFile();
}
