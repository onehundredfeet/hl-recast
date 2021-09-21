cmake_minimum_required(VERSION 3.10)
project (recast)

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
${HL_INCLUDE_DIR}
)

target_link_directories(recast.hdll PRIVATE ${HL_LIB_DIR})
target_link_libraries(recast.hdll libhl)

