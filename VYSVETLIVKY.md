Vysvětlivky funkcí/řádků
📄 Soubor: snake.cpp

	#include "raylib.h"  
	#include <deque>
	#include <algorithm>

raylib.h: Umožňuje nám kreslit okno, brát vstup z klávesnice a používat herní funkce.
<deque>: Speciální typ pole (fronta), ze které je postavené tělo hada. Umí bleskově přidávat na začátek a mazat z konce.
<algorithm>: Obsahuje funkci std::sort, kterou používáme k seřazení hráčů v žebříčku od nejvyššího skóre po nejnižší.

	void Restart(int minX, int maxX, int minY, int maxY) {
    	body.clear();
    	int stredX = minX + (maxX - minX) / 2;
    	int stredY = minY + (maxY - minY) / 2;
    	body.push_back({(float)stredX, (float)stredY});
    	direction = {1,0};
    	kTraveni = 0;
	}
Metoda pro znovuzrozeni hada - Vymaže staré tělo, matematicky spočítá střed aktuálně vybrané mapy a tam hada usadí. Resetuje směr doprava a vynuluje trávení.

Hlavní stavy hry

	enum StavHry { MENU, NASTAVENI, HRANI, KONEC_HRY, ZEBRICEK, VYHRA };

Seznam stavů hry (tzv. stavový automat) - Aby program věděl, v jaké sekci se zrovna nachází. Nemůžeme v menu nechat hada běhat na pozadí, proto pomocí této proměnné kód větvíme.

	float timeSinceLastMove = 0;
	float moveInterval = 0.2f;

Časovače pro pohyb hada.
Had se nemůže hýbat v každém framu (to by ujel 60 políček za sekundu a hráč by prohrál hned po startu). moveInterval = 0.2f znamená, že se had pohne jednou za pětinu sekundy. timeSinceLastMove neustále sčítá reálný čas, a jakmile dosáhne limitu, had skočí o políčko dál a počítadlo se vynuluje.

	for(int i = 0; i < herniMapa.prekazky.size(); i++) {
    	if(snake.body[0].x == herniMapa.prekazky[i].x && snake.body[0].y == herniMapa.prekazky[i].y) {
        	SetWindowSize(800, 600);
        	aktualniStav = KONEC_HRY;
    	}
	}

Náraz do překážek 

	if(snake.body.size() == herniMapa.policek) {
    	SetWindowSize(800, 600);
    	aktualniStav = VYHRA;
	}
podmínka na rozpoznání výhry - pokud sní had dostatek jablek, hra se dostane do okna výhra

📄 Soubor: mapa.cpp

        void Mapa::NastavMapu(TypMapy novyTyp)  {
            aktualniMapa = novyTyp;
            prekazky.clear();

zakladni funkce pro prepinani map - vymaze prekazky z mapy SBLOKY, aby bylo nachystane nove "ciste" pole

        if(aktualniMapa == SBLOKY)     {
                minX = 2; maxX = 21;
                minY = 2; maxY = 16;
                oknoSirka = 480; oknoVyska = 380;
urceni hraci plochy pro hada
Proměnné oknoSirka a oknoVyska se ukládají aby si hlavní program mohl zavolat SetWindowSize a upravit velikost herního okna na míru těmto hranicím.

        prekazky.push_back({6, 5}); prekazky.push_back({7, 5});
                // ... (další překážky) ...
                policek = (20 * 15) - prekazky.size();
            }
push_back vloží do našeho pole (vektoru) konkrétní souřadnice bloku. Na konci se vypočítá proměnná policek (celkový počet minus počet překážek). Tuto hodnotu pak hra kontroluje pro Ultimátní výhru – had ví, že nemusí zaplnit úplně celý čtverec, protože některá místa zabírá zeď.

        void Mapa::Draw() {
            DrawRectangleLinesEx({0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, 40, DARKGRAY);

            for (const auto& blok : prekazky) {
                DrawRectangle(blok.x * 20, blok.y * 20, 20, 20, BLACK);
            }
        }
GetScreenWidth() a Height() jsou dynamické funkce Raylibu. Kreslí vždy přesný okraj bez ohledu na to, jestli je okno obrovské, nebo smrsknuté pro tutoriál. Cyklus for pak jen projde vektor překážek a každou z nich namaluje jako černý čtverec.

📄 Soubor: jablko.cpp

        void Jablko::Generuj(std::deque<Vector2> snakeBody, int minX, int maxX, int minY, int maxY, std::vector<Vector2> prekazky) {
            bool volnaPozice = false;
            while(!volnaPozice) {
nekonecny cyklus hledani pozice na mape kde neni had

        pozice.x = GetRandomValue(minX, maxX);
        pozice.y = GetRandomValue(minY, maxY);
        volnaPozice = true;

GetRandomValue - vybere nahodne cislo podle souradnic map, 

        aktivni = true;
            zivotnost = 5.0f; 
        }
fungovani zlateho jablka, po 5 snedenych jablek se objevi zlate jablko a ve hre zustava 5 sekund

















            
