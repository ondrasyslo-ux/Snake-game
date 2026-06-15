#include "zebricek.h"
#include <fstream>
#include <algorithm>

Zebricek::Zebricek(std::string soubor) {
    nazevSouboru = soubor;
    nacti();
}