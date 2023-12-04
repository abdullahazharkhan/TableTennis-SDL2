#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "definitions.h"
#include "structs.h"

bool started = false;
Ball ball;
Player player1;
Player player2;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
FILE *file;

// Score Bar
SDL_Rect score = {
    .x = WIDTH / 2 - 60,
    .y = HEIGHT - 40,
    .w = 120,
    .h = 20,
};

// for color
bool player1Score = false;
bool player2Score = false;

bool Initialize(void);
void Update(float);
void End(void);
Ball CreateBall(int size);
void UpdateBall(Ball *ball, float elapsed);
void RenderBall(const Ball *);
Player MakePlayer(void);
void UpdatePlayers(float elapsed);
void RenderPlayers(void);
void UpdateScore(int player, int points);
void RenderLines(float yPos);

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    atexit(End);

    if (!Initialize())
    {
        exit(1);
    }

    bool quit = false;
    SDL_Event event;

    Uint32 lastTick = SDL_GetTicks();

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        Uint32 curTick = SDL_GetTicks();
        Uint32 diff = curTick - lastTick;
        float elapsed = diff / 1000.0f;
        Update(elapsed); // x time elapsed
        lastTick = curTick;
    }

    SDL_Quit();
    return 0;
}

bool Initialize(void)
{
    /* Initializes the timer, audio, video, joystick,
    haptic, gamecontroller and events subsystems */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    /* Create a window */
    window = SDL_CreateWindow("Table Tennis -- Rally", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    // window created but it does not stay
    // to fix the window, EVENT LOOP comes into play

    if (!window)
    {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!window)
    {
        return false;
    }

    file = fopen("ScoreBoard.txt", "w");
    if (file == NULL)
    {
        printf("Error initializing file\n");
        return false;
    }

    ball = CreateBall(BALLSIZE);
    player1 = MakePlayer();
    player2 = MakePlayer();

    return true;
}

void Update(float elapsed)
{
    // to color the screen
    SDL_SetRenderDrawColor(renderer, 55, 255, 1, 255);
    SDL_RenderClear(renderer);

    UpdateBall(&ball, elapsed);
    RenderBall(&ball);

    UpdatePlayers(elapsed);
    RenderPlayers();

    for (int i = 500; i >= 0; i -= 30)
    {
        RenderLines(i);
    }

    SDL_RenderDrawLine(renderer, 0, HEIGHT - 60, WIDTH, HEIGHT - 60);

    // score bar rendering
    if (player1Score)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else if (player2Score)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    SDL_RenderFillRect(renderer, &score);

    SDL_RenderPresent(renderer);
}

void End(void)
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    if (player1.score > player2.score)
    {
        fprintf(file, "You Lost 😶 --- 😢👎");
    }
    else if (player1.score < player2.score)
    {
        fprintf(file, "You Won 🏆 --- 🥳🎉");
    }
    else
    {
        fprintf(file, "Match Tied --- 😕👔");
    }
    fprintf(file, "\n\n----- Final Score 💯 -----\n");
    fprintf(file, "--> Bot 🤖: %d\n", player1.score);
    fprintf(file, "--> You 👦: %d\n", player2.score);
    SDL_Quit();
}

bool HeadOrTails(void)
{
    return rand() % 2 == 1 ? true : false;
}

Ball CreateBall(int size)
{
    Ball ball = {
        .x = WIDTH / 2 - size / 2,
        .y = HEIGHT / 2 - size / 2,
        .size = size,
        .xSpeed = SPEED * (HeadOrTails() ? 1 : -1),
        .ySpeed = SPEED * (HeadOrTails() ? 1 : -1),
    };
    return ball;
}

void RenderBall(const Ball *ball)
{
    int size = ball->size;
    int halfSize = size / 2;
    SDL_Rect ballRect = {
        .x = ball->x - halfSize,
        .y = ball->y - halfSize,
        .w = size,
        .h = size,
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ballRect);
}

void UpdateBall(Ball *ball, float elapsed)
{
    if (!started)
    {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        return;
    }

    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;

    // to restrict the ball inside the windnow
    if (ball->x < BALLSIZE / 2)
    {
        // ball->xSpeed = abs(ball->xSpeed);
        UpdateScore(2, 100);
    }
    if (ball->x > WIDTH - BALLSIZE / 2)
    {
        // ball->xSpeed = -abs(ball->xSpeed);
        UpdateScore(1, 100);
    }
    if (ball->y < BALLSIZE / 2)
    {
        ball->ySpeed = abs(ball->ySpeed);
    }
    if (ball->y > HEIGHT - 60 - BALLSIZE / 2)
    {
        ball->ySpeed = -abs(ball->ySpeed);
    }
}

