cmake_minimum_required(VERSION 3.10)
project (recast)


if (NOT RECAST_ROOT) 
set(RECAST_ROOT "ext/recast")
endif()

if (NOT RECAST_SRC_DIR) 
set(RECAST_SRC_DIR "${RECAST_ROOT}/Recast")
endif()

if (NOT DETOUR_SRC_DIR) 
set(DETOUR_SRC_DIR "${RECAST_ROOT}/Detour")
endif()

if (NOT HL_INCLUDE_DIR) 
    set(HL_INCLUDE_DIR "/usr/local/include")
endif()

if (NOT HL_LIB_DIR) 
    set(HL_LIB_DIR "/usr/local/lib")
endif()

if (NOT COMMON_LIB_DIR) 
    set(COMMON_LIB_DIR "/usr/local/lib")
endif()

if (NOT HDLL_DESTINATION) 
    set(HDLL_DESTINATION "/usr/local/lib")
endif()



add_library(recast.hdll SHARED
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
src/recast.cpp
src/hl-idl-helpers.cpp
${DETOUR_SRC_DIR}/Source/DetourAlloc.cpp
${DETOUR_SRC_DIR}/Source/DetourAssert.cpp
${DETOUR_SRC_DIR}/Source/DetourCommon.cpp
${DETOUR_SRC_DIR}/Source/DetourNavMesh.cpp
${DETOUR_SRC_DIR}/Source/DetourNavMeshBuilder.cpp
${DETOUR_SRC_DIR}/Source/DetourNavMeshQuery.cpp
${DETOUR_SRC_DIR}/Source/DetourNode.cpp
)

set_target_properties(recast.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME recast
SUFFIX .hdll
)

cmake_policy(SET CMP0015 NEW)

target_include_directories(recast.hdll
PRIVATE
${RECAST_SRC_DIR}/Include
${DETOUR_SRC_DIR}/Include
${HL_INCLUDE_DIR}
)


if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC  -O3  -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")

    find_library(LIBHL NAMES hl HINTS ${HL_LIB_DIR} )
else()
    find_library(LIBHL NAMES libhl HINTS ${HL_LIB_DIR} )
endif (UNIX)

target_link_libraries(recast.hdll ${LIBHL})

install(TARGETS recast.hdll DESTINATION ${HDLL_DESTINATION})
