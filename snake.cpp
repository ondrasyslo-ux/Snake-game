#include "raylib.h"
#include <deque>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "zebricek.h"
#include "jablko.h"

/*    VECI NA PRIDANI
            nastaveni - zmena velikosti okna, zmena barev?, ??jina velikost mapy, 
            jina mapa s bloky do kterych lze narazit uvnitr
            mapy:
                zakladni, tutorial na ukazku(4x5), s bloky

        predelat do vice hlavickovych souboru

    ultimatni vyhra
    specialni pamlsek co da vice bodu tela
*/

/*
ulozeni:
g++ snake.cpp zebricek.cpp jablko.cpp -o hra.exe -lraylib -lgdi32 -lwinmm
spusteni:
.\hra.exe
*/

class Snake {
public:
    std::deque<Vector2> body = { {10, 10} };
    Vector2 direction = {1, 0};
    int size = 20;

    int kTraveni = 0;

    void Update(bool snedlJablko = false) {
        Vector2 newHead = { body[0].x + direction.x, body[0].y + direction.y };
        body.push_front(newHead);

        if(snedlJablko) {
        }
        else if(kTraveni > 0) {
            kTraveni--;
        } else {
            body.pop_back();
        }
    }

    void Draw() {
        for (const auto& segment : body) {
            DrawRectangle(segment.x * size, segment.y * size, size, size, DARKGREEN);
        }
    }
        
};

enum StavHry {
    MENU,
    NASTAVENI,
    HRANI,
    KONEC_HRY,
    ZEBRICEK,
    VYHRA,
};

class herniNastaveni {
public:
    Snake snake;
    Jablko jablko;
    ZlateJablko zlateJablko;
    Zebricek spravceZebricku;

    int pocitadloJablek = 0;

    StavHry aktualniStav = MENU;

    std::string jmenoHrace = "";

    float timeSinceLastMove = 0;
    float moveInterval = 0.2f;

    herniNastaveni() : spravceZebricku("zebricek.txt") {
        jablko.Generuj(snake.body);
    }
    

    void Update() {
        if(aktualniStav == MENU) {
            if(IsKeyPressed(KEY_SPACE)) {
                aktualniStav = HRANI;
            }
            if(IsKeyPressed(KEY_TAB)) {
                aktualniStav = NASTAVENI;
            }
            if(IsKeyPressed(KEY_LEFT_CONTROL)) {
                aktualniStav = ZEBRICEK;
            }
        }
        else if(aktualniStav == HRANI) {
            if (IsKeyPressed(KEY_UP) && snake.direction.y != 1) snake.direction = {0, -1};
            if (IsKeyPressed(KEY_DOWN) && snake.direction.y != -1) snake.direction = {0, 1};
            if (IsKeyPressed(KEY_LEFT) && snake.direction.x != 1) snake.direction = {-1, 0};
            if (IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1) snake.direction = {1, 0};

            timeSinceLastMove += GetFrameTime();

            //naraz do zdi
            if(snake.body[0].x < 2 || snake.body[0].x > 37 ||
                 snake.body[0].y < 2 || snake.body[0].y > 27) {
                aktualniStav = KONEC_HRY;
                }
            
            if(zlateJablko.aktivni) {
                zlateJablko.zivotnost -= GetFrameTime();
                if(zlateJablko.zivotnost <= 0) {
                    zlateJablko.aktivni = false;
                }
            }

            if (timeSinceLastMove >= moveInterval) {
                bool snedl = false;

                //naraz do jablka
                if(snake.body[0].x == jablko.pozice.x && snake.body[0].y == jablko.pozice.y) {
                    snedl = true;
                }

                if(zlateJablko.aktivni && snake.body[0].x == zlateJablko.pozice.x 
                    && snake.body[0].y == zlateJablko.pozice.y) {
                    zlateJablko.aktivni = false;
                    snake.kTraveni += 4;
                    pocitadloJablek = 0;
                }

                snake.Update(snedl);

                //naraz do tela
                for(int i = 1; i < snake.body.size(); i++) {
                    if(snake.body[0].x == snake.body[i].x 
                        && snake.body[0].y == snake.body[i].y) {
                   aktualniStav = KONEC_HRY;
                        }
                    }

                if(snake.body.size() ==936) {
                    aktualniStav = VYHRA;
                }
                
                if(snedl == true) {
                    jablko.Generuj(snake.body);
                    moveInterval -= 0.005f;
                    if(moveInterval < 0.05f) moveInterval = 0.05f;
                        
                    pocitadloJablek++;
                    if(pocitadloJablek >= 5 && !zlateJablko.aktivni) {
                        zlateJablko.Generuj(snake.body, jablko.pozice);
                        pocitadloJablek = 0;
                    }
                }

                timeSinceLastMove = 0;
                }
        }
        else if(aktualniStav == KONEC_HRY) {
            int zklavesnice = GetCharPressed();

            while (zklavesnice > 0) {
                if((zklavesnice >= 33) && (zklavesnice <= 125) 
                && (jmenoHrace.length() < 10)) {
                    jmenoHrace += (char)zklavesnice;
                }
                zklavesnice = GetCharPressed();
            }
            
            if(IsKeyPressed(KEY_BACKSPACE) && jmenoHrace.length() > 0) {
                jmenoHrace.pop_back();
            } 
            
            if(IsKeyPressed(KEY_ENTER)) {
                int skore = snake.body.size() -1;
                spravceZebricku.ulozNovyVysledek(jmenoHrace, skore);

                snake.body.clear();
                snake.body.push_back({10, 10});
                snake.direction = {1, 0};
                moveInterval = 0.2f;

                jmenoHrace ="";
                aktualniStav = MENU;
            }
        
        }

        else if(aktualniStav == VYHRA) {
            int zklavesnice = GetCharPressed();

            while(zklavesnice > 0) {
                if((zklavesnice >= 33) && (zklavesnice <= 125) 
                && (jmenoHrace.length() < 10)) {
                    jmenoHrace += (char)zklavesnice;
                }
            }
                zklavesnice = GetCharPressed();

                if(IsKeyPressed(KEY_BACKSPACE) && jmenoHrace.length() > 0) {
                    jmenoHrace.pop_back();
                }
                if(IsKeyPressed(KEY_ENTER)) {
                    int skore = snake.body.size() -1;
                    spravceZebricku.ulozNovyVysledek(jmenoHrace, skore);

                    snake.body.clear();
                    snake.body.push_back({10, 10});
                    snake.direction = {1, 0};
                    moveInterval = 0.2f;

                    jmenoHrace = "";
                    aktualniStav = MENU;
                }
        }

        else if(aktualniStav == ZEBRICEK) {
            if(IsKeyPressed(KEY_BACKSPACE)) {
                aktualniStav = MENU;
            }
        }

        else if(aktualniStav == NASTAVENI) {
            if(IsKeyPressed(KEY_BACKSPACE)) {
                aktualniStav = MENU;
            }
        }


    }

