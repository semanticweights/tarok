import pyspiel as sp
import pytarok as tr


def main():
    tarok_game = tr.new_tarok_game({})
    print(tarok_game.num_distinct_actions())


if __name__ == '__main__':
    main()
