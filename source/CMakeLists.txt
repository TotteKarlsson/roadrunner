CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(target roadrunner)

configure_file(rrConfigure.h.in rrConfigure.h)
include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

OPTION(USE_POCO_NET "Use poco net library" ON)

include_directories(
    .
    ${THIRD_PARTY_INSTALL_FOLDER}/include
    ${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
    ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
    ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
    )

set(rrSources
    rrSteadyStateSolver
    Configurable
    rrConstants
    rrException
    rrGetOptions
    rrLogger
    rrExecutableModel
    rrRoadRunner
    rrRoadRunnerOptions
    rrStringList
    rrStringUtils
    rrUtils
    rrCVODEInterface
    rrNLEQInterface
    rrTestSuiteModelSimulation
    rrIniKey
    rrIniSection
    rrIniFile
    rrFileName
    rrRoadRunnerData
    rrSelectionRecord
    rrModelGeneratorFactory
    rrVersionInfo
    rrSparse
    rrSBMLModelSimulation
    rrSBMLReader
    c/mtrand
    c/rrStringBuilder
    c/rrStringListContainer
    c/rrSBMLSymbolDependencies
    c/rrSBMLSymbol
    c/rrHashTable
    c/rrRule
    c/rrModelSymbols
    c/rrSymbol
    c/rrSymbolList
    c/rrCompiledModelState
    c/rrNOMSupport
    c/rrPendingAssignment
    c/rrCompiledModelGenerator
    c/rrCModelGenerator
    c/rrCSharpModelGenerator
    c/rrCodeBuilder
    c/rrCCompiler
    c/rrScanner
    c/rrToken
    c/rrEvent
    c/rrModelSharedLibrary
    c/rrCModelDataUtil
    c/rrCompiledExecutableModel
    c/rrRandom
    conservation/ConservationExtension
    conservation/ConservationDocumentPlugin
    conservation/ConservedMoietyPlugin
    conservation/ConservedMoietyConverter
    )

# Add the LLVM sources to compilation
if(BUILD_LLVM)
    set(rrSources
        ${rrSources}
        llvm/AssignmentRuleEvaluator
        llvm/ASTNodeCodeGen
        llvm/ASTNodeFactory
        llvm/ModelResources
        llvm/CodeGenBase
        llvm/LLVMCompiler
        llvm/EvalConversionFactorCodeGen
        llvm/EvalInitialConditionsCodeGen
        llvm/EvalRateRuleRatesCodeGen
        llvm/EvalReactionRatesCodeGen
        llvm/EventAssignCodeGen
        llvm/EventTriggerCodeGen
        llvm/EventQueue
        llvm/FunctionResolver
        llvm/LLVMExecutableModel
        llvm/GetRateRuleValuesCodeGen
        llvm/GetValuesCodeGen
		llvm/GetInitialValuesCodeGen
        llvm/GetEventValuesCodeGen
        llvm/KineticLawParameterResolver
        llvm/LLVMModelData
        llvm/ModelDataIRBuilder
        llvm/ModelDataSymbolResolver
        llvm/ModelInitialValueSymbolResolver
        llvm/LLVMModelDataSymbols
        llvm/LLVMModelGenerator
        llvm/ModelGeneratorContext
        llvm/LLVMModelSymbols
        llvm/SetRateRuleValuesCodeGen
        llvm/SetValuesCodeGen
        llvm/SetInitialValuesCodeGen
        llvm/SBMLSupportFunctions
        llvm/EvalVolatileStoichCodeGen
        llvm/SBMLInitialValueSymbolResolver
        )
endif(BUILD_LLVM)

# set up linking libs
if(RR_BUILD_SHARED_CORE)
    #Link with static libs
    add_library(${target} SHARED ${rrSources})

    set_property(TARGET ${target}
        PROPERTY  COMPILE_DEFINITIONS
        EXPORT_RR
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_LIBSTRUCT
        STATIC_PUGI
        STATIC_NLEQ
        POCO_STATIC
        POCO_NO_AUTOMATIC_LIBS
        )

    #libraries to link with our shared lib
    if(UNIX)
        set(staticLibExt ".a")
    else()
        set(staticLibExt "")
    endif(UNIX)

    target_link_libraries (${target}
        sundials_cvode
        sundials_nvecserial
        blas
        lapack
        nleq-static
        rr-libstruct-static
        poco_foundation-static
        )

    
    # link with libxml, aperently borland can't link with a 
    # conventional C library, need to be specially converted for borland

    if(UNIX)
	target_link_libraries(${target} xml2)       	 
    elseif(${MSVC})
	target_link_libraries (${target} libxml2)
    elseif(${BORLAND})
        target_link_libraries (${target} libxml2_xe)
    endif()


    if (USE_POCO_NET)
    target_link_libraries (${target}
        poco_net-static
    )
    add_definitions(
        -DWITH_POCO_NET
    )
    endif(USE_POCO_NET)

    if(UNIX)
        target_link_libraries (${target}
            pthread
            sbml
            dl
            f2c${staticLibExt}
            )
    else()
        target_link_libraries (${target}
            libf2c
            libsbml
            )
    endif(UNIX)

    #Libxml is not built. Use a VS converted import file for Borland
    if(${BORLAND})
        target_link_libraries (${target}
            libxml2_xe
            )
    endif()


    if(${INSTALL_CXX_API})
        install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT rr_core
            LIBRARY DESTINATION lib COMPONENT rr_core
            ARCHIVE DESTINATION lib COMPONENT rr_core
            )
    endif()


    if(BUILD_LLVM)
        target_link_libraries(${target}
            ${LLVM_LIBRARIES}
            )
    endif(BUILD_LLVM)

endif(RR_BUILD_SHARED_CORE)

# static allways gets build

add_library(${target}-static STATIC ${rrSources})

set_property(TARGET ${target}-static
    PROPERTY COMPILE_DEFINITIONS
    STATIC_RR
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_PUGI
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

if (USE_POCO_NET)
target_link_libraries (${target}-static
        poco_net-static
    )
    add_definitions(
        -DWITH_POCO_NET
    )
endif()


if(WIN32)
    target_link_libraries (${target}-static
        libsbml-static
        libf2c
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

if(BUILD_LLVM)
    target_link_libraries(${target}-static
        ${LLVM_LIBRARIES}
        )
endif(BUILD_LLVM)

#Libxml we can't build right now
if(${BORLAND})
    target_link_libraries (${target}-static libxml2_xe)
endif()

if(${MSVC})
    target_link_libraries(${target}-static libxml2)
endif()

if(BUILD_TEST_TOOLS)
    if(BUILD_LLVM)
        add_subdirectory(llvm_testing)
    endif(BUILD_LLVM)
    add_subdirectory(testing)
endif()

#--- INSTALL STUFF
if(${INSTALL_CXX_API})

    #if(INSTALL_STATIC_LIB)
    install(TARGETS ${target}-static
        LIBRARY DESTINATION lib COMPONENT rr_core
        ARCHIVE DESTINATION lib COMPONENT rr_core
        )
    #endif()

    #=== HEADERS
    set(subfolders
        .
        #./mtrand
        )

    foreach(subfolder ${subfolders})
        FILE (GLOB headers ${subfolder}/*.h)
        install (   FILES ${headers}
            DESTINATION include/rr/${subfolder}
            COMPONENT rr_core)
    endforeach(subfolder)
endif() #Install CXX API

