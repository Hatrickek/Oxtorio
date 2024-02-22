# Oxtorio
Shop game with factorio-like automation made in OxylusEngine.

## Building
Currently supported and tested compilers are only: MSVC and Clang
- Install [Vulkan SDK](https://vulkan.lunarg.com/sdk/home).
- Run the root CMake script with this command to generate for MSVC:       
`cmake -S . -B ./build/ -G "Visual Studio 17 2022" -A x64`      
For clang:  
`cmake -B ./build -G Ninja -DCMAKE_CXX_COMPILER=clang++`
- Install `directx-shader-compiler` package if required. (Only required for UNIX)
- Then run this command to build it with CMake:   
`cmake --build ./build --config Release`

