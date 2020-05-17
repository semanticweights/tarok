import pyspiel as sp
import pytarok as ta


def main():
    game = ta.TarokGame({'seed': sp.GameParameter(0)})
    state = game.new_initial_tarok_state()
    print("Current player: {}".format(state.current_player()))
    print("Legal actions: {}".format(state.legal_actions()))
    print("Chance outcomes: {}\n".format(state.chance_outcomes()))

    print("Dealing cards...")
    state.apply_action(0)
    print("Current player: {}".format(state.current_player()))
    print("Player cards: {}".format(state.player_cards(0)))
    print("Talon: {}".format(state.talon()))


if __name__ == '__main__':
    main()
