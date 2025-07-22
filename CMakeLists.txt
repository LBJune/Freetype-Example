cmake_minimum_required(VERSION 3.12.0)

project(FreeType-Example VERSION 1.0.0)

add_executable(app main.cpp font_renderer.cpp font_renderer_utils.cpp)

include(FetchContent)
FetchContent_Declare(glfw
                    GIT_REPOSITORY https://github.com/glfw/glfw.git
                    GIT_TAG 3.4
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glad
                    GIT_REPOSITORY https://github.com/skaarj1989/glad.git
                    GIT_TAG gl
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(freetype
                    GIT_REPOSITORY https://github.com/freetype/freetype.git
                    GIT_TAG VER-2-13-3
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype)
FetchContent_MakeAvailable(freetype)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/freetype/include)

target_link_libraries(app PRIVATE glfw glad freetype)

set(BIN_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin")
set_target_properties(app PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL  "${BIN_PATH}"
)

set(RESOUURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
file(GLOB RESOUURCE_FILES ${RESOUURCES_DIR}/*)

file(COPY ${RESOUURCE_FILES} DESTINATION "${BIN_PATH}/resources")