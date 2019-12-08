# Endless Tetris
Classic Tetris Game but with time pressure and levels.

## What is Tetris
Tetris is a classic game that was developed by Alexey Pajitnov in 1984. The gameplay is simple, but addictive. The game consists of 10x24 board (but any size will work). Tetrominos (which are coloured shapes which come in different forms such as L, Z, squares, lines) fall from the top to the ground. The player can rotate these. The goal is to complete lines, thus increasing your score. After a line is filled, it clears, and all the tetrominos which were above fall down. If this is not clear enough, watch [this.](https://www.youtube.com/watch?v=-FAzHyXZPm0)

## Differences between Classic Tetris and Endless Tetris
The main difference here is that the game is split into levels with different goals and time pressure. Advancing a level makes the game harder by increasing the speed at which tetrominos fall. I like speedrunnning games and thought this would make Tetris more like a speedrun, and more challenging at the same time.

## Gameplay description
The player selects a difficulty (which determines the time for each level and the final score multiplier).
The player starts the game. He has a time limit to complete a number of lines, after which the game continues to the next level (the board is cleared). Each level gets progressively faster, which both increases its difficulty and its score. If a player finishes a level before its time limit, the remaining time is added to the score as a bonus. If the player fails to finish a level, the game ends and the score is calculated.

## Score
The level score is calculated as follows (values will change based on playtesting):
 - Tetromino falls to the ground: +1pt
 - Line is completed: +10pt
 - Tetromino is rushed to the ground: +distanceLeft points
 - Tetromino is rotated more than 3 times: -5pt
 - Tetromino is changed (pressing the joystick switch): -20pt
 - Bonus time left: +seconds points
 
 **Note**: each positive score is multiplied with the level modifier, which is calculated as follows:
 levelModifier = 1 + currentLevel * 0.3

The final score is calculated as follows:
finalScore = difficultyMultiplier * (lvl1Score + lvl2Score + ...)

The difficulty multiplier can either be 0.5 (easy), 1 (medium) or 2 (hard).

## Meeting technical requirements

 1. Score: see above
 2. Progress in difficulty: see levels (the speed gets faster, but also the score gets better, also the game difficulty can be accounted into this, which modifies the level time and final score multiplier)
 3. 2 other requirements: levels (see gameplay description) and time pressure (see gameplay description)
