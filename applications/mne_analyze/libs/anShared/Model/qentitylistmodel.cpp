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


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DCore/QEntity>


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
    // this is not a real model
    return QVariant();
}

//*************************************************************************************************************

Qt::ItemFlags QEntityListModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

//*************************************************************************************************************

QModelIndex QEntityListModel::index(int row, int column, const QModelIndex &parent) const
{
    // this is not a real model
    return QModelIndex();
}

//*************************************************************************************************************

QModelIndex QEntityListModel::parent(const QModelIndex &index) const
{
    // this is not a real model
    return QModelIndex();
}

//*************************************************************************************************************

int QEntityListModel::rowCount(const QModelIndex &parent) const
{
    // this is not a real model
    return 0;
}


//*************************************************************************************************************

int QEntityListModel::columnCount(const QModelIndex &parent) const
{
    // this is not a real model
    return 0;
}


//*************************************************************************************************************

bool QEntityListModel::hasChildren(const QModelIndex &parent) const
{
    // this is not a real model
    return false;
}


//*************************************************************************************************************

void QEntityListModel::addEntityTree(QSharedPointer<QEntity> pEntity)
{
    // simply emit signal for connected display
    emit entityTreeAdded(pEntity);
}


//*************************************************************************************************************

void QEntityListModel::removeEntityTree(QSharedPointer<QEntity> pEntity)
{
    // simply emit signal for connected display
    emit entityTreeRemoved(pEntity);
}
