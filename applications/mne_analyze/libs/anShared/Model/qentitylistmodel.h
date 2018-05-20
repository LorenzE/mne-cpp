//=============================================================================================================
/**
* @file     qentitylistmodel.h
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
* @brief     QEntityListModel class declaration.
*
*/

#ifndef ANSHAREDLIB_QENTITYLISTMODEL_H
#define ANSHAREDLIB_QENTITYLISTMODEL_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "abstractmodel.h"
#include "../Utils/types.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QVector>
#include <QPair>
#include <QString>
#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Qt3DCore {
    class QEntity;
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
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class QEntityListModel : public AbstractModel
{
    Q_OBJECT

public:
    typedef QSharedPointer<QEntityListModel> SPtr;            /**< Shared pointer type for QEntityListModel. */
    typedef QSharedPointer<const QEntityListModel> ConstSPtr; /**< Const shared pointer type for QEntityListModel. */

    //=========================================================================================================
    /**
    * Constructs a QEntityListModel object.
    */
    QEntityListModel(QObject* pParent = nullptr);

    //=========================================================================================================
    /**
    * Default destructor.
    */
    ~QEntityListModel() = default;

    //=========================================================================================================
    /**
    * Returns the data stored under the given role for the index.
    * Currently only Qt::DisplayRole is supported
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

    bool addEntityTree(QSharedPointer<Qt3DCore::QEntity> pTree, QString sIdentifier);
    bool removeEntityTree(QString sIdentifier);

protected:

private:

    QVector<QPair<QString, QSharedPointer<Qt3DCore::QEntity> > > m_vEntries;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline MODEL_TYPE QEntityListModel::getType() const
{
    return MODEL_TYPE::ANSHAREDLIB_QENTITYLIST_MODEL;
}

} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_QENTITYLISTMODEL_H
