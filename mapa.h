#pragma once
#include "raylib.h"
#include <vector>
//hlavickovy soubor nastaveni map

enum TypMapy {
    ZAKLADNI,
    TUTORIAL,
    KLASIKA,
    SBLOKY,
};

class Mapa {
public:
    int oknoSirka;
    int oknoVyska;

    std::vector<Vector2> prekazky;

    TypMapy aktualniMapa = ZAKLADNI;
    int minX, maxX;
    int minY, maxY;

    int policek;

    Mapa();

    void NastavMapu(TypMapy novyTyp);

    void Draw();
};