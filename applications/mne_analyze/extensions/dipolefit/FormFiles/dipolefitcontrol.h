#ifndef DIPOLEFITCONTROL_H
#define DIPOLEFITCONTROL_H

#include <QWidget>

namespace Ui {
class DipoleFitControl;
}

class DipoleFitControl : public QWidget
{
    Q_OBJECT

public:
    explicit DipoleFitControl(QWidget *parent = 0);
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

    void setMeasFilePath(const QString &path);

    void setUseRaw(bool value);

    void setSetNumber(int number);

    void setIncludeMeg(bool checked);

    void setIncludeEeg(bool checked);

    void setTMax(double value);

    void setTMin(double value);

    void setBMax(double value);

    void setBMin(double value);

    QString getFitName();
signals:

    void fitButtonClicked();

    void browseButtonClicked();

    void activeModelSelected(const QString &modelName);

    //value changed signals
    void useRawCheckStateChanged(bool checkState);

    void setNumChanged(int setNum);

    void includeMegChanged(bool checkState);
    void includeEegChanged(bool checkState);
    void tMaxChanged(double value);
    void tMinChanged(double value);
    void bMaxChanged(double value);
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

    void on_m_qCheckBoxUseRaw_stateChanged(int state);
    void on_m_qSpinBoxSetNumber_valueChanged(int value);
    void on_m_qCheckBoxIncludeMeg_stateChanged(int state);
    void on_m_qCheckBoxIncludeEeg_stateChanged(int state);
    void on_m_qDoubleSpinBoxTMax_valueChanged(double value);
    void on_m_qDoubleSpinBoxTMin_valueChanged(double value);
    void on_m_qDoubleSpinBoxBMax_valueChanged(double value);
    void on_m_qDoubleSpinBoxBMin_valueChanged(double value);

private:
    Ui::DipoleFitControl *ui;
};

#endif // DIPOLEFITCONTROL_H
