#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

typedef struct Ball
{
    float x;
    float y;
    int size;
    float xSpeed;
    float ySpeed;

} Ball;

typedef struct Player
{
    int score;
    float yPosition;
} Player;

typedef struct CenterLine
{
    float x;
    float y;
    int size;
} CenterLine;
