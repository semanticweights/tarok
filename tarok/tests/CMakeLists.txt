# tests executable
add_executable(tarok_tests game_tests.cpp)
target_link_libraries(tarok_tests PRIVATE tarok_lib gtest_main)
add_test(NAME tarok_tests COMMAND tarok_tests)
