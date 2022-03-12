include(GNUInstallDirs)

add_library(triangberg_builder STATIC)
# ALIAS target to export a namespaced target even when building in-tree
add_library(Triangberg::triangberg_builder ALIAS triangberg_builder)
# add source files
add_subdirectory(src)
# library public header files location
target_include_directories(
    triangberg_builder PUBLIC
    # different include directory path depending on if using the local or installed version of library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# pass in version of project as preprocessor definitions
target_compile_definitions(
    triangberg_builder PRIVATE
    -DTRIANGBERG_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DTRIANGBERG_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DTRIANGBERG_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DTRIANGBERG_VERSION_STRING=${TRIANGBERG_ESCAPED_VERSION_STRING}
)
# set up version and soversion for the main library object
set_target_properties(
    triangberg_builder PROPERTIES
    VERSION ${TRIANGBERG_VERSION_STRING}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Triangberg_MAJOR_VERSION ${PROJECT_VERSION_MAJOR}
    INTERFACE_Triangberg_MINOR_VERSION ${PROJECT_VERSION_MINOR}
)
set_property(
    TARGET triangberg_builder
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${Triangberg_MAJOR_VERSION}.${Triangberg_MINOR_VERSION}"
)
# inherit common project compiler options
target_link_libraries(
    triangberg_builder
        PRIVATE
            $<BUILD_INTERFACE:triangberg-compiler-options>
)

# install if we're not being built as a sub-project
if (NOT TRIANGBERG_SUBPROJECT)
    # library
    install(
        TARGETS triangberg_builder
        EXPORT TriangbergTargets
        # when a static library
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a shared library on UNIX
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # when a DLL on Windows
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    # public headers
    install(
        DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    # CMake export
    install(
        EXPORT TriangbergTargets
        FILE TriangbergTargets.cmake
        NAMESPACE Triangberg::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Triangberg"
    )
    # CMake package
    include(CMakePackageConfigHelpers)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/TriangbergConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Triangberg"
    )
    # generate the version file for the config file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/TriangbergConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY SameMinorVersion
    )
    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TriangbergConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TriangbergConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Triangberg"
    )
endif()
