set(SRC_TEST_FILES
  tarok_utils.cpp
  cards_tests.cpp
  game_tests.cpp
  state_card_dealing_phase_tests.cpp
  state_bidding_phase_tests.cpp
  state_talon_exchange_phase_tests.cpp
  state_tricks_playing_phase_tests.cpp
  state_captured_mond_tests.cpp
)

# build the test runner binary
add_executable(tarok_tests ${SRC_TEST_FILES})
target_link_libraries(tarok_tests gtest_main gmock tarok_lib)
add_test(NAME all_tests COMMAND run)

# build the debugging runner binary
add_executable(debugging_runner debugging_runner.cpp)
target_link_libraries(debugging_runner tarok_lib)
