#define RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "raygui.h"
#include "Player.h"
#include "Enemy.h"

typedef enum GameScreen
{
    TITLE = 0,
    GAME
} GameScreen;

void handleInput(Player * player);
void handleCamera(Camera * camera, Player * player);

int main()
{
    const int width = 1280;
    const int height = 720;

    InitWindow(width, height, "Space Havoc");
    SetTargetFPS(60);

    GameScreen currentScreen = TITLE;
    bool isQuittingGame = false;

    Player player;
    Enemy enemy;

    Camera camera = {0};
    camera.position = (Vector3){0.0f, 4.0f, -20.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {        
        // Update
        switch (currentScreen)
        {
            case TITLE:
            {
                DrawText("SPACE HAVOC", 500, 150, 40, LIGHTGRAY);

                if (GuiButton((Rectangle){300, 450, 100, 50}, "PLAY"))
                {
                    DisableCursor();
                    currentScreen = GAME;
                }

                if (GuiButton((Rectangle){870, 450, 100, 50}, "QUIT"))
                {
                    isQuittingGame = true;
                    break;
                }
            } break;

            case GAME:
            {
                // Update
                handleInput(&player);
                handleCamera(&camera, &player);

                enemy.move();
            } break;
        }

        if (isQuittingGame)
        {
            break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);

        switch (currentScreen)
        {
            case TITLE:
            {
            } break;

            case GAME:
            {
                BeginMode3D(camera);
                DrawModel(player.getModel(), player.getPosition(), 1.0f, WHITE);
                DrawBoundingBox(player.getBoundingBox(), LIME);

                DrawModel(enemy.getModel(), enemy.getPosition(), enemy.getScale(), WHITE);
                DrawBoundingBox(enemy.getBoundingBox(), LIME);

                DrawGrid(10, 10.0f);
                EndMode3D();
            } break;
        }

        EndDrawing();
    }

    player.destroy();
    enemy.destroy();
    CloseWindow();
    return 0;
}

void handleInput(Player * player) {
    if (IsKeyDown(KEY_LEFT))
    {
        player->moveLeft();
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        player->moveRight();
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        player->shoot();
    }
}

void handleCamera(Camera * camera, Player * player) {
    // UpdateCamera(&camera, CAMERA_THIRD_PERSON);

    // Make the camera follow the player when they move
    camera->target = (Vector3){player->getPosition().x + 1.7f, 0.0f, 0.0f};
    camera->position = (Vector3){(player->getPosition().x + 1.7f), camera->position.y, camera->position.z};
}