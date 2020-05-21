import pyspiel as sp
import pytarok as ta


def main():
    params = {'num_players': sp.GameParameter(3), 'seed': sp.GameParameter(0)}
    game = ta.TarokGame(params)
    state = game.new_initial_tarok_state()
    while(True):
        print_info(game, state)
        state.apply_action(int(input("Enter action: ")))


def print_info(game, state):
    print("Game phase: {}".format(state.current_game_phase()))
    print("Selected contract: {}".format(state.selected_contract()))
    print("Current player: {}".format(state.current_player()))
    legal_actions = state.legal_actions()
    legal_actions = zip([state.action_to_string(a)
                         for a in state.legal_actions()], legal_actions)
    print("Legal actions: {}".format([x for x in legal_actions]))
    print()
    # print_players_cards(game, state)
    # print("Talon: {}".format(state.talon()))


def print_players_cards(game, state):
    for player in range(game.num_players()):
        print("Player {:d} cards:".format(player))
        for card in state.player_cards(player):
            print(card)
        print()


if __name__ == '__main__':
    main()
