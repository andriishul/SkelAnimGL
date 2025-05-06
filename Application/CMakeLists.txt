cmake_minimum_required(VERSION 3.8)

add_executable (app "app.cpp" "app.h" "core/FreeCamera.cpp")
target_include_directories(app PRIVATE core)
target_link_libraries(app PRIVATE SkelAnimGL glfw)
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp> $<TARGET_FILE_DIR:app>)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET app PROPERTY CXX_STANDARD 20)
endif()