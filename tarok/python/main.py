import pyspiel
import pytarok


def main():
    print(pyspiel.registered_names())
    tarok_game = pytarok.new_tarok_game()
    print(tarok_game)


if __name__ == '__main__':
    main()
