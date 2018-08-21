//=============================================================================================================
/**
* @file     test_surface_model.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     April, 2018
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
* @brief    The SurfaceModel unit test
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <fs/surface.h>
#include <fs/surfaceset.h>
#include "../applications/mne_analyze/libs/anShared/Model/surfacemodel.h"
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
using namespace FSLIB;


//=============================================================================================================
/**
* DECLARE CLASS TestSurfaceModel
*
* @brief The TestSurfaceModel class provides basic verification tests
*
*/
class TestSurfaceModel : public QObject
{
    Q_OBJECT

public:
    TestSurfaceModel();

private slots:
    void initTestCase();
    void testChildCounts();
    void testDataAccess();

    void cleanupTestCase();

private:
    Surface* m_surfaceData;
    QSharedPointer<SurfaceModel> m_surfaceModel;
    AnalyzeData* m_analyzeData;
};

//*************************************************************************************************************

TestSurfaceModel::TestSurfaceModel()
{

}

//*************************************************************************************************************

void TestSurfaceModel::initTestCase()
{
    // use DataStorage for loading the model
    m_analyzeData = new AnalyzeData();
    m_surfaceModel = m_analyzeData->loadSurfaceModel("sample", 1, "pial", "./MNE-sample-data/subjects");
    // load same data again for testing reasons
    m_surfaceData = new Surface("sample", 1, "pial", "./MNE-sample-data/subjects");
}


//*************************************************************************************************************

void TestSurfaceModel::testChildCounts()
{
    //root node
    QModelIndex rootModelIndex = QModelIndex();
    QVERIFY(rootModelIndex.isValid() == false);
    QVERIFY(m_surfaceModel->rowCount(rootModelIndex) == 4);
    QVERIFY(m_surfaceModel->hasChildren(rootModelIndex) == true);

    //check 4 child nodes that store the data;
    for(int i = 0; i < 4; ++i) {
        qDebug() << "Test row count of child node " << i;
        QModelIndex childModelIndex = m_surfaceModel->index(i,0, rootModelIndex);
        QVERIFY(childModelIndex.isValid() == true);
        QVERIFY(m_surfaceModel->rowCount(childModelIndex) == 0);
        QVERIFY(m_surfaceModel->hasChildren(childModelIndex) == false);
    }

}


//*************************************************************************************************************

void TestSurfaceModel::testDataAccess()
{
    QModelIndex rootModelIndex = QModelIndex();
    //the root node should not store any data
    QVERIFY(m_surfaceModel->columnCount(rootModelIndex) == 0);
    QVERIFY(m_surfaceModel->data(rootModelIndex, Qt::DisplayRole) == QVariant());

    //check 4 child nodes that store the data;
    for(int i = 0; i < 4; ++i) {
        QModelIndex childModelIndex = m_surfaceModel->index(i,0, rootModelIndex);
        QVERIFY(childModelIndex.isValid() == true);
        int cols = m_surfaceModel->columnCount(childModelIndex);
        std::cout << "column count form model row " << i << ": " << cols << std::endl;

        if(i == 0) {
            QVERIFY(cols == m_surfaceData->rr().rows());
        }
        else if(i == 1) {
            QVERIFY(cols == m_surfaceData->nn().rows());
        }
        else if(i == 2) {
            QVERIFY(cols == m_surfaceData->tris().rows());
        }
        else if(i == 3) {
            QVERIFY(cols == m_surfaceData->curv().rows());
        }

        for(int j = 0; j < cols; ++j) {
            QModelIndex dataIndex = m_surfaceModel->index(i, j, rootModelIndex);
            QVERIFY(dataIndex.isValid() == true);
            QVERIFY(m_surfaceModel->rowCount(dataIndex) == 0);
            QVERIFY(m_surfaceModel->hasChildren(dataIndex) == false);

            //Compare recieved data
            if(i == 0) {
                Eigen::Vector3f controllVertex;
                controllVertex[0] = m_surfaceData->rr()(j, 0);
                controllVertex[1] = m_surfaceData->rr()(j, 1);
                controllVertex[2] = m_surfaceData->rr()(j, 2);

                QVERIFY(m_surfaceModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3f>() == controllVertex);
            }
            else if(i == 1) {
                Eigen::Vector3f controllNormal;
                controllNormal[0] = m_surfaceData->nn()(j, 0);
                controllNormal[1] = m_surfaceData->nn()(j, 1);
                controllNormal[2] = m_surfaceData->nn()(j, 2);

                QVERIFY(m_surfaceModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3f>() == controllNormal);
            }
            else if(i == 2) {
                Eigen::Vector3i controllIndex;
                controllIndex[0] = m_surfaceData->tris()(j, 0);
                controllIndex[1] = m_surfaceData->tris()(j, 1);
                controllIndex[2] = m_surfaceData->tris()(j, 2);

                QVERIFY(m_surfaceModel->data(dataIndex, Qt::DisplayRole).value<Eigen::Vector3i>() == controllIndex);
            }
            else if(i == 3) {
                QVERIFY(m_surfaceModel->data(dataIndex, Qt::DisplayRole).toFloat() == m_surfaceData->curv()[j]);
            }

        }

    }

}


//*************************************************************************************************************

void TestSurfaceModel::cleanupTestCase()
{
    delete m_surfaceData;
    // delete m_surfaceModel;
}

//=============================================================================================================
// MAIN
//=============================================================================================================

QTEST_APPLESS_MAIN(TestSurfaceModel)
#include "test_surface_model.moc"
