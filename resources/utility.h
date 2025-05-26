#pragma once
#include <string>
#include <conio.h>
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
            if (line.rfind("===", 0) == 0) break; //dalsi sekce
            std::cout << line << "\n";
        }
    }
    if (!found) {
        std::cerr << "Obrazek '" << name << "' nebyl nalezen v souboru.\n";
    }
}
void printCentered(const std::string& text) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int padding = (columns - text.length()) / 2;
    if (padding < 0) padding = 0;

    std::cout << std::string(padding, ' ') << text << std::endl;
}
void drawHeaderLine() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    SetColor(8, 0); //seda
    for (int i = 0; i < columns; ++i) {
        std::cout << '-';
    }
    std::cout << std::endl;
    SetColor(7, 0);
}
void waitForKeyPress() {
    SetColor(8, 0); //seda
    std::cout << "Zmacknete klavesu pro pokracovani...\n";
    SetColor(7, 0);
    _getch();
}