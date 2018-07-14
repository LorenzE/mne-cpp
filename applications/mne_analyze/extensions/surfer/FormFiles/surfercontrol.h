#ifndef SURFERCONTROL_H
#define SURFERCONTROL_H

#include <QWidget>

class QListWidgetItem;

namespace Ui {
    class SurferControl;
}

class SurferControl : public QWidget
{
    Q_OBJECT

public:
    explicit SurferControl(QWidget *parent = 0);
    ~SurferControl();

    QListWidgetItem *addSurface(const QString& sName);

signals:
    void loadNewSurface();
    void surfaceSelectionChanged(QListWidgetItem* pItem);
    void removeSurface(QListWidgetItem* pItem);

private slots:

    void on_loadNewSurfaceButton_released();
    void onSelectedSurfacesChanged(QListWidgetItem* pItem);
    void onCurrentSurfaceChanged(QListWidgetItem* pItem);

    void on_surfaceNameEdit_editingFinished();

    void on_removeSurfaceButton_released();

private:
    void listAboutToRunEmpty();

    Ui::SurferControl *ui;
};

#endif // SURFERCONTROL_H
