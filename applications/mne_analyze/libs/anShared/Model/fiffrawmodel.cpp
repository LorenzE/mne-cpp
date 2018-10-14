//=============================================================================================================
/**
* @file     fiffrawmodel.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief    FiffRawModel class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "fiffrawmodel.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtConcurrent/QtConcurrent>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;
using namespace FIFFLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

FiffRawModel::FiffRawModel(QObject *pParent)
    : AbstractModel(pParent)
{

}


//*************************************************************************************************************

FiffRawModel::FiffRawModel(QFile& inFile,
                           qint32 iSamplesPerBlock,
                           qint32 iWindowSize,
                           qint32 iPaddingSize,
                           QObject *pParent)
    : AbstractModel(pParent),
      m_iSamplesPerBlock(iSamplesPerBlock),
      m_iWindowSize(iWindowSize),
      m_iPaddingSize(iPaddingSize),
      m_iFiffCursorBegin(-1),
      m_pFiffIO(QSharedPointer<FiffIO>(new FiffIO()))
{
    // ugly hack to cheat the constness of the Model::data method:
    connect(this,
            &FiffRawModel::startToLoadBlocks,
            this,
            &FiffRawModel::onStartToLoadBlocks);

    // connect data reloading: this is done concurrently
    connect(&m_blockLoadFutureWatcher,
            &QFutureWatcher<int>::finished,
            [this]() {
                postBlockLoad(m_blockLoadFutureWatcher.future().result());
            });


    initFiffData(inFile);
}


//*************************************************************************************************************

FiffRawModel::~FiffRawModel()
{

}


//*************************************************************************************************************

void FiffRawModel::initFiffData(QFile &inFile)
{
    m_pFiffIO = QSharedPointer<FiffIO>::create(inFile);

    // build datastructure that is to be filled with data from the file
    MatrixXd data, times;
    // append a matrix pair for each block: padding left, window, padding right
    for(int i = 0; i < m_iWindowSize + 2 * m_iPaddingSize; ++i) {
        m_lData.append(qMakePair(data, times));
    }

    if(m_pFiffIO->m_qlistRaw.empty()) {
        qDebug() << "[FiffRawModel::loadFiffData] File " << inFile.fileName() << " does not contain any Fiff data";
        return;
    }
    else {
        //Set cursor somewhere into Fiff file
        m_iFiffCursorBegin = m_pFiffIO->m_qlistRaw[0]->first_samp;

        int start = m_iFiffCursorBegin;
        // for some reason the read_raw_segment function works with inclusive upper bound
        int end = start + m_iSamplesPerBlock - 1;

        // read in all needed blocks (padding left, window, padding right)
        for(int i = 0; i < m_iWindowSize + 2 * m_iPaddingSize; ++i) {
            if(m_pFiffIO->m_qlistRaw[0]->read_raw_segment(m_lData[i].first, m_lData[i].second, start, end)) {
                // qDebug() << "[FiffRawmodel::loadFiffData] Successfully read a block ";
            } else {
                qDebug() << "[FiffRawModel::loadFiffData] Could not read samples " << start << " to " << end;
                return;
            }

            start += m_iSamplesPerBlock;
            end += m_iSamplesPerBlock;
        }
    }
    qDebug() << m_lData.size();
}


//*************************************************************************************************************

QVariant FiffRawModel::data(const QModelIndex &index, int role) const
{
    // basic sanity checks
    if (index.column() < firstSample()) {
        qDebug() << "[FiffRawModel::data] Nonexistent sample requested, index too small";
        return QVariant();
    }
    else if (index.column() > lastSample())
    {
        qDebug() << "[FiffRawModel::data] Nonexistent sample requested, index too large";
        return QVariant();
    }
    // check whether we need to load new data
    if (index.column() < m_iFiffCursorBegin
            || index.column() >= m_iFiffCursorBegin + (m_iWindowSize + 2 * m_iPaddingSize) * m_iSamplesPerBlock)
    {
        // we do not have the requested data, start to load blocks
        emit startToLoadBlocks(index.column());
        // for now, return null data
        QVariant r;
        r.setValue(0.0);
        return r;
    }
    else {
        // we have already loaded the requested data
        // find out in which block the requested sample is:
        int iWhichBlock = (index.column() - m_iFiffCursorBegin) / m_iSamplesPerBlock;
        int iRelativeSampleIndex = index.column() - (iWhichBlock * m_iSamplesPerBlock);

        QVariant r;
        r.setValue(m_lData[iWhichBlock].first.coeff(index.row(), iRelativeSampleIndex));
        return r;
    }

    qDebug() << "[FiffRawModel::data] Warning, non of the presumed cases took effect";
    return QVariant();
}


//*************************************************************************************************************

Qt::ItemFlags FiffRawModel::flags(const QModelIndex &index) const
{
    // TODO implement stuff
    return QAbstractItemModel::flags(index);
}


//*************************************************************************************************************

QModelIndex FiffRawModel::index(int row, int column, const QModelIndex &parent) const
{
    // TODO implement stuff
    return QModelIndex();
}


//*************************************************************************************************************

QModelIndex FiffRawModel::parent(const QModelIndex &index) const
{
    // TODO implement stuff
    return QModelIndex();
}


//*************************************************************************************************************

int FiffRawModel::rowCount(const QModelIndex &parent) const
{
    // TODO implement stuff
    return 0;
}


//*************************************************************************************************************

int FiffRawModel::columnCount(const QModelIndex &parent) const
{
    // TODO implement stuff
    return 0;
}


//*************************************************************************************************************

bool FiffRawModel::hasChildren(const QModelIndex &parent) const
{
    // TODO implement stuff
    return false;
}


//*************************************************************************************************************

void FiffRawModel::onStartToLoadBlocks(qint32 iCursorRequested)
{
    if (iCursorRequested < m_iFiffCursorBegin) {
        // need to load earlier blocks
        QFuture<int> future = QtConcurrent::run(this, &FiffRawModel::loadEarlierBlocks, iCursorRequested);
        m_blockLoadFutureWatcher.setFuture(future);
    } else if (iCursorRequested >= m_iFiffCursorBegin + (m_iWindowSize + 2 * m_iPaddingSize) * m_iSamplesPerBlock) {
        // need to load later blocks
        QFuture<int> future = QtConcurrent::run(this, &FiffRawModel::loadLaterBlocks, iCursorRequested);
        m_blockLoadFutureWatcher.setFuture(future);
    } else {
        // something went very wrong
        qDebug() << "[FiffRawModel::onStartToLoadBlocks] Something went very wrong ";
    }
}


//*************************************************************************************************************

int FiffRawModel::loadEarlierBlocks(qint32 iCursorRequested)
{
    // calculate distance to loaded area
    qint32 iDist = m_iFiffCursorBegin - iCursorRequested;
    qint32 iBlocksToBeLoaded = iDist / m_iSamplesPerBlock;
    // throw away last blocks
    m_lData.erase(m_lData.end() - iBlocksToBeLoaded, m_lData.end());
    // build data structures to be filled from file
    MatrixXd data, times;
    for(int i = iBlocksToBeLoaded; i > 0; --i) {
        m_lData.prepend(qMakePair(data, times));
    }
    // initialize start and end indices
    int start = m_iFiffCursorBegin;
    int end = start + m_iSamplesPerBlock - 1;

    for(int i = iBlocksToBeLoaded - 1; i >= 0; --i) {
        start -= m_iSamplesPerBlock;
        end -= m_iSamplesPerBlock;
        // read data
        if(m_pFiffIO->m_qlistRaw[0]->read_raw_segment(m_lData[i].first, m_lData[i].second, start, end)) {
            // qDebug() << "[FiffRawModel::loadFiffData] Successfully read a block ";
        } else {
            qDebug() << "[FiffRawModel::loadEarlierBlocks] Could not read block ";
            return -1;
        }
    }

    // adjust fiff cursor
    m_iFiffCursorBegin = start;

    return 0;
}


//*************************************************************************************************************

int FiffRawModel::loadLaterBlocks(qint32 iCursorRequested)
{
    // calculate distance to loaded area
    qint32 iDist = iCursorRequested - m_iFiffCursorBegin;
    qint32 iBlocksToBeLoaded = iDist / m_iSamplesPerBlock;
    // throw away first blocks
    m_lData.erase(m_lData.begin(), m_lData.begin() + iBlocksToBeLoaded);
    // build data structures to be filled from file
    MatrixXd data, times;
    for(int i = 0; i < iBlocksToBeLoaded; ++i) {
        m_lData.append(qMakePair(data, times));
    }
    // initialize start and end indices
    int start = m_iFiffCursorBegin + (m_iWindowSize + 2 * m_iPaddingSize) * m_iSamplesPerBlock;
    int end = start + m_iSamplesPerBlock - 1;

    for(int i = 0; i < iBlocksToBeLoaded; ++i) {
        // read data
        if(m_pFiffIO->m_qlistRaw[0]->read_raw_segment(m_lData[i].first, m_lData[i].second, start, end)) {
            // qDebug() << "[FiffRawModel::loadFiffData] Successfully read a block ";
        } else {
            qDebug() << "[FiffRawModel::loadLaterBlocks] Could not read block ";
            return -1;
        }

        start += m_iSamplesPerBlock;
        end += m_iSamplesPerBlock;
    }

    // adjust fiff cursor
    m_iFiffCursorBegin += iBlocksToBeLoaded * m_iSamplesPerBlock;

    return 0;
}


//*************************************************************************************************************

void FiffRawModel::postBlockLoad(int result)
{
    qDebug() << "[FiffRawModel::postBlockLoad] Background loading returned: " << result;
}
