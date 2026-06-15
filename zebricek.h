#pragma once
#include <vector>
#include <string>

struct ZaznamZebricku {
    std::string jmeno;
    int skore;
};

class Zebricek {
private:
    std::vector<ZaznamZebricku> topHraci;
    std::string nazevSouboru;

public:
    Zebricek(std::string soubor);

    void nacti();
    void ulozNovyVysledek(std::string jmeno, int skore);

    std::vector<ZaznamZebricku> ZiskejTop10() const;
};
