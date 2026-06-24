#include "raylib.h"
#include <deque>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "zebricek.h"
#include "jablko.h"
#include "mapa.h"

/*    VECI NA PRIDANI
            nastaveni - zmena velikosti okna na fullscreen, zmena barev?,
            jina mapa s bloky do kterych lze narazit uvnitr
            mapy:
                zakladni, tutorial na ukazku(4x5), s bloky

        predelat do vice hlavickovych souboru

    ultimatni vyhra
    specialni pamlsek co da vice bodu tela
*/

/*
ulozeni:
g++ snake.cpp zebricek.cpp jablko.cpp mapa.cpp -o hra.exe -lraylib -lgdi32 -lwinmm
spusteni:
.\hra.exe
*/

class Snake {
public:
    std::deque<Vector2> body;
    Vector2 direction = {1, 0};
    int size = 20;
    int kTraveni = 0;

    void Restart(int minX, int maxX, int minY, int maxY) {
        body.clear();
        int stredX = minX + (maxX - minX) / 2;
        int stredY = minY + (maxY - minY) / 2;
        body.push_back({(float)stredX, (float)stredY});

        direction = {1,0};
        kTraveni = 0;
    }

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
    Mapa herniMapa;

    int pocitadloJablek = 0;

    StavHry aktualniStav = MENU;

    std::string jmenoHrace = "";

    float timeSinceLastMove = 0;
    float moveInterval = 0.2f;

    herniNastaveni() : spravceZebricku("zebricek.txt") {
        snake.Restart(herniMapa.minX, herniMapa.maxX,
             herniMapa.minY, herniMapa.maxY);
        jablko.Generuj(snake.body, herniMapa.minX, herniMapa.maxX,
             herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);
    }
    

