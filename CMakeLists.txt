cmake_minimum_required(VERSION 3.10)
project( hl-houdiniengine )

add_library(houdiniengine.hdll SHARED
#Input C++ files go here
#${HOUDINIENGINE_SRC_DIR}/INPUT FILES GO HERE.cpp

src/houdiniengine.cpp
)

set_target_properties(houdiniengine.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME houdiniengine
SUFFIX .hdll
)

cmake_policy(SET CMP0015 NEW)


target_include_directories(houdiniengine.hdll
PRIVATE
${HOUDINIENGINE_INC_DIR}
${HL_INCLUDE_DIR}
)

link_directories(houdiniengine.hdll
${HL_LIB_DIR}
)

find_library(LIBHL NAMES hl 
             HINTS ${HL_LIB_DIR} )

SET(library_name houdiniengine.hdll)

target_link_libraries(houdiniengine.hdll
${LIBHL}
)
IF(APPLE)
list(APPEND CMAKE_PREFIX_PATH "/Applications/Houdini/Houdini18.5.672/Frameworks/Houdini.framework/Versions/18.5/Resources/toolkit/cmake")
find_package(Houdini REQUIRED   )
message("Houdini Found ${Houdini_FOUND}")
message("${Houdini_INCLUDE_DIR}")
message("${HOUDINI_INCLUDE_DIR}")
include_directories(${HOUDINI_INCLUDE_DIR})

# Link against the Houdini libraries, and add required include directories and compile definitions.
target_link_libraries( ${library_name} Houdini )
# Configure several common target properties, such as its output directory.
# houdini_configure_target( ${library_name} )

MARK_AS_ADVANCED(HOUDINI_LIBRARY)
ENDIF (APPLE)

if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC  -O3  -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")
endif (UNIX)

install(TARGETS houdiniengine.hdll DESTINATION ${HDLL_DESTINATION})
