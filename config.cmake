
################# COMMON

if (NOT PATH_TO_IDL)
set(PATH_TO_IDL "${CMAKE_SOURCE_DIR}/ext/hl-idl")
endif()

if (NOT EXISTS ${PATH_TO_IDL})
message( "This library requires library hl-idl https://github.com/onehundredfeet/hl-idl.git ")
message( FATAL_ERROR "Please use -DPATH_TO_IDL=<IDLPATH> to set the path relative to directory you are running in or run `git update submodules --init --recursive`")
endif()

if (NOT RECAST_LIB_DIR) 
set(RECAST_LIB_DIR "${CMAKE_SOURCE_DIR}/ext/recast")
endif()

if (NOT EXISTS ${RECAST_LIB_DIR})
message( "This library requires a specific version of recast https://github.com/onehundredfeet/recastnavigation.git ")
message( FATAL_ERROR "Can't find recast at ${RECAST_LIB_DIR}, please specify -DRECAST_LIB_DIR=<PATH> or run `git update submodules --init --recursive`")
endif()


################# MAC AND LINUX
if (UNIX)



################# WINDOWS
elseif(WIN32)


#### CHECKS
#if (NOT EXISTS ${HL_LIB_DIR})
#    message(FATAL_ERROR "Hashlink libraries not found. They are included with the installation or build of hashlink. Please specify the hashlink library directory using HL_LIB_DIR as a -D option. See config.cmake for pathing information.")
#endif()


################# END
endif()
