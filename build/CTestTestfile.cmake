# CMake generated Testfile for 
# Source directory: C:/Users/Huawei/Documents/GitHub/tic-toc-toe
# Build directory: C:/Users/Huawei/Documents/GitHub/tic-toc-toe/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(TicTacToeTests "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/build/Debug/TicTacToeTests.exe")
  set_tests_properties(TicTacToeTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;93;add_test;C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(TicTacToeTests "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/build/Release/TicTacToeTests.exe")
  set_tests_properties(TicTacToeTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;93;add_test;C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(TicTacToeTests "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/build/MinSizeRel/TicTacToeTests.exe")
  set_tests_properties(TicTacToeTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;93;add_test;C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(TicTacToeTests "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/build/RelWithDebInfo/TicTacToeTests.exe")
  set_tests_properties(TicTacToeTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;93;add_test;C:/Users/Huawei/Documents/GitHub/tic-toc-toe/CMakeLists.txt;0;")
else()
  add_test(TicTacToeTests NOT_AVAILABLE)
endif()
subdirs("_deps/sfml-build")
subdirs("_deps/googletest-build")
