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

QEntityListModel::QEntityListModel(const QString &modelIdentifier, QObject *pParent)
    : AbstractModel(pParent),
      m_name(modelIdentifier)
{

}


//*************************************************************************************************************

QVariant QEntityListModel::data(const QModelIndex &index, int role) const
{
    QVariant output;
    if(index.isValid() && role == Qt::DisplayRole) {
        if (index.row() < m_vData.size())
        {
            output.setValue(m_vData.at(index.row()));
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
        return m_vData.size();
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

bool QEntityListModel::addEntityTree(QSharedPointer<QEntity> pTree)
{
    // check whether entity name is valid
    if (pTree->objectName().size() > 0)
    {
        // check whether name is already taken
        for (const QSharedPointer<QEntity>& p : m_vData)
        {
            if (p->objectName().compare(pTree->objectName()) == 0)
            {
                // name found, return false
                return false;
            }
        }
        // could not find name, insert into record
        m_vData.push_back(pTree);
        // emit signal for connected display
        emit entityTreeAdded(createIndex(0, m_vData.size() - 1));
        return true;
    }
    else {
        return false;
    }
}


//*************************************************************************************************************

bool QEntityListModel::removeEntityTree(QSharedPointer<QEntity> pTree)
{
    // check whether entity name is valid
    if (pTree->objectName().size() > 0)
    {
        // check whether name can be found in record
        for (int i = 0; i < m_vData.size(); ++i)
        {
            if (m_vData.at(i)->objectName().compare(pTree->objectName()) == 0)
            {
                // name found
                emit entityTreeRemoved(pTree->objectName());
                m_vData.remove(i);
                return true;
            }
        }
        // could not find name, will return false
    }
    return false;
}
