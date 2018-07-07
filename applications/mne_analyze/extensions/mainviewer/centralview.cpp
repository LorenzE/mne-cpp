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
    setRootEntity(m_pRootEntity);

    QPickingSettings *pPickSettings = renderSettings()->pickingSettings();
    pPickSettings->setFaceOrientationPickingMode(QPickingSettings::FrontAndBackFace);
    pPickSettings->setPickMethod(QPickingSettings::TrianglePicking);
    pPickSettings->setPickResultMode(QPickingSettings::NearestPick);

    QCamera *pCamera = camera();
    pCamera->setPosition(QVector3D(0,0,1));

    QFirstPersonCameraController *pCamController = new QFirstPersonCameraController(m_pRootEntity);
    pCamController->setCamera(pCamera);
    // @TODO think about whether this still makes sense
    pCamController->setObjectName("CameraController");
    m_vEntities.push_back(QSharedPointer<QEntity>(pCamController));
}


//*************************************************************************************************************

void CentralView::addEntity(QSharedPointer<QEntity> pEntity)
{
    // remember shared pointer
    m_vEntities.push_back(pEntity);

    // simply insert below root
    pEntity->setParent(m_pRootEntity);
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
        // this is an obvious memory leak, but anything different wont work
        temp->setParent(new QEntity);
    }
    else {
        qDebug() << "[CentralView] Could not find child named " << sIdentifier.toStdString().c_str();
    }
}


//*************************************************************************************************************

void CentralView::dissasEntityTree()
{
    QNodeVector vec = m_pRootEntity->childNodes();
    for (QNode* pNode : vec)
    {
        QEntity* temp = (QEntity*) pNode;
        if (temp)
        {
            // avoid double frees on program shutdown. This is an obvious memory leak, but anything different wont work
            temp->setParent(new QEntity);
        }
    }
}
