//=============================================================================================================
/**
* @file     qentitylistmodel.cpp
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
* @brief    QEntityListModel class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "qentitylistmodel.h"
#include "../Utils/metatypes.h"
#include <iostream>


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
using namespace Qt3DCore;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

QEntityListModel::QEntityListModel(QObject *pParent)
    : AbstractModel(pParent)
{

}


//*************************************************************************************************************

QVariant QEntityListModel::data(const QModelIndex &index, int role) const
{
    QVariant output;
    if(index.isValid() && role == Qt::DisplayRole) {
        if (index.row() < m_vEntries.size())
        {
            output.setValue(m_vEntries.at(index.row()).second);
        }
    }
    return output;
}

//*************************************************************************************************************

Qt::ItemFlags QEntityListModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

//*************************************************************************************************************

QModelIndex QEntityListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        return createIndex(row, column);
    }
    else {
        return QModelIndex();
    }
}

//*************************************************************************************************************

QModelIndex QEntityListModel::parent(const QModelIndex &index) const
{
    //Only the root node has childeren, therefore all parents are an invalid model index
    return QModelIndex();
}

//*************************************************************************************************************

int QEntityListModel::rowCount(const QModelIndex &parent) const
{
    //if parent == root node
    if(!parent.isValid()) {
        return 1;
    }
    else {
        // only the root node has children
        return 0;
    }
}


//*************************************************************************************************************

int QEntityListModel::columnCount(const QModelIndex &parent) const
{
    //if parent == root node
    if(!parent.isValid()) {
        return m_vEntries.size();
    }
    else {
        // only the root node has children
        return 0;
    }
}


//*************************************************************************************************************

bool QEntityListModel::hasChildren(const QModelIndex &parent) const
{
    //if parent == root node
    if(!parent.isValid()) {
        return true;
    }
    else {
        return false;
    }
}


//*************************************************************************************************************

bool QEntityListModel::addEntityTree(QSharedPointer<QEntity> pTree, QString sIdentifier)
{
    for (const QPair<QString, QSharedPointer<QEntity> >& pair : m_vEntries)
    {
        if (sIdentifier.compare(pair.first) == 0)
        {
            // identifier already used, return false
            return false;
        }
    }
    m_vEntries.push_back(qMakePair<QString, QSharedPointer<QEntity> >(sIdentifier, pTree));
    return true;
}


//*************************************************************************************************************

bool QEntityListModel::removeEntityTree(QString sIdentifier)
{
    for (int i = 0; i < m_vEntries.size(); ++i)
    {
        if (sIdentifier.compare(m_vEntries.at(i).first) == 0)
        {
            // identifier found, remove and return true
            m_vEntries.remove(i);
            return true;
        }
    }
    return false;
}
