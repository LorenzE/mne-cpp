//=============================================================================================================
/**
* @file     surfacemodel.h
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
* @brief     SurfaceModel class declaration.
*
*/

#ifndef ANSHAREDLIB_SURFACEMODEL_H
#define ANSHAREDLIB_SURFACEMODEL_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"
#include "abstractmodel.h"
#include "../Utils/types.h"
#include <fs/surface.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace FSLIB {
    class Surface;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE ANSHAREDLIB
//=============================================================================================================

namespace ANSHAREDLIB {


//*************************************************************************************************************
//=============================================================================================================
// ANSHAREDLIB FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* This model is used to access surface data.
* The Model is structured like this.
*
*           root
*             |
*     row = 0 |--Vertices [column]
*             |
*     row = 1 |--Normals [column]
*             |
*     row = 2 |--Triangles [column]
*             |
*     row = 3 |--Curvatures [column]
*
* @brief This model is used to access surface data.
*/
class ANSHAREDSHARED_EXPORT SurfaceModel : public AbstractModel
{
    Q_OBJECT

public:
    typedef QSharedPointer<SurfaceModel> SPtr;            /**< Shared pointer type for SurfaceModel. */
    typedef QSharedPointer<const SurfaceModel> ConstSPtr; /**< Const shared pointer type for SurfaceModel. */

private:
    enum  InternalId {
        VerticeItem = 1,
        NormalItem = 2,
        TriangleItem = 3,
        CurvatureItem = 4,
    };

public:

    //=========================================================================================================
    /**
    * Deleted default constructor.
    */
    SurfaceModel() = delete;

    //=========================================================================================================
    /**
    * Constructs a SurfaceModel object from a given SurfaceData object.
    *
    * @param[in] sSurfaceFilePath   The path used in Surface class for data loading.
    * @param[in] pParent            The parent of this object.
    */
    explicit SurfaceModel(const QString& sSurfaceFilePath, QObject* pParent = nullptr);

    //=========================================================================================================
    /**
    * Default destructor.
    */
    ~SurfaceModel() = default;

    //=========================================================================================================
    /**
    * Returns the data stored under the given role for the index.
    * Currently on Qt::DisplayRole is supported
    *
    * @param[in] index   The index that referres to the requested item.
    * @param[in] role    The requested role.
    */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //=========================================================================================================
    /**
    * Returns the item flags for the given index.
    *
    * @param[in] index   The index that referres to the requested item.
    */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //=========================================================================================================
    /**
    * Returns the index for the item in the model specified by the given row, column and parent index.
    *
    * @param[in] row      The specified row.
    * @param[in] column   The specified column.
    * @param[in] parent   The parent index.
    */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    //=========================================================================================================
    /**
    * Returns the parent index of the given index.
    * In this Model the parent index in always QModelIndex().
    *
    * @param[in] index   The index that referres to the child.
    */
    QModelIndex parent(const QModelIndex &index) const override;

    //=========================================================================================================
    /**
    * Returns the number of childeren for the parent node.
    *
    * @param[in] parent     The parent index.
    */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //=========================================================================================================
    /**
    * Returns the number of objects stored in the node.
    *
    * @param[in] parent     The index of the requested node.
    */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    //=========================================================================================================
    /**
    * Returns true if parent has any children; otherwise returns false.
    *
    * @param[in] parent     The index of the parent node.
    */
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    //=========================================================================================================
    /**
    * @brief getType The type of this model (SurfaceModel)
    * @return The type of this model (SurfaceModel)
    */
    inline MODEL_TYPE getType() const override;

private:

    FSLIB::Surface    m_pSurfaceData;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline MODEL_TYPE SurfaceModel::getType() const
{
    return MODEL_TYPE::ANSHAREDLIB_SURFACE_MODEL;
}

} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_SURFACEMODEL_H
