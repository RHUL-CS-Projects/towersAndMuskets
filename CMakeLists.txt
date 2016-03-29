cmake_minimum_required (VERSION 2.8)
project (TowerDefense)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if (${CMAKE_SIZEOF_VOID_P} STREQUAL 8)
	message("64 Bit")
else(${CMAKE_SIZEOF_VOID_P} STREQUAL 8)
	message("32 Bit")
endif(${CMAKE_SIZEOF_VOID_P} STREQUAL 8)


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
  src/RTSLogicSystem.cpp
  src/NodePath.cpp
  src/SteeringSystem.cpp
  src/Quadtree.cpp
  src/DebugValues.cpp
  src/WorldManager.cpp
  src/PathFinder.cpp
  src/Game.cpp
  src/StateMainMenu.cpp
  src/GuiElement.cpp 
  src/StatePlaying.cpp
  src/RTSCamera.cpp
  src/InteractionMenu.cpp
  src/GameResources.cpp
  src/MapGenerator.cpp
  src/ObjectPlacer.cpp
  src/StatePauseMenu.cpp
  src/NotificationMessage.cpp
  src/NoiseGenerator.cpp
  src/RTSVillagerLogicSystem.cpp
  src/PlayerResourceCache.cpp
  src/GroupPathMover.cpp
  src/WaveController.cpp
  src/TeamComponent.cpp
  src/RTSAISystem.cpp
  src/TransparentMaterialShader.cpp
  src/ParticleManager.cpp
  src/GuiElementPurchase.cpp
  src/StateWinLose.cpp
)

add_executable (TowerDefense ${SOURCES})
file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/build/res)
file(COPY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${CMAKE_SOURCE_DIR}/build/res) 
# Link ENet

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/enet;${CMAKE_MODULE_PATH}")
find_package(ENet)
target_link_libraries(TowerDefense ${ENet_LIBRARIES})

# Link SFML

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/sfml;${CMAKE_MODULE_PATH}")
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/lib/sfml;${CMAKE_SOURCE_DIR}/include/sfml")
find_package(SFML COMPONENTS network graphics window system audio)
target_link_libraries(TowerDefense ${SFML_LIBRARIES})


# Link Irrlicht
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/irrlicht;${CMAKE_MODULE_PATH}")
find_package(Irrlicht)
target_link_libraries(TowerDefense ${Irrlicht_LIBRARIES} -lGL -lXxf86vm -lXext -lX11 -lXcursor)
