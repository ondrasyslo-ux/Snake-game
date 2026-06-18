#pragma once
#include "raylib.h"
#include <vector>

enum TypMapy {
    ZAKLADNI,
    TUTORIAL,
    KLASIKA,
    SBLOKY,
};

class Mapa {
public:
    std::vector<Vector2> prekazky;

    TypMapy aktualniMapa = ZAKLADNI;
    int minX, maxX;
    int minY, maxY;

    int policek;

    Mapa();

    void NastavMapu(TypMapy novyTyp);

    void Draw();
};