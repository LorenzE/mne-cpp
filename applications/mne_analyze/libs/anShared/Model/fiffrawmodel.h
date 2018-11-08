//=============================================================================================================
/**
* @file     fiffrawmodel.h
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     October, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief     FiffRawModel class declaration.
*
*/

#ifndef ANSHAREDLIB_FIFFRAWMODEL_H
#define ANSHAREDLIB_FIFFRAWMODEL_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"
#include "../Utils/types.h"
#include "abstractmodel.h"

#include <fiff/fiff.h>
#include <fiff/fiff_io.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QFutureWatcher>
#include <QMutex>
#include <QLinkedList>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace FIFFLIB
{
    class FiffChInfo;
}

class QFile;


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
*
* @brief Model that holds and manages raw fiff data.
*/
class ANSHAREDSHARED_EXPORT FiffRawModel : public AbstractModel
{
    Q_OBJECT

public:
    typedef QSharedPointer<FiffRawModel> SPtr;              /**< Shared pointer type for FiffRawModel. */
    typedef QSharedPointer<const FiffRawModel> ConstSPtr;   /**< Const shared pointer type for FiffRawModel. */

    //=========================================================================================================
    /**
    * Constructs a FiffRawModel object.
    */
    FiffRawModel(QObject *pParent = nullptr);

    //=========================================================================================================
    /**
    * Constructs a FiffRawModel object.
    */
    FiffRawModel(QFile& inFile,
                 qint32 iSamplesPerBlock,
                 qint32 iWindowSize,
                 qint32 iPreloadBufferSize,
                 QObject *pParent = nullptr);

    //=========================================================================================================
    /**
    * Destructs a FiffRawModel.
    */
    ~FiffRawModel();

    //=========================================================================================================
    /**
    * Resets the model and reads new data from inFile
    *
    * @param inFile The file to read data from.
    */
    void initFiffData(QFile& inFile);

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
    * The type of this model (QEntityListModel)
    *
    * @return The type of this model (QEntityListModel)
    */
    inline MODEL_TYPE getType() const override;

    //=========================================================================================================
    /**
    * Return the first sample of the loaded Fiff file
    *
    * @return The first sample of the loaded Fiff file
    */
    inline qint32 firstSample() const;

    //=========================================================================================================
    /**
    * Returns the last sample of the loaded Fiff file
    *
    * @return The last sample of the loaded Fiff file
    */
    inline qint32 lastSample() const;

public slots:

    void updateScrollPosition(qint32 relativeFiffCursor);

private:

    //=========================================================================================================
    /**
    * This is run concurrently
    *
    * @param[in] iCursorRequested Cursor that points to the requested sample
    */
    int loadEarlierBlocks(qint32 numBlocks);

    //=========================================================================================================
    /**
    * This is run concurrently
    *
    * @param[in] iCursorRequested Cursor that points to the requested sample
    */
    int loadLaterBlocks(qint32 numBlocks);

    //=========================================================================================================
    /**
    * This is run by the FutureWatcher when its finished
    *
    * @param[in] result Code value for the result
    */
    void postBlockLoad(int result);

private:

    QLinkedList<QSharedPointer<QPair<MatrixXd, MatrixXd>>> m_lData;    /**< Data */
    QLinkedList<QSharedPointer<QPair<MatrixXd, MatrixXd>>> m_lNewData; /**< Data that is to be appended or prepended */

    qint32 m_iSamplesPerBlock;  /**< Number of samples per block */
    qint32 m_iWindowSize;       /**< Number of blocks per window */
    qint32 m_iPreloadBufferSize;/**< Number of blocks that are preloaded left and right */
    qint32 m_iTotalBlockCount;  /**< Total block count */

    // this always points to the very first sample that is currently held (in the earliest block)
    qint32 m_iFiffCursorBegin;

    // concurrent reloading
    QFutureWatcher<int> m_blockLoadFutureWatcher;   /**< QFutureWatcher for watching process of reloading fiff data. */
    mutable QMutex m_dataMutex;                     /**< Using mutable is not a pretty solution */

