# mpcs51045-spr23-project

## Installing SDL3

```{bash}
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel

#CMake >= 3.15
sudo cmake --install . --config Release
```

In C++ files, add `#include <SDL3/SDL.h>`
