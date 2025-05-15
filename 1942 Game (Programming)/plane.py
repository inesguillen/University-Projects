# THIS FILE WILL CONTAIN THE MOTHER CLASS FOR ALL THE PLANES IN THE GAME, AND ALSO WILL INCLUDE THE USER'S PLANE
import time

import pyxel

from bullets import Bullets
from constants import WIDTH, HEIGHT


class Plane:

    def __init__(self, x, y):       # atts x and y will determine the position of the plane

        self.x = x
        self.y = y
        self.alive = True
        self.sprite = ()

    # DEFINITION OF THE PROPERTIES AND SETTERS
    @property
    def x(self):
        return self.__x

    @x.setter
    def x(self, x):
        self.__x = x

    @property
    def y(self):
        return self.__y

    @y.setter
    def y(self, y):
        self.__y = y

    # This method will change the plane sprite to the explosion one
    def explode(self):

        self.sprite = (0, 48, 32, 16, 16)

        self.alive = False


# WE WILL USE INHERITANCE TO DETERMINE EACH PLANE, THIS BELOW WILL BE THE USER'S ONE
class UserPlane(Plane):

    def __init__(self, x=WIDTH/2, y=200):
        super().__init__(x, y)

        # we want an animation of the flames that go below the plane, so we will change the sprite each frame

        if pyxel.frame_count % 2 == 0:     # each even frame the sprite will be changed
            self.sprite_change = 0
        else:
            self.sprite_change = 16

        # it is the position of the design of the plane in the image bank
        self.sprite = (0, self.sprite_change, 64, 16, 16)

        self.lives = 3

    def move(self):
        """THIS METHOD MOVE WILL CHANGE THE PLANE'S POSITION AND ALSO WILL CHECK IF IT DOESN'T
        SURPASS THE BOARD BORDERS"""

        plane_size = self.sprite[3]       # the third position of the sprite is the size of the design

        # THE PLANE WILL MOVE ONLY WHEN IT IS ALIVE
        if self.alive:

            if (pyxel.btn(pyxel.KEY_RIGHT) or pyxel.btn(pyxel.KEY_D)) and self.x < (WIDTH - plane_size):
                self.x = self.x + 2

            elif (pyxel.btn(pyxel.KEY_LEFT) or pyxel.btn(pyxel.KEY_A)) and self.x > 0:
                self.x = self.x - 2

            elif (pyxel.btn(pyxel.KEY_UP) or pyxel.btn(pyxel.KEY_W)) and self.y > 0:
                self.y = self.y - 2

            elif (pyxel.btn(pyxel.KEY_DOWN) or pyxel.btn(pyxel.KEY_S)) and self.y < (HEIGHT - plane_size):
                self.y = self.y + 2

            if pyxel.btnp(pyxel.KEY_SPACE):
                Bullets(self.x, self.y)

        else:
            # If the plane has lives yet it will revive
            if self.lives > 0:
                self.alive = True
                self.lives -= 1

    def draw(self):
        pyxel.blt(self.x, self.y, *self.sprite, 0)
