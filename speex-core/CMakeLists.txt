cmake_minimum_required(VERSION 3.4.1)

# Set the name of the project and store it in PROJECT_NAME. Also set the following variables:
# PROJECT_SOURCE_DIR (usually the root directory where Oboe has been cloned e.g.)
# PROJECT_BINARY_DIR (usually the containing project's binary directory,
# e.g. ${OBOE_HOME}/samples/RhythmGame/.externalNativeBuild/cmake/ndkExtractorDebug/x86/oboe-bin)
project(speex)

enable_language(C)

set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)

include_directories("include")

file(GLOB_RECURSE ALL_PROJ_SRC "libspeexdsp/*.c")


add_library(speex ${ALL_PROJ_SRC})

add_definitions(-DEXPORT= -DFLOATING_POINT)
add_definitions(-DEXPORT= -DUSE_KISS_FFT)



find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# Compile Flags:
#     Enable -Werror when building debug config
#     Enable -Ofast
target_compile_options(speex
        PRIVATE
        -Wall
        -Ofast
        )

target_link_libraries( # Specifies the target library.
        speex

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )
# When installing oboe put the libraries in the lib/<ABI> folder e.g. lib/arm64-v8a
install(TARGETS speex
        LIBRARY DESTINATION lib/${ANDROID_ABI}
        ARCHIVE DESTINATION lib/${ANDROID_ABI})

# Also install the headers
install(DIRECTORY include/speex DESTINATION include)