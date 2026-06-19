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
        SetWindowSize(800, 600);
    }

    else if(aktualniMapa == TUTORIAL)   {
        //mapa 4x5
        minX = 2; maxX = 5;
        minY = 2; maxY = 6;
        policek = 4 * 5;
        SetWindowSize(160, 180);
    }

    else if(aktualniMapa == KLASIKA)    {
        //mapa 20x20
        minX = 2; maxX = 21;
        minY = 2; maxY = 21;
        policek = 20 * 20;
        SetWindowSize(480, 480);
    }

    else if(aktualniMapa == SBLOKY)     {
        //mapa 20x15 s bloky uvnitr
        minX = 2; maxX = 21;
        minY = 2; maxY = 16;
        policek = 20 * 15;
        SetWindowSize(480, 380);
        
        //levy horni
        prekazky.push_back({6, 5}); prekazky.push_back({7, 5});
        prekazky.push_back({6, 6}); prekazky.push_back({7, 6});
        //pravy horni
        prekazky.push_back({16, 5}); prekazky.push_back({17, 5});
        prekazky.push_back({16, 6}); prekazky.push_back({17, 6});
        //levy dolni
        prekazky.push_back({6, 12}); prekazky.push_back({7, 12});
        prekazky.push_back({6, 13}); prekazky.push_back({7, 13});
        //pravy dolni
        prekazky.push_back({16, 12}); prekazky.push_back({17, 12});
        prekazky.push_back({16, 13}); prekazky.push_back({17, 13});

        policek = (20 * 15) - prekazky.size();
    }
}

void Mapa::Draw() {
    ClearBackground(RAYWHITE);

    DrawRectangleLinesEx({0, 0, (float)GetScreenWidth(),
         (float)GetScreenHeight()}, 40, DARKGRAY);

    for(const auto& blok : prekazky) {
        DrawRectangle(blok.x * 20, blok.y *20, 20, 20, BLACK);
    }
}