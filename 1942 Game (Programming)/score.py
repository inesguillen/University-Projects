# THIS FILE WILL CONTAIN ALL THE INFORMATION RELATED TO THE SCORING OF THE GAME
import pyxel
from enemies import RegularEnemy, RedEnemy, BombardierEnemy, SuperbombardierEnemy

class Score:

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

        self.sprite = ()
        self.points = 0

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

    def regular_red_Kill(self):

        self.points += 100
        self.sprite = (0, 0, 96, 48, 16)

    def bomb_Kill(self):
        self.points += 300

    def super_bombKill(self):
        self.points += 500

    def draw_plane_points(self):
        pyxel.blt(self.x, self.y, 0, 0, 96, 48, 0)

    def draw_total_score(self):
        if 100 <= self.points < 1000:
            # The 2 zeros:
            pyxel.blt(15, 25, 0, 38, 67, 4, 11, 0)
            pyxel.blt(20, 25, 0, 38, 67, 4, 11, 0)
        elif self.points > 1000:
            # The 3 zeros:
            pyxel.blt(15, 25, 0, 38, 67, 4, 11, 0)
            pyxel.blt(20, 25, 0, 38, 67, 4, 11, 0)
            pyxel.blt(25, 25, 0, 38, 67, 4, 11, 0)

        if str(self.points)[0] == '1':
            pyxel.blt(10, 25, 0, 34, 67, 4, 11, 0)
        elif str(self.points)[0] == '2':
            pyxel.blt(10, 25, 0, 43, 67, 4, 11, 0)
        elif str(self.points)[0] == '3':
            pyxel.blt(10, 25, 0, 49, 67, 4, 11, 0)
        elif str(self.points)[0] == '4':
            pyxel.blt(10, 25, 0, 54, 67, 4, 11, 0)
        elif str(self.points)[0] == '5':
            pyxel.blt(10, 25, 0, 59, 67, 4, 11, 0)
        elif str(self.points)[0] == '6':
            pyxel.blt(10, 25, 0, 64, 67, 4, 11, 0)
        elif str(self.points)[0] == '7':
            pyxel.blt(10, 25, 0, 69, 67, 4, 11, 0)
        elif str(self.points)[0] == '8':
            pyxel.blt(10, 25, 0, 74, 67, 4, 11, 0)
        elif str(self.points)[0] == '9':
            pyxel.blt(10, 25, 0, 79, 67, 4, 11, 0)
        elif str(self.points)[0] == '0':
            pyxel.blt(10, 25, 0, 38, 67, 4, 11, 0)