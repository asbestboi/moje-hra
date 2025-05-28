#include "fight.h"
#include "utility.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

const char* attackPhrases[] = {
    "Zasahl jsi %s a zpusobil %d poskozeni!",
    "Tvuj utok na %s byl silny udelal %d dmg!",
    "Tvuj mec zasahl %s a ubral %d zivota.",
    "Rozdrtil jsi %s a vzal mu %d HP!"
};

const char* spellPhrases[] = {
    "Seslal jsi kouzlo na %s a zasahl za %d!",
    "Magie zasahla %s a zpusobila %d poskozeni!",
    "Kouzlo explodovalo na %s za %d dmg!",
    "Tvoje sila zasahla %s za %d zivota!",
    "Magicky vyboj poskodil %s za %d!"
};

void logEvent(const std::string& text) {
    std::ofstream log("debug_log.txt", std::ios::app);
    if (log.is_open()) {
        log << text << "\n";
        log.close();
    }
}

void addXP(Character& player, int amount) {
    player.xp += amount;
    while (player.xp >= 50) {
        player.xp -= 50;
        player.lvl++;
        SetColor(2, 0);
        std::cout << "LEVEL UP!\n";
        SetColor(7, 0);
        std::cout << "Jsi ted na levelu " << player.lvl << "!\n";
        player.maxHealth += 2;
        player.health = player.maxHealth;
        player.maxEnergy += 1;
        player.energy = player.maxEnergy;
        player.attack += 1;
        std::cout << "Ziskal jsi:\n";
        std::cout << " +2 max zivotu\n +1 max energie\n +1 utok\n";
        waitForKeyPress();
        clearScreen();
    }
}

bool checkIfPlayerDied(Character &player) {
    if (player.health <= 0) {
        if (rand() % 100 < player.blessingChance) {
            clearScreen();
            SetColor(6, 0);
            printAsciiArt("blessed");
            SetColor(14, 0);
            std::cout << "---BUH SE SLITOVAL, BYL JSI SPASEN---\n";
            SetColor(7, 0);
            waitForKeyPress();
            player.health = player.maxHealth;
            player.energy = player.maxEnergy;
            player.blessingChance /= 2;
            clearScreen();
            return false;
        } else {
            clearScreen();
            SetColor(4, 0);
            printAsciiArt("smrt");
            SetColor(7, 0);
            exit(0);
        }
    }
    return false;
}

void showCurrentStats(Character &player, Monster monsters[], int monsterCount) {
    SetColor(10, 0);
    std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
    SetColor(1, 0);
    std::cout << "Energie: " << player.energy << "/" << player.maxEnergy << "\n\n";
    SetColor(7, 0);

    std::cout << "Ziva monstra:\n";
    for (int i = 0, count = 1; i < monsterCount; ++i) {
        if (monsters[i].health > 0) {
            SetColor(2, 0);
            std::cout << count << ": " << (player.isBlind ? "nekdo" : monsters[i].name + " (" + std::to_string(monsters[i].health) + " HP)") << "\n";
            SetColor(7, 0);
            count++;
        }
    }
    std::cout << "\n";
}

