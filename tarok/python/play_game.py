import pyspiel as sp
import pytarok as ta


def play_game():
    game = ta.TarokGame({
        'num_players': sp.GameParameter(3),
        'seed': sp.GameParameter(0),
    })
    state = game.new_initial_state()
    while(not state.is_terminal()):
        print_info(game, state)
        state.apply_action(int(input("Enter action: ")))
        print("-" * 70, "\n")
    print(state.current_game_phase())


def print_info(game, state):
    print(state.current_game_phase())
    print(state.selected_contract())
    print("Current player: {}".format(state.current_player()))
    player_cards = state.player_cards(state.current_player())
    action_names = [state.card_action_to_string(a) for a in player_cards]
    print("\nPlayer cards: {}".format(
        [x for x in zip(action_names, player_cards)]
    ))

    if (state.current_game_phase() == ta.GamePhase.TALON_EXCHANGE):
        print_talon_exchange_info(state)
    elif (state.current_game_phase() == ta.GamePhase.TRICKS_PLAYING):
        print_tricks_playing_info(state)
    else:
        print()

    legal_actions = state.legal_actions()
    action_names = [state.action_to_string(a) for a in state.legal_actions()]
    print("Legal actions: {}\n".format(
        [x for x in zip(action_names, legal_actions)]
    ))


def print_talon_exchange_info(state):
    talon = [[state.card_action_to_string(x) for x in talon_set]
             for talon_set in state.talon_sets()]
    print("\nTalon: {}\n".format(talon))


def print_tricks_playing_info(state):
    trick_cards = state.trick_cards()
    action_names = [state.card_action_to_string(a) for a in trick_cards]
    print("\nTrick cards: {}\n".format(
        [x for x in zip(action_names, trick_cards)]
    ))


if __name__ == '__main__':
    play_game()
