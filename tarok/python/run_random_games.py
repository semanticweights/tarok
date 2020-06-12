import time

import numpy as np
import pyspiel as sp
import pytarok as ta


if __name__ == '__main__':
    game = sp.load_game('tarok(num_players=3,seed=0)')
    start_time = time.time()

    for i in range(10000):
        print("Running game number {:d}".format(i + 1))
        state = game.new_initial_state()
        while(not state.is_terminal()):
            state.apply_action(np.random.choice(state.legal_actions()))

    print("Simulation ran for {:.2f} seconds".format(time.time() - start_time))
