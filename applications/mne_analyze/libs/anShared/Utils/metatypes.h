//=============================================================================================================
/**
* @file     metatypes.h
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     November, 2015
*
* @section  LICENSE
*
* Copyright (C) 2015, Simon Heinke, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief     Register your QMetatypes here.
*
*/

#ifndef METATYPES_H
#define METATYPES_H

#include <Qt3DCore/QEntity>
#include <inverse/dipoleFit/ecd.h>
#include <Eigen/Core>
#include "../Management/event.h"

// Important: Also used qRegisterMetaTypes in AnalyzeCore::registerMetatypes
// to use custom types in QObject::connect() calls.

Q_DECLARE_METATYPE(Eigen::Vector3f);
Q_DECLARE_METATYPE(Eigen::Vector3i);
Q_DECLARE_METATYPE(QSharedPointer<Qt3DCore::QEntity>);
Q_DECLARE_METATYPE(INVERSELIB::ECD);
Q_DECLARE_METATYPE(QSharedPointer<ANSHAREDLIB::Event>);


namespace ANSHAREDLIB {

    //=========================================================================================================
    /**
    * The MODEL_TYPE enum lists all available model types.
    * Naming convention: NAMESPACE_CLASSNAME_MODEL
    */
    enum MODEL_TYPE
    {
        FSLIB_SURFACE_MODEL,
        INVERSE_ECDSET_MODEL
    };

    //=========================================================================================================
    /**
    * Public enum for all available Event types.
    */
    enum EVENT_TYPE
    {
        PING,
        DEFAULT
    };

} //NAMESPACE ANSHAREDLIB

#endif // ENUMS_H
