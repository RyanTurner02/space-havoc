#include "Enemy.h"
#include "raylib.h"
#include "raymath.h"

Enemy::Enemy()
{
    model = LoadModel("objects/enemy/Alien1/Alien1.obj");
    texture = LoadTexture("objects/enemy/Alien1/Alien1.png");
    SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
    position = {0.0f, 0.0f, 10.0f};
    boundingBox = GetMeshBoundingBox(model.meshes[0]);
    scale = 0.25f;

    // scale the bounding box
    boundingBox.min = Vector3Scale(boundingBox.min, scale);
    boundingBox.max = Vector3Scale(boundingBox.max, scale);

    // move the bounding box
    boundingBox.min = Vector3Add(boundingBox.min, position);
    boundingBox.max = Vector3Add(boundingBox.max, position);
}

Model Enemy::getModel()
{
    return model;
}

Texture2D Enemy::getTexture()
{
    return texture;
}

BoundingBox Enemy::getBoundingBox()
{
    return boundingBox;
}

Vector3 Enemy::getPosition()
{
    return position;
}

float Enemy::getScale()
{
    return scale;
}

void Enemy::move()
{
}

void Enemy::destroy()
{
    UnloadTexture(texture);
    UnloadModel(model);
}