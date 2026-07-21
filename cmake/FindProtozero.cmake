#----------------------------------------------------------------------
#
# FindProtozero.cmake
#
# Find the protozero headers.
#
#----------------------------------------------------------------------

find_path(PROTOZERO_INCLUDE_DIR protozero/version.hpp
    PATH_SUFFIXES include
    PATHS ${CMAKE_SOURCE_DIR}/../protozero
)

if(Protozero_FIND_VERSION)
    file(STRINGS "${PROTOZERO_INCLUDE_DIR}/protozero/version.hpp" _version_define REGEX "#define PROTOZERO_VERSION_STRING")
    if("${_version_define}" MATCHES "#define PROTOZERO_VERSION_STRING \"([0-9.]+)\"")
        set(_version "${CMAKE_MATCH_1}")
    else()
        set(_version "unknown")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Protozero
    REQUIRED_VARS PROTOZERO_INCLUDE_DIR
    VERSION_VAR _version)
