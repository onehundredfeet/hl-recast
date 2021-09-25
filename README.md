# hl-recast

THIS PROJECT IS IN PROGRESS - DO NOT CLONE, IT DOES NOT WORK


## Build and Install
##### Note: Most of the following steps are common on windows, linux and mac. The only difference is in the build & install step.

### 1. Prerequisites:
1.1. Install haxe: (: https://haxe.org/download)  
1.2. Install hashlink: (https://hashlink.haxe.org )  
1.3. Install heaps: (For linux: https://heaps.io/documentation/installation.html )  
1.4. Install the modified webidl from git:   
```sh
haxelib git hl-idl https://github.com/onehundredfeet/hl-idl.git
```

### 2. Build

2.1. Clone this repo into a clean directory.

Open new terminal in this directory.  
```sh
haxelib dev hl-recast hl-recast
```

This tells haxe to look for the library 'hl-recast' in the directory 'hl-recast'.  The 'dev' keyword tells haxe that the library is local and will be directly referenced instead of being installed to the library cache.

2.2 Clone recast c++ sources as a submodule in hl-recast
From the hl-recast root, run

```sh
git submodule update --init --recursive
```

2.3 Generate the binding cpp file.

Change directory into the hl-recast root.

.Open new terminal under and run 
```sh
make genhl
```

This will generate src/recast.cpp from recast/recast.idl


2.4 Generate cmake and build.  
**Note: At this step there are differences between platforms**  
    Create new dir : HL-RECAST-ROOT/build  
    Open new terminal in the directory HL-recast-ROOT/build and run:  

**For linux:**  
```sh
    cmake ..
      -DRECAST_SRC_DIR="ext/recast" # This is the /src from step 2
      -DHL_INCLUDE_DIR="<path-to-hashlink headers>" # Path to where hashlink headers (hl.h, ...) are located. Usually under ...hashlink-x.xx/src 
      -DHL_LIB_DIR="<path-to-hashlink>" # Path to where hashlink binaries (libhl.so, ...) are located. Usually ...hashlink-x.xx
      -HDLL_DESTINATION="final/install/destination" #The path of all *hdll binaries, usually this is 'usr/lib' or 'usr/local/lib'
```
```sh
    make
```
```sh
    make install
```

DEVELOPER NOTE: recast WILL REQUIRE MULTIPLE SOURCE DIRS.
They have submodules netcode.io reliable.io

specific cmake command example on linux:  
```sh
    cmake .. -Drecast_SRC_DIR="ext/recast" -DHL_INCLUDE_DIR="/usr/local/include" -DHL_LIB_DIR="/usr/local/lib" -DHDLL_DESTINATION="/usr/local/lib"
   ```

**For windows:**  

```sh
    cmake .. -A x64 -G "Visual Studio 15 2017" 
    -Drecast_SRC_DIR="<path-to-bullet3>/src" # This is the /src from step 2
      -DHL_INCLUDE_DIR="<path-to-hashlink>/include"
      -DHL_LIB_DIR="<path-to-hashlink>"  # Path to where hashlink binaries (libhl.lib, ...) are located
      -HDLL_DESTINATION="final/install/destination" # Usually <path-to-hashlink>
```

* Open and build build/Project.sln  
* Copy ./build/Debug/bullet.hdll to ../path/to/HashLink/hdll/location/ # Just in case Visual Studio install step fails.  

**For UNIX (Mac / Linux):**  
* Note: Specify the hashlink directories on your system.  On Mac, brew defaults /usr/local (lib/include).
```sh
    cmake ..  -GNinja 
    -DRECAST_SRC_DIR="ext/recast" 
    -DRECAST_INC_DIR="ext/recast" 
    -DHL_LIB_DIR="/usr/local/lib" 
    -DHL_INCLUDE_DIR="/usr/local/include" 
    -DHDLL_DESTINATION="/usr/local/lib"
```
```sh
    make
```
```sh
    make install
```


cmake .. -A x64 -G "Visual Studio 15 2017" -DRECAST_SRC_DIR="C:/Projects/RecastToHL/recastnavigation_cpp/Recast" -DHL_INCLUDE_DIR="C:/HaxeToolkit/HashLink/hl-1.11.0-win/include" -DHL_LIB_DIR="C:/HaxeToolkit/HashLink/hl-1.11.0-win" -DHDLL_DESTINATION="C:/HaxeToolkit/HashLink/hl-1.11.0-win"