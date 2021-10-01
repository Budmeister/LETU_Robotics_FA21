
import sys
from HovercraftGame import *
from GameFunctions import *
from multiprocessing import Process

if len(sys.argv) == 0:
    print("Please specify which game you would like to play.")
    quit(1)
games = {}  # games = {"Racing" : RacingGame, "Hockey" : HockeyGame, "Sumo" : SumoGame}

Game = games[sys.argv[0]]
if Game is None:
    print("Invalid game name: " + sys.argv[0])
    quit(1)

game = Game()

if game.__class__.loop is not HovercraftGame.loop:
    def loop():
        while True:
            game.loop()

    Process(target=loop).start()

if game.__class__.process_event is not HovercraftGame.process_event:
    def events():
        while True:
            game.process_event(get_event())

    Process(target=events).start()