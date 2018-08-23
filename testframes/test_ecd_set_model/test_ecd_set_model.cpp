//=============================================================================================================
/**
* @file     test_surface_model.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
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
* @brief    The EcdSetModel unit test
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <inverse/dipoleFit/dipole_fit_settings.h>
#include <inverse/dipoleFit/ecd_set.h>
#include <inverse/dipoleFit/dipole_fit.h>
#include "../applications/mne_analyze/libs/anShared/Model/ecdsetmodel.h"
#include "../applications/mne_analyze/libs/anShared/Utils/types.h"
#include "../applications/mne_analyze/libs/anShared/Utils/metatypes.h"
#include "../applications/mne_analyze/libs/anShared/Management/analyzedata.h"
#include <iostream>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtTest>

//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;
using namespace INVERSELIB;


//=============================================================================================================
/**
* DECLARE CLASS TestEcdSetModel
*
* @brief The TestEcdSetModel class provides basic verification tests
*
*/
class TestEcdSetModel : public QObject
{
    Q_OBJECT

public:
    TestEcdSetModel();

private slots:
    void initTestCase();
    void testChildCounts();
    void testDataAccess();
    void testDataChanging();
    void testModelName();

    //TODO write a test for setData()


    void cleanupTestCase();

private:
    ECDSet m_refEcdSet;
    QSharedPointer<EcdSetModel> m_EcdSetModel;
    QString m_sModelDirPath;
    QString m_sModelName;
};

//*************************************************************************************************************

TestEcdSetModel::TestEcdSetModel()
{

}

//*************************************************************************************************************

void TestEcdSetModel::initTestCase()
{
    m_sModelDirPath = ECD_SET_MODEL_DEFAULT_DIR_PATH;
    m_sModelName = QStringLiteral("Test_Model");

    // Dipole Fit Settings

    QFile testFile;

    //Following is equivalent to: --meas ./mne-cpp-test-data/MEG/sample/sample_audvis-ave.fif --set 1 --meg --eeg --tmin 32 --tmax 148 --bmin -100 --bmax 0 --dip ./mne-cpp-test-data/Result/dip_fit.dat
    DipoleFitSettings settings;
    std::cout << QDir::currentPath().toStdString() << std::endl;
    testFile.setFileName(QDir::currentPath()+"/MNE-sample-data/MEG/sample/sample_audvis-ave.fif"); QVERIFY( testFile.exists() );
    settings.measname = testFile.fileName();
    settings.is_raw = false;
    settings.setno = 1;
    settings.include_meg = true;
    settings.include_eeg = true;
    settings.tmin = 32.0f/1000.0f;
    settings.tmax = 148.0f/1000.0f;
    settings.bmin = -100.0f/1000.0f;
    settings.bmax = 0.0f/1000.0f;
    settings.dipname = QDir::currentPath()+"/MNE-sample-data/Result/dip_fit.dat";

    settings.checkIntegrity();

    // create Model
    m_EcdSetModel = QSharedPointer<EcdSetModel>::create(&settings, m_sModelDirPath + m_sModelName);

    DipoleFit dipFit(&settings);
    m_refEcdSet = dipFit.calculateFit();
    std::cout << " ecd ref set size: " << m_refEcdSet.size() << std::endl;


}


//*************************************************************************************************************

void TestEcdSetModel::testChildCounts()
{
    //root node
    QModelIndex rootModelIndex = QModelIndex();
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>> Model size: " << m_EcdSetModel->rowCount(rootModelIndex)<<" >>>>>>>>>>>>>>>>>>>>>>>>>\n";

    QVERIFY(rootModelIndex.isValid() == false);
    QVERIFY(m_EcdSetModel->rowCount(rootModelIndex) == m_refEcdSet.size());
    QVERIFY(m_EcdSetModel->hasChildren(rootModelIndex) == true);
}


//*************************************************************************************************************

void TestEcdSetModel::testDataAccess()
{
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>> Test Data Access >>>>>>>>>>>>>>>>>>>>>>>>>\n";

    QModelIndex rootModelIndex = QModelIndex();
    //the root node should not store any data
    QVERIFY(m_EcdSetModel->data(rootModelIndex, Qt::DisplayRole) == QVariant());

    for(int i = 0; i < m_EcdSetModel->rowCount(rootModelIndex); ++i) {
        QModelIndex dataIndex = m_EcdSetModel->index(i);
        QVERIFY(dataIndex.isValid() == true);

        QVERIFY(m_EcdSetModel->data(dataIndex, Qt::DisplayRole) != QVariant());
        QVERIFY(m_EcdSetModel->data(dataIndex, Qt::DisplayRole).value<INVERSELIB::ECD>().rd == m_refEcdSet[i].rd);
    }

    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>> Check out of bound Access >>>>>>>>>>>>>>>>>>>>>>>>>\n";

    QModelIndex outOfBoundIndex = m_EcdSetModel->index(m_refEcdSet.size());
    QVERIFY(m_EcdSetModel->data(outOfBoundIndex, Qt::DisplayRole) == QVariant());
}


//*************************************************************************************************************

void TestEcdSetModel::testDataChanging()
{

    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>> Test the changing of data >>>>>>>>>>>>>>>>>>>>>>>>>\n";

    ECD testEcd = m_refEcdSet[0];
    QModelIndex dataIndex = m_EcdSetModel->index(1);

    connect(m_EcdSetModel.data(), &QAbstractItemModel::dataChanged,
            this, [testEcd, this](const QModelIndex &TopLeft, const QModelIndex &bottomRight, const QVector<int> roles) {

        QVERIFY(TopLeft == bottomRight);
        QVERIFY(m_EcdSetModel->data(TopLeft).value<ECD>().rd == testEcd.rd);
    });
    bool dataWasChanged = m_EcdSetModel->setData(dataIndex,QVariant::fromValue(testEcd));

    //check this to confirm that the data changed test was executed
    QVERIFY(dataWasChanged == true);

}


//*************************************************************************************************************

void TestEcdSetModel::testModelName()
{
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>> Test model name and path >>>>>>>>>>>>>>>>>>>>>>>>>\n";

    QVERIFY(m_EcdSetModel->getModelName() == m_sModelName);
    QVERIFY(m_EcdSetModel->getModelPath() == m_sModelDirPath + m_sModelName);
}


//*************************************************************************************************************

void TestEcdSetModel::cleanupTestCase()
{

}

//=============================================================================================================
// MAIN
//=============================================================================================================

QTEST_APPLESS_MAIN(TestEcdSetModel)
#include "test_ecd_set_model.moc"
