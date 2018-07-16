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
#include <anShared/Data/dipolefitsettingswrapper.h>
#include <inverse/dipoleFit/dipole_fit_settings.h>
#include <anShared/Management/communicator.h>
#include <anShared/Model/ecdsetmodel.h>
#include <anShared/Utils/metatypes.h>

#include <algorithm>


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
, m_pMenu(Q_NULLPTR)
, m_bInitFinished(false)
{

}


//*************************************************************************************************************

DipoleFit::~DipoleFit()
{
    delete m_pCommu;
    delete m_pDipoleFitControl;
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
    // connect to event system, since we need to know when we can register our 3D stuff in a display view
    m_pCommu = new Communicator(this);

    m_pDipoleFitControl = new DipoleFitControl;

    initGuiConnections();

    connect(m_analyzeData.data(), &AnalyzeData::newModelAvailable,
            this, &DipoleFit::onNewModelAvalible);
    connect(m_analyzeData.data(), &AnalyzeData::modelPathChanged,
            this, &DipoleFit::onModelPathChanged);

    //TODO load the model with analyzeData
    QFile testFile;

    //Following is equivalent to: --meas ./mne-cpp-test-data/MEG/sample/sample_audvis-ave.fif --set 1 --meg --eeg --tmin 32 --tmax 148 --bmin -100 --bmax 0 --dip ./mne-cpp-test-data/Result/dip_fit.dat
    testFile.setFileName(QDir::currentPath()+"/MNE-sample-data/MEG/sample/sample_audvis-ave.fif");

    m_dipoleSettings.setMeasurementFilePath(testFile.fileName());
    m_dipoleSettings.setIsRaw(false);
    m_dipoleSettings.setSetNum(1);
    m_dipoleSettings.setIncludeMeg(true);
    m_dipoleSettings.setIncludeEeg(true);
    m_dipoleSettings.setTMax(148.0f/1000.0f);
    m_dipoleSettings.setTMin(32.0f/1000.0f);
    m_dipoleSettings.setBMax(0.0f/1000.0f);
    m_dipoleSettings.setBMin(-100.0f/1000.0f);
    m_dipoleSettings.setDipPath(QDir::currentPath()+"/MNE-sample-data/Result/dip_fit.dat");

    // create Model
    m_pActiveEcdSetModel = m_analyzeData->loadEcdSetModel(m_dipoleSettings.getSettings(), ECD_SET_MODEL_DEFAULT_DIR_PATH + QStringLiteral("Test"));

    qDebug() << "DipoleFit: EcdSetModel size: " << m_pActiveEcdSetModel->rowCount();

    //Build the QEntity Tree
    m_pDipoleRoot = create3DEnityTree(m_pActiveEcdSetModel);
}


//*************************************************************************************************************

void DipoleFit::unload()
{

}


//*************************************************************************************************************

QString DipoleFit::getName() const
{
    return QStringLiteral("Dipole Fit");
}


//*************************************************************************************************************

QMenu *DipoleFit::getMenu()
{
    if(!m_pMenu) {
        m_pMenu = new QMenu(tr("Dipole Fit"));

        m_pLoadfitFromFile = new QAction(tr("Load dipole fit"));
        m_pLoadfitFromFile->setStatusTip(tr("Load dipole fit"));
        connect(m_pLoadfitFromFile, &QAction::triggered,
                this, &DipoleFit::onLoadFitFilePressed);


        m_pSaveFitToFile = new QAction(tr("Save dipole fit"));
        m_pSaveFitToFile->setStatusTip(tr("Save dipole fit"));
        connect(m_pSaveFitToFile, &QAction::triggered,
                this, &DipoleFit::onSaveFitToFilePressed);

        m_pMenu->addAction(m_pLoadfitFromFile);
        m_pMenu->addAction(m_pSaveFitToFile);
    }
    return m_pMenu;
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
            m_pDisplayModel = availableDisplays.at(0);
            m_pDisplayModel->addEntityTree(m_pDipoleRoot);
            m_bInitFinished = true;
        }
        break;
    }
    default:
        qDebug() << "DipoleFit received an Event that is not handled by switch-cases";
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

