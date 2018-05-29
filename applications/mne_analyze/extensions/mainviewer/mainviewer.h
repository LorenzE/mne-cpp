//=============================================================================================================
/**
* @file     mainviewer.h
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
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
* @brief     MainViewer class declaration.
*
*/

#ifndef MAINVIEWEREXTENSION_MAINVIEWER_H
#define MAINVIEWEREXTENSION_MAINVIEWER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "mainviewer_global.h"
#include <anShared/Interfaces/IExtension.h>
#include "centralview.h"
#include "../../libs/anShared/Model/qentitylistmodel.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QtWidgets>
#include <QtCore/QtPlugin>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE MAINVIEWEREXTENSION
//=============================================================================================================

namespace MAINVIEWEREXTENSION {


//*************************************************************************************************************
//=============================================================================================================
// MAINVIEWEREXTENSION FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* This extension is the main device of displaying 3D content.
*
* @brief This extension is the main device of displaying 3D content.
*/
class MAINVIEWERSHARED_EXPORT MainViewer : public ANSHAREDLIB::IExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ansharedlib/1.0" FILE "mainviewer.json") //New Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
    // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
    Q_INTERFACES(ANSHAREDLIB::IExtension)

public:
    typedef QSharedPointer<MainViewer> SPtr;            /**< Shared pointer type for MainViewer. */
    typedef QSharedPointer<const MainViewer> ConstSPtr; /**< Const shared pointer type for MainViewer. */

    //=========================================================================================================
    /**
    * Constructs a MainViewer object.
    */
    MainViewer();

    //=========================================================================================================
    /**
    * Destroys the MainViewer
    */
    ~MainViewer();

    // IExtension functions
    virtual QSharedPointer<IExtension> clone() const override;
    virtual void init() override;
    virtual void unload() override;
    virtual QString getName() const override;
    virtual QMenu* getMenu() override;
    virtual QDockWidget* getControl() override;
    virtual QWidget* getView() override;
    virtual void handleEvent(QSharedPointer<ANSHAREDLIB::Event> e) override;
    virtual QVector<ANSHAREDLIB::EVENT_TYPE> getEventSubscriptions() const override;
    virtual void onNewModelAvailable(QSharedPointer<ANSHAREDLIB::AbstractModel> model) override;

protected:

private slots:

    //=========================================================================================================
    /**
    * This gets connected to the QEntityListmodel m_pModel, so that we will get notified when a new QEntity
    * been added to the scene.
    *
    * @param[in] index An index to the newly added QEntity that is stored inside m_pModel
    */
    void onEntityTreeAdded(const QModelIndex& index);

    //=========================================================================================================
    /**
    * This gets connected to the QEntityListModel m_pModel, so that we will be notified when a QEntity should
    * be removed from the scene.
    *
    * @param[in] sIdentifier The name of the QEntity to remove.
    */
    void onEntityTreeRemoved(const QString &sIdentifier);

private:

    QDockWidget*                                    m_pControl; /**< Control Widget */
    CentralView*                                    m_pView; /**< View */
    QSharedPointer<ANSHAREDLIB::QEntityListModel>   m_pModel; /**< Other extension register their stuff here */
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace MAINVIEWEREXTENSION

#endif // MAINVIEWEREXTENSION_MAINVIEWER_H
