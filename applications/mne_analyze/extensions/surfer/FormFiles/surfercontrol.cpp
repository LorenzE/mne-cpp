//=============================================================================================================
/**
* @file     surfercontrol.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief    SurferControl class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "surfercontrol.h"
#include "ui_surfercontrol.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDebug>
#include <QListWidgetItem>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

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


//*************************************************************************************************************

SurferControl::~SurferControl()
{
    delete ui;
}


//*************************************************************************************************************

QListWidgetItem* SurferControl::addSurface(const QString &sName)
{
    QListWidgetItem* pItem = new QListWidgetItem;
    pItem->setFlags(pItem->flags() | Qt::ItemIsUserCheckable);
    pItem->setCheckState(Qt::Checked);
    pItem->setText(sName);
    ui->loadedSurfaces->addItem(pItem);
    return pItem;
}


//*************************************************************************************************************

void SurferControl::onSelectedSurfacesChanged(QListWidgetItem* pItem)
{
    // tell surfer extension that user changed visibility of surface
    emit surfaceSelectionChanged(pItem);
}


//*************************************************************************************************************

void SurferControl::onCurrentSurfaceChanged(QListWidgetItem* pItem)
{
    if (pItem) {
        ui->surfaceNameEdit->setText(pItem->text());
    }
}


//*************************************************************************************************************

void SurferControl::on_loadNewSurfaceButton_released()
{
    // tell surfer extension to open the user dialog
    emit loadNewSurface();
}


//*************************************************************************************************************

void SurferControl::on_surfaceNameEdit_editingFinished()
{
    QString sNewText = ui->surfaceNameEdit->text();
    QListWidgetItem* pItem = ui->loadedSurfaces->currentItem();
    if (pItem) {
        pItem->setText(sNewText);
    }
}


//*************************************************************************************************************

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


//*************************************************************************************************************

void SurferControl::listAboutToRunEmpty() {
    ui->surfaceNameEdit->setText(QString(""));
}
