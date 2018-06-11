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

void DipoleFitControl::setMeasFilePath(const QString &path)
{
    ui->m_qLineEditSTCFile->setText(path);
}

void DipoleFitControl::setUseRaw(bool value)
{
    //TODO
    //ui->m_qComboBoxUseRaw->set
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

void DipoleFitControl::on_m_qPushButtonFit_released()
{
    emit fitButtonClicked();
}

void DipoleFitControl::on_m_qPushButtonBrowseMeasurementPath_released()
{
    emit browseButtonClicked();
}
