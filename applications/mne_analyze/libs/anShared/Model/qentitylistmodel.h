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
* This model is not a real model. It enables other extensions to register their 3D content in order to display
* it. This model is responsible for keeping identifiers unique and communicating with the responsible Extension
* (or view). Since we need to keep track of all registered children inside the views tree structure and must be
* able to safely remove them as well, every entry must be named with a unique identifier. A model index is
* passed whenever a new Entity is registered (since I was not able to pass a QSharedPointer using signals /slots)
* and a QString is passed when an Entity is removed.
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
    QEntityListModel(const QString& modelIdentifier, QObject* pParent = nullptr);

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

    //=========================================================================================================
    /**
    * This is called whenever somebody wants to display smth using this model.
    * The newly added Entity must be named. Its name must not already be registered in this model.
    * This method will add the Entity to the model, notify the responsible view and return true when all of
    * the above conditions are met. Otherwise it will return false.
    *
    * @param[in] pTree The Entity to be added.
    * @return Whether the Entity was successfully added
    */
    bool addEntityTree(QSharedPointer<Qt3DCore::QEntity> pTree);

    //=========================================================================================================
    /**
    * This is called whenever somebody wants to remove an Entity that was previously registered.
    * In case the Entity's name can be found, the Entity will be removed from this model, the responsible view
    * will be notified and updated and the method will return true. Otherwise, it will return false.
    *
    * @param[in] pTree The Entity to remove.
    * @return Whether the Entity was successfully removed.
    */
    bool removeEntityTree(QSharedPointer<Qt3DCore::QEntity> pTree);

signals:

    //=========================================================================================================
    /**
    * This is emitted when a Entity Tree was successfully registered.
    *
    * @param index An index to the newly registered Entity
    */
    void entityTreeAdded(const QModelIndex& index);

    //=========================================================================================================
    /**
     * This is emitted when an Entity was successfully removed from the model.
     *
     * @param sIdentifier The name of the Entity to be removed.
     */
    void entityTreeRemoved(const QString& sIdentifier);

protected:

private:

    QString m_name;                                         /**< Name of the model */
    QVector<QSharedPointer<Qt3DCore::QEntity> > m_vData;    /**< All registered QEntities */
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
