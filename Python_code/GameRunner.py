
import sys
from HelloWorldGame import HelloWorldGame
from HovercraftGame import *
from threading import Thread
import serial
import serial.tools.list_ports

if __name__ == "__main__":
    games = {"Hello_World": HelloWorldGame}  # games = {"Racing" : RacingGame, "Hockey" : HockeyGame, "Sumo" : SumoGame}
    game = None
    port = None
    ports = serial.tools.list_ports.comports()
    def port_exists(port):
        for p in ports:
            if p[0] == port:
                return True
        return False

    if len(sys.argv) == 2:
        game = sys.argv[1]
        if game not in games:
            game = None
            port = sys.argv[1]
            if not port_exists(port):
                print("Invalid game name: " + sys.argv[1])
                quit(1)
    if len(sys.argv) > 2:
        game = sys.argv[1]
        port = sys.argv[2]
        if game not in games:
            print("Invalid game name: " + game)
            quit(1)
        if not port_exists(port):
            print("Invalid port name: " + port)
            quit(1)

    if game is None:
        print("Which game?")
        for g in games.keys():
            print("  " + g)
        game = input()
        if game not in games:
            print("Invalid game name: " + game)
            quit(1)

    if port is None:
        print()
        print("Which port?")
        for p in ports:
            print("  " + p[0] + " - " + p[1])
        port = input().strip()
        if not port_exists(port):
            print("Invalid port name: " + port)
            quit(1)
    
    print()
    print("Starting " + game)
    Game = games[game]
    game = Game()
    
    baud = 9600
    ser = serial.Serial(port, baud, timeout=0.1)
    buffer_string = ''

    def event_ready():
        global buffer_string
        buffer_string += ser.read(ser.inWaiting()).decode()
        return '\n' in buffer_string

    def get_event():
        global buffer_string
        try:
            endline = buffer_string.index('\n')
        except ValueError:
            return None
        event = buffer_string[:endline]
        buffer_string = buffer_string[endline+1:0]
        return event

    if game.__class__.loop is not HovercraftGame.loop:
        def loop():
            try:
                running = True
                while running:
                    if event_ready():
                        running = game.loop(get_event())
                    else:
                        running = game.loop(None)
            finally:
                ser.close()
                print("Serial closed")

        print("Starting loop thread")
        Thread(target=loop, daemon=False).start()
    else:
        ser.close()
        print("Serial closed")