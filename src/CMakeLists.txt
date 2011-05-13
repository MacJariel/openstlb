project (openstlb)

cmake_minimum_required(VERSION 2.6)

# FIXME: find a way to make this portable
add_definitions(-std=c++0x)

find_package (SDL 1.2 REQUIRED)
#find_package (SDL_image 1.2 REQUIRED)

set(BOOST_COMPONENTS
  integer
)

#find_package(Boost 1.41 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
find_package(Boost 1.41 REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${openstlb_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${openstlb_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${openstlb_BINARY_DIR}/bin)

include_directories(".")

set(SRCS
  main.cpp
  BattleMapSystem/BattleMap.cpp
  BattleMapSystem/BattleMapStructures.cpp
  BattleMapSystem/BattleMapGameplay.cpp
  BattleMapSystem/UnitType.cpp
  BattleMapSystem/Units.cpp
  GfxSystem/Palette.cpp
  GfxSystem/Sprite.cpp
  GfxSystem/SpriteAnimation.cpp
  GfxSystem/SpritePanimation.cpp
  ResourceSystem/Archives/ArchiveMgr.cpp
  ResourceSystem/Archives/FsArchive.cpp
  ResourceSystem/Archives/FsuArchive.cpp
  Utils/DataContainer.cpp
  Utils/MemoryReader.cpp
  Utils/Lzw.cpp
  Utils/LzwCodec.cpp
 
)

include_directories (
  ${SDL_INCUDE_DIR}
)


link_libraries (
  ${SDL_LIBRARY}
#  ${SDLIMAGE_LIBRARY}
  ${Boost_LIBRARIES}
  SDLmain
  SDL_ttf
)

add_executable (
  openstlb-main
  ${SRCS}
)
