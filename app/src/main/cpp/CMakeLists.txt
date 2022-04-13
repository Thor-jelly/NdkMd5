# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.18.1)

project("ndkmd5")

add_library(
             ndkmd5
             SHARED
             native-lib.cpp )

find_library(
              log-lib
              log )

target_link_libraries(
                       ndkmd5
                       ${log-lib} )