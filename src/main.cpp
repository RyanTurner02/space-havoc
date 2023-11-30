#include "raylib.h"
#include "Player.h"

int main()
{
    const int width = 1280;
    const int height = 720;

    InitWindow(width, height, "Space Havoc");
    SetTargetFPS(60);
    DisableCursor();

    Player player;

    Camera camera = {0};
    camera.position = (Vector3){0.0f, 4.0f, -20.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);

        BeginMode3D(camera);
        DrawModel(player.getModel(), player.getPosition(), 1.0f, WHITE);
        DrawBoundingBox(player.getBoundingBox(), LIME);
        DrawGrid(10, 10.0f);
        EndMode3D();

        EndDrawing();
    }

    player.destroy();
    CloseWindow();
    return 0;
}