void DipoleFit::initGuiConnections()
{
    connect(m_pDipoleFitControl, &DipoleFitControl::browseButtonClicked,
            this, &DipoleFit::onBrowseButtonClicked);
    connect(m_pDipoleFitControl, &DipoleFitControl::fitButtonClicked,
            this, &DipoleFit::onFitButtonClicked);
    connect(m_pDipoleFitControl, &DipoleFitControl::activeModelSelected,
            this, &DipoleFit::onActiveModelSelected);

    //make output to gui connections
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::measurementFilePathChanged,
            m_pDipoleFitControl, &DipoleFitControl::setMeasFilePath);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::isRawChanged,
            m_pDipoleFitControl, &DipoleFitControl::setUseRaw);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::setNumChanged,
            m_pDipoleFitControl, &DipoleFitControl::setSetNumber);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::includeMegChanged,
            m_pDipoleFitControl, &DipoleFitControl::setIncludeMeg);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::includeEegChanged,
            m_pDipoleFitControl, &DipoleFitControl::setIncludeEeg);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::tMaxChanged,
            m_pDipoleFitControl, &DipoleFitControl::setTMax);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::tMinChanged,
            m_pDipoleFitControl, &DipoleFitControl::setTMin);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::bMaxChanged,
            m_pDipoleFitControl, &DipoleFitControl::setBMax);
    connect(&m_dipoleSettings, &DipoleFitSettingsWrapper::bMinChanged,
            m_pDipoleFitControl, &DipoleFitControl::setBMin);

    //make input from gui connections
    connect(m_pDipoleFitControl, &DipoleFitControl::useRawCheckStateChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setIsRaw);
    connect(m_pDipoleFitControl, &DipoleFitControl::setNumChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setSetNum);
    connect(m_pDipoleFitControl, &DipoleFitControl::includeMegChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setIncludeMeg);
    connect(m_pDipoleFitControl, &DipoleFitControl::includeEegChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setIncludeEeg);
    connect(m_pDipoleFitControl, &DipoleFitControl::tMaxChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setTMax);
    connect(m_pDipoleFitControl, &DipoleFitControl::tMinChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setTMin);
    connect(m_pDipoleFitControl, &DipoleFitControl::bMaxChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setBMax);
    connect(m_pDipoleFitControl, &DipoleFitControl::bMinChanged,
            &m_dipoleSettings, &DipoleFitSettingsWrapper::setBMin);
}


//*************************************************************************************************************

QSharedPointer<QEntity> DipoleFit::create3DEnityTree(QSharedPointer<EcdSetModel> pModel) const
{
    QSharedPointer<QEntity> pRootEntity = QSharedPointer<QEntity>::create();
    pRootEntity->setObjectName(QStringLiteral("DipoleEntityTree"));

    QVector3D pos, to, from;
    from = QVector3D(0.0, 1.0, 0.0);
    double norm;

    for(int i = 0; i < pModel->rowCount(); ++i) {
        QModelIndex dataIndex = pModel->index(i);
        INVERSELIB::ECD tempEcd = pModel->data(dataIndex, Qt::DisplayRole).value<INVERSELIB::ECD>();

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

        Qt3DCore::QEntity *pEntity = new QEntity(pRootEntity.data());

        pEntity->addComponent(pTransform);
        pEntity->addComponent(pMaterial);
        pEntity->addComponent(pDipolGeometry);
    }

    return pRootEntity;
}


//*************************************************************************************************************

void DipoleFit::onBrowseButtonClicked()
{
    //Get the path
    QString filePath = QFileDialog::getOpenFileName(m_pDipoleFitControl,
                                tr("Open File"),
                                QDir::currentPath()+"/MNE-sample-data",
                                tr("fif File(*.fif)"));
    if(!filePath.isNull()) {
        m_dipoleSettings.setMeasurementFilePath(filePath);

    }
}


//*************************************************************************************************************

