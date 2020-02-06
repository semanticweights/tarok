import open_tarok
import pyspiel


def main():
    num_games = len(pyspiel.registered_names())
    print("Number of registered games in open_spiel: {:d}".format(num_games))
    tarok = open_tarok.TarokGame()
    print(tarok.get_name())


if __name__ == '__main__':
    main()
