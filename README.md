# unit tests
## install packages
```bash
git clone https://github.com/google/googletest.git
git checkout release-1.12.1 # latest version that supports c++11
cmake -DCMAKE_CXX_STANDARD=11 .
make
make install
```

## libs will be places in `../libs/bin`

# Project configure
## Windows project configure
### Install Mysys
1. Download MySys: `https://www.msys2.org`

### Install MinGW-64 (Cmake, gcc, make, gdb)
1. Open Mysys-uct64
2. ```pacman -S mingw-w64-ucrt-x86_64-cmake```
3. ```pacman -S mingw-w64-ucrt-x86_64-make```
3. ```pacman -S mingw-w64-ucrt-x86_64-gcc```
3. ```pacman -S mingw-w64-ucrt-x86_64-gdb```


### Install G++
```sudo apt install g++```

### Install SDL2
1. Download SDL2 devel minGW: `https://github.com/libsdl-org/SDL/releases/tag/release-2.26.4`
2. Extractlib folder to same root as project folder

### Install GLEW
1. Download GLEW: `http://glew.sourceforge.net/index.html`
2. Go the glew folder and search for the location of `CMakeLists.txt`. It’s here: `glew-2.1.0\build\cmake\CMakeLists.txt`
3. ```cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX=C:\msys64\ucrt64\x86_64-w64-mingw32```
4. ```mingw32-make all```
5. ```mingw32-make install```

### Unit Test install
1. ```git clone https://github.com/google/googletest.git```
2. ```cd googletest```
3. ```mkdir build & cd build & cmake -G "MinGW Makefiles" ..```
3. ```mingw32-make```

## Linux (Debian/Ubuntu) project
### Install Glew
1. ```sudo apt install libglew-dev```

### Install SDL2
1. ```sudo apt install libsdl2-dev```

### Install SDL2_image
1. ```sudo apt install libsdl2-image-dev```

### Install SDL2_ttf 
1. ```sudo apt-get install libsdl2-ttf-dev```

## Android project
1. Download Android Studio
2. Install NDK 
3. Set path's to SDK and NDK: 
```
export ANDROID_SDK_ROOT=/home/inyragvar/Android/Sdk
export ANDROID_NDK_ROOT=$ANDROID_SDK_ROOT/ndk/25.2.9519653
export PATH=$PATH:$ANDROID_SDK_ROOT/platform-tools:$ANDROID_NDK_ROOT
``` 
4. Download SDL2, SDL2_image source code and craete symlink to folder like this: 
```ln -s /home/inyragvar/projects/git/c++/libs/SDL2 ./SDL2```
```ln -s /home/inyragvar/projects/git/c++/libs/SDL2_image/ ./SDL2_image```
```ln -s /home/inyragvar/projects/git/c++/libs/SDL2_ttf/ ./SDL2_ttf```

## iOS Project
1. 

## WebGL Project
1. Install Emscripten
```git clone https://github.com/emscripten-core/emsdk.git```
```cd emsdk```
```./emsdk install latest```
```./emsdk activate latest```
```source ../../../libs/emsdk/emsdk_env.sh```

2. Create link for assets
```ln -s /home/inyragvar/projects/git/c++/alisa-engine/assets ./assets```


### C++ config
```
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/local/include",
                "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1",
                "/usr/local/Cellar/glew/2.2.0_1/include"
            ],
            "defines": [],
            "macFrameworkPath": [
                "/System/Library/Frameworks",
                "/Library/Frameworks"
            ],
            "compilerPath": "/usr/bin/clang++",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "clang-x64"
        },
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/src"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "c++14",
            "intelliSenseMode": "linux-clang-x64"
        },
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/src"
            ],
            "defines": [],
            "compilerPath": "C:\\msys64\\ucrt64\\bin\\g++",
            "cStandard": "c17",
            "cppStandard": "c++14",
            "intelliSenseMode": "windows-gcc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        }
    ],
    "version": 4
}
```