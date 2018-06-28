#include "dipolefitcontrol.h"
#include "ui_dipolefitcontrol.h"



DipoleFitControl::DipoleFitControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DipoleFitControl)
{
    ui->setupUi(this);
}

DipoleFitControl::~DipoleFitControl()
{
    delete ui;
}

void DipoleFitControl::addModel(const QString &modelName)
{
    ui->m_qComboBoxFitSelector->addItem(modelName);
    qint32 index = ui->m_qComboBoxFitSelector->findText(modelName);
    ui->m_qComboBoxFitSelector->setCurrentIndex(index);
}

void DipoleFitControl::setMeasFilePath(const QString &path)
{
    ui->m_qLineEditSTCFile->setText(path);
}

void DipoleFitControl::setUseRaw(bool value)
{
    ui->m_qCheckBoxUseRaw->setChecked(value);
}

void DipoleFitControl::setSetNumber(int number)
{
    ui->m_qSpinBoxSetNumber->setValue(number);
}

void DipoleFitControl::setIncludeMeg(bool checked)
{
    ui->m_qCheckBoxIncludeMeg->setChecked(checked);
}

void DipoleFitControl::setIncludeEeg(bool checked)
{
    ui->m_qCheckBoxIncludeEeg->setChecked(checked);
}

void DipoleFitControl::setTMax(double value)
{
    ui->m_qDoubleSpinBoxTMax->setValue(value);
}

void DipoleFitControl::setTMin(double value)
{
    ui->m_qDoubleSpinBoxTMin->setValue(value);
}

void DipoleFitControl::setBMax(double value)
{
    ui->m_qDoubleSpinBoxBMax->setValue(value);
}

void DipoleFitControl::setBMin(double value)
{
    ui->m_qDoubleSpinBoxBMin->setValue(value);
}

QString DipoleFitControl::getFitName()
{
    return ui->m_qLineEditFitName->text();
}

void DipoleFitControl::on_m_qPushButtonFit_released()
{
    emit fitButtonClicked();
}

void DipoleFitControl::on_m_qPushButtonBrowseMeasurementPath_released()
{
    emit browseButtonClicked();
}

void DipoleFitControl::on_m_qComboBoxFitSelector_currentIndexChanged(const QString &text)
{
    emit activeModelSelected(text);
}

void DipoleFitControl::on_m_qCheckBoxUseRaw_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked) {
        emit useRawCheckStateChanged(true);
    }
    else if(state == Qt::CheckState::Unchecked) {
        emit useRawCheckStateChanged(false);
    }
}

void DipoleFitControl::on_m_qSpinBoxSetNumber_valueChanged(int value)
{
    emit setNumChanged(value);
}

void DipoleFitControl::on_m_qCheckBoxIncludeMeg_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked) {
        emit includeMegChanged(true);
    }
    else if(state == Qt::CheckState::Unchecked) {
        emit includeMegChanged(false);
    }
}

void DipoleFitControl::on_m_qCheckBoxIncludeEeg_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked) {
        emit includeEegChanged(true);
    }
    else if(state == Qt::CheckState::Unchecked) {
        emit includeEegChanged(false);
    }
}

void DipoleFitControl::on_m_qDoubleSpinBoxTMax_valueChanged(double value)
{
    emit tMaxChanged(value);
}

void DipoleFitControl::on_m_qDoubleSpinBoxTMin_valueChanged(double value)
{
    emit tMinChanged(value);
}

void DipoleFitControl::on_m_qDoubleSpinBoxBMax_valueChanged(double value)
{
    emit bMaxChanged(value);
}

void DipoleFitControl::on_m_qDoubleSpinBoxBMin_valueChanged(double value)
{
    emit bMinChanged(value);
}
