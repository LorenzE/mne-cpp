//=============================================================================================================
/**
* @file     analyzedata.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017, Christoph Dinh, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains declaration of AnalyzeData Container class.
*
*/

#ifndef ANALYZEDATA_H
#define ANALYZEDATA_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"
#include "../Model/abstractmodel.h"
#include "../Model/surfacemodel.h"
#include "../Utils/types.h"
#include "../Model/qentitylistmodel.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QString>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Qt3DCore {
    class QEntity;
}

namespace INVERSELIB {
    class DipoleFitSettings;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE ANSHAREDLIB
//=============================================================================================================

namespace ANSHAREDLIB
{
    class EcdSetModel;


//*************************************************************************************************************
//=============================================================================================================
// ENUMERATIONS
//=============================================================================================================


//=========================================================================================================
/**
* DECLARE CLASS AnalyzeData
*
* @brief The AnalyzeData class is the base data container.
*/
class ANSHAREDSHARED_EXPORT AnalyzeData : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<AnalyzeData> SPtr;               /**< Shared pointer type for AnalyzeData. */
    typedef QSharedPointer<const AnalyzeData> ConstSPtr;    /**< Const shared pointer type for AnalyzeData. */

    //=========================================================================================================
    /**
    * Constructs the Analyze Data.
    */
    AnalyzeData(QObject* pParent = nullptr);

    //=========================================================================================================
    /**
    * Destroys the Analyze Data.
    */
    ~AnalyzeData();

    //=========================================================================================================
    /**
    * Returns a vector of all loaded models that have the specified type
    *
    * @param[in] mtype              The type to search for
    *
    * @return                       Vector of models that have the specified type
    */
    QVector<QSharedPointer<AbstractModel> > getObjectsOfType(MODEL_TYPE mtype);

    //=========================================================================================================
    /**
    * Returns the requested model.
    * If the path name is not used a nullptr is returned.
    *
    * @param[in] sName               Model name
    *
    * @return                       Pointer to the model
    */
    QSharedPointer<AbstractModel> getModel(const QString &sName);

    //=========================================================================================================
    /**
    * This is the main function for instanciating models. It simply calls the models constructor with the
    * provided path and inserts the model into the hash. NO ERROR CHECKING IS PERFORMED !
    */
    template<class T>
    QSharedPointer<T> loadModel(const QString sPath)
    {
        // check if model was already loaded:
        if (m_data.contains(sPath))
        {
            return qSharedPointerDynamicCast<T>(m_data.value(sPath));
        }
        else
        {
            QSharedPointer<T> sm = QSharedPointer<T>::create(sPath);
            QSharedPointer<AbstractModel> temp = qSharedPointerCast<AbstractModel>(sm);
            m_data.insert(sPath, temp);
            emit this->newModelAvailable(temp);
            return sm;
        }
    }

    //=========================================================================================================
    /**
    * Loads a Surface from the specified filepath (only if the object is not loaded yet)
    *
    * @param[in] sPath               The path of the object to load
    *
    * @return                       SurfaceModel that contains the specified surface
    */
    QSharedPointer<SurfaceModel> loadSurfaceModel(const QString& sPath);

    //=========================================================================================================
    /**
    * Constructs a filepath out of the passed parameters and calls the single-parameter version of loadSurface.
    * Path Construction is copied from Surface::read.
    *
    * @param[in] sSubject_id         Name of subject
    * @param[in] iHemi               Which hemisphere to load {0 -> lh, 1 -> rh}
    * @param[in] sSurf               Name of the surface to load (eg. inflated, orig ...)
    * @param[in] sSubjects_dir       Subjects directory
    *
    * @return                       SurfaceModel that contains the specified surface
    */
    QSharedPointer<SurfaceModel> loadSurfaceModel(const QString &sSubject_id, qint32 iHemi, const QString &sSurf, const QString &sSubjects_dir);

    //=========================================================================================================
    /**
    * Loads a ECD set from the specified filepath (only if the object is not loaded yet)
    *
    * @param[in] sPath               The path of the object to load
    *
    * @return                       EcdSetModel that contains the specified surface
    */
    QSharedPointer<EcdSetModel> loadEcdSetModel(const QString &sPath);

    //=========================================================================================================
    /**
    * Creates a ECD set from the specified settings (only if the DIP path is not used yes).
    * The function returns a nullptr when model loading fails.
    *
    * @param[in] pSettings          The settings used to construct the model.
    * @param[in] sPath              The path where the Model is saved.
    *
    * @return                       EcdSetModel that contains the specified ECD set.
    */
    QSharedPointer<EcdSetModel> loadEcdSetModel(INVERSELIB::DipoleFitSettings *pSettings, const QString &sPath);

    //=========================================================================================================
    /**
    * This creates (and thus registers) a new QEntityListModel that can be used by other parts of MNEAnalyze
    * to display content. A unique name is required, we recommend using the name of the caller (plus further
    * letters / identifiers in case of multiple displays).
    *
    * @param sModelIdentifier       The name of the new model to be created
    * @return                       A shared pointer to the newly created model
    */
    QSharedPointer<QEntityListModel> createQEntityListModel(const QString &sModelIdentifier);

    //=========================================================================================================
    /**
    * This is a convenience function for retrieving all available "displays", meaning all QEntityListModels
    * that were registered so far.
    *
    * @return A Vector of QEntityListModels.
    */
    QVector<QSharedPointer<QEntityListModel> > availableDisplays() const;

    //=========================================================================================================
    /**
    * Removes model stored under the given path.
    *
    * @param[in] sModelPath     The model path.
    */
    void removeModel(const QString &sModelPath);

    //=========================================================================================================
    /**
    * Changes the path where the model is stored.
    *
    * @param[in] sOldModelPath     The old model path.
    * @param[in] sNewModelPath     The new model path.
    */
    void changeModelPath(const QString &sOldModelPath,const QString &sNewModelPath);

    //=========================================================================================================
    /**
    * Returns all models.
    */
    QList<QSharedPointer<AbstractModel>> getModels();

protected:

private:

    QHash<QString, QSharedPointer<AbstractModel> >        m_data;

signals:

    //=========================================================================================================
    /**
    * This is emitted whenever a new model is loaded.
    *
    * @param[in] pModel      The newly available model
    */
    void newModelAvailable(QSharedPointer<AbstractModel> pModel);

    //=========================================================================================================
    /**
    * This is emitted whenever a model is removed.
    *
    * @param[in] sModelPath      The path where the model is stored.
    */
    void modelRemoved(const QString &sModelPath);

    //=========================================================================================================
    /**
    * This is emitted whenever a model changes its path.
    *
    * @param[in] pModel             Pointer to the model.
    * @param[in] sOldModelPath      Old model path.
    * @param[in] sNewModelPath      New model path.
    */
    void modelPathChanged(QSharedPointer<AbstractModel> pModel, const QString &sOldModelPath, const QString &sNewModelPath);
};

} //Namespace

#endif //ANALYZEDATA_H