     // Fiff
    QSharedPointer<FIFFLIB::FiffIO> m_pFiffIO;      /**< Fiff IO */
    FIFFLIB::FiffInfo::SPtr m_pFiffInfo;            /**< Fiff info of whole fiff file */
    QList<FIFFLIB::FiffChInfo> m_ChannelInfoList;   /**< List of FiffChInfo objects that holds the corresponding channels information */
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline MODEL_TYPE FiffRawModel::getType() const
{
    return MODEL_TYPE::ANSHAREDLIB_FIFFRAW_MODEL;
}


//*************************************************************************************************************

inline qint32 FiffRawModel::firstSample() const {
    if(m_pFiffIO->m_qlistRaw.empty() == false)
        return m_pFiffIO->m_qlistRaw[0]->first_samp;
    else
    {
        qDebug() << "[FiffRawModel::firstSample] Raw list is empty, returning -1";
        return -1;
    }
}


//*************************************************************************************************************

inline qint32 FiffRawModel::lastSample() const {
    if(m_pFiffIO->m_qlistRaw.empty() == false)
        return m_pFiffIO->m_qlistRaw[0]->last_samp;
    else
    {
        qDebug() << "[FiffRawModel::lastSample] Raw list is empty, returning -1";
        return -1;
    }
}


//*************************************************************************************************************
//=============================================================================================================
// CHANNELDATA / CHANNELITERATOR DEFINITION
//=============================================================================================================

/**
* The ChannelData class is meant to serve as a wrapper / container for more convenient access of channel-row data.
* It supports range-based looping (for-each), as well as random access of data.
*/
class ChannelData
{

private:
    QVector<QPair<const double*, qint32> > m_Pairs;
    unsigned long m_NumSamples;

public:

    /**
    * This nested class enables the range-based looping.
    */
    class ChannelIterator : public std::iterator<std::random_access_iterator_tag, const double>
    {
    private:
        const ChannelData* cd;  /**< Pointer to the associated ChannelData container */
        // Remember at which point we are currently (this is NOT the absolute sample number,
        // but the index relativ to all stored samples in the associated ChannelData container):
        unsigned long currentIndex;
        unsigned long currentBlockToAccess; /**< Remember in which block we are currently */
        unsigned long currentRelativeIndex; /**< Remember the relative sample in the current block */

    public:
        ChannelIterator(const ChannelData* cd, unsigned long index)
            : std::iterator<std::random_access_iterator_tag, const double>(),
              cd(cd),
              currentIndex(index),
              currentBlockToAccess(0),
              currentRelativeIndex(0)
        {
            // calculate current block to access and current relative index
            unsigned long temp = currentIndex;
            // comparing temp against 0 to avoid index-out-of bound scenario for ChannelData::end()
            while (temp > 0 && temp >= cd->m_Pairs[currentBlockToAccess].second) {
                temp -= cd->m_Pairs[currentBlockToAccess].second;
                currentBlockToAccess++;
            }

            currentRelativeIndex = temp;
        }

        ChannelIterator& operator ++ (int)
        {
            currentIndex++;
            currentRelativeIndex++;
            if (currentRelativeIndex >= cd->m_Pairs[currentBlockToAccess].second) {
                currentRelativeIndex -= cd->m_Pairs[currentBlockToAccess].second;
                currentBlockToAccess++;
            }

            return *this;
        }

        ChannelIterator& operator ++ ()
        {
            currentIndex++;
            currentRelativeIndex++;
            if (currentRelativeIndex >= cd->m_Pairs[currentBlockToAccess].second) {
                currentRelativeIndex -= cd->m_Pairs[currentBlockToAccess].second;
                currentBlockToAccess++;
            }

            return *this;
        }

        bool operator != (ChannelIterator rhs)
        {
            return currentIndex != rhs.currentIndex;
        }

        const double operator * ()
        {
            return *(cd->m_Pairs[currentBlockToAccess].first + currentRelativeIndex);
        }
    };

    ChannelData(const QVector<QPair<const double*, qint32> >& startAndLengthPairs)
        : m_Pairs(startAndLengthPairs),
          m_NumSamples(0)
    {
        for (const auto &a : m_Pairs) {
            m_NumSamples += a.second;
        }
    }

    // we need a public copy constructor in order to register this as QMetaType
    ChannelData(const ChannelData& other)
          : ChannelData(other.m_Pairs)
    {

    }

    // we need a public default constructor in order to register this as QMetaType
    ChannelData()
          : m_Pairs(),
            m_NumSamples(0)
    {
        // do nothing in default constructor
    }

    // we need a public destructor in order to register this as QMetaType
    ~ChannelData() = default;

    // this is comparatively expensive to call, better use the range based for loop
    double operator [] (unsigned long i)
    {
        // see which block we have to access
        int blockToAccess = 0;
        while (i >= m_Pairs[blockToAccess].second)
        {
            i -= m_Pairs[blockToAccess].second;
            blockToAccess++;
        }

        return *(m_Pairs[blockToAccess].first + i);
    }

    unsigned long size() const {
        return m_NumSamples;
    }

    ChannelIterator begin() const
    {
        static ChannelIterator begin(this, 0);
        return begin;
    }

    ChannelIterator end() const
    {
        static ChannelIterator end(this, m_NumSamples);
        return end;
    }
};


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_FIFFRAWMODEL_H
