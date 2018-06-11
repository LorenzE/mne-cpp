//=============================================================================================================
/**
* @file     dipolefit.cpp
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
* @brief    Definition of the DipoleFit class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dipolefit.h"
#include "FormFiles/dipolefitcontrol.h"
#include <inverse/dipoleFit/dipole_fit_settings.h>
#include <anShared/Management/communicator.h>
#include <anShared/Model/ecdsetmodel.h>
#include <anShared/Utils/metatypes.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <QFileDialog>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace Qt3DCore;
using namespace DIPOLEFITEXTENSION;
using namespace ANSHAREDLIB;
using namespace INVERSELIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

DipoleFit::DipoleFit()
: m_pControl(Q_NULLPTR)
, m_pDipoleFitControl(Q_NULLPTR)
{

}


//*************************************************************************************************************

DipoleFit::~DipoleFit()
{

}


//*************************************************************************************************************

QSharedPointer<IExtension> DipoleFit::clone() const
{
    QSharedPointer<DipoleFit> pDipoleFitClone(new DipoleFit);
    return pDipoleFitClone;
}


//*************************************************************************************************************

void DipoleFit::init()
{
    m_pDipoleFitControl = new DipoleFitControl;
    connect(m_pDipoleFitControl, &DipoleFitControl::browseButtonClicked,
            this, &DipoleFit::onBrowseButtonClicked);
    connect(this, &DipoleFit::measFilePathChanged,
            m_pDipoleFitControl, &DipoleFitControl::setMeasFilePath);
    connect(this, &DipoleFit::settingIsRawChanged,
            m_pDipoleFitControl, &DipoleFitControl::setUseRaw);
    connect(this, &DipoleFit::settingSetNumChanged,
            m_pDipoleFitControl, &DipoleFitControl::setSetNumber);
    connect(this, &DipoleFit::settingIncludeMegChanged,
            m_pDipoleFitControl, &DipoleFitControl::setIncludeMeg);
    connect(this, &DipoleFit::settingIncludeEegChanged,
            m_pDipoleFitControl, &DipoleFitControl::setIncludeEeg);
    connect(this, &DipoleFit::settingTMaxChanged,
            m_pDipoleFitControl, &DipoleFitControl::setTMax);
    connect(this, &DipoleFit::settingTMinChanged,
            m_pDipoleFitControl, &DipoleFitControl::setTMin);
    connect(this, &DipoleFit::settingBMaxChanged,
            m_pDipoleFitControl, &DipoleFitControl::setBMax);
    connect(this, &DipoleFit::settingBMinChanged,
            m_pDipoleFitControl, &DipoleFitControl::setBMin);

    setSettingIsRaw(false);
    setSettingSetNum(1);
    setSettingIncludeMeg(true);
    setSettingIncludeEeg(true);
    setSettingTMax(148.0f/1000.0f);
    setSettingTMin(32.0f/1000.0f);
    setSettingBMax(0.0f/1000.0f);
    setSettingBMin(-100.0f/1000.0f);

    // connect to event system, since we need to know when we can register our 3D stuff in a display view
    m_pCommu = new Communicator(this);

    //TODO load the model with analyzeData
    QFile testFile;


    //Following is equivalent to: --meas ./mne-cpp-test-data/MEG/sample/sample_audvis-ave.fif --set 1 --meg --eeg --tmin 32 --tmax 148 --bmin -100 --bmax 0 --dip ./mne-cpp-test-data/Result/dip_fit.dat
    DipoleFitSettings settings;
    testFile.setFileName(QDir::currentPath()+"/MNE-sample-data/MEG/sample/sample_audvis-ave.fif");
    settings.measname = testFile.fileName();
    settings.is_raw = m_bDipolSettIsRaw;
    settings.setno = m_iDipolSettSetNum;
    settings.include_meg = m_bDipolSettIncMeg;
    settings.include_eeg = m_bDipolSettIncEeg;
    settings.tmin = m_dDipolSettTMin;
    settings.tmax = m_dDipolSettTMax;
    settings.bmin = m_dDipolSettBMin;
    settings.bmax = m_dDipolSettBMax;
    settings.dipname = QDir::currentPath()+"/MNE-sample-data/Result/dip_fit.dat";

    settings.checkIntegrity();

    // create Model
    m_pEcdSetModel = QSharedPointer<EcdSetModel>::create(&settings);


    //Build the QEntity Tree
    m_pDipoleRoot = QSharedPointer<QEntity>::create();
    m_pDipoleRoot->setObjectName(QString("DipoleEntityTree"));

    QVector3D pos, to, from;
    from = QVector3D(0.0, 1.0, 0.0);
    double norm;

    for(int i = 0; i < m_pEcdSetModel->rowCount(); ++i) {
        QModelIndex dataIndex = m_pEcdSetModel->index(i);
        INVERSELIB::ECD tempEcd = m_pEcdSetModel->data(dataIndex, Qt::DisplayRole).value<INVERSELIB::ECD>();

        pos.setX(tempEcd.rd(0));
        pos.setY(tempEcd.rd(1));
        pos.setZ(tempEcd.rd(2));

        norm = sqrt(pow(tempEcd.Q(0),2)+pow(tempEcd.Q(1),2)+pow(tempEcd.Q(2),2));

        to.setX(tempEcd.Q(0)/norm);
        to.setY(tempEcd.Q(1)/norm);
        to.setZ(tempEcd.Q(2)/norm);

//        qDebug()<<"EcdDataTreeItem::plotDipoles - from" << from;
//        qDebug()<<"EcdDataTreeItem::plotDipoles - to" << to;

        QQuaternion final = QQuaternion::rotationTo(from, to);

        //Set dipole position and orientation
        QMatrix4x4 m;
        m.translate(pos);
        m.rotate(final);

        Qt3DCore::QTransform *pTransform = new Qt3DCore::QTransform();
        pTransform->setMatrix(m);

        Qt3DExtras::QPhongMaterial *pMaterial = new Qt3DExtras::QPhongMaterial();
        pMaterial->setAmbient(QColor(rand()%255, rand()%255, rand()%255));

        Qt3DExtras::QConeMesh *pDipolGeometry = new Qt3DExtras::QConeMesh();
        pDipolGeometry->setBottomRadius(0.001f);
        pDipolGeometry->setLength(0.003f);

        Qt3DCore::QEntity *pEntity = new QEntity(m_pDipoleRoot.data());

        pEntity->addComponent(pTransform);
        pEntity->addComponent(pMaterial);
        pEntity->addComponent(pDipolGeometry);
    }
}


//*************************************************************************************************************

void DipoleFit::unload()
{

}


//*************************************************************************************************************

QString DipoleFit::getName() const
{
    return "Dipole Fit";
}


//*************************************************************************************************************

QMenu *DipoleFit::getMenu()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

QDockWidget *DipoleFit::getControl()
{
    if(!m_pControl) {
        m_pControl = new QDockWidget(tr("Dipole Fit"));
        m_pControl->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        m_pControl->setMinimumWidth(180);
        m_pControl->setWidget(m_pDipoleFitControl);
    }

    return m_pControl;
}


//*************************************************************************************************************

QWidget *DipoleFit::getView()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

void DipoleFit::handleEvent(QSharedPointer<Event> e)
{
    switch(e->getType())
    {
    case EXTENSION_INIT_FINISHED:
    {
        QVector<QSharedPointer<QEntityListModel> > availableDisplays = m_analyzeData->availableDisplays();
        if (availableDisplays.size() >= 1)
        {
            availableDisplays.at(0)->addEntityTree(m_pDipoleRoot);
        }
        break;
    }
    default:
        qDebug() << "Surfer received an Event that is not handled by switch-cases";
        break;
    }
}


//*************************************************************************************************************

QVector<EVENT_TYPE> DipoleFit::getEventSubscriptions(void) const
{
    QVector<EVENT_TYPE> temp {EXTENSION_INIT_FINISHED};
    return temp;
}


//*************************************************************************************************************

void DipoleFit::onBrowseButtonClicked()
{
    qDebug() <<"browse clicked";
    //Get the path
    QString filePath = QFileDialog::getOpenFileName(m_pDipoleFitControl,
                                tr("Open File"),
                                QDir::currentPath()+"/MNE-sample-data",
                                tr("fif File(*.fif)"));
    if(!filePath.isNull()) {
        m_sMeasFilePath = filePath;
        emit measFilePathChanged(m_sMeasFilePath);
    }
}


//*************************************************************************************************************

void DipoleFit::setSettingIsRaw(bool value)
{
    m_bDipolSettIsRaw = value;
    emit settingIsRawChanged(m_bDipolSettIsRaw);
}


//*************************************************************************************************************

void DipoleFit::setSettingSetNum(int value)
{
    m_iDipolSettSetNum = value;
    emit settingSetNumChanged(m_iDipolSettSetNum);
}


//*************************************************************************************************************

void DipoleFit::setSettingIncludeMeg(bool value)
{
    m_bDipolSettIncMeg = value;
    emit settingIncludeMegChanged(m_bDipolSettIncMeg);
}


//*************************************************************************************************************

void DipoleFit::setSettingIncludeEeg(bool value)
{
    m_bDipolSettIncEeg = value;
    emit settingIncludeEegChanged(m_bDipolSettIncEeg);
}


//*************************************************************************************************************

void DipoleFit::setSettingTMax(double value)
{
    m_dDipolSettTMax = value;
    emit settingTMaxChanged(value);
}


//*************************************************************************************************************

void DipoleFit::setSettingTMin(double value)
{
    m_dDipolSettTMin = value;
    emit settingTMinChanged(value);
}


//*************************************************************************************************************
void DipoleFit::setSettingBMax(double value)
{
    m_dDipolSettBMax = value;
    emit settingBMaxChanged(value);
}


//*************************************************************************************************************
void DipoleFit::setSettingBMin(double value)
{
    m_dDipolSettBMin = value;
    emit settingBMinChanged(value);
}


//*************************************************************************************************************
