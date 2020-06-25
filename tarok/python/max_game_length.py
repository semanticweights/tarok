import pyspiel as sp
import pytarok as ta

# Recursively computes the longest game up to tricks playing.
# Results:
#
# Number of players: 3
# Longest round up to tricks playing: 17
# History: [0, 3, 3, 4, 4, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 0, 12]
# Selected contract: Contract.VALAT_WITHOUT
#
# Number of players: 4
# Longest round up to tricks playing: 24
# History: [0, 0, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 0, 12]
# Selected contract: Contract.VALAT_WITHOUT

ROUND_LENGTHS = []


def max_game_length():
    game = ta.TarokGame({
        'num_players': sp.GameParameter(4),
    })
    state = game.new_initial_state()
    state.apply_action(0)
    dft(state)

    print("Number of players: {}".format(game.num_players()))
    longest_round = max(ROUND_LENGTHS, key=lambda x: len(x[1]))
    print("Longest round up to tricks playing: {}".format(
        len(longest_round[1]) - 1))
    print("History: {}".format(longest_round[1][1:]))
    print("Selected contract: {}".format(longest_round[0]))


def dft(state):
    if state.current_game_phase() == ta.GamePhase.TRICKS_PLAYING:
        ROUND_LENGTHS.append((state.selected_contract(), state.history()))
        return
    for action in state.legal_actions():
        current_state = state.clone()
        current_state.apply_action(action)
        dft(current_state)


if __name__ == '__main__':
    max_game_length()
