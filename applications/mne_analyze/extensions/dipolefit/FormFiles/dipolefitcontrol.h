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

    void setMeasFilePath(const QString &path);

    void setUseRaw(bool value);

    void setSetNumber(int number);

    void setIncludeMeg(bool checked);

    void setIncludeEeg(bool checked);

    void setTMax(double value);

    void setTMin(double value);

    void setBMax(double value);

    void setBMin(double value);
signals:

    void fitButtonClicked();

    void browseButtonClicked();

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

private:
    Ui::DipoleFitControl *ui;
};

#endif // DIPOLEFITCONTROL_H
