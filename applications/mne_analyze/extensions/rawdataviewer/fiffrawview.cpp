//=============================================================================================================
/**
* @file     fiffrawview.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>
*           Lars Debor <lars.debor@tu-ilmenau.de>
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2018
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
* @brief    Definition of FiffRawView Class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "fiffrawview.h"

#include "fiffrawdelegate.h"
#include <anShared/Model/fiffrawmodel.h>


//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace RAWDATAVIEWEREXTENSION;
using namespace ANSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

FiffRawView::FiffRawView(QWidget *parent)
    : QWidget(parent)
    , m_pTableView(Q_NULLPTR)
{
    m_pTableView = new QTableView;

    //set vertical layout
    QVBoxLayout *neLayout = new QVBoxLayout(this);

    neLayout->addWidget(m_pTableView);

    //set layouts
    this->setLayout(neLayout);
}


//*************************************************************************************************************

FiffRawView::~FiffRawView()
{

}


//*************************************************************************************************************

void FiffRawView::setModel(QSharedPointer<FiffRawModel> pModel)
{
    m_pTableView->setModel(pModel.data());
}


void FiffRawView::setDelegate(QSharedPointer<FiffRawDelegate>& pDelegate)
{
    m_pTableView->setItemDelegate(pDelegate.data());
}
