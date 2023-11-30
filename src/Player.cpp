#include "Player.h"
#include "raylib.h"
#include "raymath.h"

Player::Player()
{
    model = LoadModel("objects/player/InterstellarRunner.obj");
    texture = LoadTexture("objects/player/InterstellarRunner.png");
    SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
    position = {-1.7f, 0.0f, 0.0f};
    boundingBox = GetMeshBoundingBox(model.meshes[0]);
    updateBoundingBox();
}

Model Player::getModel()
{
    return model;
}

Texture2D Player::getTexture()
{
    return texture;
}

void Player::updateBoundingBox()
{
    boundingBox.min = Vector3Add(boundingBox.min, position);
    boundingBox.max = Vector3Add(boundingBox.max, position);
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
    // TODO
}

void Player::moveRight()
{
    // TODO
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