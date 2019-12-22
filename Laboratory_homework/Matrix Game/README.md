# Endless Tetris
Classic Tetris Game but with time pressure and levels.
For a demo, see LINK

## Introdruction
Tetris is a classic game that was developed by Alexey Pajitnov in 1984. The gameplay is simple, but addictive. The game consists of 10x24 board (but any size will work). Tetrominos (which are coloured shapes which come in different forms such as L, Z, squares, lines) fall from the top to the ground. The player can rotate these. The goal is to complete lines, thus increasing your score. After a line is filled, it clears, and all the tetrominos which were above fall down. For a gameplay sample of the original, watch [this.](https://www.youtube.com/watch?v=-FAzHyXZPm0)

## How is Endless Tetris different from the classic game?
The gameplay is exactly the same, except that the game has a certain **goal** per level (number of lines to complete) and a time pressure to complete them. If the player manages to complete the goal, then the board is cleared and the next level starts. The difference between levels is the difficulty of the game, which increases slowly, but surely. The piece spawning delay, the fall interval and even the scores are affected, making it for a challening, but fun experience. 

## How to play
TODO



## Score
Let us denote **i** as the current level at which the player is at. The score is calculated as follows:
* Tetromino falls to the ground - i * 10
* Tetromino is rushed one block down - i * 1
* Line is completed - i * 20
* Tetromino is rotated more than 3 times - i * (-15)
* Tetromino is swapped - i * (-20)

The score is the sum of all of these actions. Obviously, advancing in level (completing the number of lines for a particular level) makes it easier to reach higher score, but also penalizes mistakes way more harshly.

## Meeting technical requirements

 1. Score: see above
 2. Progress in difficulty: see levels (the speed gets faster, but also the score gets better, also the game difficulty can be accounted into this, which modifies the level time and final score multiplier)
 3. 2 other requirements: levels (see gameplay description) and time pressure (see gameplay description)
 
 ## Hardware needed
 Here is a minimal list of hardware needed to get something similar to my setup
 
 * Arduino UNO or similar (needs at least 10 pins)
 * Breadboard (if you don't plan to solder it, of course)
 * LCD Display (I used a 16x2 for this project)
 * Joystick
 * 8x8 LED Matrix (I used a common cathode one)
 * MAX 7219 Matrix Driver or similar (this makes managing the matrix way easier)
 * Resistors (I used 10k resistors because I hate bright lights)
 * Capacitors (these are used for noise filtering, I used a 10 mF and a 104 one)
 * Lots of wires
 * OPTIONAL: Buzzers (if you want music or SFX)
 
 This is how the final setup should look like: 
