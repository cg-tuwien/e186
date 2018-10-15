#.rst:
#
# ===========
# Find Assimp
# ===========
#
# Try to find Assimp: Open Asset Import Library.
#
# The module defines the following variables:
# - ASSIMP_FOUND - True if Assimp development files were found
# - ASSIMP_INCLUDE_DIR - Assimp include directories
# - ASSIMP_LIBRARIES
# - ASSIMP_LIBRARY_RELEASE - Assimp libraries to link in release configuration
# - ASSIMP_LIBRARY_DEBUG - Assimp libraries to link in debug configuration
# - ASSIMP_DLL_RELEASE - Assimp DLL library (Windows only)
# - ASSIMP_DLL_DEBUG - Assimp DLL library (Windows only)
#
# The following variables can be set as arguments for the module.
# - ASSIMP_ROOT_DIR: Root library directory of Assimp
#

# Additional modules
#include(FindPackageHandleStandardArgs)
include(ArchBitSize)
include(XPlatform)


Set(_PF86 "PROGRAMFILES(x86)")
SET(ASSIMP_SEARCH_PATHS
    # Top project's local external libraries
    ${CMAKE_SOURCE_DIR}/external/assimp
    # Current project's local external libraries
    ${PROJECT_SOURCE_DIR}/external/assimp
    # Unix like systems
    /usr/local /usr /opt
    # Windows specific
    "$ENV{PROGRAMFILES}"
    "$ENV{_PF86}"
    "$ENV{SYSTEMDRIVE}"
    # MacOS specific
    ~/Library/Frameworks /Library/Frameworks
    # Other exotic options (Fink, DarwinPorts & Blastwave)
    /sw /opt/local /opt/csw
)

if (NOT DEFINED ASSIMP_ROOT_DIR)
    FIND_PATH(ASSIMP_ROOT_DIR include/assimp/version.h
        PATH_SUFFIXES
            assimp ASSIMP
        PATHS
            ${ASSIMP_SEARCH_PATHS}
    )
endif (NOT DEFINED ASSIMP_ROOT_DIR)

SET(ASSIMP_SEARCH_PATHS
    ${ASSIMP_SEARCH_PATHS}
    # Can be set as this module's argument
    ${ASSIMP_ROOT_DIR}
)

FIND_PATH(ASSIMP_INCLUDE_DIR
    NAMES
        assimp/version.h
    PATH_SUFFIXES
        include
    PATHS
        ${ASSIMP_SEARCH_PATHS}
    DOC
        "The directory where assimp's development headers reside"
)


# Specify 64 bit architectures search paths
if (${XXBITS} STREQUAL x86_64)
    SET(ASSIMP_XXBITS x64)
    SET(ASSIMP_SEARCH_LIB_PATH_SUFFIXES lib64)

    SET(ASSIMP_BUILD_TYPE Release)
    SET(ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES
        lib/${ASSIMP_XXBITS}/${ASSIMP_BUILD_TYPE}
        lib/${ASSIMP_BUILD_TYPE}/${ASSIMP_XXBITS}
    )

    SET(ASSIMP_BUILD_TYPE Debug)
    SET(ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES
        lib/${ASSIMP_XXBITS}/${ASSIMP_BUILD_TYPE}
        lib/${ASSIMP_BUILD_TYPE}/${ASSIMP_XXBITS}
    )

    if (WIN32)
        SET(ASSIMP_BUILD_TYPE release)
        SET(ASSIMP_SEARCH_RELEASE_DLL_PATH_SUFFIXES
            bin64
            bin/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )
        SET(ASSIMP_BUILD_TYPE debug)
        SET(ASSIMP_SEARCH_DEBUG_DLL_PATH_SUFFIXES
            bin64
            bin/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )
    endif (WIN32)
    if (MSVC)
        SET(ASSIMP_SEARCH_LIB_PATH_SUFFIXES
            ${ASSIMP_SEARCH_LIB_PATH_SUFFIXES}
            lib/${ASSIMP_XXBITS}
        )

        SET(ASSIMP_BUILD_TYPE release)
        SET(ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES
            ${ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES}
            lib/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )

        SET(ASSIMP_BUILD_TYPE debug)
        SET(ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES
            ${ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES}
            lib/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )
    else (MSVC)
        SET(ASSIMP_XXBITS win64)
        SET(ASSIMP_BUILD_TYPE release)
        SET(ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES
            ${ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES}
            lib/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )

        SET(ASSIMP_BUILD_TYPE debug)
        SET(ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES
            ${ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES}
            lib/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )
    endif (MSVC)
