set(PROJECT_NAME OxtorioModule)

# Source groups
file(GLOB src_root "src/*.h" "src/*.cpp")
source_group("src" FILES ${src_root})
set(ALL_FILES ${src_root})

# Target
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

foreach(OUTPUTCONFIG IN LISTS CMAKE_CONFIGURATION_TYPES)
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Oxtorio)
  set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Oxtorio)
endforeach()

set(ROOT_NAMESPACE OxtorioModule)

# Target name
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "OxtorioModule"
    TARGET_NAME_RELEASE "OxtorioModule"
    TARGET_NAME_Distribution    "OxtorioModule"
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Oxtorio/src"
)

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
  "$<$<CONFIG:Debug>:"
  "OX_DEBUG;"
  "_DEBUG;"
  ">"
  "$<$<CONFIG:Release>:"
  "OX_RELEASE;"
  "NDEBUG;"
  ">"
  "$<$<CONFIG:Distribution>:"
  "OX_DISTRIBUTION;"
  "NDEBUG"
  ">"
  "_CRT_SECURE_NO_WARNINGS"
  "GLFW_INCLUDE_NONE"
  "_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS"
)

# Compile and link options
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /std:c++20 /permissive-) 
  set(CMAKE_SHARED_LINKER_FLAGS_DISTRIBUTION ${CMAKE_SHARED_LINKER_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION ${CMAKE_EXE_LINKER_FLAGS})
endif ()

if (MSVC AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions("/MP")
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/cpm.cmake)
  file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.7/CPM.cmake ${CMAKE_SOURCE_DIR}/cpm.cmake SHOW_PROGRESS)
endif()

include(${CMAKE_SOURCE_DIR}/cpm.cmake)
include(FetchContent)

# Link with oxylus.
CPMAddPackage("gh:Hatrickek/OxylusEngine#dev")
target_link_libraries(${PROJECT_NAME} PRIVATE Oxylus)
target_include_directories(${PROJECT_NAME} PUBLIC ${Oxylus_SOURCE_DIR}/Oxylus/src)

