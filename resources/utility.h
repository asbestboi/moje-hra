#pragma once
#include <string>

void SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) + textColor);
}
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
void printAsciiArt(const std::string& name, const std::string& filePath = "resources/ascii.txt") {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Nepodarilo se otevrit soubor: " << filePath << "\n";
        return;
    }
    std::string line;
    bool found = false;
    while (std::getline(file, line)) {
        if (line == "=== " + name + " ===") {
            found = true;
            continue;
        }
        if (found) {
            if (line.rfind("===", 0) == 0) break; // dalsi sekce
            std::cout << line << "\n";
        }
    }
    if (!found) {
        std::cerr << "Obrazek '" << name << "' nebyl nalezen v souboru.\n";
    }
}
