//=============================================================================================================
/**
* @file     view3dsurfer.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     August, 2018
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
* @brief     View3DSurfer class declaration.
*
*/

#ifndef SURFEREXTENSION_VIEW3DSURFER_H
#define SURFEREXTENSION_VIEW3DSURFER_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <mne/mne_bem_surface.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QWidget>
#include <QPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Qt3DRender {
    class QGeometryRenderer;
    class QPickEvent;
}

namespace Qt3DCore {
    class QEntity;
    class QTransform;
}

namespace Qt3DExtras {
    class QSphereMesh;
}

class QGridLayout;
class QAbstractItemModel;
class QItemSelectionModel;

namespace DISP3DLIB {
    class CustomMesh;
}

namespace ANSHAREDLIB {
    class AbstractModel;
}

//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE SURFEREXTENSION
//=============================================================================================================

namespace SURFEREXTENSION {


//*************************************************************************************************************
//=============================================================================================================
// SURFEREXTENSION FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class View3DSurfer : public QWidget
{

public:
    typedef QSharedPointer<View3DSurfer> SPtr;            /**< Shared pointer type for View3DSurfer. */
    typedef QSharedPointer<const View3DSurfer> ConstSPtr; /**< Const shared pointer type for View3DSurfer. */

    //=========================================================================================================
    /**
    * Constructs a View3DSurfer object.
    */
    View3DSurfer();

    //=========================================================================================================
    /**
    * Default destructor.
    */
    ~View3DSurfer() = default;

    //=========================================================================================================
    /**
    * Sets the model for the view to present.
    *
    * This function will create and set a new selection model,
    * replacing any model that was previously set with setSelectionModel().
    * However, the old selection model will not be deleted as it may be shared between several views.
    *
    * @param[in]    pModel   The new item model.
    */
    void setModel(QSharedPointer<ANSHAREDLIB::AbstractModel> pModel);

    //=========================================================================================================
    /**
    * Sets the current selection model to the given selectionModel.
    *
    * Note that, if you call setModel() after this function,
    * the given selectionModel will be replaced by one created by the view.
    *
    * @param[in]    pSelectionModel     The new selection model.
    */
    void setSelectionModel(QItemSelectionModel *pSelectionModel);

private:

    //=========================================================================================================
    /**
    * Initializes the 3d view.
    */
    void init();

    //=========================================================================================================
    /**
    * Creates the QEntity tree for the scene.
    */
    Qt3DCore::QEntity *createEntityTree();

    //=========================================================================================================
    /**
    * This function creates a mesh from the data given by a SurfaceModel.
    */
    void updateSurfaceModelMesh();

    Qt3DCore::QEntity *createLightEntity();

    void testPicking(Qt3DRender::QPickEvent *event);

    inline float squared(float x);

    //Layout
    QWidget *m_view3d_container;
    QGridLayout *m_view3d_gridlayout;

    DISP3DLIB::CustomMesh *m_pSurfaceMesh;

    Qt3DExtras::QSphereMesh *m_pointMesh;
    Qt3DCore::QTransform *pSphereTransform;

    QSharedPointer<QAbstractItemModel> m_pItemModel;
    QPointer<QItemSelectionModel> m_pSelectionModel;

};

//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


inline float View3DSurfer::squared(float x)
{
    return x * x;
}

} // namespace SURFEREXTENSION

#endif // SURFEREXTENSION_VIEW3DSURFER_H
