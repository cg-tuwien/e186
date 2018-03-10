#.rst:
#
# =======================
# Architecture's bit size
# =======================
#
# Sets architecture's bit size
#
# The module defines the following variables::
#
#     OSXXBITS - On windows it equals to ``Win32`` for 32 bits ``Win64`` for 64 bits
#                For other OSes, it equals to ``x86`` for 32 bits, and ``x86_64`` for 64 bits
#     XXBITS   - For all OSes it equals to ``x86`` for 32 bits, and ``x86_64`` for 64 bits
#


if (NOT DEFINED XXBITS)
    # Test 32/64 bits
    if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message(STATUS "Target is 64 bits")
        set(XXBITS x86_64)
        if (WIN32)
            set(OSXXBITS win64)
        else (WIN32)
            set(OSXXBITS x86_64)
        endif (WIN32)
    else ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message(STATUS "Target is 32 bits")
        set(XXBITS x86)
        if (WIN32)
            set(OSXXBITS win32)
        else (WIN32)
            set(OSXXBITS x86)
        endif(WIN32)
    endif ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
endif (NOT DEFINED XXBITS)
