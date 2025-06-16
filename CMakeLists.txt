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

#копирование ресурсов
add_custom_command(TARGET TicTacToe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:TicTacToe>/assets
    COMMENT "Copying assets"
)


option(BUILD_TESTS "Build test targets" ON)

if(BUILD_TESTS)
    message(STATUS "Configuring tests...")
    
    #загрузка GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
    
    #тестовые исходники
    set(TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/tests/main.cpp
        ${CMAKE_SOURCE_DIR}/tests/core/test_game.cpp
        ${CMAKE_SOURCE_DIR}/tests/core/test_bot.cpp
    )
    
    #создание тестового исполняемого файла
    add_executable(TicTacToeTests ${TEST_SOURCES} 
        ${SRC_DIR}/core/Game.cpp
        ${SRC_DIR}/core/Bot.cpp
    )
    
    #настройка включения директорий для тестов
    target_include_directories(TicTacToeTests PRIVATE
        ${SRC_DIR}/core
        ${CMAKE_SOURCE_DIR}/tests/core
        ${googletest_SOURCE_DIR}/include
    )
    
    #подключение библиотек
    target_link_libraries(TicTacToeTests PRIVATE
        GTest::gtest_main
    )
    
    #настройка тестирования
    enable_testing()
    add_test(NAME TicTacToeTests COMMAND TicTacToeTests)
    
    #дополнительная цель для удобного запуска
    add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS TicTacToeTests
        COMMENT "Running tests..."
    )
    
    if(WIN32)
        target_compile_definitions(TicTacToeTests PRIVATE 
            _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
        )
        
        set_target_properties(TicTacToeTests PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
        )
    endif()
endif()