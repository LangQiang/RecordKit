# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("recordkit")

set (PROJECT_ROOT_DIR ../../../../)

#第二个参数默认目录是cmake的Entering directory
add_subdirectory(${PROJECT_ROOT_DIR}/oboe-core ../../../../../oboe-bin)
add_subdirectory(${PROJECT_ROOT_DIR}/speex-core ../../../../../speex-bin)

MESSAGE("current dir: "${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_ROOT_DIR}/oboe-core/include ${PROJECT_ROOT_DIR}/oboe-core/shared ${PROJECT_ROOT_DIR}/speex-core/include ${PROJECT_ROOT_DIR}/speex-core/libspeexdsp)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#set (APP_SOURCES
#        native-lib.cpp
#        jni_bridge.cpp
#        AudioEngine.h
#        AudioEngine.cpp
#        AudioEngineCallback.h
#        AudioEngineCallback.cpp
#        RecordingCallBack.h
#        RecordingCallBack.cpp
#        SoundRecording.h
#        SoundRecording.cpp
#        Log.h
#        BlockingQueue.h
#        wave/WaveFormat.h
#        wave/WaveFormat.cpp
#        AudioConfig.h
#        AudioConfig.cpp
#        )
aux_source_directory(../cpp APP_SOURCES)
aux_source_directory(../cpp/wave APP_SOURCES)
aux_source_directory(../cpp/utils APP_SOURCES)
aux_source_directory(../cpp/global APP_SOURCES)
aux_source_directory(../cpp/fft APP_SOURCES)


add_library( # Sets the name of the library.
        recordkit

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${APP_SOURCES})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        recordkit
#        android
        oboe
        speex
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

#target_compile_options(recordkit PRIVATE -Wall -Werror "$<$<CONFIG:RELEASE>:-Ofast>")
