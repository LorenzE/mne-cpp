//=============================================================================================================
/**
* @file     channelviewer.h
* @author   Lars Debor <lars.debor@tu-ilmeau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     October, 2018
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
* @brief     ChannelViewer class declaration.
*
*/

#ifndef RAWDATAVIEWEREXTENSION_CHANNELVIEWER_H
#define RAWDATAVIEWEREXTENSION_CHANNELVIEWER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "rawdataviewer_global.h"



//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QAbstractScrollArea>
#include <QMap>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace QtCharts {
    class QChart;
    class QLineSeries;
    class QValueAxis;
}

class QScrollBar;

class QPointF;
class QTimer;

namespace ANSHAREDLIB {
    class FiffRawModel;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE RAWDATAVIEWEREXTENSION
//=============================================================================================================

namespace RAWDATAVIEWEREXTENSION {


//*************************************************************************************************************
//=============================================================================================================
// RAWDATAVIEWEREXTENSION FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class RAWDATAVIEWERSHARED_EXPORT ChannelViewer : public QAbstractScrollArea
{
    Q_OBJECT

public:
    typedef QSharedPointer<ChannelViewer> SPtr;            /**< Shared pointer type for ChannelViewer. */
    typedef QSharedPointer<const ChannelViewer> ConstSPtr; /**< Const shared pointer type for ChannelViewer. */

    //=========================================================================================================
    /**
    * Constructs a ChannelViewer object.
    */
    ChannelViewer(QWidget *parent = nullptr);

    //=========================================================================================================
    /**
    * Destructor.
    */
    virtual ~ChannelViewer();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:

    void generateSeries();


    double getChannelMaxValue(const QModelIndex &modelIndex);

    void onVerticalScrolling(int value);

    void onHorizontalScrolling(int value);

    void onNewBlocksLoaded();

    QSharedPointer<ANSHAREDLIB::FiffRawModel> m_pRawModel;

    QtCharts::QChart *m_pChart;
    QtCharts::QValueAxis *m_pYAxis;
    QtCharts::QValueAxis *m_pXAxis;
    QVector<QtCharts::QLineSeries*> m_vSeries;
    int m_numSeries;
    int m_iCurrentLoadedFirstSample;
    int m_iCurrentLoadedLastSample;
    QMap<QString, double> m_scaleMap;        /**< Map with all channel types and their current scaling value.*/

    QScrollBar *m_pHorizontalScrollBar;
    QScrollBar *m_pVerticalScrollBar;
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace RAWDATAVIEWEREXTENSION

#endif // RAWDATAVIEWEREXTENSION_CHANNELVIEWER_H
