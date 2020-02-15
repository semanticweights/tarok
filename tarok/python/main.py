import pyspiel as sp
import pytarok as ta


def main():
    tarok_game = ta.new_tarok_game({})
    print(tarok_game.num_distinct_actions())


if __name__ == '__main__':
    main()
