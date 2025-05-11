cmake_minimum_required(VERSION 3.15)
project(MySFMLProject)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS OFF) 

include(FetchContent)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.1
)

FetchContent_MakeAvailable(sfml)

add_executable(MySFMLApp main.cpp)

target_link_libraries(MySFMLApp
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

if(WIN32)
    target_compile_definitions(MySFMLApp PRIVATE SFML_STATIC)
endif()