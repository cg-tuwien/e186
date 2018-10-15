if (NOT DEFINED FINISHED_XPLATFORM_FIND_LIBRARY_PREFIXES_SETUP)
    set(FINISHED_XPLATFORM_FIND_LIBRARY_PREFIXES_SETUP 1)
    # Check if on decent OS
    if (UNIX)
        SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
    # Deal with lesser OSes
    elseif (WIN32)
        if (MSVC)
            SET(CMAKE_FIND_LIBRARY_PREFIXES "")
            SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
        else (MSVC)
            # Can accept lib<name>.a or <name>.lib.
            # More info:
            # http://www.mingw.org/wiki/specify_the_libraries_for_the_linker_to_use
            SET(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
            SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".a" "dll.a")
        endif (WIN32)
    endif (UNIX)
endif (NOT DEFINED FINISHED_XPLATFORM_FIND_LIBRARY_PREFIXES_SETUP)
