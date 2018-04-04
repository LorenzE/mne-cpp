//=============================================================================================================
/**
* @file     surfacemodel.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     March, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor and Matti Hamalainen. All rights reserved.
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

SurfaceModel::SurfaceModel(SurfaceData* pSurfaceData, QObject *pParent)
: QAbstractItemModel(pParent)
, m_pSurfaceData(pSurfaceData)
{

}


//*************************************************************************************************************

QVariant SurfaceModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole) {
        QVariant output;
        // Vertices:
        if(index.row() == 0) {
            output.setValue(m_pSurfaceData->vertexAt(index.column()));
            return output;
        }

        // Normals:
        if(index.row() == 1) {
            output.setValue(m_pSurfaceData->normalAt(index.column()));
            return output;
        }

        // Triangles:
        if(index.row() == 2) {
            output.setValue(m_pSurfaceData->triAt(index.column()));
            return output;
        }

        // Curvature:
        if(index.row() == 3) {
            output.setValue(m_pSurfaceData->curvAt(index.column()));
            return output;
        }
    }

   return QVariant();

}


//*************************************************************************************************************

Qt::ItemFlags SurfaceModel::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}


//*************************************************************************************************************

QVariant SurfaceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}


//*************************************************************************************************************

QModelIndex SurfaceModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)) {
        return QModelIndex();
    }


    if(!parent.isValid()) {
        if(row == 0) {

        }
    }
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
    return 2;
}


//*************************************************************************************************************
