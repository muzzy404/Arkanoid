# Arkanoid
This is a clone of popular Arkanoid game with my own graphics. This game is my first quite serious C++ program. Before this project I made only simple university labs.

**Tools:**
- VS 2013 as IDE
- SFML (Simple and Fast Multimedia Library) for graphics and music
- GIMP for creation of all game graphic elements

**Description of game**
The player controls a paddle which prevents a ball from falling from the playing field, attempting to bounce it against a number of bricks. The ball striking a brick causes the brick to disappear. When all the bricks are gone, the player goes to the next level, where another pattern of bricks appears. Every brick has own random durability - the number of hits to make a brick to disappear.

**Creation of level maps**
1. create file by template lvl_01.txt (not more than 6 symbols)
2. size of map 18x6
3. bricks are set by numbers from 0 to 5

    | Number | Brick |
    | ------ | ------ |
    | 0 | no brick |
    | 1 | blue |
    | 2 | red |
    | 3 | green |
    | 4 | orange |
    | 5 | cyan |

**Screenshots**

![arkanoid screenshot](https://cldup.com/70Myv67JSh.png)
![arkanoid screenshot](https://cldup.com/4X89To6c7e.png)
![arkanoid screenshot](https://cldup.com/n1tr09CgDU.png)
![arkanoid screenshot](https://cldup.com/ghR5LMvK5X.png)
![arkanoid screenshot](https://cldup.com/tgxTr8jYXT.png)