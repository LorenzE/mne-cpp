//=============================================================================================================
/**
* @file     view3dsurfer.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     August, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief    View3DSurfer class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "view3dsurfer.h"
#include <disp3D/engine/model/3dhelpers/custommesh.h>
#include <mne/mne_bem.h>
#include <mne/mne_bem_surface.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include <QGridLayout>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace SURFEREXTENSION;
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

View3DSurfer::View3DSurfer()
: QWidget()
, m_view3d_container(nullptr)
, m_view3d_gridlayout(nullptr)
{
    init();
}


//*************************************************************************************************************

void View3DSurfer::init()
{
    //Resizing the created QMdiSubwindow to fit the minimun size of the container used later
    this->resize(256,256);
    //QGridLayout is used so the container constantly resizes to the size of the QMdiSubwindow
    m_view3d_gridlayout = new QGridLayout(this);


    Qt3DExtras::Qt3DWindow *pWindow = new Qt3DExtras::Qt3DWindow();
    QEntity *pRootEntity = createEntityTree();
    pWindow->setRootEntity(pRootEntity);

    QPickingSettings *pPickSettings = pWindow->renderSettings()->pickingSettings();
    pPickSettings->setFaceOrientationPickingMode(QPickingSettings::FrontAndBackFace);
    pPickSettings->setPickMethod(QPickingSettings::TrianglePicking);
    pPickSettings->setPickResultMode(QPickingSettings::NearestPick);


    QCamera *pCamera = pWindow->camera();
    pCamera->setPosition(QVector3D(0,0,1));


    Qt3DExtras::QFirstPersonCameraController *pCamController = new Qt3DExtras::QFirstPersonCameraController(pRootEntity);
    pCamController->setCamera(pCamera);



    //A container is created to contain the QWindow that comes from BrainView, then a minimum size is set
    m_view3d_container = QWidget::createWindowContainer(pWindow);
    m_view3d_container->setMinimumSize(256,256);
    //m_view3d_container->setMaximumSize(256,256);
    m_view3d_container->setFocusPolicy(Qt::TabFocus);
    //The loaded surfaces, as a QWindow is added to the created container
    m_view3d_gridlayout->addWidget(m_view3d_container);
}


//*************************************************************************************************************

QEntity *View3DSurfer::createEntityTree()
{
    QEntity *pRootEntity = new QEntity();

    //Create mesh entity
    QEntity *pMeshEntity = new QEntity(pRootEntity);
    pMeshEntity->addComponent(createMesh());

    Qt3DExtras::QPhongMaterial *pMaterial = new Qt3DExtras::QPhongMaterial();
    pMeshEntity->addComponent(pMaterial);

    Qt3DCore::QTransform *pTransform = new Qt3DCore::QTransform();
    pTransform->setRotationX(270);
    pTransform->setRotationY(180);
    pMeshEntity->addComponent(pTransform);

    //Add object picker
    QObjectPicker *pObjectPicker = new QObjectPicker;
    pObjectPicker->setDragEnabled(false);
    pObjectPicker->setHoverEnabled(false);
    connect(pObjectPicker, &QObjectPicker::pressed,
            this, &View3DSurfer::testPicking);
    pMeshEntity->addComponent(pObjectPicker);

    qDebug() << "connected";

    return pRootEntity;

}


//*************************************************************************************************************

Qt3DRender::QGeometryRenderer *View3DSurfer::createMesh()
{
    //acquire surface data
    QFile t_filesensorSurfaceVV("./MNE-sample-data/subjects/sample/bem/sample-head.fif");
    MNELIB::MNEBem t_sensorSurfaceVV(t_filesensorSurfaceVV);
    m_surface = t_sensorSurfaceVV[0];

    //Create Mesh
    CustomMesh *mesh = new CustomMesh();
    mesh->setVertex(m_surface.rr);
    mesh->setNormals(m_surface.nn);
    mesh->setIndex(m_surface.tris);

    return mesh;

}

void View3DSurfer::testPicking(QPickEvent *event)
{
    if(QPickTriangleEvent* tri = dynamic_cast<QPickTriangleEvent*>(event)) {
        qDebug() << "picked triangle index: " << tri->triangleIndex();
        qDebug() << tri->vertex1Index() << " " << tri->vertex2Index() << " " << tri->vertex3Index() << " ";
        qDebug() << m_surface.tris(tri->triangleIndex(), 0);
        qDebug() << "local intersection " << tri->localIntersection();

    }
    else {
        qDebug() << "failed";
    }

}


//*************************************************************************************************************
