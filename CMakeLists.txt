cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
project (SkeletalAnimationApp)


add_definitions(-DDEMO_ASSETS_DIR="${CMAKE_SOURCE_DIR}/demo_assets/")

# LIBRARIES
set(BUILD_SHARED_LIBS OFF CACHE STRING "Static library type for glm" FORCE)
add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)
add_subdirectory(libs/fmt)
set(BUILD_SHARED_LIBS ON CACHE STRING "Dynamic library type for assimp" FORCE)
add_subdirectory(libs/assimp)
# LIBRARY OF IMPLEMENTATION
add_subdirectory(SkelAnimGL)
# MAIN APPLICATION TO RUN
add_subdirectory(Application)