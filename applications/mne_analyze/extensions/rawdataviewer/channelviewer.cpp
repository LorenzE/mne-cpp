//=============================================================================================================
/**
* @file     channelviewer.cpp
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
* @brief    ChannelViewer class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "channelviewer.h"
#include <anShared/Model/fiffrawmodel.h>
#include <anShared/Utils/metatypes.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QPointF>
#include <QDateTime>
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QPen>
#include <QRandomGenerator>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/qscrollbar.h>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace RAWDATAVIEWEREXTENSION;
using namespace QtCharts;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

ChannelViewer::ChannelViewer(QWidget *parent)
    : QAbstractScrollArea(parent)
    , m_pChart(new QChart)
    , m_iSamplesPerBlock(500)
    , m_iVisibleBlocks(2)
    , m_iBufferBlocks(6)
{
    //Init m_scaleMap
    m_scaleMap["MEG_grad"] = 400 * 1e-15 * 100; //*100 because data in fiff files is stored as fT/m not fT/cm
    m_scaleMap["MEG_mag"] = 1.2 * 1e-12;
    m_scaleMap["MEG_EEG"] = 30 * 1e-06;
    m_scaleMap["MEG_EOG"] = 150 * 1e-06;
    m_scaleMap["MEG_EMG"] = 1 * 1e-03;
    m_scaleMap["MEG_ECG"] = 1 * 1e-03;
    m_scaleMap["MEG_MISC"] = 1 * 1;
    m_scaleMap["MEG_STIM"] = 5 * 1;


    //TODO
    m_pRawModel = QSharedPointer<ANSHAREDLIB::FiffRawModel>::create(QDir::currentPath() + "/MNE-sample-data/MEG/sample/ernoise_raw.fif",
                                                                    m_iSamplesPerBlock,
                                                                    m_iVisibleBlocks,
                                                                    m_iBufferBlocks);
    m_numSeries = m_pRawModel->rowCount();

    m_pChartView = new QChartView(m_pChart);
    m_pChartView->setRubberBand(QChartView::RectangleRubberBand);
    m_pChartView->setRenderHint(QPainter::Antialiasing, false);

    // Vertical scroll bar
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &ChannelViewer::onVerticalScrolling);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    this->verticalScrollBar()->setMaximum(280);
    this->verticalScrollBar()->setMinimum(0);
    this->verticalScrollBar()->setTracking(true);

    //Horizontal scrollbar

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    this->horizontalScrollBar()->setMaximum(m_pRawModel->absoluteLastSample() - m_pRawModel->SampleWindowSize());
    this->horizontalScrollBar()->setMinimum(m_pRawModel->absoluteFirstSample());
    this->horizontalScrollBar()->setValue(m_pRawModel->absoluteFirstSample());
    this->horizontalScrollBar()->setTracking(true);
    connect(this->horizontalScrollBar(), &QScrollBar::valueChanged,
            this, &ChannelViewer::onHorizontalScrolling);

//    m_pHorizontalScrollBar = new QScrollBar(Qt::Horizontal);
//    m_pVerticalScrollBar = new QScrollBar(Qt::Vertical);
//    chartView->addScrollBarWidget(m_pHorizontalScrollBar, Qt::AlignBottom);
//    chartView->addScrollBarWidget(m_pVerticalScrollBar, Qt::AlignLeft);

    m_pXAxis = new QValueAxis;
    //axisX->setRange(0, XYSeriesIODevice::sampleCount);
    m_pXAxis->setLabelFormat("%g");
    m_pXAxis->setTitleText("Samples");
    m_pYAxis = new QCategoryAxis;
    //m_chart->setAxisX(axisX, m_series);
    generateYAxisChannelNames();
    m_pChart->addAxis(m_pXAxis, Qt::AlignBottom);
    m_pChart->addAxis(m_pYAxis, Qt::AlignLeft);

    //m_chart->setAxisY(axisY, m_series);
    m_pChart->legend()->hide();

    for(int i = 0; i < m_numSeries; ++i) {
        QLineSeries *pTempSeries = new QLineSeries;

        QPen pen;
        pen.setStyle(Qt::DotLine);
        pen.setWidthF(0.5);
        pen.setColor(QColor::fromRgb(QRandomGenerator::global()->generate()));
        pTempSeries->setPen(pen);

        pTempSeries->setUseOpenGL(true);
        m_pChart->addSeries(pTempSeries);
        pTempSeries->attachAxis(m_pXAxis);
        pTempSeries->attachAxis(m_pYAxis);
        m_vSeries.append(pTempSeries);
    }
//    QVBoxLayout *mainLayout = new QVBoxLayout(this);

//    mainLayout->addWidget(chartView);
    this->setViewport(m_pChartView);
    this->viewport()->adjustSize();
    generateSeries();

    m_pYAxis->setRange(0.0, 30.0);
    m_pXAxis->setRange(m_pRawModel->absoluteFirstSample(), m_pRawModel->absoluteFirstSample() + m_pRawModel->SampleWindowSize());

    connect(m_pRawModel.data(), &ANSHAREDLIB::FiffRawModel::newBlocksLoaded,
            this, &ChannelViewer::onNewBlocksLoaded);

}


//*************************************************************************************************************

ChannelViewer::~ChannelViewer()
{

}


//*************************************************************************************************************

QSize ChannelViewer::sizeHint() const
{
    // return a very large value, lets hope the user never wants a bigger chart
    return QSize(3000, 3000);
}


//*************************************************************************************************************

void ChannelViewer::resizeEvent(QResizeEvent *event)
{
    // this makes little sense, but it works
    m_pChart->setGeometry(m_pChartView->geometry());
    QAbstractScrollArea::resizeEvent(event);
}


//*************************************************************************************************************

void ChannelViewer::generateSeries()
{
    for(int i = 0; i < m_pRawModel->rowCount(); ++i) {
        QModelIndex modelIndex = m_pRawModel->index(i, 1);

        double dMaxValue = getChannelMaxValue(modelIndex);
        double dScaleY = 1.0 / (2.0 * dMaxValue);

        ANSHAREDLIB::ChannelData channel = m_pRawModel->data(modelIndex).value<ANSHAREDLIB::ChannelData>();

        int iSampleNum = m_pRawModel->currentFirstSample();
        QVector<QPointF> points;
        points.reserve(static_cast<int>(channel.size()));
        for(double channelValue : channel) {
            //TODO remove this we we have correct scaling
            if(channelValue * dScaleY > 3.0 || channelValue * dScaleY < -3.0) {
                //qDebug() << "channel " << i << " " << channelValue * dScaleY;
                channelValue = 0.0;
            }

            QPointF tempPoint(iSampleNum, channelValue * dScaleY + i + 0.5);
            //qDebug() << "channel " << i << " value " << channelValue * dScaleY;
            points.append(std::move(tempPoint));
            iSampleNum++;

        }

        QLineSeries *tempSeries  = m_vSeries[i];
        tempSeries->replace(points);
    }
    m_iCurrentLoadedFirstSample = m_pRawModel->currentFirstSample();
    m_iCurrentLoadedLastSample = m_pRawModel->currentLastSample();
}


//*************************************************************************************************************

void ChannelViewer::generateYAxisChannelNames()
{
    QFont labelsFont;
    labelsFont.setPixelSize(10);
    m_pYAxis->setLabelsFont(labelsFont);
    //m_pYAxis->setLabelFormat()
    double upperBound = 1.0;
    for(int i = 0; i < m_pRawModel->rowCount(); ++i) {
        QModelIndex modelIndex = m_pRawModel->index(i, 0);
        QString channelName = m_pRawModel->data(modelIndex).toString();
        m_pYAxis->append(channelName, upperBound);
        upperBound++;
    }
}

//*************************************************************************************************************


double ChannelViewer::getChannelMaxValue(const QModelIndex &modelIndex)
{
    //TODO change the extraction of the channel type
    //get maximum range of respective channel type (range value in FiffChInfo does not seem to contain a reasonable value)
    qint32 kind = m_pRawModel->m_ChannelInfoList[modelIndex.row()].kind;
    double dMaxValue = 1e-9;

    switch(kind) {
        case FIFFV_MEG_CH: {
            qint32 unit = m_pRawModel->m_pFiffIO->m_qlistRaw[0]->info.chs[modelIndex.row()].unit;
            if(unit == FIFF_UNIT_T_M) {
                dMaxValue = m_scaleMap["MEG_grad"];
            }
            else if(unit == FIFF_UNIT_T)
                dMaxValue = m_scaleMap["MEG_mag"];
            break;
        }
        case FIFFV_EEG_CH: {
            dMaxValue = m_scaleMap["MEG_EEG"];
            break;
        }
        case FIFFV_EOG_CH: {
            dMaxValue = m_scaleMap["MEG_EOG"];
            break;
        }
        case FIFFV_STIM_CH: {
            dMaxValue = m_scaleMap["MEG_STIM"];
            break;
        }
        case FIFFV_EMG_CH: {
            dMaxValue = m_scaleMap["MEG_EMG"];
            break;
        }
        case FIFFV_MISC_CH: {
            dMaxValue = m_scaleMap["MEG_MISC"];
            break;
        }
    }

    return dMaxValue;
}


void ChannelViewer::onVerticalScrolling(int value)
{
    qDebug() << value;
    m_pChart->axisY()->setRange(value, value + 30);
}


void ChannelViewer::onHorizontalScrolling(int scrollWindowBegin)
{
    qDebug() << "onHorizontalScrolling";
    m_pRawModel->updateScrollPosition(scrollWindowBegin);

    qDebug() << "current first loaded sample: " << m_iCurrentLoadedFirstSample;
    qDebug() << "current last loaded sample: " <<m_iCurrentLoadedLastSample;
    qDebug() << "begin of scroll window: " << scrollWindowBegin;
    int scrollWindowEnd = scrollWindowBegin + m_pRawModel->SampleWindowSize();
    qDebug() << "end of sroll window: " << scrollWindowEnd;

    m_pChart->axisX()->setRange(scrollWindowBegin, scrollWindowEnd);
}


void ChannelViewer::onNewBlocksLoaded()
{
    generateSeries();
}



//*************************************************************************************************************
