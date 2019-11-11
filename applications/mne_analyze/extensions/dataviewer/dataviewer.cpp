//=============================================================================================================
/**
* @file     dataviewer.cpp
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
* @brief    Definition of the DataViewer class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dataviewer.h"
#include "FormFiles/dataviewercontrol.h"
#include <anShared/Management/analyzedata.h>
#include "anShared/Utils/metatypes.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QListWidgetItem>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace DATAVIEWEREXTENSION;
using namespace ANSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

DataViewer::DataViewer()
    : m_pControlDock(Q_NULLPTR)
    , m_pDataViewerControl(Q_NULLPTR)
{

}


//*************************************************************************************************************

DataViewer::~DataViewer()
{
    delete m_pDataViewerControl;
}


//*************************************************************************************************************

QSharedPointer<IExtension> DataViewer::clone() const
{
    QSharedPointer<DataViewer> pDataViewerClone = QSharedPointer<DataViewer>::create();
    return pDataViewerClone;
}


//*************************************************************************************************************

void DataViewer::init()
{
    m_pDataViewerControl = new DataViewerControl;

    updateListWidget();

    connect(m_analyzeData.data(), &AnalyzeData::newModelAvailable,
            this, &DataViewer::updateListWidget);
    connect(m_analyzeData.data(), &AnalyzeData::modelPathChanged,
            this, &DataViewer::updateListWidget);
    connect(m_analyzeData.data(), &AnalyzeData::modelRemoved,
            this, &DataViewer::updateListWidget);

}


//*************************************************************************************************************

void DataViewer::unload()
{
}


//*************************************************************************************************************

QString DataViewer::getName() const
{
    return "DataViewer";
}


//*************************************************************************************************************

QMenu *DataViewer::getMenu()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

QDockWidget *DataViewer::getControl()
{
    if(!m_pControlDock) {
        m_pControlDock = new QDockWidget(tr("Data View Control"));
        m_pControlDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        m_pControlDock->setWidget(m_pDataViewerControl);
    }

    return m_pControlDock;
}


//*************************************************************************************************************

QWidget *DataViewer::getView()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

void DataViewer::handleEvent(QSharedPointer<Event> e)
{
    qDebug() << "[DataViewer::handleEvent] received an Event that is not handled by switch-cases";
}


//*************************************************************************************************************

QVector<EVENT_TYPE> DataViewer::getEventSubscriptions(void) const
{
    QVector<EVENT_TYPE> temp;
    temp.push_back(EXTENSION_INIT_FINISHED);
    return temp;
}


//*************************************************************************************************************

void DataViewer::updateListWidget()
{
    m_pDataViewerControl->clearList();

    QList<QSharedPointer<AbstractModel>> lModels = m_analyzeData->getModels();

    //add all model names to the listView
    for(QSharedPointer<AbstractModel> pModel: lModels) {

        if(pModel->getType() == MODEL_TYPE::ANSHAREDLIB_QENTITYLIST_MODEL) {
            continue;
        }

        QListWidgetItem* tempListItem = new QListWidgetItem;
        tempListItem->setText(pModel->getModelName());
        tempListItem->setToolTip(pModel->getModelPath());
        m_pDataViewerControl->addListItem(tempListItem);
    }
}


//*************************************************************************************************************
