#include "jablko.h"
#include <vector>

void Jablko::Generuj(std::deque<Vector2> snakeBody,
    int minX, int maxX, int minY, int maxY, std::vector<Vector2> prekazky) {
   
    bool volnaPozice = false;
    while(!volnaPozice) {
        pozice.x = GetRandomValue(minX, maxX);
        pozice.y = GetRandomValue(minY, maxY);

        volnaPozice = true;

        for(int i = 0; i < snakeBody.size(); i++) {
            if((pozice.x == snakeBody[i].x) && (pozice.y == snakeBody[i].y)){
                volnaPozice = false;
                break;
            }
        }

        for(int i = 0; i < prekazky.size(); i++) {
            if (pozice.x == prekazky[i].x && pozice.y == prekazky[i].y) {
                volnaPozice = false;
            }
        }
    }
}

void Jablko::Draw() {
    DrawRectangle(pozice.x * size, pozice.y * size, size, size, RED);
}

void ZlateJablko::Generuj(std::deque<Vector2> snakeBody, Vector2 poziceCerveneho,
    int minX, int maxX, int minY, int maxY, std::vector<Vector2> prekazky) {
    
    bool volnaPozice = false;
    while (!volnaPozice)    {
        pozice.x = GetRandomValue(minX, maxX);
        pozice.y = GetRandomValue(minY, maxY);

        volnaPozice = true;

        for(int i = 0; i < snakeBody.size(); i++) {
            if((pozice.x == snakeBody[i].x) && (pozice.y == snakeBody[i].y)) {
                volnaPozice = false;
                break;
            }
        }
        
        if(pozice.x == poziceCerveneho.x && pozice.y == poziceCerveneho.y) {
            volnaPozice = false;
        }

        for(int i = 0; i < prekazky.size(); i++) {
            if (pozice.x == prekazky[i].x && pozice.y == prekazky[i].y) {
                volnaPozice = false;
            }
        }
    }

    aktivni = true;
    zivotnost = 5.0f;
    
}

void ZlateJablko::Draw() {
    if(aktivni) {
            DrawRectangle(pozice.x * size, pozice.y * size, size, size, GOLD);
    }   
}