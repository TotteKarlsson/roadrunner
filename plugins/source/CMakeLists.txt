CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(target rrplugins_api)

include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(
    ${RR_ROOT}/source
    ${THIRD_PARTY_INSTALL_FOLDER}/include
    )

set(sources
rrPluginParameter
rrCapabilities
rrCapability
rrCPlugin
rrCPPPlugin
rrParameters
rrPlugin
rrPluginManager
)


##SHARED VERSION ----------------
##Link with static libs
#add_library(${target} SHARED ${sources})
#
#set_property(TARGET ${target}
#    PROPERTY  COMPILE_DEFINITIONS
#    EXPORT_RRP
#    LIBSBML_STATIC
#    LIBLAX_STATIC
#    STATIC_LIBSTRUCT
#    STATIC_PUGI
#    STATIC_NLEQ
#    POCO_STATIC
#    POCO_NO_AUTOMATIC_LIBS
#    )
#
##libraries to link with our shared lib
#if(UNIX)
#    set(staticLibExt ".a")
#else()
#    set(staticLibExt "")
#endif(UNIX)
#
#target_link_libraries (${target}
#    sundials_cvode
#    sundials_nvecserial
#    blas
#    lapack
#    nleq-static
#    rr-libstruct-static
#    poco_foundation-static
#    )
#
#if(UNIX)
#    target_link_libraries (${target}
#        pthread
#        sbml
#        dl
#        f2c${staticLibExt}
#        )
#else()
#    target_link_libraries (${target}
#        libf2c
#        libsbml
#        )
#endif(UNIX)
#
#install(TARGETS ${target}
#        RUNTIME DESTINATION bin COMPONENT plugins_api
#        LIBRARY DESTINATION lib COMPONENT plugins_api
#        ARCHIVE DESTINATION lib COMPONENT plugins_api
#        )
#
##END OF SHARED VERSION ----------------

#STATIC VERSION
add_library(${target}-static STATIC ${sources})

set_property(TARGET ${target}-static
    PROPERTY COMPILE_DEFINITIONS
#    STATIC_RR
    STATIC_PLUGINS_API
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_NLEQ
    POCO_STATIC
    POCO_NO_AUTOMATIC_LIBS
    )

#libraries to link with static lib
#======== We need to split up this one to satisfy linking in both windows and posix.
#=== windows need libsbml-static and libf2c and posix uses sbml-static and f2c
target_link_libraries (${target}-static
    sundials_cvode
    sundials_nvecserial
    blas
    lapack
    nleq-static
    rr-libstruct-static
    poco_foundation-static
    )

if(WIN32)
    target_link_libraries (${target}-static
        libsbml-static
        libf2c
        iphlpapi
        )
endif(WIN32)


if(UNIX)
    target_link_libraries (${target}-static
        sbml-static
        f2c
        xml2
        dl
        pthread
        )
endif(UNIX)

#Libxml we can't build right now
if(${BORLAND})
    target_link_libraries (${target}-static
        libxml2_xe
        )
endif()

if(${MSVC})
    target_link_libraries(${target}-static libxml2) 
    #Disable some VS Warnings 
    #           4251 About exporting std classes
    #           4018 Comparing unsigned/signed ints
    #           4996 Deprecated functions   
add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4251 /wd4018 /wd4996 /nologo" )

endif()

if(BUILD_TESTS)
    add_subdirectory(testing)
endif()

#--- INSTALL STUFF
install(TARGETS ${target}-static
    LIBRARY DESTINATION lib COMPONENT rrplugins_api
    ARCHIVE DESTINATION lib COMPONENT rrplugins_api
    )

#=== HEADERS
set(folders
    .
    )

foreach(folder ${folders})
    FILE (GLOB headers ${folder}/*.h)
    install (   
        FILES ${headers}
        DESTINATION include/pluginslib/cpp
        COMPONENT pluginsLib
        )
endforeach(folder)

