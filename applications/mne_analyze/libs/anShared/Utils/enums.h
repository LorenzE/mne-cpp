#ifndef ENUMS_H
#define ENUMS_H


namespace ANSHAREDLIB {

    //=========================================================================================================
    /**
    * @brief The MODEL_TYPE enum lists all available model types.
    *        Naming convention: NAMESPACE_CLASSNAME_MODEL
    */
    enum MODEL_TYPE
    {
        FSLIB_SURFACE_MODEL
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

}

#endif // ENUMS_H
