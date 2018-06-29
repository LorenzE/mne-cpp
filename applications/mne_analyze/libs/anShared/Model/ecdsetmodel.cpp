//=============================================================================================================
/**
* @file     ecdsetmodel.cpp
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
* @brief    EcdSetModel class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "ecdsetmodel.h"
#include "../Utils/types.h"
#include "../Utils/metatypes.h"
#include <inverse/dipoleFit/ecd_set.h>
#include <inverse/dipoleFit/dipole_fit.h>
#include <inverse/dipoleFit/dipole_fit_settings.h>


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
using namespace INVERSELIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

EcdSetModel::EcdSetModel(DipoleFitSettings *pDipolSettings, const QString &sPath, QObject *pParent)
    : AbstractModel(sPath, pParent)
{
    pDipolSettings->checkIntegrity();
    DipoleFit dipFit(pDipolSettings);
    m_ecdSet = dipFit.calculateFit();
}


//*************************************************************************************************************

EcdSetModel::EcdSetModel(const QString &sDipFileName, QObject *pParent)
    :AbstractModel(sDipFileName, pParent)
{
    m_ecdSet = ECDSet::read_dipoles_dip(sDipFileName);
}


//*************************************************************************************************************

QVariant EcdSetModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= m_ecdSet.size() || index.row() < 0) {
        return QVariant();
    }

    if(role == Qt::DisplayRole) {
        return QVariant::fromValue(m_ecdSet[index.row()]);
    }

    return QVariant();
}


//*************************************************************************************************************

Qt::ItemFlags EcdSetModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        flags |= Qt::ItemNeverHasChildren;
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}


//*************************************************************************************************************

QModelIndex EcdSetModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}


//*************************************************************************************************************

QModelIndex EcdSetModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}


//*************************************************************************************************************

int EcdSetModel::rowCount(const QModelIndex &parent) const
{
    return m_ecdSet.size();
}


//*************************************************************************************************************

bool EcdSetModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}


//*************************************************************************************************************

bool EcdSetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || index.row() >= m_ecdSet.size() || index.row() < 0) {
        return false;
    }

    if(role == Qt::DisplayRole) {
        if(value.canConvert<ECD>()) {
            m_ecdSet[index.row()] = value.value<ECD>();
            emit dataChanged(index, index, QVector<int>{role});
            return true;
        }
    }

    return false;
}


//*************************************************************************************************************

int EcdSetModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}


//*************************************************************************************************************
