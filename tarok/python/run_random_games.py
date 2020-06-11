import time

import numpy as np
import pyspiel as sp
import pytarok as ta


def play_random_game(game):
    state = game.new_initial_state()
    while(state.current_game_phase() != ta.GamePhase.FINISHED):
        legal_actions = state.legal_actions()
        state.apply_action(np.random.choice(legal_actions))


if __name__ == '__main__':
    game = sp.load_game('tarok(num_players=3,seed=0)')
    start_time = time.time()
    for i in range(10000):
        print("Running game number {:d}".format(i + 1))
        play_random_game(game)
    print("Simulation ran for {:.4f} seconds".format(time.time() - start_time))
