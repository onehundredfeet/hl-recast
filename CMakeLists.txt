cmake_minimum_required(VERSION 3.10)
project (recast C CXX)

set(PATH_TO_IDL "../hl-idl")
# a bunch of pre-configured options that may or may not be useful
# it attempts to cover most of what is used for basic libraries
# you can reference the paths defined here to help make your cmake code more portable
include(${PATH_TO_IDL}/h-idl-defines.cmake)


if (NOT RECAST_ROOT) 
set(RECAST_ROOT "ext/recast")
endif()

if (NOT RECAST_SRC_DIR) 
set(RECAST_SRC_DIR "${RECAST_ROOT}/Recast")
endif()

if (NOT DETOUR_SRC_DIR) 
set(DETOUR_SRC_DIR "${RECAST_ROOT}/Detour")
endif()

if (NOT DETOUR_TILE_CACHE_ROOT) 
set(DETOUR_TILE_CACHE_ROOT "${RECAST_ROOT}/DetourTileCache")
endif()

if (NOT DETOUR_DEBUG_UTIL_ROOT) 
set(DETOUR_DEBUG_UTIL_ROOT "${RECAST_ROOT}/DebugUtils")
endif()

if (NOT RECAST_DEMO_ROOT) 
set(RECAST_DEMO_ROOT "${RECAST_ROOT}/RecastDemo")
endif()



set( PROJECT_ADDITIONAL_SOURCES 
${RECAST_SRC_DIR}/Source/Recast.cpp
${RECAST_SRC_DIR}/Source/RecastAlloc.cpp
${RECAST_SRC_DIR}/Source/RecastArea.cpp
${RECAST_SRC_DIR}/Source/RecastAssert.cpp
${RECAST_SRC_DIR}/Source/RecastContour.cpp
${RECAST_SRC_DIR}/Source/RecastFilter.cpp
${RECAST_SRC_DIR}/Source/RecastLayers.cpp
${RECAST_SRC_DIR}/Source/RecastMesh.cpp
${RECAST_SRC_DIR}/Source/RecastMeshDetail.cpp
${RECAST_SRC_DIR}/Source/RecastRasterization.cpp
${RECAST_SRC_DIR}/Source/RecastRegion.cpp
${DETOUR_SRC_DIR}/Source/DetourAlloc.cpp
${DETOUR_SRC_DIR}/Source/DetourAssert.cpp
${DETOUR_SRC_DIR}/Source/DetourCommon.cpp
${DETOUR_SRC_DIR}/Source/DetourNavMesh.cpp
${DETOUR_SRC_DIR}/Source/DetourNavMeshBuilder.cpp
${DETOUR_SRC_DIR}/Source/DetourNavMeshQuery.cpp
${DETOUR_SRC_DIR}/Source/DetourNode.cpp
${DETOUR_TILE_CACHE_ROOT}/Source/DetourTileCache.cpp
${DETOUR_TILE_CACHE_ROOT}/Source/DetourTileCacheBuilder.cpp
${RECAST_DEMO_ROOT}/Source/PerfTimer.cpp
${DETOUR_DEBUG_UTIL_ROOT}/Source/DebugDraw.cpp
${DETOUR_DEBUG_UTIL_ROOT}/Source/DetourDebugDraw.cpp
${DETOUR_DEBUG_UTIL_ROOT}/Source/RecastDebugDraw.cpp
src/ChunkyTriMesh.cpp
src/hl-idl-helpers.cpp
src/hl-recast.cpp
src/recast.cpp
src/fastlz/fastlz.c
)

set (PROJECT_ADDITIONAL_INCLUDES
# otherlib/include
${RECAST_SRC_DIR}/Include
${DETOUR_SRC_DIR}/Include
${DETOUR_SRC_DIR}/Include
${DETOUR_TILE_CACHE_ROOT}/Include
${RECAST_DEMO_ROOT}/Include
${DETOUR_DEBUG_UTIL_ROOT}/Include
)

include(${PATH_TO_IDL}/h-idl-configure.cmake)