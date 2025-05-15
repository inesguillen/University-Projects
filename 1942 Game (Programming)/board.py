import pyxel
import random
import time

from constants import WIDTH, HEIGHT, POS_REG, POS_RED, POS_BOM, POS_SUP, SIZE_ENE
from plane import UserPlane
from enemies import RegularEnemy, RedEnemy, BombardierEnemy, SuperbombardierEnemy
from bullets import Bullets,EnemyBullets
from bakground import Background1, Background2
from score import Score


class Board:
    # attributes will be the weight and height of the screen/board

    def __init__(self, width: int = WIDTH, height: int = HEIGHT):
        self.width = width
        self.height = height

        pyxel.init(self.width, self.height, "1942")   # initialization of pyxel

        pyxel.load("assets/resources.pyxres")

        # OBJECTS DEFINITION ...........................................................................................
        # plane user
        self.userPlane = UserPlane((self.width//2), 200)

        self.bulletList = Bullets.bulletList
        self.enemybulletList = EnemyBullets.bulletList

        # ENEMIES: 21 REGULARS, 6 REDS, 2 BOMBARDIERS, 1 SUPER BOMBARDIER
        self.enemyList = []

        self.x_sup = 0
        self.y_sup = 0

        self.create_enemy()

        # SELF.ENEMY WILL DECIDE WHICH ENEMY WILL APPEAR ON SCREEN
        self.enemy = random.randint(0, 1)
        self.enemy = 0
        # background objects
        self.background1 = Background1()
        self.background2 = Background2()

        # SCORE POINTS:
        self.score = Score()

        # TRIES FOR THE ENEMIES
        # self.enemyTry = RegularEnemy()

        # RUNNING OF THE GAME ..........................................................................................
        pyxel.run(self.update, self.draw)       # to run the game

    def create_enemy(self,enemy=0):
        # regular enemies: 21 enemies. 7 groups of 3
        if enemy <= POS_SUP:

            if enemy == POS_REG:
                regularList = []
                for j in range(1):
                    regularGroup = []
                    sum = 0
                    for k in range(3):
                        # This "sum" operation will make the 3 planes appear together but not in formation
                        regular_enemy = RegularEnemy()

                        regular_enemy.x += sum

                        regularGroup.append(regular_enemy)
                        sum += 20
                        #random.randint(20, HEIGHT - 16)

                    regularList.append(regularGroup)

                self.enemyList.append(regularList)

            elif enemy == POS_RED:

                # red enemies: 6 enemies. 2 groups of 3
                redList = []
                for j in range(2):
                    redGroup = []
                    sum = 0

                    for k in range(3):
                        # This "sum" operations will make the 3 planes go in group and in formation
                        red_enemy = RedEnemy()

                        red_enemy.x += sum
                        red_enemy.y += sum

                        redGroup.append(red_enemy)

                        sum += 20

                    redList.append(redGroup)

                self.enemyList.append(redList)
            elif enemy == POS_BOM:
                # bombardier enemies: 2 enemies
                bombardierList = []
                for j in range(2):
                    bombardierList.append(BombardierEnemy())

                self.enemyList.append(bombardierList)
            else:
                # super bombardier enemy: 1 enemy
                self.enemyList.append(SuperbombardierEnemy())

            self.create_enemy(enemy+1)
    def reposicionar_enemigos(self):
        if self.enemy == POS_REG:
            for k in range(len(self.enemyList[self.enemy])):
               sum = 0
               for item in self.enemyList[self.enemy][k]:
                    item.x += sum
                    item.y = (0 + item.sprite[4])
                    sum += 20
                    #random.randint(20, HEIGHT - 16)
        elif self.enemy == POS_RED:
            for k in range(len(self.enemyList[self.enemy])):
                sum = 0
                for item in self.enemyList[self.enemy][k]:
                    item.x += sum
                    item.y += sum
                    sum += 20
        elif self.enemy == POS_BOM:
            for item in self.enemyList[self.enemy]:
                item.x=0
                item.y=0
        else:
            self.enemyList[self.enemy].x = (self.enemyList[self.enemy].sprite[3])
            self.enemyList[self.enemy].y = (self.enemyList[self.enemy].sprite[4])
    # UPDATE METHOD ----------------------------------------------------------------------------------------------------
    def update(self):
        """THIS METHOD WILL CHECK EVERYTHING AT ALL TIMES"""

        if pyxel.btn(pyxel.KEY_Q) or pyxel.btn(pyxel.KEY_ESCAPE):
            pyxel.quit()

        # User's plane: MOVEMENT .......................................................................................
        self.userPlane.move()

        # Bullets of the user's plane: MOVEMENT ........................................................................
        for bullet in self.bulletList:
            bullet.userMove()

        for enemybullet in self.enemybulletList:
            enemybullet.userMove()

        # Enemies: MOVEMENT ...............................................................................

        '''random.seed()
        e.x = random.randrange(SIZE_ENE, WIDTH)
        e.y = random.randrange(-HEIGHT, -SIZE_ENE)
        '''
        x, y = 0, 0
        if (self.enemy == POS_REG) or (self.enemy == POS_RED):
            for k in range(len(self.enemyList[self.enemy])):

                for item in self.enemyList[self.enemy][k]:
                    '''if item.y > HEIGHT:
                        i=random.randint(1, 4)
                        item.move(i)
                    else:'''
                    item.move()

                    # ENEMIES: EXPLOSIONS ..............................................................................
                    for bullet in self.bulletList:
                        print(item.x , item.y, bullet.x, bullet.y)
                        if (item.x <= bullet.x <= (item.x + item.sprite[3])) and \
                                (item.y <= bullet.y <= (item.y + item.sprite[4])):
                            self.score.x = item.x
                            self.score.y = item.y
                            self.score.draw_plane_points()

                            item.explode()

                            # Removal of the item from the enemies list
                            self.enemyList[self.enemy][k].remove(item)

                            # Removal of the bullet from the bullets list
                            self.bulletList.remove(bullet)

                            # Score punctuation
                            self.score.regular_red_Kill()

        elif self.enemy == POS_BOM:

            for item in self.enemyList[self.enemy]:
                self.x_sup,  self.y_sup = item.move()
        else:
            self.x_sup,  self.y_sup = self.enemyList[self.enemy].move(self.x_sup,  self.y_sup)
            print(self.x_sup,self.y_sup)
            #self.enemyList[self.enemy].y+=30
        if self.y_sup > HEIGHT or self.y_sup < 0 or self.x_sup<0 or self.x_sup > WIDTH:
            self.enemy =0
        # background objects: MOVEMENT .................................................................................
        # for background1 in self.backgroundList:
        self.background1.move()
        self.background2.move()


    # DRAW METHOD ------------------------------------------------------------------------------------------------------
    def draw(self,enemy=0):
        """THIS METHOD WILL SHOW THE OBJECTS ON SCREEN"""

        # BACKGROUND
        pyxel.cls(1)

        pyxel.blt(self.background1.x, self.background1.y, *self.background1.sprite1, 0)
        pyxel.blt(self.background2.x, self.background2.y, *self.background2.sprite2, 0)

        kill = False

        # IMAGE OF THE REGULAR PLANES AND THEIR BULLETS
        if (self.enemy == POS_REG) or (self.enemy == POS_RED):

            for j in range(len(self.enemyList[self.enemy])):

                for item in self.enemyList[self.enemy][j]:
                    item.draw()

        elif self.enemy == POS_BOM:

            for item in self.enemyList[self.enemy]:
                item.draw()
        else:
            self.enemyList[self.enemy].draw(self.x_sup, self.y_sup)



        # IMAGE OF THE USER'S PLANE
        self.userPlane.draw()

        # IMAGE OF THE BULLETS FOR THE USER PLANE
        for bullet in self.bulletList:
            bullet.draw()

        # IMAGE OF THE BULLETS FOR THE USER PLANE

        for enemibullets in self.enemybulletList:
            enemibullets.draw()

        # IMAGE OF THE SCORE BOARD
        pyxel.blt(5, 5, 0, 0, 80, 48, 16, 0)
        self.score.draw_total_score()
        self.score.draw_plane_points()
