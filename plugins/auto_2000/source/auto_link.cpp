
#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "rre_api-static.lib")
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "libAuto-static.lib")
        #pragma comment(lib, "rrplugins_api-static.lib")
        #pragma comment(lib, "rrplugins_c_api-static.lib")
        #pragma comment(lib, "rrAutoInterface-static.lib")

        #pragma comment(lib, "libsbml-static.lib")
        #pragma comment(lib, "libxml2_xe.lib")
        #pragma comment(lib, "sundials_cvode.lib")
        #pragma comment(lib, "sundials_nvecserial.lib")
        #pragma comment(lib, "nleq-static.lib")
        #pragma comment(lib, "libf2c.lib")
        #pragma comment(lib, "lapack.lib")
        #pragma comment(lib, "blas.lib")
        #pragma comment(lib, "rr-libstruct-static.lib")
    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrplugins_api.lib")
        #pragma comment(lib, "rrAutoInterface.lib")
    #endif

//    #pragma comment(lib, "libxml2_xe.lib")
//    #pragma comment(lib, "sundials_cvode.lib")
//    #pragma comment(lib, "sundials_nvecserial.lib")
//    #pragma comment(lib, "libsbml-static.lib")
    #pragma comment(lib, "poco_foundation-static.lib")
#endif


