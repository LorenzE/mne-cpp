//=============================================================================================================
/**
* @file     dataviewercontrol.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     August, 2018
*
* @section  LICENSE
*
* Copyright (C) 2017 Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the declaration of the DataViewerControl class.
*
*/

#ifndef DATAVIEWERCONTROL_H
#define DATAVIEWERCONTROL_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <QWidget>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Ui {
    class DataViewerControl;
}

class QListWidgetItem;


//=============================================================================================================
/**
* DataViewerControl Extension Control
*
* @brief The MusicControl class provides the extension control.
*/
class DataViewerControl : public QWidget
{
    Q_OBJECT

public:
    //=========================================================================================================
    /**
    * Constructs the DataViewerControl
    *
    * @param[in] parent     If parent is not NULL the QWidget becomes a child of QWidget inside parent.
    */
    explicit DataViewerControl(QWidget *parent = 0);

    //=========================================================================================================
    /**
    * Destroys the DataViewerControl.
    */
    virtual ~DataViewerControl();

    //=========================================================================================================
    /**
    * Removes all entries from the QListWidget.
    *
    * @param[in] pNewItem       The new item.
    */
    void addListItem(QListWidgetItem* pNewItem);

    //=========================================================================================================
    /**
    * Removes all entries from the QListWidget.
    */
    void clearList();

private:

    Ui::DataViewerControl *ui;   /**< The user interface */
};

#endif // DATAVIEWERCONTROL_H
