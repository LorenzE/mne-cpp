//=============================================================================================================
/**
* @file     surfercontrol.h
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
* @brief    SurferControl class declaration
*
*/

#ifndef SURFERCONTROL_H
#define SURFERCONTROL_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QWidget>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class QListWidgetItem;

namespace Ui {
    class SurferControl;
}


//=============================================================================================================
/**
* @brief The SurferControl class enables graphical user interaction.
*/
class SurferControl : public QWidget
{
    Q_OBJECT

public:
    //=========================================================================================================
    /**
    * Constructs a SurferControl
    */
    explicit SurferControl(QWidget *parent = 0);

    //=========================================================================================================
    /**
    * Destructs a SurferControl
    */
    ~SurferControl();

    //=========================================================================================================
    /**
    * This gets called by the Surfer extension whenever a new surface was loaded and should be added the UI.
    *
    * @param[in] sName The name of the surface that should be displayed in the view.
    *
    * @return A pointer to a newly created QListWidgetItem.
    */
    QListWidgetItem *addSurface(const QString& sName);

signals:
    //=========================================================================================================
    /**
    * This signal tells the Surfer extension to open a user dialog in order to load a new surface file
    */
    void loadNewSurface();

    //=========================================================================================================
    /**
    * This notifies the Surfer extension that visibility was toggled for a surface.
    *
    * @param pItem The corresponding item in the list view
    */
    void surfaceSelectionChanged(const QListWidgetItem* pItem);

    //=========================================================================================================
    /**
    * This notifies the Surfer extension that a surface should be unloaded.
    *
    * @param pItem The corresponing item in the list view
    */
    void removeSurface(QListWidgetItem* pItem);

private slots:

    //=========================================================================================================
    /**
    * This is used for the custom interaction concerning visibility toggling
    *
    * @param pItem The item that was toggled
    */
    void onSelectedSurfacesChanged(QListWidgetItem* pItem);

    //=========================================================================================================
    /**
    * This is used for the custom interaction concerning the currently highlighted item / surface
    *
    * @param pItem The item that was clicked (not toggled) / highlighted
    */
    void onCurrentSurfaceChanged(QListWidgetItem* pItem);

    // auto-generated:
    void on_loadNewSurfaceButton_released();

    void on_surfaceNameEdit_editingFinished();

    void on_removeSurfaceButton_released();

private:
    //=========================================================================================================
    /**
    * This is a helper function that holds all the operations necessary to avoid problems when the list runs empty.
    */
    void listAboutToRunEmpty();

    Ui::SurferControl *ui;
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

#endif // SURFERCONTROL_H
