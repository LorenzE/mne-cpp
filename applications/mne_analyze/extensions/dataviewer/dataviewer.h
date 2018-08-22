//=============================================================================================================
/**
* @file     dataviewer.h
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
* @brief    Contains the declaration of the DataViewer class.
*
*/

#ifndef DATAVIEWER_H
#define DATAVIEWER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dataviewer_global.h"
#include <anShared/Interfaces/IExtension.h>
#include <anShared/Management/communicator.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtWidgets>
#include <QtCore/QtPlugin>
#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class DataViewerControl;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE SURFEREXTENSION
//=============================================================================================================

namespace DATAVIEWEREXTENSION
{


//=============================================================================================================
/**
* DataViewer Extension
*
* @brief The DataViewer class provides a view with all currently loaded models.
*/
class DATAVIEWERSHARED_EXPORT DataViewer : public ANSHAREDLIB::IExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ansharedlib/1.0" FILE "dataviewer.json") //New Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
    // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
    Q_INTERFACES(ANSHAREDLIB::IExtension)

public:
    //=========================================================================================================
    /**
    * Constructs a DataViewer.
    */
    DataViewer();

    //=========================================================================================================
    /**
    * Destroys the DataViewer.
    */
    virtual ~DataViewer();

    // IExtension functions
    virtual QSharedPointer<IExtension> clone() const override;
    virtual void init() override;
    virtual void unload() override;
    virtual QString getName() const override;
    virtual QMenu* getMenu() override;
    virtual QDockWidget* getControl() override;
    virtual QWidget* getView() override;
    virtual void handleEvent(QSharedPointer<ANSHAREDLIB::Event> e) override;
    virtual QVector<ANSHAREDLIB::EVENT_TYPE> getEventSubscriptions() const override;

private:

    //=========================================================================================================
    /**
    * Updated the content of the list widget.
    */
    void updateListWidget();

    // Control
    QDockWidget*        m_pControl; /**< Control Widget */
    DataViewerControl*  m_pDataViewerControl;
};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // NAMESPACE

#endif // DATAVIEWER_H
