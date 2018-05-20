//=============================================================================================================
/**
* @file     centralview.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    CentralView class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "centralview.h"
#include <disp3D/engine/model/3dhelpers/custommesh.h>
#include <mne/mne_bem.h>
#include <mne/mne_bem_surface.h>
#include <iostream>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DRender>
#include <Qt3DExtras>
#include <QPickingSettings>
#include <QCamera>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MAINVIEWEREXTENSION;
using namespace DISP3DLIB;
using namespace Qt3DRender;
using namespace Qt3DCore;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

CentralView::CentralView()
    : QWidget(),
      m_view3d_container(nullptr),
      m_view3d_gridlayout(nullptr),
      m_pRootEntity(new QEntity)
{
    init();
}


//*************************************************************************************************************

void CentralView::init()
{
    //Resizing the created QMdiSubwindow to fit the minimun size of the container used later
    this->resize(256,256);
    //QGridLayout is used so the container constantly resizes to the size of the QMdiSubwindow
    m_view3d_gridlayout = new QGridLayout(this);

    Qt3DExtras::Qt3DWindow *pWindow = new Qt3DExtras::Qt3DWindow();
    pWindow->setRootEntity(m_pRootEntity);

    QPickingSettings *pPickSettings = pWindow->renderSettings()->pickingSettings();
    pPickSettings->setFaceOrientationPickingMode(QPickingSettings::FrontAndBackFace);
    pPickSettings->setPickMethod(QPickingSettings::TrianglePicking);
    pPickSettings->setPickResultMode(QPickingSettings::NearestPick);

    QCamera *pCamera = pWindow->camera();
    pCamera->setPosition(QVector3D(0,0,1));

    Qt3DExtras::QFirstPersonCameraController *pCamController = new Qt3DExtras::QFirstPersonCameraController(m_pRootEntity);
    pCamController->setCamera(pCamera);

    //A container is created to contain the QWindow that comes from BrainView, then a minimum size is set
    m_view3d_container = QWidget::createWindowContainer(pWindow);
    m_view3d_container->setMinimumSize(256,256);
    //m_view3d_container->setMaximumSize(256,256);
    m_view3d_container->setFocusPolicy(Qt::TabFocus);
    //The loaded surfaces, as a QWindow is added to the created container
    m_view3d_gridlayout->addWidget(m_view3d_container);
}

void CentralView::addEntity(QSharedPointer<QEntity> pEntity)
{
    pEntity->setParent(m_pRootEntity);
}
