
FIND_PATH(Irrlicht_INCLUDE_DIRS irrlicht.h
    PATHS
    /usr/local
    /usr
    ${CMAKE_SOURCE_DIR}/include/irrlicht
    PATH_SUFFIXES include
    )

FIND_LIBRARY(Irrlicht_LIBRARY
    NAMES Irrlicht
    PATHS
    /usr/local
    /usr
    ${CMAKE_SOURCE_DIR}/lib/irrlicht
    PATH_SUFFIXES lib
    )


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Irrlicht DEFAULT_MSG Irrlicht_LIBRARY Irrlicht_INCLUDE_DIRS)

IF (IRRLICHT_FOUND)
	SET(Irrlicht_LIBRARIES ${Irrlicht_LIBRARY})
ENDIF (IRRLICHT_FOUND)

MARK_AS_ADVANCED(Irrlicht_LIBRARY Irrlicht_LIBRARIES Irrlicht_INCLUDE_DIRS)
