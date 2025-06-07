#pragma once
#include "utility.h"
void village(Character &player) {
    int drunkness = 0;
    bool visitedChurch = false;
    PlaySound("resources/sounds/appear.wav", NULL, SND_FILENAME | SND_ASYNC);
    printAsciiArt("vesnice");

    waitForKeyPress();
    PlaySound("resources/shop.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
        drawHeaderLine();
        SetColor(11, 0);
        printCentered("VESNICE");
        drawHeaderLine();
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
            if (drunkness == 3) {
                    clearScreen();
                    PlaySound(NULL, NULL, 0);
                    std::cout << "Ty hlupaku! Opil jses do nemoty, probouzis se na uplne nahodnem miste mimo vesnici.\n";
                    waitForKeyPress();
                    break;
                } else {
                    int cena = 5;

                    //sleva podle charisma
                    if (player.charisma >= 55) {
                        cena = 2;
                    } else if (player.charisma >= 45) {
                        cena = 3;
                    } else if (player.charisma >= 30) {
                        cena = 4;
                    }

                    if (player.gold >= cena) {
                        clearScreen();
                        std::cout << "V krcme je veselo. Das si pivko a na chvilku si odpocines. (obnovil sis zivoty a energii)\n";
                        std::cout << "Zaplatil jsi " << cena << " zlatych.\n";
                        drunkness++;
                        if (drunkness == 1) {
                            std::cout << "Osvezujici pivo!\n";
                        } else if (drunkness == 2) {
                            std::cout << "V krcme je velka zabava!\n";
                        } else if (drunkness == 3) {
                            std::cout << "Citis se opily, asi bys mel prestat pit.\n";
                        }
                        player.gold -= cena;
                        player.charisma += 3;
                        player.health = player.maxHealth;
                        player.energy = player.maxEnergy;
                        waitForKeyPress();
                        clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        waitForKeyPress();
                        clearScreen();
                    }
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
            waitForKeyPress();
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
                        waitForKeyPress();
                        clearScreen();
                    }
                } else if (shopChoice == 2) {
                    if (player.gold >= 20) {
                        player.gold -= 20;
                        player.maxEnergy += 5;
                        clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        waitForKeyPress();
                        clearScreen();
                    }
                } else if (shopChoice == 3) {
                    if (player.gold >= 20) {
                        player.gold -= 20;
                        player.attack += 2;
                        clearScreen();
                    } else {
                        std::cout << "Nemas dostatek zlata!\n";
                        waitForKeyPress();
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
            PlaySound(NULL, NULL, 0);
            waitForKeyPress();
            break;
        } else {
            clearScreen();
        }
    }
}
