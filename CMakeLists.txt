cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include_directories(
        ${PROJECT_SOURCE_DIR}/src/
)


file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/headers/*.h"
        "${PROJECT_SOURCE_DIR}/src/headers/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/fonts/*.ttf"
        "${PROJECT_SOURCE_DIR}/src/sfx/*.wav"
        "${PROJECT_SOURCE_DIR}/src/sfx/*.mp3"
        )

if(WIN32)
        add_executable(Pong WIN32 ${all_SRCS})
else()
        add_executable(Pong ${all_SRCS})
endif()

target_link_libraries(Pong PRIVATE sfml-graphics sfml-audio)
target_compile_features(Pong PRIVATE cxx_std_17)


if(WIN32)
    add_custom_command(
        TARGET Pong
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Pong>
        VERBATIM)
endif()

add_custom_command(
        TARGET Pong
        COMMENT "Copy SFX"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/sfx/" $<TARGET_FILE_DIR:Pong>/sfx
        VERBATIM)

add_custom_command(
        TARGET Pong
        COMMENT "Copy Font"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/fonts/" $<TARGET_FILE_DIR:Pong>/fonts
        VERBATIM)