# Specify 32 bit architectures search paths
else (${XXBITS} STREQUAL x86_64)
    SET(ASSIMP_SEARCH_LIB_PATH_SUFFIXES lib/${XXBITS})
    SET(ASSIMP_XXBITS ${XXBITS})

    SET(ASSIMP_BUILD_TYPE Release)
    SET(ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES
        lib/${ASSIMP_XXBITS}/${ASSIMP_BUILD_TYPE}
        lib/${ASSIMP_BUILD_TYPE}/${ASSIMP_XXBITS}
    )

    SET(ASSIMP_BUILD_TYPE Debug)
    SET(ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES
        lib/${ASSIMP_XXBITS}/${ASSIMP_BUILD_TYPE}
        lib/${ASSIMP_BUILD_TYPE}/${ASSIMP_XXBITS}
    )

    SET(ASSIMP_XXBITS win32)

    SET(ASSIMP_BUILD_TYPE release)
    SET(ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES
        ${ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES}
        lib/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
    )
    SET(ASSIMP_BUILD_TYPE debug)
    SET(ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES
        ${ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES}
        lib/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
    )

    if (NOT MSVC)
        SET(ASSIMP_SEARCH_LIB_PATH_SUFFIXES
            ${ASSIMP_SEARCH_LIB_PATH_SUFFIXES}
            lib
            lib32
            lib/mingw
        )
    endif (NOT MSVC)
    if (WIN32)
        SET(ASSIMP_BUILD_TYPE release)
        SET(ASSIMP_SEARCH_RELEASE_DLL_PATH_SUFFIXES
            bin32
            bin/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )
        SET(ASSIMP_BUILD_TYPE debug)
        SET(ASSIMP_SEARCH_DEBUG_DLL_PATH_SUFFIXES
            bin32
            bin/assimp_${ASSIMP_BUILD_TYPE}-dll_${ASSIMP_XXBITS}
        )
    endif (WIN32)
endif (${XXBITS} STREQUAL x86_64)


FIND_PATH(ASSIMP_LIBRARY_RELEASE_DIR assimp.lib
    PATH_SUFFIXES
        ${ASSIMP_SEARCH_LIB_PATH_SUFFIXES}
        ${ASSIMP_SEARCH_RELEASE_LIB_PATH_SUFFIXES}
    PATHS
        ${ASSIMP_SEARCH_PATHS}
)

FIND_PATH(ASSIMP_LIBRARY_DEBUG_DIR assimpD.lib
    PATH_SUFFIXES
        ${ASSIMP_SEARCH_LIB_PATH_SUFFIXES}
        ${ASSIMP_SEARCH_DEBUG_LIB_PATH_SUFFIXES}
    PATHS
        ${ASSIMP_SEARCH_PATHS}
)

FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
    NAMES
        assimp
    PATHS
        ${ASSIMP_LIBRARY_RELEASE_DIR}
)

FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
    NAMES
        assimpD
    PATHS
        ${ASSIMP_LIBRARY_DEBUG_DIR}
)


# Find dll paths using path suffixes computed above
FIND_PATH(ASSIMP_DLL_RELEASE_DIR
    NAMES assimp.dll assimp.exe Assimp32.dll Assimp64.dll
    PATH_SUFFIXES
        ${ASSIMP_SEARCH_RELEASE_DLL_PATH_SUFFIXES}
    PATHS
        ${ASSIMP_SEARCH_PATHS}
)


FIND_PATH(ASSIMP_DLL_DEBUG_DIR
    NAMES assimp.dll assimp.exe Assimp32d.dll Assimp64d.dll
    PATH_SUFFIXES
        ${ASSIMP_SEARCH_RELEASE_DLL_PATH_SUFFIXES}
    PATHS
        ${ASSIMP_SEARCH_PATHS}
)

# Release runtime dlls
FILE(GLOB ASSIMP_DLL_RELEASE
    "${ASSIMP_DLL_RELEASE_DIR}/*.dll"
    "${ASSIMP_DLL_RELEASE_DIR}/*.exe"
)

# Debug runtime dlls
FILE(GLOB ASSIMP_DLL_RELEASE
    "${ASSIMP_DLL_DEBUG_DIR}/*.dll"
    "${ASSIMP_DLL_DEBUG_DIR}/*.exe"
)


MARK_AS_ADVANCED(ASSIMP_LIBRARY_RELEASE_DIR)
MARK_AS_ADVANCED(ASSIMP_LIBRARY_DEBUG_DIR)
MARK_AS_ADVANCED(ASSIMP_LIBRARY_RELEASE)
MARK_AS_ADVANCED(ASSIMP_LIBRARY_DEBUG_DIR)

find_package_handle_standard_args(
    ASSIMP
    DEFAULT_MSG
    ASSIMP_LIBRARY_RELEASE
    ASSIMP_INCLUDE_DIR
)

if (ASSIMP_FOUND)
    if (ASSIMP_LIBRARY_DEBUG)
        SET(ASSIMP_LIBRARIES optimized ${ASSIMP_LIBRARY_RELEASE} debug ${ASSIMP_LIBRARY_DEBUG} CACHE STRING "Assimp libraries")
        SET(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARY_RELEASE_DIR} ${ASSIMP_LIBRARY_DEBUG_DIR})
    else()
        SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE} CACHE STRING "Assimp libraries")
        SET(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARY_RELEASE_DIR})
    endif()
endif (ASSIMP_FOUND)