    void Draw() {
        ClearBackground(RAYWHITE);
        DrawRectangleLinesEx({0, 0, 800, 600}, 40, DARKGRAY);

        if(aktualniStav == MENU) {
            DrawText("SNAKE GAME", 265, 200, 40, DARKPURPLE);
            DrawText("Zmackni MEZERNIK pro start", 250, 300, 20, BLACK);
            DrawText("Zmackni TAB pro nastaveni", 255, 340, 20, LIGHTGRAY);
            DrawText("Zmackni CTRL pro zebricek", 255, 380, 20, LIGHTGRAY);
            DrawText("Zmackni ESC pro ukonceni", 255, 420, 20, RED);
        }
        else if(aktualniStav == HRANI) {
            int skore = snake.body.size() -1;
            DrawText(TextFormat("Skore: %i", skore), 10, 10, 20, PINK);

            snake.Draw();
            jablko.Draw();
            zlateJablko.Draw();
        }
        else if(aktualniStav == NASTAVENI) {
            DrawText("Nastaveni", 265, 200, 40, DARKPURPLE);
            DrawText("Zmackni BACKSPACE pro menu", 250, 300, 20, BLACK);

        }
        else if(aktualniStav == ZEBRICEK) {
            DrawText("TOP 10 HRACU:", 260, 80, 40, DARKGREEN);
            std::vector<ZaznamZebricku> dataZebricku = spravceZebricku.ZiskejTop10();

            for(int i = 0; i < dataZebricku.size(); i++) {
                const char* textZaznamu = TextFormat("%d. %s - %i", i + 1, 
                    dataZebricku[i].jmeno.c_str(), dataZebricku[i].skore);
                DrawText( textZaznamu, 200, 180 +(i * 30), 20, BLACK);
            }
            DrawText("Zmackni BACKSPACE pro menu", 50, 530, 20, BLACK);
        }
        else if(aktualniStav == KONEC_HRY) {
            DrawText("KONEC HRY!", 265, 150, 40, RED);

            int skore = snake.body.size() -1;
            DrawText(TextFormat("Tvoje skore: %i", skore), 315, 220, 20, DARKBROWN);

            DrawText("Zadejte sve jmeno:", 305, 280, 20, GREEN);
            DrawText(jmenoHrace.c_str(), 300, 310, 30, DARKPURPLE);

            DrawRectangle(300, 345, 200, 2, BLACK);

            DrawText("Stiskni ENTER pro navrat do Menu", 225, 420, 20, DARKGRAY);
        }
        else if(aktualniStav == VYHRA) {
            DrawText("VYHRAL JSI!", 265, 150, 40, GOLD);

            int skore = snake.body.size() -1;
            DrawText(TextFormat("Vyhral jsi, skore: %i", skore), 250, 220, 40, DARKBROWN);

            DrawText("Zadejte sve jmeno:", 305, 280, 20, GREEN);
            DrawText(jmenoHrace.c_str(), 300, 310, 30, DARKPURPLE);

            DrawRectangle(300, 345, 200, 2, BLACK);

            DrawText("Stiskni ENTER pro navrat do Menu", 225, 420, 20, DARKGRAY);
        }
    }

};

int main()  {
    InitWindow(800, 600, "Snake");
    SetTargetFPS(60);

    herniNastaveni hra;

    while (!WindowShouldClose()){
        hra.Update();

        BeginDrawing();
        hra.Draw(); 
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}