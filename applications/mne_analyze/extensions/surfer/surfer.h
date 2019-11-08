//=============================================================================================================
/**
* @file     surfer.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017 Christoph Dinh, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the declaration of the Surfer class.
*
*/

#ifndef SURFER_H
#define SURFER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "surfer_global.h"
#include <anShared/Interfaces/IExtension.h>
#include <anShared/Management/communicator.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtWidgets>
#include <QtCore/QtPlugin>
#include <QPickingSettings>
#include <QMap>
#include <QPair>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class SurferControl;

namespace Qt3DCore {
    class QEntity;
    class QTransform;
}

namespace Qt3DRender {
    class QPickEvent;
}

namespace DISP3DLIB {
    class CustomMesh;
}

namespace ANSHAREDLIB {
    class SurfaceModel;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE SURFEREXTENSION
//=============================================================================================================

namespace SURFEREXTENSION
{


typedef QPair<QSharedPointer<ANSHAREDLIB::SurfaceModel>, QSharedPointer<Qt3DCore::QEntity> > LoadedSurface;


//=============================================================================================================
/**
* The surfer extension allows loading, managing and displaying of surface files.
* It communicates with an instance of SurferControl in order to pass user arguments back and forth.
*
*/
class SURFERSHARED_EXPORT Surfer : public ANSHAREDLIB::IExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ansharedlib/1.0" FILE "surfer.json") //New Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
    // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
    Q_INTERFACES(ANSHAREDLIB::IExtension)

public:
    //=========================================================================================================
    /**
    * Constructs a Surfer.
    */
    Surfer();

    //=========================================================================================================
    /**
    * Destroys the Surfer.
    */
    ~Surfer();

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

private slots:

    //=========================================================================================================
    /**
    * This is called by the SurferControl (via a connect).
    * It opens a file dialog so that the user can choose the file to load.
    */
    void onLoadNewSurface();

    //=========================================================================================================
    /**
    * This is called by the SurferControl (via a connect).
    * It toggles visibility for the corresponding surface (see m_mLoadedSurfaces).
    *
    * @param[in] pItem The Item that was changed.
    */
    void onSurfaceVisibilityChanged(const QListWidgetItem* pItem);

    //=========================================================================================================
    /**
    * This is called by the SurferControl (via a connect).
    * It removes the corresponding surface (see m_mLoadedSurfaces) and destructs its entity tree.
    * CAUTION this method will DELETE the passed pointer. See SurferControl::on_removeSurfaceButton_released for
    * more details
    *
    * @param[in] pItem The Item that was removed.
    */
    void onRemoveSurface(QListWidgetItem* pItem);

private:

    //=========================================================================================================
    /**
    * Returns the squared of x.
    *
    * @param[in]    the number that should be squared.
    *
    * @returns the squared of x.
    */
    inline float squared(float x);

    //=========================================================================================================
    /**
    * This is a helper function for loading a new surface. This includes registration whithin AnalyzeData,
    * creation of a new item inside SurferControl's listview and construction of a new entity tree that is added
    * below the Surfer's root.
    *
    * @param[in] filePath The path of the surface to load.
    */
    void helpLoadNewSurface(const QString& filePath);

    //=========================================================================================================
    /**
    * This fills the passed mesh with data taken from the passed SurfaceModel.
    *
    * @param[in] pModel The model that the data is read from.
    * @param[in, out] pMesh The mesh to be filled.
    */
    void updateSurfaceModelMesh(QSharedPointer<ANSHAREDLIB::SurfaceModel> pModel, DISP3DLIB::CustomMesh* pMesh);

    //=========================================================================================================
    /**
    * This sets the selection for the surfer (not implemented yet)
    *
    * @param[in] pSelectionModel The new selection model for the surfer
    */
    void setSelectionModel(QItemSelectionModel *pSelectionModel);

    //=========================================================================================================
    /**
    * This enables user interaction
    *
    * @param[in] event The event that has happened
    */
    void onClick(Qt3DRender::QPickEvent *event);

    //=========================================================================================================

    ANSHAREDLIB::Communicator* m_pCommu;                /**< Connection to the event system */

    // Control
    QDockWidget*        m_pDock;                     /**< Control Dock */
    SurferControl*      m_pSurferControl;               /**< Control Widget */

    // entity tree root
    QSharedPointer<Qt3DCore::QEntity> m_pSurferRoot;    /**< Surfer root */

    /**
    * This connects the SurferControl's item list with the data stored inside the Surfer.
    * It utilizes pointers to items for keys, this enables easy communication and synchronisation between
    * SurferControl and Surfer.
    */
    QMap<const QListWidgetItem*, LoadedSurface> m_mLoadedSurfaces;
};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


inline float Surfer::squared(float x)
{
    return x * x;
}

} // NAMESPACE

#endif // SURFER_H
