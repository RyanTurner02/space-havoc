#include "Player.h"
#include "raylib.h"
#include "raymath.h"

#define STARTING_POSITION {-1.7f, 0.0f, -50.0f}
#define SPEED 15
#define SPRINTING_SPEED 40

Player::Player()
{
    model = LoadModel("objects/player/InterstellarRunner.obj");
    texture = LoadTexture("objects/player/InterstellarRunner.png");
    SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
    position = STARTING_POSITION;
    boundingBox = GetMeshBoundingBox(model.meshes[0]);
    boundingBox.min = Vector3Add(boundingBox.min, position);
    boundingBox.max = Vector3Add(boundingBox.max, position);
    isSprinting = false;
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

Vector3 Player::getStartingPosition()
{
    return STARTING_POSITION;
}

void Player::setPosition(Vector3 position)
{
    this->position = position;
    boundingBox = GetMeshBoundingBox(model.meshes[0]);
    boundingBox.min = Vector3Add(boundingBox.min, position);
    boundingBox.max = Vector3Add(boundingBox.max, position);
}

bool Player::getIsSprinting()
{
    return isSprinting;
}

void Player::setIsSprinting(bool isSprinting) {
    this->isSprinting = isSprinting;
}

void Player::moveLeft()
{
    float deltaX = SPEED * GetFrameTime();

    if(isSprinting) {
        deltaX = SPRINTING_SPEED * GetFrameTime();
    }

    position.x += deltaX;
    boundingBox.min = Vector3Add(boundingBox.min, (Vector3){deltaX, 0.0f, 0.0f});
    boundingBox.max = Vector3Add(boundingBox.max, (Vector3){deltaX, 0.0f, 0.0f});
}

void Player::moveRight()
{
    float deltaX = SPEED * GetFrameTime();

    if(isSprinting) {
        deltaX = SPRINTING_SPEED * GetFrameTime();
    }

    position.x -= deltaX;
    boundingBox.min = Vector3Add(boundingBox.min, (Vector3){-deltaX, 0.0f, 0.0f});
    boundingBox.max = Vector3Add(boundingBox.max, (Vector3){-deltaX, 0.0f, 0.0f});
}

void Player::destroy()
{
    UnloadTexture(texture);
    UnloadModel(model);
}