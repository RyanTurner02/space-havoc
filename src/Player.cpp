#include "Player.h"
#include "raylib.h"
#include "raymath.h"

#define SPEED 5

Player::Player()
{
    model = LoadModel("objects/player/InterstellarRunner.obj");
    texture = LoadTexture("objects/player/InterstellarRunner.png");
    SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
    position = {-1.7f, 0.0f, 0.0f};
    boundingBox = GetMeshBoundingBox(model.meshes[0]);
    boundingBox.min = Vector3Add(boundingBox.min, position);
    boundingBox.max = Vector3Add(boundingBox.max, position);
}

Model Player::getModel()
{
    return model;
}

Texture2D Player::getTexture()
{
    return texture;
}

BoundingBox Player::getBoundingBox()
{
    return boundingBox;
}

Vector3 Player::getPosition()
{
    return position;
}

void Player::moveLeft()
{
    float deltaX = SPEED * GetFrameTime();

    position.x += deltaX;
    boundingBox.min = Vector3Add(boundingBox.min, (Vector3){deltaX, 0.0f, 0.0f});
    boundingBox.max = Vector3Add(boundingBox.max, (Vector3){deltaX, 0.0f, 0.0f});
}

void Player::moveRight()
{
    float deltaX = SPEED * GetFrameTime();

    position.x -= deltaX;
    boundingBox.min = Vector3Add(boundingBox.min, (Vector3){-deltaX, 0.0f, 0.0f});
    boundingBox.max = Vector3Add(boundingBox.max, (Vector3){-deltaX, 0.0f, 0.0f});
}

void Player::shoot()
{
    // TODO
}

void Player::destroy()
{
    UnloadTexture(texture);
    UnloadModel(model);
}