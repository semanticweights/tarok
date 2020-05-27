import pyspiel as sp
import pytarok as ta


def play_game():
    game = ta.TarokGame({
        'num_players': sp.GameParameter(4),
        'seed': sp.GameParameter(0),
    })
    state = game.new_initial_tarok_state()

    while(True):
        print_info(game, state)
        state.apply_action(int(input("Enter action: ")))


def print_info(game, state):
    print("Game phase: {}".format(state.current_game_phase()))
    print("Selected contract: {}".format(state.selected_contract()))
    print("Current player: {}".format(state.current_player()))
    legal_actions = state.legal_actions()
    action_names = [state.action_to_string(a) for a in state.legal_actions()]
    print("Legal actions: {}".format(
        [x for x in zip(action_names, legal_actions)]
    ))
    print()


if __name__ == '__main__':
    play_game()
