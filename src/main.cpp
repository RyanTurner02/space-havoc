#define RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "raygui.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#define ENEMY_SPAWN_TIME 2.5f
#define PLAYER_SHOOTING_TIME 0.25f

typedef enum Sounds
{
    BOOM = 0,
    GAME_OVER,
    FALLING_DOWN,
    SHOOT,
    THUMP
} Sounds;

typedef enum GameScreen
{
    TITLE = 0,
    GAME,
    EXIT
} GameScreen;

void initializeSounds();
void destroySounds();
void initializeGame(Player * player);
void initializeCamera(Camera * camera);
void handleInput(Player * player);
void handleCamera(Camera * camera, Player * player);
void drawPlayer(Player * player);
void spawnEnemy();
void drawEnemies(Player * player);
void moveEnemies();
void moveBullets();
void drawBullets();
void saveScore();
void drawScore();
int readHighScoreFile();
void writeToHighScoreFile();

std::vector<Sound> sounds;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

int lives = 3;
int score = 0;
int highScore = 0;

float enemySpawnDelay = 0.0f;
float playerShootingDelay = 0.0f;

bool isShowingFPS = false;
float hudOffset = 25.0f;

bool isShowingHitBoxes = false;

int main()
{
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Space Havoc");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set the FPS to the current monitor's refresh rate
    
    InitAudioDevice();
    initializeSounds();

    GameScreen currentScreen = TITLE;
    bool isQuittingGame = false;

    Player player;

    Camera camera;
    initializeCamera(&camera);

    while (!WindowShouldClose())
    {        
        // Update

        // Graphic toggles
        if(IsKeyPressed(KEY_F)) {
            isShowingFPS = !isShowingFPS;
        }

        if(IsKeyPressed(KEY_H)) {
            isShowingHitBoxes = !isShowingHitBoxes;
        }

        switch (currentScreen)
        {
            case TITLE:
            {
                DrawText("SPACE HAVOC", GetScreenWidth() / 2 - MeasureText("SPACE HAVOC", 40) / 2, GetScreenHeight() / 4, 40, LIGHTGRAY);

                float buttonX = GetScreenWidth() / 2.0 - 200.0 / 2.0;
                float playButtonY = GetScreenHeight() / 2.0 - 70.0 / 2.0;
                float quitButtonY = playButtonY + 70.0 + 20.0;

                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

                if (GuiButton((Rectangle){buttonX, playButtonY, 200, 70}, "PLAY"))
                {
                    DisableCursor();
                    initializeGame(&player);
                    currentScreen = GAME;
                }

                if (GuiButton((Rectangle){buttonX, quitButtonY, 200,70}, "QUIT"))
                {
                    isQuittingGame = true;
                    break;
                }
        } break;

            case GAME:
            {
                // Check for game over
                if (lives <= 0 || score < 0) {
                    enemies.clear();
                    bullets.clear();
                    saveScore();
                    EnableCursor();
                    PlaySound(sounds[GAME_OVER]);
                    currentScreen = EXIT;
                }

                handleInput(&player);
                handleCamera(&camera, &player);

                // Spawn the enemies after the delay
                if(enemySpawnDelay >= ENEMY_SPAWN_TIME) {
                    spawnEnemy();
                    enemySpawnDelay = 0.0f;
                }

                moveEnemies();
                moveBullets();

                // Update the delays
                enemySpawnDelay += 1.0f * GetFrameTime();
                playerShootingDelay += 1.0f * GetFrameTime();
            } break;

            case EXIT:
            {
            } break;
        }

        if (isQuittingGame)
        {
            break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        if(isShowingFPS) {
            DrawFPS(10, 10);
            hudOffset = 25.0f;
        } else {
            hudOffset = 0.0f;
        }

        switch (currentScreen)
        {
            case TITLE:
            {
            } break;

            case GAME:
            {
                DrawText(TextFormat("Lives: %d", lives), 10, 10 + hudOffset, 20, WHITE);
                DrawText(TextFormat("Score: %d", score), 10, 35 + hudOffset, 20, WHITE);

                BeginMode3D(camera);
                drawPlayer(&player);
                drawEnemies(&player);
                DrawGrid(10, 10.0f);
                drawBullets();

                EndMode3D();
            } break;

            case EXIT:
            {
                drawScore();

                float buttonY = GetScreenHeight() / 2.0 + 100.0 / 2.0;
                float continueButtonX = GetScreenWidth() / 2.0 - 200.0 / 2.0;
                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

                if (GuiButton((Rectangle){continueButtonX, buttonY, 200, 70}, "CONTINUE?"))
                {
                    currentScreen = TITLE;
                    score = 0;
                    lives = 2;
                }
            } break;
        }

        EndDrawing();
    }

    // De-Initialization
    player.destroy();
    enemies.clear();
    bullets.clear();
    destroySounds();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void initializeSounds() {
    sounds.push_back(LoadSound("sounds/boom_c_06-102838.mp3"));
    sounds.push_back(LoadSound("sounds/game-over-arcade-6435.mp3"));
    sounds.push_back(LoadSound("sounds/retro-falling-down-sfx-85575.mp3"));
    sounds.push_back(LoadSound("sounds/shoot02wav-14562.mp3"));
    sounds.push_back(LoadSound("sounds/thump-105302.mp3"));
}

void destroySounds() {
    for(int i = 0; i < sounds.size(); i++) {
        UnloadSound(sounds[i]);
    }
    sounds.clear();
}

void initializeGame(Player * player) {
    lives = 3;
    score = 0;
    enemySpawnDelay = 0.0f;
    playerShootingDelay = 0.0f;
    player->setPosition(player->getStartingPosition());
}

void initializeCamera(Camera * camera) {
    *camera = {0};
    camera->position = (Vector3){0.0f, 0.0f, 0.0f};
    camera->target = (Vector3){0.0f, 0.0f, 0.0f};
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    camera->fovy = 30.0f;
    camera->projection = CAMERA_PERSPECTIVE;
}

void handleInput(Player * player) {
    if (IsKeyDown(KEY_LEFT_SHIFT) && !player->getIsSprinting()) {
        player->setIsSprinting(true);
    } else {
        player->setIsSprinting(false);
    }

    if (IsKeyDown(KEY_LEFT))
    {
        player->moveLeft();
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        player->moveRight();
    }

    if (IsKeyPressed(KEY_SPACE) && playerShootingDelay >= PLAYER_SHOOTING_TIME)
    {
        bullets.push_back(Bullet(player->getPosition()));
        PlaySound(sounds[SHOOT]);
        playerShootingDelay = 0.0f;
    }
}

void handleCamera(Camera * camera, Player * player) {
    // Make the camera follow the player when they move
    camera->target = (Vector3){player->getPosition().x + 1.7f, 0.0f, 0.0f};
    camera->position = (Vector3){(player->getPosition().x + 1.7f), 4.0f, player->getPosition().z - 20.0f};
}

void drawPlayer(Player * player) {
    if(isShowingHitBoxes) {
        DrawBoundingBox(player->getBoundingBox(), LIME);
    }
    DrawModel(player->getModel(), player->getPosition(), 1.0f, WHITE);
}

void spawnEnemy() {
    // Spawn the enemies at [48, -48] on the x-coordinate
    int offset = -48;
    int randomNumber = (rand() % 97) + offset;
    enemies.push_back(Enemy((Vector3){randomNumber, 0.0f, 40.0f}));
}

void drawEnemies(Player * player) {
    for (int i = 0; i < enemies.size(); i++)
    {
        // Draw the current enemy
        if(isShowingHitBoxes) {
            DrawBoundingBox(enemies[i].getBoundingBox(), LIME);
        }

        DrawModelEx(enemies[i].getModel(), enemies[i].getPosition(), (Vector3){0.0f, 1.0f, 0.0f}, 180, (Vector3){enemies[i].getScale(), enemies[i].getScale(), enemies[i].getScale()}, WHITE);

        // Check if the player collides with an enemy
        if (CheckCollisionBoxes(player->getBoundingBox(), enemies[i].getBoundingBox()))
        {
            enemies[i].destroy();
            enemies.erase(enemies.begin() + i);
            PlaySound(sounds[THUMP]);

            lives--;
        }
    }
}

void moveEnemies() {
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i].move();

        // Check if the enemy goes behind the player
        if(enemies[i].getPosition().z <= -60.0f) {
            enemies.erase(enemies.begin() + i);
            PlaySound(sounds[FALLING_DOWN]);

            lives--;
            continue;
        }
    }
}

void moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].move();

        // Check if the bullet goes beyond the grid
        if(bullets[i].getPosition().z >= 600.0f) {
            bullets.erase(bullets.begin() + i);
            continue;
        }

        // Check for a enemy bullet collision
        for(int j = 0; j < enemies.size(); j++) {
            if (bullets[i].getPosition().x - 1.25f <= enemies[j].getPosition().x && // right-side
                bullets[i].getPosition().x + 0.85f >= enemies[j].getPosition().x && // left-side
                bullets[i].getPosition().z >= enemies[j].getPosition().z) {
                enemies[j].destroy();
                enemies.erase(enemies.begin() + j);
                bullets.erase(bullets.begin() + i);
                PlaySound(sounds[BOOM]);

                score += 5;
            }
        }
    }
}

void drawBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        if(isShowingHitBoxes) {
            DrawSphereWires(bullets[i].getPosition(), bullets[i].getRadius(), 15, 15, LIME);
        }
        bullets[i].draw();
    }
}

void saveScore(){
    highScore = readHighScoreFile();
    highScore = std::max(score, highScore);
    writeToHighScoreFile();
}

void drawScore() {
    DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 40) / 2, GetScreenHeight() / 4, 40, LIGHTGRAY);
    DrawText(TextFormat("SCORE: %d", score),  GetScreenWidth() / 2 - MeasureText("SCORE", 40) / 2, GetScreenHeight() / 3, 30, WHITE);
    DrawText(TextFormat("HIGH SCORE: %d", highScore),  GetScreenWidth() / 2 - MeasureText("HIGH SCORE", 40) / 2.25, GetScreenHeight() / 2.55, 30, WHITE);
}

int readHighScoreFile() {
    std::ifstream highScoreFile("src/highscore.txt");
    std::string text;

    if (highScoreFile.is_open()) {
        highScoreFile >> text; 
        highScoreFile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return -1;
    }

    return std::stoi(text);
}

void writeToHighScoreFile() {
    std::ofstream highScoreFile("src/highscore.txt");
    std::string text;

    if(highScoreFile.is_open()) {
        highScoreFile << highScore;
        highScoreFile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
}