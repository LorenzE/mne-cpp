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
using namespace Qt3DRender;
using namespace Qt3DCore;
using namespace Qt3DExtras;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

CentralView::CentralView()
    : Qt3DWindow(),
      m_pRootEntity(new QEntity)
{
    init();
}

//*************************************************************************************************************

void CentralView::init()
{
    // initialize 3D Window
    QPickingSettings *pPickSettings = renderSettings()->pickingSettings();
    pPickSettings->setFaceOrientationPickingMode(QPickingSettings::FrontAndBackFace);
    pPickSettings->setPickMethod(QPickingSettings::TrianglePicking);
    pPickSettings->setPickResultMode(QPickingSettings::NearestPick);

    QCamera *pCamera = camera();
    pCamera->setPosition(QVector3D(0,0,1));

    QFirstPersonCameraController *pCamController = new QFirstPersonCameraController(m_pRootEntity);
    pCamController->setCamera(pCamera);
    // we introduced the convention that every entity below root should be named
    pCamController->setObjectName("MainViewer/CameraController");
    m_vEntities.push_back(QSharedPointer<QEntity>(pCamController));

    setRootEntity(m_pRootEntity);
}


//*************************************************************************************************************

void CentralView::addEntity(QSharedPointer<QEntity> pEntity)
{
    // remember shared pointer
    m_vEntities.push_back(pEntity);

    // simply insert below root
    pEntity->setParent(m_pRootEntity);

    // use this as an opportunity to check for unused anti crash nodes
    checkForUnusedAntiCrashNodes();
}


//*************************************************************************************************************

void CentralView::removeEntity(const QString &sIdentifier)
{
    // only search for direct children, since entities are always added below root
    QEntity* temp = m_pRootEntity->findChild<QEntity* >(sIdentifier, Qt::FindDirectChildrenOnly);
    if (temp) {
        for (int i = 0; i < m_vEntities.size(); ++i)
        {
            if (m_vEntities.at(i)->objectName().compare(sIdentifier) == 0)
            {
                // remove from pointer storage
                m_vEntities.remove(i);
                // we trust in uniqueness of identifiers, break
                break;
            }
        }
        // create a new entity and make it the new parent of the remove-candidate
        temp->setParent(createNewAntiCrashNode());
    }
    else {
        qDebug() << "[CentralView] Could not find child named " << sIdentifier.toStdString().c_str();
    }
}


//*************************************************************************************************************

void CentralView::shutdown()
{
    // dissassemble the used entity tree
    QNodeVector vChildren = m_pRootEntity->childNodes();
    for (QNode* pNode : vChildren)
    {
        QEntity* temp = (QEntity*) pNode;
        if (temp)
        {
            // avoid double frees on program shutdown, caused by shared ownership
            temp->setParent((QEntity*) nullptr);
        }
    }

    // take care of dangling anti crash nodes
    for (int i = 0; i < m_vAntiCrashNodes.size(); ++i)
    {
        int numChildren = m_vAntiCrashNodes.at(i)->childNodes().count();
        if (numChildren == 0) {
            // not used anymore, simply wait for vector destructor
        }
        else if (numChildren == 1) {
            // still used, need to seperate the child from the anti crash nodes in order to avoid double frees
            m_vAntiCrashNodes.at(i)->childNodes().at(0)->setParent((QEntity*) nullptr);
        }
        else {
            qDebug() << "[CentralView] FATAL Shutdown: found anti crash node with more than one child !";
            // best thing we can do is to seperate the parent anti crash node from every child
            for (QNode* qn : m_vAntiCrashNodes.at(i)->childNodes())
            {
                qn->setParent((QEntity*) nullptr);
            }
        }
    }
}

//*************************************************************************************************************

QEntity* CentralView::createNewAntiCrashNode()
{
    QSharedPointer<QEntity> antiCrashNode = QSharedPointer<QEntity>::create();
    m_vAntiCrashNodes.push_back(antiCrashNode);
    return antiCrashNode.data();
}


//*************************************************************************************************************

void CentralView::checkForUnusedAntiCrashNodes()
{
    for (int i = 0; i < m_vAntiCrashNodes.size(); ++i)
    {
        if (m_vAntiCrashNodes.at(i)->childNodes().count() == 0)
        {
            // no longer needed, delete and remove
            m_vAntiCrashNodes[i]->deleteLater();
            m_vAntiCrashNodes.remove(i);
            --i;
        }
    }
}
