//=============================================================================================================
/**
* @file     surfer.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017 Christoph Dinh, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the implementation of the Surfer class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "surfer.h"
#include "FormFiles/surfercontrol.h"

#include <anShared/Model/surfacemodel.h>
#include <anShared/Data/surfacedata.h>
#include <anShared/Model/qentitylistmodel.h>
#include <anShared/Management/analyzedata.h>
#include <anShared/Utils/metatypes.h>

#include <disp3D/engine/model/3dhelpers/custommesh.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include <QEntity>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace SURFEREXTENSION;
using namespace ANSHAREDLIB;
using namespace DISP3DLIB;
using namespace ANSHAREDLIB;
using namespace Qt3DRender;
using namespace Qt3DCore;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

Surfer::Surfer()
: m_pCommu(Q_NULLPTR)
, m_pDock(Q_NULLPTR)
, m_pSurferControl(Q_NULLPTR)
, m_pSurferRoot(Q_NULLPTR)
, m_mLoadedSurfaces()
{

}


//*************************************************************************************************************

Surfer::~Surfer()
{
    delete m_pCommu;
    delete m_pSurferControl;
}


//*************************************************************************************************************

QSharedPointer<IExtension> Surfer::clone() const
{
    QSharedPointer<Surfer> pSurferClone = QSharedPointer<Surfer>::create();
    return pSurferClone;
}


//*************************************************************************************************************

void Surfer::init()
{
    // connect to event system, since we need to know when we can register our 3D stuff in a display view
    m_pCommu = new Communicator(this);

    // create entity tree root node and name it
    m_pSurferRoot = QSharedPointer<QEntity>::create();
    m_pSurferRoot->setObjectName(QStringLiteral("SurferEntityTree"));

    // add lighting
    QEntity *pLightRoot = new QEntity(m_pSurferRoot.data());
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

    // build control widget
    if (! m_pSurferControl) {
        m_pSurferControl = new SurferControl;
        connect(m_pSurferControl,
                &SurferControl::loadNewSurface,
                this,
                &Surfer::onLoadNewSurface);
        connect(m_pSurferControl,
                &SurferControl::surfaceSelectionChanged,
                this,
                &Surfer::onSurfaceSelectionChanged);
        connect(m_pSurferControl,
                &SurferControl::removeSurface,
                this,
                &Surfer::onRemoveSurface);
    }

    // load initially displayed surfaces:
    QString lh(QDir::currentPath() + "/MNE-sample-data/subjects/sample/surf/lh.pial");
    QString rh(QDir::currentPath() + "/MNE-sample-data/subjects/sample/surf/rh.pial");

    helpLoadNewSurface(lh);
    helpLoadNewSurface(rh);
}


//*************************************************************************************************************

void Surfer::unload()
{
}


//*************************************************************************************************************

QString Surfer::getName() const
{
    return "Surfer";
}


//*************************************************************************************************************

QMenu *Surfer::getMenu()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

QDockWidget *Surfer::getControl()
{
    if(!m_pDock) {
        m_pDock = new QDockWidget(tr("Surfer Control"));
        m_pDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        m_pDock->setMinimumWidth(180);
        if (m_pSurferControl) {
            m_pDock->setWidget(m_pSurferControl);
        }
    }

    return m_pDock;
}


//*************************************************************************************************************

QWidget *Surfer::getView()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

void Surfer::handleEvent(QSharedPointer<Event> e)
{
    switch(e->getType())
    {
    case EXTENSION_INIT_FINISHED:
    {
        QVector<QSharedPointer<QEntityListModel> > availableDisplays = m_analyzeData->availableDisplays();
        if (availableDisplays.size() >= 1) {
            availableDisplays.at(0)->addEntityTree(m_pSurferRoot);
            qDebug() << "[Surfer] successfully registered Surfer root";
        }
        else {
            qDebug() << "[Surfer] Could not find any displays...";
        }
        break;
    }
    default:
        qDebug() << "[Surfer] Received an Event that is not handled by switch-cases";
        break;
    }
}


//*************************************************************************************************************

QVector<EVENT_TYPE> Surfer::getEventSubscriptions(void) const
{
    QVector<EVENT_TYPE> temp;
    temp.push_back(EXTENSION_INIT_FINISHED);
    return temp;
}


//*************************************************************************************************************

void Surfer::onLoadNewSurface()
{
    QString filePath = QFileDialog::getOpenFileName(m_pDock,
                                                    tr("Open Surface File"),
                                                    QDir::currentPath() + "/MNE-sample-data");
    if (filePath.isEmpty() == false) {
        helpLoadNewSurface(filePath);
    }
    else {
        qDebug() << "[Surfer] Empty filepath, returning...";
        return;
    }
}


//*************************************************************************************************************

void Surfer::onSurfaceSelectionChanged(const QListWidgetItem *pItem)
{
    // reestablish consistency of visibility
    QSharedPointer<QEntity> pEntity = m_mLoadedSurfaces.value(pItem).second;
    if (pItem->checkState() == Qt::Checked) {
        if (pEntity->isEnabled() == false) {
            pEntity->setEnabled(true);
        }
    }
    else {
        if (pEntity->isEnabled()) {
            pEntity->setEnabled(false);
        }
    }
}


//*************************************************************************************************************

void Surfer::onRemoveSurface(QListWidgetItem *pItem)
{
    // we leave the model loaded in AnalyzeData, so we only have to care about the entity tree
    QSharedPointer<QEntity> pEntity = m_mLoadedSurfaces.value(pItem).second;
    // until we find a better solution, we use the same trick as in MainViewer/CentralView
    pEntity->setParent(new QEntity);    // setParent(nullptr) would break the Qt3D backend
    pEntity->parent()->deleteLater();

    // remove from record
    m_mLoadedSurfaces.remove(pItem);

    // Qt does not take care of deletion of items, do it manually.
    // This is not a problem since it was removed by calling takeItem
    delete pItem;
}


//*************************************************************************************************************

void Surfer::helpLoadNewSurface(const QString& filePath)
{
    // request model from AnalyzeData
    QSharedPointer<SurfaceModel> pModel = m_analyzeData->loadSurfaceModel(filePath);
    // prepare entity tree
    QSharedPointer<QEntity> pEntity = QSharedPointer<QEntity>::create();
    // name is technically superfluous, since surfer root is named
    pEntity->setObjectName(filePath);

    // add mesh entity
    QEntity *pMeshEntity = new QEntity(pEntity.data());
    CustomMesh* pMesh = new CustomMesh;
    pMeshEntity->addComponent(pMesh);

    Qt3DExtras::QPerVertexColorMaterial *pMaterial = new Qt3DExtras::QPerVertexColorMaterial();
    pMeshEntity->addComponent(pMaterial);

    Qt3DCore::QTransform *pTransform = new Qt3DCore::QTransform();
    pMeshEntity->addComponent(pTransform);

    //Add object picker
    QObjectPicker *pObjectPicker = new QObjectPicker;
    pObjectPicker->setDragEnabled(false);
    pObjectPicker->setHoverEnabled(false);
    connect(pObjectPicker,
            &QObjectPicker::pressed,
            this,
            &Surfer::onClick);
    pMeshEntity->addComponent(pObjectPicker);

    // fill mesh with data from model
    updateSurfaceModelMesh(pModel, pMesh);

    // tell UI about new surface and insert into record
    QListWidgetItem* pTempItem = m_pSurferControl->addSurface(filePath);
    m_mLoadedSurfaces.insert(pTempItem, LoadedSurface(pModel, pEntity));

    // add newly loaded surface to surfer entity tree
    pEntity->setParent(m_pSurferRoot.data());
}


//*************************************************************************************************************

void Surfer::updateSurfaceModelMesh(QSharedPointer<SurfaceModel> pModel, CustomMesh* pMesh)
{
    if (! pModel) {
        qDebug() << "[Surfer] updateSurfaceModelMesh: model is null";
        return;
    }

    if (! pMesh) {
        qDebug() << "[Surfer] updateSurfaceModelMesh: mesh is null";
        return;
    }

    //TODO refactor the initialization  template with Matrix<T,Dynamic,Dynamic> ?
    // Vertices
    QModelIndex dataIndex = pModel->index(0, 0, QModelIndex());
    Eigen::MatrixX3f vertices(pModel->columnCount(dataIndex), 3);

    for(int i = 0; i < pModel->columnCount(dataIndex); ++i) {
        dataIndex = pModel->index(0, i, QModelIndex());
        Eigen::Vector3f tempVec = pModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3f>();
        vertices(i, 0) = tempVec[0];
        vertices(i, 1) = tempVec[1];
        vertices(i, 2) = tempVec[2];
    }
    pMesh->setVertex(vertices);

    // Normals
    dataIndex = pModel->index(1, 0, QModelIndex());
    Eigen::MatrixX3f normals(pModel->columnCount(dataIndex), 3);

    for(int i = 0; i < pModel->columnCount(dataIndex); ++i) {
        dataIndex = pModel->index(1, i, QModelIndex());
        Eigen::Vector3f tempVec = pModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3f>();
        normals(i, 0) = tempVec[0];
        normals(i, 1) = tempVec[1];
        normals(i, 2) = tempVec[2];
    }
    pMesh->setNormals(normals);

    // Tris
    dataIndex = pModel->index(2, 0, QModelIndex());
    Eigen::MatrixX3i tris(pModel->columnCount(dataIndex), 3);

    for(int i = 0; i < pModel->columnCount(dataIndex); ++i) {
        dataIndex = pModel->index(2, i, QModelIndex());
        Eigen::Vector3i tempVec = pModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3i>();
        tris(i, 0) = tempVec[0];
        tris(i, 1) = tempVec[1];
        tris(i, 2) = tempVec[2];
    }
    pMesh->setIndex(tris);

    // Colors
    Eigen::MatrixX3f colors;
    int rows = pModel->columnCount(pModel->index(0, 0, QModelIndex()));
    colors.resize(rows, 3);
    colors.setConstant(rows, 3, 0.6);
    pMesh->setColor(colors);
}


//*************************************************************************************************************

void Surfer::setSelectionModel(QItemSelectionModel *pSelectionModel)
{
    //TODO implement this
}


//*************************************************************************************************************

void Surfer::onClick(QPickEvent *event)
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
