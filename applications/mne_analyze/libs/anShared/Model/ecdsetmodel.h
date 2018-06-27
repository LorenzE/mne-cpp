//=============================================================================================================
/**
* @file     ecdsetmodel.h
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
* @brief     EcdSetModel class declaration.
*
*/

#ifndef ANSHAREDLIB_ECDSETMODEL_H
#define ANSHAREDLIB_ECDSETMODEL_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"
#include "abstractmodel.h"
#include <inverse/dipoleFit/ecd_set.h>


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

namespace INVERSELIB {
    class ECDSet;
    class DipoleFitSettings;
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
* This model stores a Electric Current Dipole Set.
* It uses a list model structure to access each ECD.
*
* @brief This model stores a Electric Current Dipole Set.
*/
class ANSHAREDSHARED_EXPORT EcdSetModel : public AbstractModel
{
    Q_OBJECT

public:
    typedef QSharedPointer<EcdSetModel> SPtr;            /**< Shared pointer type for EcdSetModel. */
    typedef QSharedPointer<const EcdSetModel> ConstSPtr; /**< Const shared pointer type for EcdSetModel. */

    //=========================================================================================================
    /**
    * Deleted default constructor.
    */
    EcdSetModel() = delete;

    //=========================================================================================================
    /**
    * Constructs a EcdSetModel object from DipoleFitSettings.
    *
    * @param[in] pDipolSettings     The settings used for construction.
    * @param[in] sPath              The path where the model is stored.
    * @param[in] pParent            Pointer to the parent object.
    */
    EcdSetModel(INVERSELIB::DipoleFitSettings *pDipolSettings, const QString &sPath, QObject *pParent = nullptr);

    //=========================================================================================================
    /**
    * Constructs a EcdSetModel object from a dip format file compatible with mrilab.
    *
    * @param[in] sDipFileName       The dip format file name.
    * @param[in] pParent        Pointer to the parent object.
    */
    EcdSetModel(const QString &sDipFileName, QObject *pParent = nullptr);

    //=========================================================================================================
    /**
    * Default destructor.
    */
    ~EcdSetModel() = default;

    //=========================================================================================================
    // Inherited by QAbstractItemModel:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual bool hasChildren(const QModelIndex &parent) const override;
    //fucntions for an editable model
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

    //=========================================================================================================
    /**
    * Returns the type of this model (SurfaceModel)
    * @return The type of this model (SurfaceModel)
    */
    inline MODEL_TYPE getType() const override;

protected:

private:

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    INVERSELIB::ECDSet  m_ecdSet;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline MODEL_TYPE EcdSetModel::getType() const
{
    return MODEL_TYPE::ANSHAREDLIB_ECDSET_MODEL;
}


//*************************************************************************************************************

} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_ECDSETMODEL_H
