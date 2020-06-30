import pyspiel as sp
import pytarok as ta


if __name__ == '__main__':
    game = ta.TarokGame({
        'num_players': sp.GameParameter(4),
        # 'seed': sp.GameParameter(0),
    })
    state = game.new_initial_state()

    while not state.is_terminal():
        legal_actions = state.legal_actions()
        action_names = [state.action_to_string(a)
                        for a in state.legal_actions()]
        print("Legal actions: {}\n".format(
            [x for x in zip(action_names, legal_actions)]
        ))
        state.apply_action(int(input("Enter action: ")))
        print("Info states: ")
        for player in range(game.num_players()):
            print(state.information_state_string(player))
        print()
