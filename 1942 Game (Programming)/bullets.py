# THIS FILE WILL CONTAIN ALL THE INFORMATION RELATED TO BULLET'S MOVEMENT
import pyxel


class Bullets:

    bulletList = []         # this list will contain the created bullets

    def __init__(self, x, y):

        self.x = x
        self.y = y

        self.sprite = (0, 16, 16, 16, 16)

        Bullets.bulletList.append(self)

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

    def userMove(self):
        """THIS METHOD MOVE WILL CHANGE THE BULLET'S POSITIONS"""
        self.y -= 10

    def draw(self):
        pyxel.blt(self.x, self.y, *self.sprite, 0)


# BULLETS OF THE ENEMIES
class EnemyBullets():

    bulletList = []         # this list will contain the created bullets

    def __init__(self, x, y):

        self.x = x
        self.y = y

        self.sprite = (0, 32, 16, 16, 16)

        EnemyBullets.bulletList.append(self)
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

    def userMove(self,variant=0):
        """THIS METHOD MOVE WILL CHANGE THE BULLET'S POSITIONS"""
        self.y += 10

    def draw(self):
        print(self.x,self.y)
        pyxel.blt(self.x, self.y, *self.sprite, 0)

    def remove(self):
        EnemyBullets.bulletList.remove(self)