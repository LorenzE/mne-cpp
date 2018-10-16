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


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QPointF>
#include <QDateTime>
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>

#include <QtWidgets/QVBoxLayout>


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
    : QWidget(parent)
    , m_chart(new QChart)
    , m_series(new QLineSeries)
    , m_timer(new QTimer)
{
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(800, 600);
    m_chart->addSeries(m_series);

    QValueAxis *axisX = new QValueAxis;
    //axisX->setRange(0, XYSeriesIODevice::sampleCount);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTitleText("Audio level");
    //m_chart->setAxisX(axisX, m_series);
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisX);
    m_series->attachAxis(axisY);
    //m_chart->setAxisY(axisY, m_series);
    m_chart->legend()->hide();
    m_chart->setTitle("Data ");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView);

    //Connect temporary data source
    connect(m_timer, &QTimer::timeout, this, &ChannelViewer::generateNewData);
    m_timer->setSingleShot(false);
    m_timer->start(100);
}

ChannelViewer::~ChannelViewer()
{
    m_timer->stop();
}

void ChannelViewer::generateNewData()
{
    qint64 secSinceEpoch = QDateTime::currentMSecsSinceEpoch();

    //generate numbers between -1.0 and 1.0
    float newValue = QRandomGenerator::global()->bounded(2.0) - 1.0;

    addNewDataPoint(newValue, secSinceEpoch);
}

void ChannelViewer::addNewDataPoint(float fValue, qint64 secSinceEpoch)
{
    qDebug() << m_series->count();
    if(m_series->count() >= m_maxNumPoints) {
        QVector<QPointF> points = m_series->pointsVector();
        points.removeFirst();
        m_series->replace(points);
    }
    m_chart->axisX()->setRange(m_series->at(0).x(), secSinceEpoch);
    qDebug() << "Data added: " << fValue << " " << secSinceEpoch;
    m_series->append(secSinceEpoch, fValue);

}


//*************************************************************************************************************
