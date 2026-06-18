#include "jablko.h"

void Jablko::Generuj(std::deque<Vector2> snakeBody) {
    bool volnaPozice = false;
    while(!volnaPozice) {
        pozice.x = GetRandomValue(2, 37);
        pozice.y = GetRandomValue(2, 27);

        volnaPozice = true;

        for(int i = 0; i < snakeBody.size(); i++) {
            if((pozice.x == snakeBody[i].x) && (pozice.y == snakeBody[i].y)){
                volnaPozice = false;
                break;
            }
        }
    }
}

void Jablko::Draw() {
    DrawRectangle(pozice.x * size, pozice.y * size, size, size, RED);
}

void ZlateJablko::Generuj(std::deque<Vector2> snakeBody, Vector2 poziceCerveneho) {
    bool volnaPozice = false;
    while (!volnaPozice)    {
        pozice.x = GetRandomValue(2, 37);
        pozice.y = GetRandomValue(2, 27);

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
    }

    aktivni = true;
    zivotnost = 5.0f;
    
}

void ZlateJablko::Draw() {
    if(aktivni) {
            DrawRectangle(pozice.x * size, pozice.y * size, size, size, GOLD);
    }   
}