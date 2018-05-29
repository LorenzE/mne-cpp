//=============================================================================================================
/**
* @file     centralview.h
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief     CentralView class declaration.
*
*/

#ifndef MAINVIEWEREXTENSION_CENTRALVIEW_H
#define MAINVIEWEREXTENSION_CENTRALVIEW_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QWidget>
#include <QGridLayout>
#include <Qt3DCore>
#include <QCloseEvent>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Qt3DRender {
    class QPickEvent;
}

namespace Qt3DCore {
    class QEntity;
}

namespace Qt3DExtras {
    class QSphereMesh;
}

namespace DISP3DLIB {
    class CustomMesh;
}


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
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class CentralView : public QWidget
{

public:
    typedef QSharedPointer<CentralView> SPtr;            /**< Shared pointer type for CentralView. */
    typedef QSharedPointer<const CentralView> ConstSPtr; /**< Const shared pointer type for CentralView. */

    //=========================================================================================================
    /**
    * Constructs a CentralView object.
    */
    CentralView();

    //=========================================================================================================
    /**
    * Default destructor
    */
    ~CentralView() = default;

    //=========================================================================================================
    /**
    * This will insert the passed QEntity below the views root.
    *
    * @param[in] pEntity The QEntity to be added.
    */
    void addEntity(QSharedPointer<Qt3DCore::QEntity> pEntity);

    //=========================================================================================================
    /**
    * This will remove the child named with sIdentifier or give out a warning in case the child could not be found.
    *
    * @param[in] sIdentifier The name of the child to be removed.
    */
    void removeEntity(const QString& sIdentifier);

    //=========================================================================================================
    /**
    * We override this since the default implementation crashes
    *
    * @param[in] event The event that has happened
    */
    void closeEvent(QCloseEvent *event) override;

protected:

private:

    //=========================================================================================================
    /**
    * Initializes the 3D view.
    */
    void init();

    QWidget *m_view3d_container;            /**< Container */
    QGridLayout *m_view3d_gridlayout;       /**< Layout */

    Qt3DCore::QEntity *m_pRootEntity;       /**< Root entity */
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace MAINVIEWEREXTENSION

#endif // MAINVIEWEREXTENSION_CENTRALVIEW_H