void DipoleFit::onFitButtonClicked()
{
    QString sFitName = m_pDipoleFitControl->getFitName();
    QString sModelPath = ECD_SET_MODEL_DEFAULT_DIR_PATH + sFitName;
    if(sFitName.isEmpty()) {
        m_pCommu->publishEvent(STATUS_BAR_MSG, QStringLiteral("Fit name invalid"));
        return;
    }

    m_analyzeData->loadEcdSetModel(m_dipoleSettings.getSettings(), sModelPath);
}


//*************************************************************************************************************

void DipoleFit::onActiveModelSelected(const QString &sModelName)
{
    if(m_bInitFinished == false) {
        return;
    }

    //find selected model
    auto result = std::find_if(m_vEcdSetModels.begin(), m_vEcdSetModels.end(),
                 [sModelName]( const QPair<QSharedPointer<EcdSetModel>, QSharedPointer<QEntity>> &modelPair) {
        return modelPair.first->getModelName() == sModelName;
    });

    if(result != m_vEcdSetModels.end()) {
        //change visible 3D-model
        bool removeSuccessful = m_pDisplayModel->removeEntityTree(m_pDipoleRoot);
        if(removeSuccessful) {
            m_pActiveEcdSetModel = result->first;

            if(result->second.isNull()) {
                //create qentity tree if non exists
                result->second = create3DEnityTree(m_pActiveEcdSetModel);
                qDebug() << "new DipoleFit entity tree created";
            }

            m_pDipoleRoot = result->second;
            m_pDisplayModel->addEntityTree(m_pDipoleRoot);
        }
        else {
            qDebug() << "DipoleFit: Unable to remove current 3D-Model!";
        }

        qDebug() << "New active model: " << m_pActiveEcdSetModel->getModelPath();
    }
}


//*************************************************************************************************************

void DipoleFit::onNewModelAvalible(QSharedPointer<AbstractModel> pNewModel)
{
    if(pNewModel->getType() == MODEL_TYPE::ANSHAREDLIB_ECDSET_MODEL) {
        //add the new model to the list with no 3d entity tree
        m_vEcdSetModels.push_back(qMakePair(qSharedPointerCast<EcdSetModel>(pNewModel), QSharedPointer<QEntity>()));
        m_pDipoleFitControl->addModel(pNewModel->getModelName());
        qDebug() << "New model added to vector and menu: " << pNewModel->getModelPath();
    }
}


//*************************************************************************************************************

void DipoleFit::onModelPathChanged(QSharedPointer<AbstractModel> pModel, const QString &sOldModelPath, const QString &sNewModelPath)
{
    //find model
    auto result = std::find_if(m_vEcdSetModels.cbegin(), m_vEcdSetModels.cend(),
                 [pModel]( const QPair<QSharedPointer<EcdSetModel>, QSharedPointer<QEntity>> &modelPair) {
        return modelPair.first == pModel;
    });

    if(pModel->getType() == MODEL_TYPE::ANSHAREDLIB_ECDSET_MODEL && result != m_vEcdSetModels.cend()) {
        //update gui
        m_pDipoleFitControl->removeModel(sOldModelPath.section('/', -1));
        m_pDipoleFitControl->addModel(pModel->getModelName());
    }
}


//*************************************************************************************************************

void DipoleFit::onLoadFitFilePressed()
{
    //Get the path
    QString filePath = QFileDialog::getOpenFileName(m_pMenu,
                                tr("Open File"),
                                QDir::currentPath()+"/MNE-sample-data",
                                tr("dip File(*.dip)"));
    if(!filePath.isNull()) {
        m_analyzeData->loadEcdSetModel(filePath);
    }
}


//*************************************************************************************************************

void DipoleFit::onSaveFitToFilePressed()
{
    QString filePath = QFileDialog::getSaveFileName(m_pMenu, tr("Save File"),
                               QDir::currentPath(),
                               tr("Dipole Fits (*.dip)"));
    if(!filePath.isNull()) {
        qDebug() << "save to " << filePath;

        //check if name change is needed
        if(filePath != m_pActiveEcdSetModel->getModelPath()) {
            m_analyzeData->changeModelPath(m_pActiveEcdSetModel->getModelPath(), filePath);
        }

        m_pActiveEcdSetModel->saveToFile();
    }
}


//*************************************************************************************************************
