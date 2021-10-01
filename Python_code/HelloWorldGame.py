from HovercraftGame import *

class HelloWorldGame(HovercraftGame):

    def __init__(self):
        self.count = 0

    def loop(self, event):
        if event is not None:
            print(event)
            self.count += 1
        return self.count < 10