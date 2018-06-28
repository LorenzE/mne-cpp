//=============================================================================================================
/**
* @file     dipolefit.h
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
* @brief    Contains the declaration of the FiffIO class.
*
*/

#ifndef DIPOLEFITEXTENSION_DIPOLEFIT_H
#define DIPOLEFITEXTENSION_DIPOLEFIT_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dipolefit_global.h"

#include <anShared/Interfaces/IExtension.h>
#include <anShared/Data/dipolefitsettingswrapper.h>
#include <inverse/dipoleFit/dipole_fit_settings.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtWidgets>
#include <QtCore/QtPlugin>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

class DipoleFitControl;

namespace INVERSELIB {
    class DipoleFitSettings;
}

namespace ANSHAREDLIB {
    class Communicator;
    class EcdSetModel;
    class DipoleFitSettingsWrapper;
}

namespace Qt3DCore {
    class QEntity;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DIPOLEFITEXTENSION
//=============================================================================================================

namespace DIPOLEFITEXTENSION
{


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* DipoleFit Extension
*
* @brief The DipoleFit class provides input and output capabilities for the fiff file format.
*/
class DIPOLEFITSHARED_EXPORT DipoleFit : public ANSHAREDLIB::IExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ansharedlib/1.0" FILE "dipolefit.json") //New Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
    // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
    Q_INTERFACES(ANSHAREDLIB::IExtension)

public:
    //=========================================================================================================
    /**
    * Constructs a DipoleFit.
    */
    DipoleFit();

    //=========================================================================================================
    /**
    * Destroys the DipoleFit.
    */
    ~DipoleFit();

    // IExtension functions
    virtual QSharedPointer<IExtension> clone() const;
    virtual void init();
    virtual void unload();
    virtual QString getName() const;

    virtual QMenu* getMenu();
    virtual QDockWidget* getControl();
    virtual QWidget* getView();

    virtual void handleEvent(QSharedPointer<ANSHAREDLIB::Event> e) override;
    virtual QVector<ANSHAREDLIB::EVENT_TYPE> getEventSubscriptions() const override;

protected:

private:

    //=========================================================================================================
    /**
    * This functions creates all connection to the gui.
    */
    void initGuiConnections();

    //=========================================================================================================
    /**
    * Creates a new QEntity tree for the input model.
    *
    * @param[in] pModel     The input model.
    */
    QSharedPointer<Qt3DCore::QEntity> create3DEnityTree(QSharedPointer<ANSHAREDLIB::EcdSetModel> pModel) const;

    void onBrowseButtonClicked();

    void onFitButtonClicked();

    //=========================================================================================================
    /**
    * This functions is called when a new active model is selected.
    *
    * @param[in]        The name of the selected model.
    */
    void onActiveModelSelected(const QString &sModelName);

    //=========================================================================================================
    /**
    * This functions is called when a new model is added to AnalyzeData.
    *
    * @param[in]        Pointer to the model.
    */
    void onNewModelAvalible(QSharedPointer<ANSHAREDLIB::AbstractModel> pNewModel);

    // Control
    QDockWidget*        m_pControl;             /**< Control Widget */
    DipoleFitControl*   m_pDipoleFitControl;    /**< The Dipole Fit Control Widget */

    ANSHAREDLIB::Communicator *m_pCommu;

    QVector<QSharedPointer<ANSHAREDLIB::EcdSetModel>> m_vEcdSetModels;      /**< This vector stores all loaded EcdSetModels. */
    QSharedPointer<ANSHAREDLIB::EcdSetModel> m_pActiveEcdSetModel;          /**< The currently active / displayed EcdSetModel. */
    QSharedPointer<Qt3DCore::QEntity> m_pDipoleRoot;

    //Dipole settings
    ANSHAREDLIB::DipoleFitSettingsWrapper m_dipoleSettings;
};

} // NAMESPACE

#endif // DIPOLEFITEXTENSION_DIPOLEFIT_H
