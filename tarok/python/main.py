import pyspiel as sp
import pytarok as ta


def main():
    tarok_game = ta.TarokGame({})
    print("Number of players: {:d}".format(tarok_game.num_players()))
    tarok_state = tarok_game.new_initial_tarok_state()
    print("Legal actions: {}".format(tarok_state.legal_actions()))
    cfr_solver = sp.CFRSolver(tarok_game)
    policy = cfr_solver.current_policy().action_probabilities(tarok_state)
    print("CFR policy: {}".format(policy))


if __name__ == '__main__':
    main()
