# build the test runner binary
add_executable(tarok_tests game_tests.cpp)
target_link_libraries(tarok_tests gtest_main tarok_lib)
add_test(NAME all_tests COMMAND run)
