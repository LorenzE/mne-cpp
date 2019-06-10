//=============================================================================================================
/**
* @file     main.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     April, 2019
*
* @section  LICENSE
*
* Copyright (C) 2019, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Example of using the EDF utilities
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <iostream>
#include <vector>

#include <fiff/fiff_ch_info.h>

#include "edf_info.h"
#include "edf_raw_data.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace EDFINFOEXAMPLE;
using namespace FIFFLIB;
using namespace QtCharts;


//*************************************************************************************************************
//=============================================================================================================
// MAIN
//=============================================================================================================


//=============================================================================================================
/**
* The function main marks the entry point of the program.
* By default, main has the storage class extern.
*
* @param [in] argc (argument count) is an integer that indicates how many arguments were entered on the command line when the program was started.
* @param [in] argv (argument vector) is an array of pointers to arrays of character objects. The array objects are null-terminated strings, representing the arguments that were entered on the command line when the program was started.
* @return the value that was set to exit() (which is 0 if exit() is called via quit()).
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file("C:\\Users\\Simon\\Desktop\\hiwi\\edf_files\\00000929_s005_t000.edf");

    // initialize raw data
    EDFRawData raw(&file);
    // print basic info
    EDFInfo info = raw.getInfo();
    // qDebug().noquote() << info.getAsString();

    FiffChInfo fiffChanInfo = info.getAllChannelInfos()[1].toFiffChInfo();
    qDebug() << fiffChanInfo.scanNo;
    qDebug() << fiffChanInfo.logNo;
    qDebug() << fiffChanInfo.kind;
    qDebug() << fiffChanInfo.range;
    qDebug() << fiffChanInfo.cal;
    // qDebug() << fiffChanInfo.chpos;
    qDebug() << fiffChanInfo.unit;
    qDebug() << fiffChanInfo.unit_mul;
    qDebug() << fiffChanInfo.ch_name;

    /*
    // read some raw data, second 1 to 2
    Eigen::MatrixXf rawChunk = raw.read_raw_segment(1.0f, 2.0f);
    qDebug() << "raw chunk rows: " << rawChunk.rows() << ", raw chunk cols: " << rawChunk.cols();

    // display the data
    QMainWindow* temp = new QMainWindow();
    QChart* chart = new QChart();
    chart->legend()->hide();
    QLineSeries* series = new QLineSeries();
    Eigen::RowVectorXf channelSlice = rawChunk.row(4);  // take 5th channel
    for(int i = 0; i < channelSlice.size(); ++i)
        series->append(i, static_cast<double>(channelSlice[i]));
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Singular EEG Channel");
    QChartView* cView = new QChartView(chart);
    temp->setCentralWidget(cView);
    temp->resize(1000, 300);
    temp->show();
    */

    return app.exec();
}
