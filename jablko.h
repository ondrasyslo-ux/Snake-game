#pragma once
#include "raylib.h"
#include <deque>

class Jablko {
public:
    Vector2 pozice;
    int size = 20;

    void Generuj(std::deque<Vector2> snakeBody);
    void Draw();
};

class ZlateJablko {
public:
    Vector2 pozice;
    int size = 20;
    bool aktivni = false;
    float zivotnost = 0.0f;

    void Generuj(std::deque<Vector2> snakeBody, Vector2 poziceCerveneho);
    void Draw();
};
