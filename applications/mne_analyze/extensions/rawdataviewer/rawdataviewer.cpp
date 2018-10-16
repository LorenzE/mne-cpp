//=============================================================================================================
/**
* @file     rawdataviewer.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     October, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018 Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Definition of the RawDataViewer class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "rawdataviewer.h"
#include "FormFiles/rawdataviewercontrol.h"
#include <anShared/Management/analyzedata.h>
#include "anShared/Utils/metatypes.h"
#include "channelviewer.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace RAWDATAVIEWEREXTENSION;
using namespace ANSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

RawDataViewer::RawDataViewer()
    : m_pControlDock(Q_NULLPTR)
    , m_pRawDataViewerControl(Q_NULLPTR)
    , m_bDisplayCreated(false)
{

}


//*************************************************************************************************************

RawDataViewer::~RawDataViewer()
{
    delete m_pRawDataViewerControl;
}


//*************************************************************************************************************

QSharedPointer<IExtension> RawDataViewer::clone() const
{
    QSharedPointer<RawDataViewer> pRawDataViewerClone = QSharedPointer<RawDataViewer>::create();
    return pRawDataViewerClone;
}


//*************************************************************************************************************

void RawDataViewer::init()
{
    m_pRawDataViewerControl = new RawDataViewerControl;

}


//*************************************************************************************************************

void RawDataViewer::unload()
{
}


//*************************************************************************************************************

QString RawDataViewer::getName() const
{
    return "RawDataViewer";
}


//*************************************************************************************************************

QMenu *RawDataViewer::getMenu()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

QDockWidget *RawDataViewer::getControl()
{
    if(!m_pControlDock) {
        m_pControlDock = new QDockWidget(tr("Raw Data View Control"));
        m_pControlDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        m_pControlDock->setWidget(m_pRawDataViewerControl);
    }

    return m_pControlDock;
}


//*************************************************************************************************************

QWidget *RawDataViewer::getView()
{
    if (m_bDisplayCreated == false)
    {
        createDisplay();
    }

    return m_pSubWindow;
}


//*************************************************************************************************************

void RawDataViewer::handleEvent(QSharedPointer<Event> e)
{
    qDebug() << "[RawDataViewer::handleEvent] received an Event that is not handled!";
}


//*************************************************************************************************************

QVector<EVENT_TYPE> RawDataViewer::getEventSubscriptions(void) const
{
    QVector<EVENT_TYPE> temp = {EXTENSION_INIT_FINISHED};
    return temp;
}


//*************************************************************************************************************

void RawDataViewer::createDisplay()
{

    m_pChannelDisplay = new ChannelViewer();
    m_pChannelDisplay->setMinimumSize(256, 256);
    m_pChannelDisplay->setFocusPolicy(Qt::TabFocus);
    m_pChannelDisplay->setAttribute(Qt::WA_DeleteOnClose, false);


    // we need this since the top-level main window runs "QMdiView::addSubWindow()", which requires a subwindow
    // to be passed (if a non-window would be passed, QMdiView would silently create a new QMidSubWindow )
    m_pSubWindow = new QMdiSubWindow();
    m_pSubWindow->setWidget(m_pChannelDisplay);
    m_pSubWindow->setWindowTitle(QString("Channel Display"));
    m_pSubWindow->setAttribute(Qt::WA_DeleteOnClose, false);

    // let Qt know that we want the MainViewer maximized
    //m_pSubWindow->showMaximized();

    // remember that the display was built
    m_bDisplayCreated = true;

}


//*************************************************************************************************************
