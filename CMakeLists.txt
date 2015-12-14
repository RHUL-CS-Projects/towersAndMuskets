cmake_minimum_required (VERSION 2.8)
project (TowerDefense)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lX11 -lGL -lGLU -lXrandr -lXext")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lXxf86vm -lXext -lX11 -lXcursor")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -lX11 -lGL -lGLU -lXrandr -lXext")


if (${CMAKE_SIZEOF_VOID_P} STREQUAL 8)
	message("64 Bit")
else(${CMAKE_SIZEOF_VOID_P} STREQUAL 8)
	message("32 Bit")
endif(${CMAKE_SIZEOF_VOID_P} STREQUAL 8)
#set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")

include_directories (
  ${CMAKE_SOURCE_DIR}/include 
  ${CMAKE_SOURCE_DIR}/include/ogre 
  ${CMAKE_SOURCE_DIR}/include/sfml
  ${CMAKE_SOURCE_DIR}/include/enet
  ${CMAKE_SOURCE_DIR}/include/boost
)
 
set (SOURCES 
  src/Main.cpp
  src/network/Server.cpp
  src/network/Client.cpp
  src/ObjectManager.cpp
  src/RenderManager.cpp
  src/RenderSystem.cpp
  src/CameraSystem.cpp
  src/SelectionSystem.cpp
  src/EventReceiver.cpp
  src/PathMovementSystem.cpp
  src/FaceDirectionSystem.cpp
  src/RTSMovementSystem.cpp
  src/NodePath.cpp
  src/SteeringSystem.cpp
  src/Quadtree.cpp
  src/DebugValues.cpp
  src/WorldManager.cpp
  src/PathFinder.cpp
)

#set(BUILD_SHARED_LIBS OFF)
#set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static" )
#set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES}) 

add_executable (TowerDefense ${SOURCES})
#file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR }/res)
file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/build/res)
file(COPY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${CMAKE_SOURCE_DIR}/build/res) 
#file(COPY ${CMAKE_SOURCE_DIR }/src/resources.cfg DESTINATION ${CMAKE_SOURCE_DIR }/bin)
#file(COPY ${CMAKE_SOURCE_DIR }/src/plugins.cfg DESTINATION ${CMAKE_SOURCE_DIR }/bin)

# Link ENet

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/enet;${CMAKE_MODULE_PATH}")
find_package(ENet)

#find_path(ENet_INCLUDE_DIR enet/enet.h
#    ${CMAKE_SOURCE_DIR}/include
#)

#find_library(ENet_LIBRARY
#    NAMES enet
#    PATHS ${CMAKE_SOURCE_DIR}/lib/enet
#)
  
target_link_libraries(TowerDefense ${ENet_LIBRARIES})

# Link SFML

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/sfml;${CMAKE_MODULE_PATH}")
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/lib/sfml;${CMAKE_SOURCE_DIR}/include/sfml")
find_package(SFML COMPONENTS network graphics window system audio)
target_link_libraries(TowerDefense ${SFML_LIBRARIES})


# Find irrlicht dependencies
#find_package(X11)
#find_package(OpenGL)
#find_package(GLUT)
#find_package(ZLIB)

#target_link_libraries(TowerDefense
#	${X11_LIBRARIES}
#	${OPENGL_LIBRARIES}
#	${GLUT_LIBRARIES}
#	${ZLIB_LIBRARIES}
#)

# Link Irrlicht
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/irrlicht;${CMAKE_MODULE_PATH}")
find_package(Irrlicht)
target_link_libraries(TowerDefense ${Irrlicht_LIBRARIES} -lGL -lXxf86vm -lXext -lX11 -lXcursor)
