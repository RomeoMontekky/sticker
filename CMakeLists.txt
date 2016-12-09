cmake_minimum_required(VERSION 3.0)

project(sticker)

set(BINARY_NAME "sticker")

set(CPP_FILES 
   "src/graphic_objects.cpp"
   "src/main.cpp"
   "src/sticker.cpp"
   "src/sticker_objects.cpp"
   "src/window.cpp"
   "src/window_class.cpp"
)

set(HEADER_FILES
   "src/graphic_objects.h"
   "src/window.h"
   "src/sticker.h"
   "src/sticker_objects.h"
   "src/window_class.h"
)

add_executable(${BINARY_NAME} ${CPP_FILES} ${HEADER_FILES})

target_link_libraries(${BINARY_NAME} "gdiplus")

if (MINGW)
   if (CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14")
      add_custom_command(
         TARGET ${BINARY_NAME} POST_BUILD
         COMMAND ${CMAKE_STRIP} ARGS "--strip-unneeded" "$<TARGET_FILE:${BINARY_NAME}>")
   else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++14")
   endif()
else()
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
endif()

add_custom_command(
   TARGET ${BINARY_NAME} POST_BUILD 
   COMMAND ${CMAKE_COMMAND} -E "make_directory" ARGS "${CMAKE_SOURCE_DIR}/bin"
   COMMAND ${CMAKE_COMMAND} -E "copy" ARGS "$<TARGET_FILE:${BINARY_NAME}>" "${CMAKE_SOURCE_DIR}/bin")

# Tell linker that entry point is WinMain.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--subsystem,windows")
