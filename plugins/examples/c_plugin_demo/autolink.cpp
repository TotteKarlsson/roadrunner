//We only need to give the linker the folder where libs are
//using the pragma comment. Automatic linking, using pragma comment works for MSVC and codegear

#if defined(STATIC_APP)
    #pragma comment(lib, "roadrunner-static.lib")
    #pragma comment(lib, "rrc_api-static.lib")
    #pragma comment(lib, "rrplugins_api-static.lib")
    #pragma comment(lib, "rrplugins_c_api-static.lib")
#else
    #pragma comment(lib, "rrplugins_c_api.lib")
    #pragma comment(lib, "rrc_api.lib")
#endif


//#pragma comment(lib, "poco_foundation-static.lib")

