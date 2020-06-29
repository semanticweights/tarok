import time

import numpy as np
import pyspiel as sp
import pytarok as ta


if __name__ == '__main__':
    game = ta.TarokGame({})
    start_time = time.time()

    for i in range(10000):
        state = game.new_initial_state()
        while not state.is_terminal():
            state.apply_action(np.random.choice(state.legal_actions()))
        print("Game {:d}, {}, rewards: {}".format(
            i + 1, state.selected_contract(), state.rewards()))

    print("Simulation ran for {:.2f} seconds".format(time.time() - start_time))
