# Headers

Utillity single file libraries written in C99 C, though most libraries depend on the "core" ``types.h`` library, and some may depend on other of single file libraries.

## Building the test program

The test program is used for testing and playing around with all the libraries, and for making sure everything compiles ocrrectly.
The test program can be built using any C99 compatible compiler, build.bat will build the program for windows __assuming environment variables for ``cl`` have been set__ while build.sh will build the program using cæang.

Building on windows (_adjust the path to vcvarsx64.bat to change arch or compiler version_)

```bat
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise17\VC\Auxiliary\Build\vcvarsall.bat" x64
.\build.bat
```

Building on mac or linux

```sh
./build.sh
```