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
                           qint32 iBlockSize,
                           qint32 iWindowSize,
                           qint32 iPaddingSize,
                           QObject *pParent)
    : m_iBlockSize(iBlockSize),
      m_iWindowSize(iWindowSize),
      m_iPaddingSize(iPaddingSize),
      AbstractModel(pParent)
{
    loadFiffData(inFile);
}


//*************************************************************************************************************

FiffRawModel::~FiffRawModel()
{

}


//*************************************************************************************************************

void FiffRawModel::loadFiffData(QFile &inFile)
{
    m_pFiffIO = QSharedPointer<FiffIO>::create(inFile);

    // build datastructure that is to be filled with data from the file
    MatrixXd data, times;
    // append a matrix pair for each block: padding left, window, padding right
    for(int i = 0; i < m_iWindowSize + 2 * m_iPaddingSize; ++i) {
        m_lData.append(qMakePair(data, times));
    }

    if(m_pFiffIO->m_qlistRaw.empty()) {
        qDebug() << "[FiffRawmodel::loadFiffData] File " << inFile.fileName() << " does not contain any Fiff data";
        return;
    }
    else {
        //Set cursor somewhere into Fiff file
        m_iFiffCursorBegin = m_pFiffIO->m_qlistRaw[0]->first_samp;

        int start = m_iFiffCursorBegin;
        // for some reason the read_raw_segment function works with inclusive upper bound
        int end = start + m_iBlockSize - 1;

        // read in all needed blocks (padding left, window, padding right)
        for(int i = 0; i < m_iWindowSize + 2 * m_iPaddingSize; ++i) {
            if(m_pFiffIO->m_qlistRaw[0]->read_raw_segment(m_lData[i].first, m_lData[i].second, start, end))
            {
                // qDebug() << "[FiffRawmodel::loadFiffData] Successfully read a block ";
            }
            else {
                qDebug() << "[FiffRawmodel::loadFiffData] Could not read samples " << start << " to " << end;
                return;
            }

            start += m_iBlockSize;
            end += m_iBlockSize;
        }
    }
    qDebug() << m_lData.size();
}


//*************************************************************************************************************

QVariant FiffRawModel::data(const QModelIndex &index, int role) const
{
    // TODO implement stuff
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
