# THIS FILE WILL CONTAIN ALL THE ENEMY PLANES INFORMATION
import pyxel
import random

from plane import Plane
from bullets import Bullets, EnemyBullets
from constants import HEIGHT, WIDTH, ENEMY_SPEED,SIZE_ENE



# regular enemies
class RegularEnemy(Plane,EnemyBullets):
    def __init__(self, x=0, y=0, variant=0):
        super().__init__(x, y)

        self.sprite = (0, 16, 0, SIZE_ENE, SIZE_ENE)

        self.y = (0+self.sprite[4])
        self.desplace=random.randint(0, HEIGHT//4)
        self.x = random.randint(20, HEIGHT//2)-self.desplace

        self.variant=variant


        self.bullets=EnemyBullets(self.x, self.y)

    def move(self, variant=random.randint(1, 2)):
        """This method will determine the movement of regular planes, the idea is to have 3 predetermined
        movements that the plane can do and make them random.
        One shot is needed to die."""

        if self.alive:
            #self.variant = variant
            bExit=False
            if self.x > WIDTH or self.y > HEIGHT: # or self.y < 0: '''or self.x < 0 or'''
                self.x = random.randint(0, WIDTH-16) #- self.desplace
                self.y = (0 + self.sprite[4])
               # bExit = True
            else:

                # variant 1,2 and 3: straight forward in different possitions of x

                if variant == 1:
                    self.x = (WIDTH//4)+self.desplace
                    self.y += ENEMY_SPEED
                # variant 4: goes from the left up corner to the center and goes back to the right up corner

                elif variant == 2:
                    #self.x += ENEMY_SPEED
                    if self.x < WIDTH/2:
                        self.y += ENEMY_SPEED
                        self.x += ENEMY_SPEED
                    else:
                        self.sprite = (0, 0, 0, 16, 16)
                        self.x += ENEMY_SPEED
                        self.y -= ENEMY_SPEED
                '''elif variant == 2:
                    #self.x = (WIDTH//4*3)-self.desplace
                    self.y += ENEMY_SPEED

                elif variant == 3:
                    #self.x = (WIDTH//2)+self.desplace
                    self.y += ENEMY_SPEED'''



        if self.bullets.y > HEIGHT or self.bullets.x > WIDTH:
            self.bullets.y = self.y
            self.bullets.x = self.x

        print("REGULAR", variant,self.desplace, self.alive,self.x, self.y)
        if (bExit):
            self.move(random.randint(1, 4))
        return self.x, self.y

    def draw(self):
        pyxel.blt(self.x, self.y, *self.sprite, 0)


# red enemies don't usually shoot, but go in groups. Three loops and they disappear
class RedEnemy(Plane):
    # We will use inheritance with the Plane class
    def __init__(self, x = 0, y = 0):
        super().__init__(x, y)
        self.sprite = (0, 0, 48, SIZE_ENE, SIZE_ENE)

    def move(self, variant=1):             #random.randint(1, 2)):
        """This method will determine the movement of regular planes, red enemies will go in groups of 3,
        when reaching to certain height do 3 loops and disappear. They do not shoot.
        Two shots are needed for them to die"""

        # We have to check if the plane is not dead

        if self.alive:
            if variant == 1:
                if self.x < (WIDTH/2 - 32):
                    self.x += ENEMY_SPEED
                    self.y += ENEMY_SPEED

                elif (WIDTH/2 - 30) <= self.x <= (WIDTH/2 - 25):
                    self.x += ENEMY_SPEED - 1
                    self.y += ENEMY_SPEED

                    self.sprite = (0, 32, 48, SIZE_ENE, SIZE_ENE)

                elif (WIDTH/2 - 25) < self.x <= (WIDTH/2 - 20):
                    self.x += ENEMY_SPEED -1
                    self.y += ENEMY_SPEED

                    self.sprite = (0, 16, 48, SIZE_ENE, SIZE_ENE)

                elif (WIDTH/2 - 20) < self.x <= (WIDTH/2 - 15):
                    self.x += ENEMY_SPEED -1
                    self.y += ENEMY_SPEED

                    self.sprite = (0, 48, 48, 16, 16)

                elif (WIDTH/2 - 15) < self.x <= (WIDTH/2 - 10):
                    self.x += ENEMY_SPEED -1
                    self.y += ENEMY_SPEED

                    self.sprite = (0, 0, 48, 16, 16)

                elif (WIDTH/2 - 10) < self.x <= (WIDTH/2 - 5):
                    self.x += ENEMY_SPEED -1
                    self.y += ENEMY_SPEED

                    self.sprite = (0, 32, 48, 16, 16)

                else: #if self.y > ENEMY_SPEED :
                    self.sprite = (0, 16, 48, 16, 16)
                    self.x += ENEMY_SPEED
                    self.y -= ENEMY_SPEED
        print("RED", variant, self.x, self.y)
        #return self.x, self.y


    def draw(self):
        pyxel.blt(self.x, self.y, *self.sprite, 0)


class BombardierEnemy(Plane):

    def __init__(self, x=0, y=0):
        super().__init__(x, y)

        self.sprite = (0, 48, 0, SIZE_ENE, SIZE_ENE)

    def move(self, variant=random.randint(1, 2)):
        """This method will determine the movement of regular planes, the idea is to have 3 predetermined
        movements that the plane can do and make them random"""

        # variant 1: straight forward
        if variant == 1:
            self.y += ENEMY_SPEED

        # variant 2: up and down from the left to the right
        elif variant == 2:
            self.y += ENEMY_SPEED
            if self.y < HEIGHT - HEIGHT/6:
                if self.x < WIDTH - self.sprite[3]:
                    self.x += ENEMY_SPEED
                elif self.x > WIDTH - self.sprite[3]:
                    self.x -= ENEMY_SPEED
        print("BONBARDER", variant, self.x, self.y)
        return self.x, self.y

    def draw(self):
        pyxel.blt(self.x, self.y, *self.sprite, 0)


class SuperbombardierEnemy(Plane):

    def __init__(self, x=0, y=0):
        super().__init__(x, y)

        self.sprite = (0, 0, 16, 16, 16)

        # this positions will put the plane in the left up corner of the board
        self.x = random.randint(0, WIDTH-self.sprite[3])
        self.y = (self.sprite[4])

    def move(self, x=0, y=0,variant=1):
        """This method will determine the movement of super bombardiers"""
        """if x != self.x:
            self.x = x
        if y != self.y:
            self.y = y"""
        self.y += ENEMY_SPEED

        if variant == 1:
            if self.y < HEIGHT - HEIGHT / 6:
                if self.x < WIDTH - self.sprite[3]:
                    self.x += ENEMY_SPEED
                elif self.x > (WIDTH - self.sprite[3])/2:
                    self.x -= ENEMY_SPEED
                else:
                    self.x = random.randint(0, WIDTH-self.sprite[3])
                    self.y = (self.sprite[4])
        print("SUPER", variant, self.x, self.y)
        return self.x, self.y

    def draw(self, x=0, y=0):

        if x != self.x:
            self.x = x

        if y != self.y:
            self.y = y

        pyxel.blt(self.x, self.y, *self.sprite, 0)

        return self.x, self.y


"""# variant 3: follows the user's plane position
if variant == 2:
    if self.x < UserPlane.x:
        self.x += 2
    if self.x > UserPlane.x:
        self.x -= 2
    if self.y < UserPlane.y:
        self.y += 2
    if self.y > UserPlane.y:
        self.y -= 2"""