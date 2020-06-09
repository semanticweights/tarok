import pyspiel as sp
import pytarok as ta


def play_game():
    game = sp.load_game('tarok(num_players=3,seed=0)')
    state = game.new_initial_state()
    while(True):
        print_info(game, state)
        state.apply_action(int(input("Enter action: ")))


def print_info(game, state):
    print(state.current_game_phase())
    print(state.selected_contract())
    print("Current player: {}".format(state.current_player()))

    if (state.current_game_phase() == ta.GamePhase.TALON_EXCHANGE):
        talon = [[state.card_action_to_string(x) for x in talon_set]
                 for talon_set in state.talon()]
        print("Talon: {}".format(talon))

    legal_actions = state.legal_actions()
    action_names = [state.action_to_string(a) for a in state.legal_actions()]
    print("Legal actions: {}".format(
        [x for x in zip(action_names, legal_actions)]
    ))
    print()


if __name__ == '__main__':
    play_game()
