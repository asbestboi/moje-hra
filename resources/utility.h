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
void addXP(Character& player, int amount) {
    player.xp += amount;
    while (player.xp >= 50) {
        player.xp -= 50;
        player.lvl++;
        SetColor(2, 0);
        std::cout << "\nLEVEL UP!\n";
        SetColor(7, 0); //bila
        std::cout << "Jsi ted na levelu " << player.lvl << "!\n";
        player.maxHealth += 2;
        player.health = player.maxHealth;
        player.maxEnergy += 1;
        player.energy = player.maxEnergy;
        player.attack += 1;
        std::cout << "Ziskal jsi:\n";
        std::cout << " +2 max zivotu\n +1 max energie\n +1 utok\n";
        system("pause");
        clearScreen();
    }
}
void village(Character &player) {
    bool visitedChurch = false;
    printAsciiArt("vesnice");

    system("pause");

    if (player.gamble == true) {
        player.gold = rand() % 151;
    }

    while (true) {
        clearScreen();
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "/" << player.maxEnergy << "\n";
        SetColor(5, 0);
        std::cout << "Utok: " << player.attack << "\n";
        SetColor(6, 0);
        std::cout << "Zlato: " << player.gold << "\n";
        SetColor(11, 0);
        std::cout << "---VESNICE---\n";
        SetColor(7, 0);
        std::cout << "[1] Jit do krcmy (obnovis si zivoty a energii a vylepsis charisma za 5 zlata)\n";
        std::cout << "[2] Jit do kostela (zde se modlis a ziskas sanci ze te buh ochrani)\n";
        std::cout << "[3] Jit do obchodu (muzes nakupovat upgrady za zlato)\n";
        SetColor(4, 0);
        std::cout << "[4] Odejit z vesnice\n";
        SetColor(7, 0);

        int mainChoice;
        std::cin >> mainChoice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            continue;
        }

        if (mainChoice == 1) {
            if (player.gold >= 5) {
                player.gold -= 5;
                clearScreen();
                std::cout << "V krcme je veselo. Das si pivko a na chvilku si odpocines. (obnovil sis zivoty a energii)\n";
                player.charisma += 3;
                player.health = player.maxHealth;
                player.energy = player.maxEnergy;
                system("pause");
                clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        system("pause");
                        clearScreen();
                    }
        } else if (mainChoice == 2) {
            clearScreen();
            if (player.vampire) {
                std::cout << "Jako upir nemuzes vstoupit do svateho mista.\n";
            } else {
                if (visitedChurch) {
                    std::cout << "Uz ses dnes modlil. Buh te slysel.\n";
                } else {
                    if (player.blessingChance < 100) {
                        player.blessingChance += 10;
                        if (player.blessingChance > 100) player.blessingChance = 100;
                        std::cout << "Pomodlil ses v kostele. Buh vidi skrze tve lzi. Sance ze te buh ochrani je " << player.blessingChance << "%.\n";
                    } else {
                        std::cout << "Buh te miluje.\n";
                    }
                    visitedChurch = true;
                }
            }
            system("pause");
        } else if (mainChoice == 3) {
            while (true) {
                clearScreen();
                SetColor(10, 0);
                std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
                SetColor(1, 0);
                std::cout << "Energie: " << player.energy << "/" << player.maxEnergy << "\n";
                SetColor(5, 0);
                std::cout << "Utok: " << player.attack << "\n";
                SetColor(6, 0);
                std::cout << "Zlato: " << player.gold << "\n";
                SetColor(7, 0);
                std::cout << "---OBCHOD---\n";
                std::cout << "["; SetColor(10, 0); std::cout << "1"; SetColor(7, 0); std::cout << "] Vylepsit zivoty o 5"; SetColor(6, 0); std::cout << " (15 zlata)\n";SetColor(7, 0);
                std::cout << "["; SetColor(1, 0); std::cout << "2"; SetColor(7, 0); std::cout << "] Vylepsit energii o 5"; SetColor(6, 0); std::cout << " (20 zlata)\n";SetColor(7, 0);
                std::cout << "["; SetColor(5, 0); std::cout << "3"; SetColor(7, 0); std::cout << "] Vylepsit utok o 2"; SetColor(6, 0); std::cout << " (20 zlata)\n";
                SetColor(4, 0); std::cout << "[4] Zpet do vesnice\n";
                SetColor(7, 0);

                int shopChoice;
                std::cin >> shopChoice;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    clearScreen();
                    continue;
                }

                if (shopChoice == 1) {
                    if (player.gold >= 15) {
                        player.gold -= 15;
                        player.maxHealth += 5;
                        clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        system("pause");
                        clearScreen();
                    }
                } else if (shopChoice == 2) {
                    if (player.gold >= 20) {
                        player.gold -= 20;
                        player.maxEnergy += 5;
                        clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        system("pause");
                        clearScreen();
                    }
                } else if (shopChoice == 3) {
                    if (player.gold >= 20) {
                        player.gold -= 20;
                        player.attack += 2;
                        clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        system("pause");
                        clearScreen();
                    }
                } else if (shopChoice == 4) {
                    break;
                } else {
                    clearScreen();
                }
            }
        } else if (mainChoice == 4) {
            clearScreen();
            std::cout << "Opustil jsi vesnici.\n";
            system("pause");
            break;
        } else {
            clearScreen();
        }
    }
}