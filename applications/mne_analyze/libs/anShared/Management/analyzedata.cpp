//=============================================================================================================
/**
* @file     analyzedata.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Definition of the Analyze Data Container Class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "analyzedata.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QVector>
#include <QSharedPointer>
#include <QString>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

AnalyzeData::AnalyzeData(QObject *parent)
: QObject(parent)
{

}


//*************************************************************************************************************

AnalyzeData::~AnalyzeData()
{
    // @TODO make sure all objects are safely deleted
}


//*************************************************************************************************************

QVector<QSharedPointer<AbstractModel> > AnalyzeData::getObjectsOfType(AbstractModel::MODEL_TYPE mtype)
{
    // simply iterate over map, number of objects in memory should be small enough
    QVector<QSharedPointer<AbstractModel> > result;
    QHash<QString, QSharedPointer<AbstractModel> >::ConstIterator iter = m_data.begin();
    for (; iter != m_data.end(); iter++)
    {
        if (iter.value()->getType() == mtype)
        {
            result.push_back(iter.value());
        }
    }
    return result;
}

//*************************************************************************************************************


QSharedPointer<SurfaceModel> AnalyzeData::loadSurface(const QString &path)
{
    // check if file was already loaded:
    if (m_data.contains(path))
    {
        return qSharedPointerDynamicCast<SurfaceModel>(m_data.value(path));
    }
    else
    {
        QSharedPointer<SurfaceModel> sm = QSharedPointer<SurfaceModel>::create(path);
        m_data.insert(path, qSharedPointerCast<AbstractModel>(sm));
        return sm;
    }
}

//*************************************************************************************************************


QSharedPointer<SurfaceModel> AnalyzeData::loadSurface(const QString &subject_id, qint32 hemi, const QString &surf, const QString &subjects_dir)
{
    // copied from Surface::read
    QString p_sFile = QString("%1/%2/surf/%3.%4").arg(subjects_dir).arg(subject_id).arg(hemi == 0 ? "lh" : "rh").arg(surf);
    return loadSurface(p_sFile);
}

//*************************************************************************************************************
