//=============================================================================================================
/**
* @file     surfacemodel.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     March, 2018
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
* @brief    SurfaceModel class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "surfacemodel.h"
#include "../Utils/types.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;
using namespace Eigen;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

SurfaceModel::SurfaceModel(const QString &sSurfaceFilePath, QObject *pParent)
: AbstractModel(pParent)
, m_pSurfaceData(sSurfaceFilePath)
{

}


//*************************************************************************************************************

QVariant SurfaceModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole) {
        QVariant output;
        // Vertices:
        if(index.internalId() == InternalId::VerticeItem) {
            output.setValue(m_pSurfaceData.vertexAt(index.column()));
            return output;
        }

        // Normals:
        else if(index.internalId() == InternalId::NormalItem) {
            output.setValue(m_pSurfaceData.normalAt(index.column()));
            return output;
        }

        // Triangles:
        else if(index.internalId() == InternalId::TriangleItem) {
            output.setValue(m_pSurfaceData.triAt(index.column()));
            return output;
        }

        // Curvature:
        else if(index.internalId() == InternalId::CurvatureItem) {
            output.setValue(m_pSurfaceData.curvAt(index.column()));
            return output;
        }
    }

   return QVariant();
}


//*************************************************************************************************************

Qt::ItemFlags SurfaceModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}


//*************************************************************************************************************

QModelIndex SurfaceModel::index(int row, int column, const QModelIndex &parent) const
{
    //TODO rework this
    if(!parent.isValid()) {
        if(row == 0) {
            return createIndex(row, column, InternalId::VerticeItem);
        }
        else if(row == 1) {
            return createIndex(row, column, InternalId::NormalItem);
        }
        else if(row == 2) {
            return createIndex(row, column, InternalId::TriangleItem);
        }
        else if(row == 3) {
            return createIndex(row, column, InternalId::CurvatureItem);
        }
    }

    return QModelIndex();
}


//*************************************************************************************************************

QModelIndex SurfaceModel::parent(const QModelIndex &index) const
{
    //Only the root node has childeren, therefore all parents are an invalid model index
    return QModelIndex();
}


//*************************************************************************************************************

int SurfaceModel::rowCount(const QModelIndex &parent) const
{
    //if parent == root node
    if(!parent.isValid()) {
        return 4;
    }
    else {
        // only the root node has children
        return 0;
    }
}


//*************************************************************************************************************

int SurfaceModel::columnCount(const QModelIndex &parent) const
{
    int col = 0;

    switch(parent.internalId()) {
        case InternalId::VerticeItem:
            col = m_pSurfaceData.vertices().rows();
            break;

        case InternalId::NormalItem:
            col = m_pSurfaceData.normals().rows();
            break;

        case InternalId::TriangleItem:
            col = m_pSurfaceData.tris().rows();
            break;

        case InternalId::CurvatureItem:
            col = m_pSurfaceData.curvature().rows();
            break;

        default:
            break;
    }

    return col;
}


//*************************************************************************************************************

bool SurfaceModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}


//*************************************************************************************************************
