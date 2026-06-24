#include "zebricek.h"
#include <fstream>
#include <algorithm>
//soubor pro praci se souborem zebricek.txt

Zebricek::Zebricek(std::string soubor) {
    nazevSouboru = soubor;
    nacti();
}

void Zebricek::nacti() {
    topHraci.clear();

    std::ifstream soubor(nazevSouboru);
    if(soubor.is_open()) {
        std::string nJmeno = ""; int nSkore = 0;
        
        while (soubor >> nJmeno >> nSkore) {
            topHraci.push_back({nJmeno, nSkore});
        }
      soubor.close();  
    }
    std::sort(topHraci.begin(), topHraci.end(), 
    [](const ZaznamZebricku& a, const ZaznamZebricku& b) {
        return a.skore > b.skore;
    });
    
    if(topHraci.size() > 10 ) {
        topHraci.resize(10);
    }
}

void Zebricek::ulozNovyVysledek(std::string jmeno, int skore) {
    std::ofstream soubor(nazevSouboru, std::ios::app);
    if(soubor.is_open()) {
        soubor << jmeno << " " << skore << " \n";
        soubor.close();
    }
    nacti();
}

std::vector<ZaznamZebricku> Zebricek::ZiskejTop10() const {
    return topHraci;
}
