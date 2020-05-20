import pyspiel as sp
import pytarok as ta


def main():
    game = ta.TarokGame({'seed': sp.GameParameter(0)})
    state = game.new_initial_tarok_state()
    print_info(game, state)
    state.apply_action(0)
    print_info(game, state)


def print_info(game, state):
    print("Game phase: {}".format(state.current_game_phase()))
    print("Current player: {}".format(state.current_player()))
    print("Legal actions: {}".format(
        [state.action_to_string(a) for a in state.legal_actions()]))
    print("Chance outcomes: {}\n".format(state.chance_outcomes()))
    print_players_cards(game, state)
    print("Talon: {}".format(state.talon()))


def print_players_cards(game, state):
    for player in range(game.num_players()):
        print("Player {:d} cards:".format(player))
        for card in state.player_cards(player):
            print(card)
        print()


if __name__ == '__main__':
    main()