void fight(Character &player, Monster monsters[], int monsterCount) {
    drawHeaderLine();
    SetColor(4, 0);
    std::cout << "Pred tebou stoji " << monsterCount;
    if (monsterCount < 5 && monsterCount != 1) {
        std::cout << " nepratele!\n";
    } else {
        std::cout << " nepratel!\n";
    }
    SetColor(7, 0);
    waitForKeyPress();
    clearScreen();

    bool activeBoss = false;
    for (int i = 0; i < monsterCount; ++i) {
        if (monsters[i].isBoss) {
            activeBoss = true;
            break;
        }
    }

    while (player.health > 0) {
        //clearScreen();
        drawHeaderLine();
        showCurrentStats(player, monsters, monsterCount);
        drawHeaderLine();

        int indexMap[10];
        int aliveCount = 0;
        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health > 0) {
                indexMap[aliveCount++] = i;
            }
        }

        std::cout << "Chces zautocit ["; SetColor(5, 0); std::cout << "1"; SetColor(7, 0);
        std::cout << "], pouzit kouzlo ["; SetColor(1, 0); std::cout << "2"; SetColor(7, 0);
        std::cout << "] nebo leceni ["; SetColor(10, 0); std::cout << "3"; SetColor(7, 0);
        std::cout << "]? ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        int target = -1;
        if (choice == 1 || choice == 2) {
            if (aliveCount == 1) {
                target = indexMap[0];
            } else {
                std::cout << "Na ktere monstrum utocis <1-" << aliveCount << ">? ";
                int pick; std::cin >> pick; pick--;
                if (pick < 0 || pick >= aliveCount) {
                    clearScreen();
                    continue;
                }
                target = indexMap[pick];
            }
        }

        if (choice == 1) {
            clearScreen();
            int damage = player.attack;
            if (player.isBlind) damage *= 1.5;
            monsters[target].health -= damage;

            const char* phrase = attackPhrases[rand() % 5];
            char buffer[256];
            sprintf(buffer, phrase, (player.isBlind ? "nekdo" : monsters[target].name.c_str()), damage);
            SetColor(5, 0);
            std::cout << buffer << "\n";
            SetColor(7, 0);

            logEvent("Utocis na " + monsters[target].name + " za " + std::to_string(damage));
            addXP(player, 5);

            if (player.vampire && monsters[target].health <= 0) {
                int heal = player.maxHealth / 4;
                player.health = std::min(player.maxHealth, player.health + heal);
                SetColor(14, 0);
                std::cout << "Jako upir sis vylecil " << heal << " zivotu\n";
                SetColor(7, 0);
            }

            if (player.energy < player.maxEnergy) player.energy++;

        } else if (choice == 2) {
            clearScreen();
            if (player.energy >= 3) {
                player.energy -= 3;
                int spellDamage = player.attack * 2;
                monsters[target].health -= spellDamage;

                const char* phrase = spellPhrases[rand() % 5];
                char buffer[256];
                sprintf(buffer, phrase, monsters[target].name.c_str(), spellDamage);
                SetColor(1, 0);
                std::cout << buffer << "\n";
                SetColor(7, 0);

                logEvent("Pouzil jsi kouzlo na " + monsters[target].name + " za " + std::to_string(spellDamage));
                addXP(player, 5);
            } else {
                SetColor(4, 0);
                std::cout << "Nemas dost energie na kouzlo!\n";
                SetColor(7, 0);
                waitForKeyPress();
                clearScreen();
                continue;
            }
        } else if (choice == 3) {
            if (player.energy >= 2) {
                clearScreen();
                player.energy -= 2;
                int healAmount = 4;
                player.health = std::min(player.maxHealth, player.health + healAmount);
                SetColor(10, 0);
                std::cout << "Vylecil sis " << healAmount << " zivotu!\n";
                SetColor(7, 0);

                logEvent("Vylecil sis " + std::to_string(healAmount) + " zivotu");
                addXP(player, 5);
            } else {
                SetColor(4, 0);
                std::cout << "Nemas dost energie na leceni!\n";
                SetColor(7, 0);
                waitForKeyPress();
                clearScreen();
                continue;
            }
        } else {
            continue;
        }

        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health <= 0) continue;

            int hitChance = player.dodge ? 45 : 25;
            if (rand() % 100 < hitChance) continue;

            int damage = rand() % (monsters[i].maxAttack - monsters[i].minAttack + 1) + monsters[i].minAttack;
            player.health -= damage;

            logEvent((player.isBlind ? "nekdo" : monsters[i].name) + " zasahl hrace za " + std::to_string(damage));

            SetColor(4, 0);
            std::cout << (player.isBlind ? "nekdo" : monsters[i].name) << " te zasahl za " << damage << " zivotu!\n";
            SetColor(7, 0);



            if (checkIfPlayerDied(player)) return;
        }

        bool allDead = true;
        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health > 0) {
                allDead = false;
                break;
            }
        }
        if (allDead) {
            SetColor(6, 0);
            std::cout << "Vyhral jsi!\n";

            int chanceToGetGold = activeBoss ? 100 : (player.dodge ? 65 : 50);
            if (rand() % 100 < chanceToGetGold) {
                int gold = rand() % 30 + 10;
                player.gold += gold;
                std::cout << "Ziskal jsi " << gold << " zlata. Mas celkem " << player.gold << " zlata.\n";
            }

            SetColor(7, 0);
            addXP(player, 5);
            waitForKeyPress();
            clearScreen();
            return;
        }

        //waitForKeyPress();
    }
}
