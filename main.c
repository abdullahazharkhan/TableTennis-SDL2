#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define WIDTH 640
#define HEIGHT 480
#define BALLSIZE 15
#define PLAYERWIDTH 20
#define PLAYERHEIGHT 75
#define PLAYERMARGIN 10
const float PLAYER_MOVE_SPEED = 150.0f;
float SPEED = 120;

bool served = false;

typedef struct Ball
{
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;

} Ball;

typedef struct Player
{
    int score;
    float yPosition;
} Player;

Ball ball;

Player player1;
Player player2;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool Initialize(void);
void Update(float);
void Shutdown(void);

Ball MakeBall(int size);
void UpdateBall(Ball *ball, float elapsed);
void RenderBall(const Ball *);

Player MakePlayer(void);
void UpdatePlayers(float elapsed);
void RenderPlayers(void);

void UpdateScore(int player, int points);

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    atexit(Shutdown);

    if (!Initialize())
    {
        exit(1);
    }

    /* Create Event loop*/
    // SDL_Event event;
    // while (SDL_WaitEvent(&event))
    // {
    //     if (event.type == SDL_QUIT)
    //     {
    //         break;
    //     }
    // }

    bool quit = false;
    SDL_Event event;
    // pollEvent is better than waitEvent

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
    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    // window created but it does not stay
    // to fix window, EVENT LOOP plays a role

    if (!window)
    {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!window)
    {
        return false;
    }

    ball = MakeBall(BALLSIZE);
    player1 = MakePlayer();
    player2 = MakePlayer();

    return true;
}
void Update(float elapsed)
{
    // to color the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // // drawing ball
    // SDL_Rect ballRect = {
    //     .x = WIDTH / 2 - BALLSIZE / 2,
    //     .y = HEIGHT / 2 - BALLSIZE / 2,
    //     .w = BALLSIZE,
    //     .h = BALLSIZE,
    // };
    // // render ball
    // SDL_RenderFillRect(renderer, &ballRect);

    UpdateBall(&ball, elapsed);
    // upar ka kaam neeche func kay andar
    RenderBall(&ball);

    UpdatePlayers(elapsed);
    RenderPlayers();
    SDL_RenderPresent(renderer);
}
void Shutdown(void)
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

bool CoinFlip(void)
{
    return rand() % 2 == 1 ? true : false;
}

Ball MakeBall(int size)
{
    Ball ball = {
        .x = WIDTH / 2 - size / 2,
        .y = HEIGHT / 2 - size / 2,
        .size = size,
        .xSpeed = SPEED * (CoinFlip() ? 1 : -1),
        .ySpeed = SPEED * (CoinFlip() ? 1 : -1),
    };
    return ball;
}

void RenderBall(const Ball *ball)
{
    int size = ball->size;
    int halfSize = size / 2;
    SDL_Rect rect = {
        .x = ball->x - halfSize,
        .y = ball->y - halfSize,
        .w = size,
        .h = size,
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void UpdateBall(Ball *ball, float elapsed)
{
    if (!served)
    {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        return;
    }

    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;

    // takay ball window se bahir na jaa ske
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
    if (ball->y > HEIGHT - BALLSIZE / 2)
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
        served = true;
    }

    if (keyboardState[SDL_SCANCODE_W])
    {
        if (player1.yPosition > PLAYERHEIGHT / 2 + 10)
            player1.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        if (player1.yPosition < HEIGHT - PLAYERHEIGHT / 2 - 10)
            player1.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }
    if (keyboardState[SDL_SCANCODE_UP])
    {
        if (player2.yPosition > PLAYERHEIGHT / 2 + 10)
            player2.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }
    if (keyboardState[SDL_SCANCODE_DOWN])
    {
        if (player2.yPosition < HEIGHT - PLAYERHEIGHT / 2 - 10)
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
        ball.xSpeed = -abs(ball.xSpeed); // make ball go right
    }
}
void RenderPlayers(void)
{
    // Render Player 1 (left, red)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect player1rect = {
        .x = PLAYERMARGIN,
        .y = (int)(player1.yPosition) - PLAYERHEIGHT / 2,
        .w = PLAYERWIDTH,
        .h = PLAYERHEIGHT,
    };
    SDL_RenderFillRect(renderer, &player1rect);

    // Render Player 2 (right, blue)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
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
    // to restart the game
    served = false;
    if (player == 1)
    {
        player1.score += points;
    }
    if (player == 2)
    {
        player2.score += points;
    }
    char *fmt = "Player 1: %d || Player 2: %d";
    int len = snprintf(NULL, 0, fmt, player1.score, player2.score);
    char buf[len + 1];
    snprintf(buf, len + 1, fmt, player1.score, player2.score);

    SDL_SetWindowTitle(window, buf);
}