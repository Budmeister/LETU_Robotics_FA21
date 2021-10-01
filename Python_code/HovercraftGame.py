
# The parent hovercraft game class
# All hovercraft games will inherit from this class
class HovercraftGame:

    # Params: event (str) if a message was received from the arduino
    # this frame, else None
    # Returns: output to Arduino (str) if any, else None or "Done" if 
    # the game is finished
    def loop(self, event):
        return None
