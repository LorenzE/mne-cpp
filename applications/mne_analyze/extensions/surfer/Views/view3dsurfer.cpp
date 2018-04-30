//=============================================================================================================
/**
* @file     view3dsurfer.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     August, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
#include <anShared/Model/surfacemodel.h>
#include <anShared/Utils/types.h>

#include <cmath>
#include <limits>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include <QGridLayout>
#include <QAbstractItemModel>


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
, m_pSurfaceMesh(new CustomMesh)
{
    init();
}


//*************************************************************************************************************

void View3DSurfer::setModel(QAbstractItemModel *pModel)
{
    m_pItemModel = pModel;

    updateSurfaceModelMesh();

}


//*************************************************************************************************************

void View3DSurfer::setSelectionModel(QItemSelectionModel *pSelectionModel)
{
    //TODO implement this
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

QEntity *View3DSurfer::createEntityTree()
{
    QEntity *pRootEntity = new QEntity();

    //Create mesh entity
    QEntity *pMeshEntity = new QEntity(pRootEntity);
//    Qt3DExtras::QSphereMesh *customMesh = new Qt3DExtras::QSphereMesh;
//    pMeshEntity->addComponent(customMesh);



    pMeshEntity->addComponent(m_pSurfaceMesh);

    Qt3DExtras::QPerVertexColorMaterial *pMaterial = new Qt3DExtras::QPerVertexColorMaterial();
    pMeshEntity->addComponent(pMaterial);

//    Qt3DExtras::QPhongMaterial *pMaterial = new Qt3DExtras::QPhongMaterial;
//    pMaterial->setAmbient(Qt::blue);
//    pMeshEntity->addComponent(pMaterial);

    Qt3DCore::QTransform *pTransform = new Qt3DCore::QTransform();
    //pTransform->setRotationX(270);
    //pTransform->setRotationY(180);
    pMeshEntity->addComponent(pTransform);

    //Add object picker
    QObjectPicker *pObjectPicker = new QObjectPicker;
    pObjectPicker->setDragEnabled(false);
    pObjectPicker->setHoverEnabled(false);
    connect(pObjectPicker, &QObjectPicker::pressed,
            this, &View3DSurfer::testPicking);
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

//Qt3DRender::QGeometryRenderer *View3DSurfer::createMesh()
//{
//    //acquire surface data
//    QFile t_filesensorSurfaceVV("./MNE-sample-data/subjects/sample/bem/sample-5120-5120-5120-bem.fif");
//    MNELIB::MNEBem t_sensorSurfaceVV(t_filesensorSurfaceVV);
//    m_surface = t_sensorSurfaceVV[0];

//    //Create Mesh
//    m_pSurfaceMesh = new CustomMesh();
//    m_pSurfaceMesh->setVertex(m_surface.rr);
//    m_pSurfaceMesh->setNormals(m_surface.nn);
//    m_pSurfaceMesh->setIndex(m_surface.tris);
//    Eigen::MatrixX3f colors;
//    colors.resize(m_surface.rr.rows(), 3);
//    colors.setZero(m_surface.rr.rows(), 3);
//    m_pSurfaceMesh->setColor(colors);


//    return m_pSurfaceMesh;

//}


//*************************************************************************************************************

void View3DSurfer::updateSurfaceModelMesh()
{
    if(dynamic_cast<ANSHAREDLIB::SurfaceModel*>(m_pItemModel.data()) == nullptr) {
        return;
    }

    if(m_pSurfaceMesh == nullptr) {
        m_pSurfaceMesh = new CustomMesh;
    }

    //TODO refactor the initialization  template with Matrix<T,Dynamic,Dynamic> ?
    // Vertices
    QModelIndex dataIndex = m_pItemModel->index(0, 0, QModelIndex());
    Eigen::MatrixX3f vertices(m_pItemModel->columnCount(dataIndex), 3);

    for(int i = 0; i < m_pItemModel->columnCount(dataIndex); ++i) {
        dataIndex = m_pItemModel->index(0, i, QModelIndex());
        Eigen::Vector3f tempVec = m_pItemModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3f>();
        vertices(i, 0) = tempVec[0];
        vertices(i, 1) = tempVec[1];
        vertices(i, 2) = tempVec[2];
    }
    m_pSurfaceMesh->setVertex(vertices);

    // Normals
    dataIndex = m_pItemModel->index(1, 0, QModelIndex());
    Eigen::MatrixX3f normals(m_pItemModel->columnCount(dataIndex), 3);

    for(int i = 0; i < m_pItemModel->columnCount(dataIndex); ++i) {
        dataIndex = m_pItemModel->index(1, i, QModelIndex());
        Eigen::Vector3f tempVec = m_pItemModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3f>();
        normals(i, 0) = tempVec[0];
        normals(i, 1) = tempVec[1];
        normals(i, 2) = tempVec[2];
    }
    m_pSurfaceMesh->setNormals(normals);

    // Tris
    dataIndex = m_pItemModel->index(2, 0, QModelIndex());
    Eigen::MatrixX3i tris(m_pItemModel->columnCount(dataIndex), 3);

    for(int i = 0; i < m_pItemModel->columnCount(dataIndex); ++i) {
        dataIndex = m_pItemModel->index(2, i, QModelIndex());
        Eigen::Vector3i tempVec = m_pItemModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3i>();
        tris(i, 0) = tempVec[0];
        tris(i, 1) = tempVec[1];
        tris(i, 2) = tempVec[2];
    }
    m_pSurfaceMesh->setIndex(tris);

    // Colors
    Eigen::MatrixX3f colors;
    int rows = m_pItemModel->columnCount(m_pItemModel->index(0, 0, QModelIndex()));
    colors.resize(rows, 3);
    colors.setZero(rows, 3);
    m_pSurfaceMesh->setColor(colors);
}


//*************************************************************************************************************

QEntity *View3DSurfer::createLightEntity()
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

        //m_lLightSources.append(pPointLight);

        //Uncomment the following to visualize the light sources for debugging:
//        Qt3DExtras::QSphereMesh* lightSphere = new Qt3DExtras::QSphereMesh(pLightEntity);
//        lightSphere->setRadius(0.1f);
//        pLightEntity->addComponent(lightSphere);
//        Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial(pLightEntity);
//        material->setAmbient(lightColor);
//        pLightEntity->addComponent(material);
    }
    return pLightRoot;
}


//*************************************************************************************************************

void View3DSurfer::testPicking(QPickEvent *event)
{
    if(QPickTriangleEvent* tri = dynamic_cast<QPickTriangleEvent*>(event)) {
        qDebug() << "picked triangle index: " << tri->triangleIndex();
        qDebug() << tri->vertex1Index() << " " << tri->vertex2Index() << " " << tri->vertex3Index() << " ";
        //qDebug() << m_surface.tris(tri->triangleIndex(), 0);
        qDebug() << "local intersection " << tri->localIntersection();

        //calculate the nearest triangle vertex to the local intersection
//        Q_ASSERT(m_surface.tris.cols() == 3);
//        float fMinDistance = std::numeric_limits<float>::max();
//        int iMinVertIdx = tri->vertex1Index();
//        for(int i = 0; i < m_surface.tris.cols(); ++i) {
//            int iVertIdx = m_surface.tris(tri->triangleIndex(), i);
//            float fDistance = std::sqrt(squared(m_surface.rr(iVertIdx, 0) - tri->localIntersection().x()) +
//                                        squared(m_surface.rr(iVertIdx, 1) - tri->localIntersection().y()) +
//                                        squared(m_surface.rr(iVertIdx, 2) - tri->localIntersection().z()));
//            qDebug() << "Distance " << i << " : " << fDistance;
//            if(fDistance < fMinDistance) {
//                fMinDistance = fDistance;
//                iMinVertIdx = iVertIdx;
//            }
//        }

//        //Color the clicked vertex red
//        Eigen::MatrixX3f colors;
//        colors.resize(m_surface.rr.rows(), 3);
//        //colors.setOnes(m_surface.rr.rows(), 3);
//        for(int i = 0; i < colors.rows(); ++i) {
//            colors(i, 0) = 0.2f;
//            colors(i, 1) = 0.2f;
//            colors(i, 2) = 0.2f;
//        }
//        colors(iMinVertIdx, 0) = 1.0f;
//        colors(iMinVertIdx, 1) = 0.0f;
//        colors(iMinVertIdx, 2) = 0.0f;

//        m_mesh->setColor(colors);

//        pSphereTransform->setTranslation(tri->worldIntersection());

    }
    else {
        qDebug() << "failed";
    }

}


//*************************************************************************************************************
