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
    // connect to event system, since we need to know when we can register our 3D stuff in a display view
    m_pCommu = new Communicator(this);

    m_pDipoleFitControl = new DipoleFitControl;

    initGuiConnections();

    connect(m_analyzeData.data(), &AnalyzeData::newModelAvailable,
            this, &DipoleFit::onNewModelAvalible);

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
    m_pDipoleRoot = QSharedPointer<QEntity>::create();
    m_pDipoleRoot->setObjectName(QString("DipoleEntityTree"));

    QVector3D pos, to, from;
    from = QVector3D(0.0, 1.0, 0.0);
    double norm;

    for(int i = 0; i < m_pActiveEcdSetModel->rowCount(); ++i) {
        QModelIndex dataIndex = m_pActiveEcdSetModel->index(i);
        INVERSELIB::ECD tempEcd = m_pActiveEcdSetModel->data(dataIndex, Qt::DisplayRole).value<INVERSELIB::ECD>();

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

void DipoleFit::onBrowseButtonClicked()
{
    qDebug() <<"browse clicked";
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
    if(sFitName.isEmpty() || m_analyzeData->getModel(sModelPath) != nullptr) {
        qDebug() << "ERROR: FitName invalid!";
        return;
    }

    m_analyzeData->loadEcdSetModel(m_dipoleSettings.getSettings(), sFitName);
}


//*************************************************************************************************************

void DipoleFit::onActiveModelSelected(const QString &sModelName)
{
    auto result = std::find_if(m_vEcdSetModels.cbegin(), m_vEcdSetModels.cend(),
                 [sModelName]( const QSharedPointer<EcdSetModel> &model) {
        return model->getModelName() == sModelName;
    });

    if(result != m_vEcdSetModels.end()) {
        m_pActiveEcdSetModel = *result;
        qDebug() << "New active model: " << m_pActiveEcdSetModel->getModelPath();
        //TODO  build new entity tree and send to main viewer
    }


}


//*************************************************************************************************************

void DipoleFit::onNewModelAvalible(QSharedPointer<AbstractModel> pNewModel)
{
    if(pNewModel->getType() == MODEL_TYPE::ANSHAREDLIB_ECDSET_MODEL) {
        m_vEcdSetModels.push_back(qSharedPointerCast<EcdSetModel>(pNewModel));
        m_pDipoleFitControl->addModel(pNewModel->getModelName());
    }
    qDebug() << "New model added to vector and menu: " << pNewModel->getModelPath();
}


//*************************************************************************************************************
