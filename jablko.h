#pragma once
#include "raylib.h"
#include <deque>
#include <vector>
//hlavickovy soubor s fungovanim jablka

class Jablko {
public:
    Vector2 pozice;
    int size = 20;

    void Generuj(std::deque<Vector2> snakeBody, 
        int minX, int maxX, int minY, int maxY, std::vector<Vector2> prekazky);
    void Draw();
};

class ZlateJablko {
public:
    Vector2 pozice;
    int size = 20;
    bool aktivni = false;
    float zivotnost = 0.0f;

    void Generuj(std::deque<Vector2> snakeBody, Vector2 poziceCerveneho,
         int minX, int maxX, int minY, int maxY, std::vector<Vector2> prekazky);
    void Draw();
};
