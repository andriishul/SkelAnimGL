cmake_minimum_required(VERSION 3.8)
project(SkelAnimGL)

find_package(OpenGL REQUIRED)

set(STB_HEADER ${CMAKE_SOURCE_DIR}/libs/stb)

set(UTILS_SOURCES 
	src/utils/_internal/ConsoleLogger.cpp
	src/utils/Logger.cpp
)

set(OPENGL_SOURCES 
	src/openGL/glBuffer.cpp
	src/openGL/glShader.cpp
	src/openGL/glTexture.cpp
	src/openGL/glRenderer.cpp
)

set(MODEL_SOURCES 
	src/model/components/Model.cpp
	src/model/components/Skeleton.h
	src/model/components/Mesh.cpp
	src/model/components/Animation.cpp
	src/model/components/Animator.cpp
	src/model/components/MeshRenderer.cpp 
	src/model/loaders/ModelImporter.cpp
)

set(BASIC_SOURCES
	src/core/Light.cpp
	src/core/Scene.cpp
)

set(GRAPHICS_SOURCES 
	src/graphics/IRenderer.cpp
	src/graphics/RendererContext.cpp
	src/graphics/Texture.cpp
	src/graphics/Buffer.cpp
	src/graphics/Shader.cpp
)

set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/SkelAnimGL/shaders/")
configure_file(src/graphics/Shader.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/Shader.cpp @ONLY)

set(SRC_FILES ${OPENGL_SOURCES} ${BASIC_SOURCES} ${GRAPHICS_SOURCES} ${MODEL_SOURCES} ${UTILS_SOURCES})

add_library(SkelAnimGL STATIC ${SRC_FILES})

target_include_directories(SkelAnimGL PUBLIC src PRIVATE ${STB_HEADER} PUBLIC src/model INTERFACE src/model/loaders src/model/components)
target_link_libraries(SkelAnimGL PUBLIC glm glad fmt-header-only PRIVATE ${OPENGL_LIBRARIES} assimp glfw)
target_compile_definitions(SkelAnimGL PRIVATE STB_IMAGE_IMPLEMENTATION)