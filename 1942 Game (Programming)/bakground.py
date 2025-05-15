# THIS FILE WILL CONTAIN ALL THE ELEMENTS THAT APPEAR IN THE BACKGROUND OF THE MAIN GAME

import random

from constants import WIDTH, HEIGHT

class BackgroundObjects:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def move(self):
        """This method will manage the movements and appearance of all the background elements"""
        if self.y > HEIGHT:
            self.y = - 230
        else:
            self.y += 2

#Use of inheritance to define the 2 backgrounds that will be changing
class Background1(BackgroundObjects):
    def __init__(self, x=0, y=0):
        super().__init__(x, y)

        self.sprite1 = (1,0, 0, 255, 255)


class Background2(BackgroundObjects):
    def __init__(self, x=0, y=-250):
        super().__init__(x,y)

        self.sprite2 = (2, 0, 0, 255, 255)




