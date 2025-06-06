cmake_minimum_required(VERSION 3.15)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

#настройка путей
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(MAIN_CPP ${CMAKE_SOURCE_DIR}/main.cpp)

#поиск исходных файлов
file(GLOB_RECURSE CORE_SOURCES "${SRC_DIR}/core/*.cpp")
file(GLOB_RECURSE UI_SOURCES "${SRC_DIR}/ui/*.cpp")

#создание списка исходных файлов
add_executable(TicTacToe 
    ${MAIN_CPP}
    ${CORE_SOURCES}
    ${UI_SOURCES}
)

#настройка SFML
include(FetchContent)
FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
FetchContent_MakeAvailable(sfml)

#подключение SFML
target_link_libraries(TicTacToe PRIVATE sfml-graphics sfml-window sfml-system)

#пути включения
target_include_directories(TicTacToe PRIVATE 
    ${CMAKE_SOURCE_DIR}  
    ${SRC_DIR}            
    ${sfml_SOURCE_DIR}/include
)

if(WIN32)
    target_compile_definitions(TicTacToe PRIVATE SFML_STATIC)
    target_link_libraries(TicTacToe PRIVATE opengl32 winmm gdi32)
endif()

# Копирование ресурсов
add_custom_command(TARGET TicTacToe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:TicTacToe>/assets
    COMMENT "Copying assets"
)