    void Update() {
        if(aktualniStav == MENU) {
            if(IsKeyPressed(KEY_SPACE)) {
                SetWindowSize(herniMapa.oknoSirka, herniMapa.oknoVyska);
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
            if(snake.body[0].x < herniMapa.minX || snake.body[0].x > herniMapa.maxX ||
                 snake.body[0].y < herniMapa.minY || snake.body[0].y > herniMapa.maxY) {
                SetWindowSize(800, 600);
                aktualniStav = KONEC_HRY;
                
                }

            for(int i = 0; i < herniMapa.prekazky.size(); i++) {
                if(snake.body[0].x == herniMapa.prekazky[i].x 
                    && snake.body[0].y == herniMapa.prekazky[i].y) {
                    SetWindowSize(800, 600);
                    aktualniStav = KONEC_HRY;
                }
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
                if(snake.body[0].x == jablko.pozice.x 
                    && snake.body[0].y == jablko.pozice.y) {
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
                    SetWindowSize(800, 600);
                   aktualniStav = KONEC_HRY;
                        }
                    }

                if(snake.body.size() ==herniMapa.policek) {
                    SetWindowSize(800, 600);
                    aktualniStav = VYHRA;
                }
                
                if(snedl == true) {
                    jablko.Generuj(snake.body, herniMapa.minX, herniMapa.maxX,
                        herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);
                    
                         moveInterval -= 0.002f;
                    if(moveInterval < 0.07f) moveInterval = 0.07f;
                        
                    pocitadloJablek++;
                    if(pocitadloJablek >= 5 && !zlateJablko.aktivni) {
                        zlateJablko.Generuj(snake.body, jablko.pozice, 
                            herniMapa.minX, herniMapa.maxX, 
                            herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);
                        
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

                snake.Restart(herniMapa.minX, herniMapa.maxX,
                     herniMapa.minY, herniMapa.maxY);
                jablko.Generuj(snake.body, herniMapa.minX, herniMapa.maxX,
                herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);
                
                if(herniMapa.aktualniMapa == TUTORIAL) {
                    moveInterval = 0.4f;
                }   else {
                    moveInterval = 0.2f;
                }

                pocitadloJablek = 0;
                zlateJablko.aktivni = false;

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

                    snake.Restart(herniMapa.minX, herniMapa.maxX,
                         herniMapa.minY, herniMapa.maxY);
                    jablko.Generuj(snake.body, herniMapa.minX, herniMapa.maxX,
                    herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);

                    if(herniMapa.aktualniMapa == TUTORIAL) {
                    moveInterval = 0.4f;
                    }   else {
                    moveInterval = 0.2f;
                    }

                    pocitadloJablek = 0;
                    zlateJablko.aktivni = false;

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
            if(IsKeyPressed(KEY_F)) {
                ToggleFullscreen();
            }
            if(IsKeyPressed(KEY_B)) {
                ToggleBorderlessWindowed();
            }
            if(IsKeyPressed(KEY_BACKSPACE)) {
                aktualniStav = MENU;
            }
            bool zmenaMapy = false;
            if(IsKeyPressed(KEY_ONE)){
                herniMapa.NastavMapu(ZAKLADNI);
                zmenaMapy = true;
            }
            if(IsKeyPressed(KEY_TWO)){
                herniMapa.NastavMapu(KLASIKA);
                zmenaMapy = true;
            }
            if(IsKeyPressed(KEY_THREE)){
                herniMapa.NastavMapu(SBLOKY);
                zmenaMapy = true;
            }
            if(IsKeyPressed(KEY_FOUR)){
                herniMapa.NastavMapu(TUTORIAL);
                zmenaMapy = true;
            }

            if(zmenaMapy == true) {
                snake.Restart(herniMapa.minX, herniMapa.maxX,
                    herniMapa.minY, herniMapa.maxY);
                jablko.Generuj(snake.body, herniMapa.minX, herniMapa.maxX,
                    herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);

                if(herniMapa.aktualniMapa == TUTORIAL) {
                    moveInterval = 0.4f;
                    }   else {
                    moveInterval = 0.2f;
                    }
                pocitadloJablek = 0;
                zlateJablko.aktivni = false;
            }
        }
    }

    void Draw() {
        ClearBackground(RAYWHITE);
        if(aktualniStav != HRANI) {
            DrawRectangleLinesEx({0, 0, 800, 600}, 40, DARKGRAY);
        }

        if(aktualniStav == MENU) {
            DrawText("SNAKE GAME", 265, 200, 40, DARKPURPLE);
            DrawText("Zmackni MEZERNIK pro start", 250, 300, 20, BLACK);
            DrawText("Zmackni TAB pro nastaveni", 255, 340, 20, GRAY);
            DrawText("Zmackni CTRL pro zebricek", 255, 380, 20, GRAY);
            DrawText("Zmackni ESC pro ukonceni", 255, 420, 20, RED);
        }
        else if(aktualniStav == HRANI) {
            herniMapa.Draw();

            int skore = snake.body.size() -1;
            DrawText(TextFormat("Skore: %i", skore), 10, 10, 20, PINK);

            snake.Draw();
            jablko.Draw();
            zlateJablko.Draw();
        }
        else if(aktualniStav == NASTAVENI) {
            DrawText("Nastaveni", 280, 100, 40, DARKPURPLE);
            DrawText("Vyber mapu stiskem 1-4", 230, 180, 20, DARKGRAY);

            Color barva1 = (herniMapa.aktualniMapa == ZAKLADNI) ? RED : BLACK;
            Color barva2 = (herniMapa.aktualniMapa == KLASIKA) ? RED : BLACK;
            Color barva3 = (herniMapa.aktualniMapa == SBLOKY) ? RED : BLACK;
            Color barva4 = (herniMapa.aktualniMapa == TUTORIAL) ? RED : BLACK;

            DrawText("1 - Zakladni (35x25)", 250, 220, 20, barva1);
            DrawText("2 - Klasika (20x20)", 250, 260, 20, barva2);
            DrawText("3 - Sbloky (20x15)", 250, 300, 20, barva3);
            DrawText("4 - Tutorial (4x5)", 250, 340, 20, barva4);

            DrawText("Zmackni F pro FULLSCREEN", 200, 430, 20, DARKBLUE);
            DrawText("Zmackni B pro okno", 200, 470, 20, DARKBLUE);

            DrawText("Zmackni BACKSPACE pro menu", 55, 515, 20, BLACK);

        }
        else if(aktualniStav == ZEBRICEK) {
            DrawText("TOP 10 HRACU:", 260, 80, 40, DARKGREEN);
            std::vector<ZaznamZebricku> dataZebricku = spravceZebricku.ZiskejTop10();

            for(int i = 0; i < dataZebricku.size(); i++) {
                const char* textZaznamu = TextFormat("%d. %s - %i", i + 1, 
                    dataZebricku[i].jmeno.c_str(), dataZebricku[i].skore);
                DrawText( textZaznamu, 200, 180 +(i * 30), 20, BLACK);
            }
            DrawText("Zmackni BACKSPACE pro menu", 55, 515, 20, BLACK);
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