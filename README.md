                                                      Můj vylepšený Snake (v C++ a Raylib)        

Tohle je moje verze klasického starého Hada. Místo, aby hra nebyla jenom čistá kopie nokiácké klasiky, rozhodl jsem se vymyslet několik dodatků, které celou hru vylepší. Přidal jsem herní menu, ukládání nejlepších skóre, možnost měnit mapy (včetně překážek) a pár speciálních herních pravidel.

Celé je to napsané v C++ s pomocí knihovny Raylib. Vzal jsem v potaz potřebu zjednodušit přehlednost celého kódu, takže je celý program rozdělen na několik rozdělených souborů

📂 Co najdete v jednotlivých souborech
Abych se v tom sám neztratil, rozdělil jsem hru do několika souborů, kde každý dělá jen tu svou práci:

snake.cpp – Hlavní motor hry. Tady se hra spouští, řeší se tu přepínání mezi menu a hrou, samotný pohyb hada a vykresluje se tady většina textů na obrazovce.

mapa.h / mapa.cpp – Stavař map. Tyhle soubory si pamatují, jak je vybraná mapa velká, kolik má políček (abychom věděli, kdy hráč vyhrál), kde jsou stěny a kde leží pevné překážky.

jablko.h / jablko.cpp – Sklad s ovocem. Algoritmy, které se starají o to, aby se červená i zlatá jablka objevila na obrazovce vždy bezpečně – tedy aby nespadla hadovi přímo do těla nebo dovnitř černé zdi.

zebricek.h / zebricek.cpp – Stará se o to, aby se po dohrání skóre uložilo do textového souboru, seřadí hráče od nejlepšího a ukáže TOP 10.


🚀 Vychytávky, co funguje trochu jinak než obvykle
V základním Hadovi fungují věci hodně jednoduše. Já jsem vyřešil pár zajímavých problémů:

1. Had potravu skutečně "tráví"
V obyčejných hrách had po snězení bonusu prostě ze sekundy na sekundu povyroste o 5 políček naráz. To vypadá divně a může to rozbít hru. Můj had má systém trávení. Když sní zlaté jablko, jen si zapamatuje, že má narůst. Jak se pak hýbe dál po mapě, postupně a plynule se mu prodlužuje ocas, dokud jablko "nestráví".
void Update(bool snedlJablko = false) {
    Vector2 newHead = { body[0].x + direction.x, body[0].y + direction.y };
    body.push_front(newHead);
    if(snedlJablko) {
        // Had roste okamžitě
    }
    else if(kTraveni > 0) {
        kTraveni--; // Had roste postupně, tráví bonus
    } else {
        body.pop_back(); // Klasický pohyb
    }
}

2. Okno hry, které se samo přizpůsobuje
Menu a žebříčky jsou velké a přehledné (okno 800x600). Ale když si v nastavení vyberete malou mapu (třeba zkušební tutoriál o velikosti 4x5 políček), okno se po startu hry samo fyzicky smrskne jen na velikost té mapy. Když umřete nebo vyhrajete, okno se zase plynule nafoukne zpátky, abyste mohli pohodlně napsat své jméno.
if(IsKeyPressed(KEY_FOUR)){
    herniMapa.NastavMapu(TUTORIAL);
    zmenaMapy = true;
}
if(zmenaMapy == true) {
    snake.Restart(herniMapa.minX, herniMapa.maxX, herniMapa.minY, herniMapa.maxY);
    jablko.Generuj(snake.body, herniMapa.minX, herniMapa.maxX, herniMapa.minY, herniMapa.maxY, herniMapa.prekazky);
    // Změna velikosti okna podle vybrané mapy
    SetWindowSize(herniMapa.oknoSirka, herniMapa.oknoVyska);
}

3. Neprůstřelné psaní jména
Raylib občas nestíhá, když hráč píše jméno do žebříčku moc rychle (např. zmáčkne dvě klávesy v jednom zlomku vteřiny) a některá písmenka pak chybí. Přepsal jsem to tak, že si hra stisky ukládá do dočasné "čekárny" a pak je zpracuje všechny najednou. Mazání nebo potvrzení Enterem funguje naprosto nezávisle na tom.
int zklavesnice = GetCharPressed();

while (zklavesnice > 0) {
    if((zklavesnice >= 33) && (zklavesnice <= 125) && (jmenoHrace.length() < 10)) {
        jmenoHrace += (char)zklavesnice;
    }
    zklavesnice = GetCharPressed();
}

4. Zlaté jablko a skutečný čas
Zlaté jablko se objeví po každých pěti červených a za 5 sekund zmizí. Tyto sekundy se ale nepočítají podle rychlosti pohybu hada nebo podle snímků za vteřinu, ale hra si pomocí vnitřních stopek neustále měří reálný čas. Jablko tak zmizí vždy přesně za 5 vteřin, ať už se hra trochu sekne, nebo jede na maximum.


🛠 Jak šel čas s vývojem
Kód jsem nepsal rovnou do téhle podoby, ale rostl mi postupně se upravoval podle objevování nových funkcí a systémů zápisu:

Všechno na jedné hromadě: Nejdřív jen čtvereček, co se hýbe po obrazovce, jí jablko a umře o stěnu. Vše v jednom obřím souboru.

Paměť na skóre: Přidání žebříčku a komunikace s .txt souborem, aby se nejlepší výkony po vypnutí hry neztratily.

Skutečná výhra: Přidání podmínky, která hlídá, jestli už had nezabral celou plochu. Konečně se hra dá reálně dohrát.

Chytřejší jablka a rytmus hry: Vytvoření systému, který počítá snědená jablka a posílá na mapu mizející zlaté bonusy.

Velký úklid kódu: Vytrhání logiky jablek a map ven z hlavního souboru, aby se to dalo vůbec číst a upravovat.

Různé mapy: Finální dodělání výběru různých map (od obrovských po malé s překážkami) a vyladění okna, které se k nim umí přizpůsobit.


⚙️ Pro spuštění hry je potřeba:
mít staženou knihovnu Raylib, a přes command line přidat.

dále je třeba kompilovat všechny soubory pro vytvoření a možnost  spuštění samotné hry HRA.EXE
g++ snake.cpp zebricek.cpp jablko.cpp mapa.cpp -o hra.exe -lraylib -lgdi32 -lwinmm

a poté už jenom jednoduše spustit hru
.\hra.exe 

