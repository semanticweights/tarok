# build the test runner binary
add_executable(tarok_tests game_tests.cpp)
target_link_libraries(tarok_tests gtest_main tarok_lib)
add_test(NAME all_tests COMMAND run)

# build the debugging runner binary
add_executable(debugging_runner debugging_runner.cpp)
target_link_libraries(debugging_runner tarok_lib)