Player MakePlayer(void)
{
    Player player = {
        .yPosition = HEIGHT / 2,
    };
    return player;
}

void UpdatePlayers(float elapsed)
{
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_SPACE])
    {
        started = true;
    }

    // probability
    if ((player1.yPosition + PLAYERHEIGHT / 2) > (ball.y + BALLSIZE / 2) && HeadOrTails())
    {
        if (player1.yPosition > PLAYERHEIGHT / 2 + 10)
            player1.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }
    else if ((player1.yPosition + PLAYERHEIGHT / 2) < (ball.y + BALLSIZE / 2))
    {
        if (player1.yPosition < HEIGHT - PLAYERHEIGHT / 2 - 70)
            player1.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }
    if (keyboardState[SDL_SCANCODE_UP])
    {
        if (player2.yPosition > PLAYERHEIGHT / 2 + 10)
            player2.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }
    if (keyboardState[SDL_SCANCODE_DOWN])
    {
        if (player2.yPosition < HEIGHT - PLAYERHEIGHT / 2 - 70)
            player2.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    // check if ball collides with bars
    SDL_Rect ballRect = {
        .x = ball.x - ball.size / 2,
        .y = ball.y - ball.size / 2,
        .w = ball.size,
        .h = ball.size,
    };

    SDL_Rect player1rect = {
        .x = PLAYERMARGIN,
        .y = (int)(player1.yPosition) - PLAYERHEIGHT / 2,
        .w = PLAYERWIDTH,
        .h = PLAYERHEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &player1rect))
    {
        ball.xSpeed = abs(ball.xSpeed); // make ball go right
    }

    SDL_Rect player2rect = {
        .x = WIDTH - PLAYERWIDTH - PLAYERMARGIN,
        .y = (int)(player2.yPosition) - PLAYERHEIGHT / 2,
        .w = PLAYERWIDTH,
        .h = PLAYERHEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &player2rect))
    {
        ball.xSpeed = -abs(ball.xSpeed); // make ball go left
    }
}
void RenderPlayers(void)
{
    // Render Player 1 (left, red)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect player1rect = {
        .x = PLAYERMARGIN,
        .y = (int)(player1.yPosition) - PLAYERHEIGHT / 2,
        .w = PLAYERWIDTH,
        .h = PLAYERHEIGHT,
    };
    SDL_RenderFillRect(renderer, &player1rect);

    // Render Player 2 (right, blue)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect player2rect = {
        .x = WIDTH - PLAYERWIDTH - PLAYERMARGIN,
        .y = (int)(player2.yPosition) - PLAYERHEIGHT / 2,
        .w = PLAYERWIDTH,
        .h = PLAYERHEIGHT,
    };
    SDL_RenderFillRect(renderer, &player2rect);
}

void UpdateScore(int player, int points)
{
    // to reInitialize the game
    started = false;
    ball.xSpeed += 40;
    ball.ySpeed += 40;
    if (player == 1)
    {
        player1Score = true;
        player2Score = false;
        player1.score += points;
        fprintf(file, "---> Bot scored a point --- 😑😒\n");
        fprintf(file, "-------------------------------------\n");
        if (score.x > 30)
            score.x -= 50;
    }
    if (player == 2)
    {
        player2Score = true;
        player1Score = false;
        player2.score += points;
        fprintf(file, "---> You scored a point --- 😎😏\n");
        fprintf(file, "-------------------------------------\n");
        if (score.x + 120 < WIDTH)
            score.x += 50;
    }
    char *formatString = "Player 1: %d || Player 2: %d";
    int len = snprintf(NULL, 0, formatString, player1.score, player2.score);
    char buf[len + 1];
    snprintf(buf, len + 1, formatString, player1.score, player2.score);

    SDL_SetWindowTitle(window, buf);
}

void RenderLines(float yPos)
{
    SDL_Rect lineRect = {
        .x = (WIDTH / 2) - 1,
        .y = yPos,
        .w = 2,
        .h = 20,
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &lineRect);
}