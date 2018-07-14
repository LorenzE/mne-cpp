#include "surfercontrol.h"
#include "ui_surfercontrol.h"

#include <QDebug>
#include <QListWidgetItem>

SurferControl::SurferControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SurferControl)
{
    ui->setupUi(this);
    // necessary connects
    connect(ui->loadedSurfaces,
            &QListWidget::itemChanged,
            this,
            &SurferControl::onSelectedSurfacesChanged);
    connect(ui->loadedSurfaces,
            &QListWidget::currentItemChanged,
            this,
            &SurferControl::onCurrentSurfaceChanged);
}

SurferControl::~SurferControl()
{
    delete ui;
}

void SurferControl::on_loadNewSurfaceButton_released()
{
    // tell surfer extension to open the user dialog
    emit loadNewSurface();
}

QListWidgetItem* SurferControl::addSurface(const QString &sName)
{
    QListWidgetItem* pItem = new QListWidgetItem;
    pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);
    pItem->setCheckState(Qt::Checked);
    pItem->setText(sName);
    ui->loadedSurfaces->addItem(pItem);
    return pItem;
}

void SurferControl::onSelectedSurfacesChanged(QListWidgetItem* pItem)
{
    // tell surfer extension that user changed visibility of surface
    emit surfaceSelectionChanged(pItem);
}

void SurferControl::onCurrentSurfaceChanged(QListWidgetItem* pItem)
{
    if (pItem) {
        ui->surfaceNameEdit->setText(pItem->text());
    }
}

void SurferControl::on_surfaceNameEdit_editingFinished()
{
    QString sNewText = ui->surfaceNameEdit->text();
    QListWidgetItem* pItem = ui->loadedSurfaces->currentItem();
    if (pItem) {
        pItem->setText(sNewText);
    }
}

void SurferControl::on_removeSurfaceButton_released()
{
    QListWidgetItem* pCurrentItem = ui->loadedSurfaces->currentItem();
    if (pCurrentItem) {
        // check if this is the last item in the list
        if (ui->loadedSurfaces->count() == 1) {
            // list about to rum empty
            listAboutToRunEmpty();
        }

        // takeItem will remove it from the list. Qt docu says that the removed item must
        // be deleted manually, but since the Surfer extension still needs it for the map access,
        // we let the Surfer extension delete it when its done.
        emit removeSurface(ui->loadedSurfaces->takeItem(ui->loadedSurfaces->row(pCurrentItem)));
    }
}

void SurferControl::listAboutToRunEmpty() {
    ui->surfaceNameEdit->setText(QString(""));
}
