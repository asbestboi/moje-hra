#include "fight.h"
#include "utility.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
void attacksound() {
    PlaySound("resources/sounds/attack.wav", NULL, SND_FILENAME | SND_ASYNC);
}
void damagesound() {
    PlaySound("resources/sounds/damage.wav", NULL, SND_FILENAME | SND_ASYNC);
}
void healsound(){
    PlaySound("resources/sounds/heal.wav", NULL, SND_FILENAME | SND_ASYNC);
}
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
bool areAllMonstersDead(Monster monsters[], int count) {
    for (int i = 0; i < count; ++i)
        if (monsters[i].health > 0) return false;
    return true;
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
        healsound();
        waitForKeyPress();
        clearScreen();
    }
}

bool checkIfPlayerDied(Character &player) {
    if (player.health <= 0) {
        if (rand() % 100 < player.blessingChance) {
            clearScreen();
            Sleep(1300);
            PlaySound(NULL, NULL, 0);
            PlaySound("resources/sounds/blessed.wav", NULL, SND_FILENAME | SND_ASYNC);
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
            Sleep(1300);
            SetColor(4, 0);
            printAsciiArt("smrt");
            SetColor(7, 0);
            PlaySound("resources/sounds/death.wav", NULL, SND_FILENAME | SND_ASYNC);
            Sleep(1000);
            waitForKeyPress();
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
    PlaySound("resources/sounds/appear.wav", NULL, SND_FILENAME | SND_ASYNC);
    SetColor(4, 0);
    std::cout << "Pred tebou stoji " << monsterCount;
    if (monsterCount < 5 && monsterCount != 1) {
        std::cout << " nepratele!\n";
    } else {
        std::cout << " nepritel!\n";
    }
    SetColor(7, 0);
    waitForKeyPress();
    clearScreen();
    PlaySound("resources/sounds/encounter.wav", NULL, SND_FILENAME | SND_ASYNC);
    bool bossHasAttacked = false;
    bool activeBoss = false;
    for (int i = 0; i < monsterCount; ++i) {
        if (monsters[i].isBoss) {
            activeBoss = true;
            break;
        }
    }

    while (player.health > 0) {
        if (activeBoss && !bossHasAttacked) {
            for (int i = 0; i < monsterCount; ++i) {
            if (!monsters[i].isBoss || monsters[i].health <= 0) continue;

            int hitChance = player.dodge ? 45 : 25;
            if (rand() % 100 < hitChance) continue;

            int damage = rand() % (monsters[i].maxAttack - monsters[i].minAttack + 1) + monsters[i].minAttack;
            player.health -= damage;

            logEvent("Boss " + monsters[i].name + " zahajil utok na hrace za " + std::to_string(damage));
            SetColor(4, 0);
            std::cout << monsters[i].name << " zahajil utok a zasahl te za " << damage << " zivotu!\n";
            damagesound();
            SetColor(7, 0);

            if (checkIfPlayerDied(player)) return;
    }
    bossHasAttacked = true;
}

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
        std::cout << "], otevrit inventar ["; SetColor(10, 0); std::cout << "3"; SetColor(7, 0);
        std::cout << "] nebo schopnost ["; SetColor(14, 0); std::cout << "4"; SetColor(7, 0); std::cout << "]? ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
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
            if (monsterCount == 1 && monsters[target].name == "Chlapec" && monsters[target].health <= player.attack) {
                    SetColor(14, 0);
                    std::cout << "Chlapec je na pokraji smrti...\n";
                    Sleep(1000);
                    std::cout << "Chces mu dat milost [1] nebo ho dorazit [2]? ";
                    int volba;
                    std::cin >> volba;
                    if (volba == 1) {
                        std::cout << "Udelil jsi milost. Chlapec te mlcky opusti...\n";
                        monsters[target].health = 0;
                        waitForKeyPress();
                        clearScreen();
                        break;
                    }
                    player.mercy = false;
                }
            monsters[target].health -= damage;

            const char* phrase = attackPhrases[rand() % 5];
            char buffer[256];
            sprintf(buffer, phrase, (player.isBlind ? "nekdo" : monsters[target].name.c_str()), damage);
            SetColor(5, 0);
            std::cout << buffer << "\n";
            SetColor(7, 0);

            logEvent("Utocis na " + monsters[target].name + " za " + std::to_string(damage));
            attacksound();
            addXP(player, 5);
            if (player.vampire && monsters[target].health <= 0) {
                int heal = player.maxHealth / 4;
                player.health = std::min(player.maxHealth, player.health + heal);
                SetColor(14, 0);
                std::cout << "Jako upir sis vylecil " << heal << " zivotu\n";
                healsound();
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
                attacksound();
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
            if (player.inventory.empty()) {
                std::cout << "Tvuj inventar je prazdny!\n";
                waitForKeyPress();
                clearScreen();
                continue;
            }
            std::cout << "Tvuj inventar:\n";
            for (size_t i = 0; i < player.inventory.size(); ++i) {
                std::cout << i + 1 << ". " << player.inventory[i] << "\n";
            }
            std::cout << "Vyber cislo itemu: ";
            int itemChoice;
            std::cin >> itemChoice;
            itemChoice--;

            if (itemChoice < 0 || itemChoice >= player.inventory.size()) {
                std::cout << "Neplatny vyber.\n";
                waitForKeyPress();
                clearScreen();
                continue;
            }

            std::string item = player.inventory[itemChoice];
            if (item == "Lektvar leceni") {
                int heal = 6;
                player.health = std::min(player.maxHealth, player.health + heal);
                healsound();
                std::cout << "Pouzil jsi lektvar a vylecil " << heal << " zivotu.\n";
            } else if (item == "Holy Hand Grenade") {
                for (int i = 0; i < monsterCount; ++i)
                    if (monsters[i].health > 0) monsters[i].health -= 3;
                    if (areAllMonstersDead(monsters, monsterCount)) {
                    waitForKeyPress(); clearScreen(); break;
                    attacksound();
                    }
                std::cout << "Hodil jsi Holy Hand Grenade! Vsechna monstra utrpela 3 poskozeni.\n";
            } else if (item == "Crucifix") {
                int damage = 3 + rand() % 3;
                int heal = 2 + rand() % 5;

                for (int i = 0; i < monsterCount; ++i) {
                    if (monsters[i].health > 0)
                        monsters[i].health -= damage;
                if (areAllMonstersDead(monsters, monsterCount)) {
                PlaySound("resources/sounds/blessed.wav", NULL, SND_FILENAME | SND_ASYNC);
                waitForKeyPress(); clearScreen(); break;
                }
                }

                player.health = std::min(player.maxHealth, player.health + heal);

                std::cout << "Crucifix ozaril vsechny nepratele a zpusobil " << damage << " poskozeni.\n";
                std::cout << "Zaroven jsi byl vylecen o " << heal << " zivotu.\n";
            } else if (item == "Totem") {
                int roll = rand() % 2;

                if (roll == 0) {
                    player.health = 1;
                    std::cout << "Totem selhal... prezivas jen s 1 zivotem.\n";
                    damagesound();
                } else {
                    player.health = player.maxHealth;
                    player.energy = player.maxEnergy;
                    std::cout << "Totem zablikal silou a plne te obnovil!\n";
                    healsound();
                }
            } else {
                std::cout << "Neznamy item!\n";
            }
            player.inventory.erase(player.inventory.begin() + itemChoice);
            waitForKeyPress();
            clearScreen();
            continue;
            }
        else if (choice == 4) {
            clearScreen();
            if (player.energy < 2) {
                std::cout << "Nemas dost energie na schopnost!\n";
                waitForKeyPress(); clearScreen(); continue;
            }

            if (player.name == "slepec") {
                    player.energy -= 2;
                    int dmg = 2 + rand() % 3;
                    for (int i = 0; i < monsterCount; ++i)
                        if (monsters[i].health > 0)
                            monsters[i].health -= dmg;

                    std::cout << "Slepy hnev zasahl vsechny nepratele za " << dmg << " poskozeni.\n";
                    damagesound();
                    if (areAllMonstersDead(monsters, monsterCount)) {
                        waitForKeyPress(); clearScreen(); break;
                    }
            } else if (player.name == "mnich") {
                player.energy -= 2;
                int heal = 6 + rand() % 5;
                player.health = std::min(player.maxHealth, player.health + heal);
                std::cout << "Pomoci modlitby sis vylecil " << heal << " zivotu.\n";
                healsound();
                if (rand() % 100 < 15) {
                    player.blessingChance += 10;
                    std::cout << "Citis, ze Buh te slysi. (+10% blessing chance)\n";
                }
            } else if (player.name == "upir") {
                player.energy += 4;
                player.health = std::max(player.maxHealth, player.health - 2);
                std::cout << "Provedl jsi krvavy ritual! Ziskal jsi 4 energie, ale prisel o 2 zivoty.\n";
                damagesound();
            } else if (player.name == "gambler") {
                player.energy -= 2;
                int roll = rand() % 2;
                if (roll == 0) {
                    player.health = std::max(1, player.health / 2);
                    std::cout << "Smula! Prisels o polovinu svych zivotu!\n";
                    damagesound();
                } else {
                    player.health = player.maxHealth;
                    player.energy = player.maxEnergy;
                    std::cout << "Stesti! Mas plne zivoty i energii!\n";
                    healsound();
                }
            } else if (player.name == "zlodej") {
                player.energy -= 2;
                int stolenGold = 0;
                for (int i = 0; i < monsterCount; ++i)
                    if (monsters[i].health <= 0)
                        stolenGold += 5 + rand() % 20;
                if (stolenGold > 0) {
                    player.gold += stolenGold;
                    std::cout << "Ukradls " << stolenGold << " zlata z kapes porazenych monster.\n";
                } else {
                    std::cout << "Nebyl nikdo k okradeni.\n";
                }
            }

            waitForKeyPress(); clearScreen();
            continue;
        }
        else {
            clearScreen();
            continue;
        }

        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health <= 0) continue;

            int hitChance = player.dodge ? 45 : 25;
            if (rand() % 100 < hitChance) continue;
            Sleep(400);
            int damage = rand() % (monsters[i].maxAttack - monsters[i].minAttack + 1) + monsters[i].minAttack;
            player.health -= damage;
            damagesound();
            logEvent((player.isBlind ? "nekdo" : monsters[i].name) + " zasahl hrace za " + std::to_string(damage));

            SetColor(4, 0);
            std::cout << (player.isBlind ? "nekdo" : monsters[i].name) << " te zasahl za " << damage << " zivotu!\n";
            //(Sleep(400);
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
                int lootRoll = rand() % 100;

            if (lootRoll < 30) {
                player.inventory.push_back("Lektvar leceni");
                std::cout << "Nasel jsi Lektvar leceni!\n";
            } else if (lootRoll < 50) {
                player.inventory.push_back("Holy Hand Grenade");
                std::cout << "Nasel jsi Holy Hand Grenade!\n";
            } else if (lootRoll < 60) {
                player.inventory.push_back("Crucifix");
                std::cout << "Nasel jsi Crucifix!\n";
            } else if (lootRoll < 65) {
                player.inventory.push_back("Totem");
                std::cout << "Nasel jsi zvlastni totem.\n";
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
