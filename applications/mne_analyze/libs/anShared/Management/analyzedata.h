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
#include "../Utils/enums.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================



//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE ANSHAREDLIB
//=============================================================================================================

namespace ANSHAREDLIB
{


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
    AnalyzeData(QObject* parent = nullptr);

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
    * Loads a Surface from the specified filepath (only if the object is not loaded yet)
    *
    * @param[in] path               The path of the object to load
    *
    * @return                       SurfaceModel that contains the specified surface
    */
    QSharedPointer<SurfaceModel> loadSurface(const QString& path);

    //=========================================================================================================
    /**
    * Constructs a filepath out of the passed parameters and calls the single-parameter version of loadSurface.
    * Path Construction is copied from Surface::read.
    *
    * @param[in] subject_id         Name of subject
    * @param[in] hemi               Which hemisphere to load {0 -> lh, 1 -> rh}
    * @param[in] surf               Name of the surface to load (eg. inflated, orig ...)
    * @param[in] subjects_dir       Subjects directory
    *
    * @return                       SurfaceModel that contains the specified surface
    */
    QSharedPointer<SurfaceModel> loadSurface(const QString &subject_id, qint32 hemi, const QString &surf, const QString &subjects_dir);

protected:

private:

    QHash<QString, QSharedPointer<AbstractModel> >        m_data;

signals:

    void newModelAvailable(QSharedPointer<AbstractModel> model);
};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

} //Namespace

#endif //ANALYZEDATA_H
