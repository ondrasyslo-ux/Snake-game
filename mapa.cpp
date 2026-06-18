#include "mapa.h"

Mapa::Mapa() {
    NastavMapu(ZAKLADNI);
}

void Mapa::NastavMapu(TypMapy novyTyp)  {
    aktualniMapa = novyTyp;
    prekazky.clear();

    if(aktualniMapa == ZAKLADNI)        {//
        //mapa 35x25
        minX = 2; maxX = 36;
        minY = 2; maxY = 26;
        policek = 35 * 25;
    }

    else if(aktualniMapa == TUTORIAL)   {
        //mapa 4x5
        minX = 18; maxX = 21;
        minY = 12; maxY = 16;
        policek = 4 * 5;
    }

    else if(aktualniMapa == KLASIKA)    {
        //mapa 20x20
        minX = 10; maxX = 29;
        minY = 5; maxY = 24;
        policek = 20 * 20;
    }

    else if(aktualniMapa == SBLOKY)     {
        //mapa 20x15 s bloky uvnitr
        minX = 10; maxX = 29;
        minY = 7; maxY = 21;
        policek = 20 * 15;

        prekazky.push_back({14, 10}); prekazky.push_back({15, 10});//levy horni
        prekazky.push_back({14, 11}); prekazky.push_back({15, 11});

        prekazky.push_back({24, 10}); prekazky.push_back({25, 10});//pravy horni
        prekazky.push_back({24, 11}); prekazky.push_back({25, 11});

        prekazky.push_back({14, 17}); prekazky.push_back({15, 17});//levy dolni
        prekazky.push_back({14, 18}); prekazky.push_back({15, 18});

        prekazky.push_back({24, 17}); prekazky.push_back({25, 17});//pravy dolni
        prekazky.push_back({24, 18}); prekazky.push_back({25, 18});

        policek = (20 * 15) - prekazky.size();
    }
}

void Mapa::Draw() {
    ClearBackground(DARKGRAY);

    int sirka = ((maxX - minX + 1) * 20);
    int vyska = ((maxY - minY + 1) * 20);

    DrawRectangle(minX * 20, minY * 20, sirka, vyska, RAYWHITE);

    for(const auto& blok : prekazky) {
        DrawRectangle(blok.x * 20, blok.y *20, 20, 20, BLACK);
    }
}