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


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


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
      m_pSurfaceMesh(new CustomMesh)
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
    QEntity *pRootEntity = createEntityTree();
    pWindow->setRootEntity(pRootEntity);

    //create lights
    QEntity *pLightRoot = createLightEntity();
    pLightRoot->setParent(pRootEntity);

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

QEntity* CentralView::createEntityTree()
{
    QEntity *pRootEntity = new QEntity();

    //Create mesh entity
    QEntity *pMeshEntity = new QEntity(pRootEntity);
    pMeshEntity->addComponent(m_pSurfaceMesh);

    Qt3DExtras::QPerVertexColorMaterial *pMaterial = new Qt3DExtras::QPerVertexColorMaterial();
    pMeshEntity->addComponent(pMaterial);

    Qt3DCore::QTransform *pTransform = new Qt3DCore::QTransform();
    pMeshEntity->addComponent(pTransform);

    //Add object picker
    QObjectPicker *pObjectPicker = new QObjectPicker;
    pObjectPicker->setDragEnabled(false);
    pObjectPicker->setHoverEnabled(false);
    connect(pObjectPicker, &QObjectPicker::pressed,
            this, &CentralView::userClicked);
    pMeshEntity->addComponent(pObjectPicker);

    //Create click point
    QEntity *pSphereEntity = new QEntity(pRootEntity);
    m_pointMesh = new Qt3DExtras::QSphereMesh;
    m_pointMesh->setRadius(0.001f);
    pSphereTransform = new Qt3DCore::QTransform();
    pSphereEntity->addComponent(m_pointMesh);
    pSphereEntity->addComponent(pSphereTransform);
    Qt3DExtras::QPhongMaterial *pSphereMat = new Qt3DExtras::QPhongMaterial;
    pSphereMat->setAmbient(Qt::red);
    pSphereEntity->addComponent(pSphereMat);

    return pRootEntity;
}


//*************************************************************************************************************

QEntity* CentralView::createLightEntity()
{
    QEntity *pLightRoot = new QEntity();
    //Setup light positions, intensities and color
    QList<QVector3D> lLightPositions;
    const QColor lightColor(255,255,255);
    const float lightIntensity = 0.2f;

    lLightPositions << QVector3D(-0.5,0,0) << QVector3D(0,0,-0.5)
                    << QVector3D(0.5,0,0) << QVector3D(0,0,0.5)
                    << QVector3D(0,0.5,0) << QVector3D(0,-0.5,0);

    //Create all the lights - make it shine
    for(int i = 0; i < lLightPositions.size(); ++i) {
        //Light source
        Qt3DCore::QEntity* pLightEntity = new Qt3DCore::QEntity(pLightRoot);

        Qt3DCore::QTransform* pTransform = new Qt3DCore::QTransform();
        pTransform->setTranslation(lLightPositions.at(i));
        pLightEntity->addComponent(pTransform);

        Qt3DRender::QPointLight *pPointLight = new Qt3DRender::QPointLight(pLightEntity);
        pPointLight->setColor(lightColor);
        pPointLight->setIntensity(lightIntensity);
        pLightEntity->addComponent(pPointLight);
    }
    return pLightRoot;
}


//*************************************************************************************************************

void CentralView::userClicked(QPickEvent *event)
{

}
