#include "fight.h"
#include "utility.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
const char* attackPhrases[] = {
    "Zasahl jsi %s a zpusobil %d poskozeni!",
    "Tvuj utok na %s byl silny udelal %d dmg!",
    "Dorazil jsi %s za %d!",
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

void fight(Character &player, Monster monsters[], int monsterCount) {
    SetColor(4, 0); //cervena
    std::cout << "---Pred tebou stoji " << monsterCount;
    if (monsterCount < 5 && monsterCount!= 1) {
    std::cout << " nepratele!---\n";
    }
    else {
    std::cout << " nepratel!---\n";
    }  
    SetColor(7, 0); //bila
    system("pause");
    clearScreen();
    while (player.health > 0) {
        // kontrola
        bool allDead = true;
        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health > 0) {
                allDead = false;
                break;
            }
        }
        if (allDead) {
            SetColor(6, 0);
            std::cout << "\nVyhral jsi!\n";
            player.gold += rand() % 30 + 10;
            std::cout << "Ziskal jsi zlato. Mas " << player.gold << " zlata.\n";
            SetColor(7, 0);
            addXP(player, 10);
            system("pause");
            clearScreen();
            return;
        }

        // Vypis zivych monster
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "/" << player.maxEnergy << "\n";
        SetColor(7, 0);
        std::cout << "\nZiva monstra:\n";
        int indexMap[10];
        int aliveCount = 0;
        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health > 0) {
                SetColor(2, 0);
                if (player.isBlind == true){
                    std::cout << (aliveCount + 1) << ": " << (player.isBlind ? "nekdo" : monsters[i].name) << "\n";
                }
                else{
                    std::cout << (aliveCount + 1) << ": " << monsters[i].name << " (" << monsters[i].health << " HP)\n";
                }
                SetColor(7, 0); //bila
                indexMap[aliveCount] = i;
                aliveCount++;
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
            clearScreen();
            continue;
        }
        
        int target = -1;
        if (choice == 1 || choice == 2) {
            if (aliveCount == 1) {
                target = indexMap[0];
            } else {
                std::cout << "Na ktere monstrum utocis <1-" << aliveCount << ">? ";
                int pick;
                std::cin >> pick;
                pick--;
                if (pick < 0 || pick >= aliveCount) {
                    clearScreen();
                    continue;
                }
                target = indexMap[pick];
            }
        }

        if (choice == 1) {
            int damage = player.attack;
            if (player.isBlind) damage *= 1.5;
            monsters[target].health -= damage;
            addXP(player, 5);
            if (player.vampire && monsters[target].health <= 0) {
                player.health = std::min(player.maxHealth, player.health + player.maxHealth/4);
                SetColor(14, 0);
                std::cout << "Jako upir sis vylecil " << player.maxHealth/4 << " zivotu\n";
                SetColor(7, 0);
                system("pause");
            }
            //toto pujde videt jen na konci souboje
            if (allDead == true){
            SetColor(5, 0);
            const char* phrase = attackPhrases[rand() % 5];
            printf(phrase, (player.isBlind ? "nekoho" : monsters[target].name.c_str()), damage);
            }
            if (player.energy < player.maxEnergy) player.energy++;
            SetColor(7, 0);
        } else if (choice == 2) {
            if (player.energy >= 3) {
                player.energy -= 3;
                int spellDamage = player.attack * 2;
                monsters[target].health -= spellDamage;
                addXP(player, 5);
                //toto pujde videt jen na konci souboje
                if (allDead == true){
                    SetColor(5, 0);
                    const char* phrase = spellPhrases[rand() % 5];
                    printf(phrase, (player.isBlind ? "nekoho" : monsters[target].name.c_str()), spellDamage);
                }
                SetColor(7, 0); //bila
            } else {
                SetColor(4, 0); //cervena
                std::cout << "Nemas dost energie na kouzlo!\n";
                SetColor(7, 0); //bila
                continue;
            }
        } else if (choice == 3) {
            if (player.energy >= 2) {
                player.energy -= 2;
                addXP(player, 5);
                int healAmount = 4;
                player.health = std::min(player.maxHealth, player.health + healAmount);
                clearScreen();
                continue;
            } else {
                SetColor(4, 0); //cervena
                std::cout << "Nemas dost energie na leceni!\n";
                SetColor(7, 0); //bila
                continue;
            }
        } else {
            clearScreen();
            continue;
        }

        // Kazde zive monstrum zautoci
        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health <= 0) continue;

            if ((!player.dodge && rand() % 100 < 25) || (player.dodge && rand() % 100 < 45)) {
                SetColor(4, 0); //cervena
                //std::cout << (player.isBlind ? "nekdo" : monsters[i].name) << " minul!\n"; - mozna tento cout vyuziju jinak
                SetColor(7, 0); //bila
                clearScreen();
                continue;
            }

            int damage = rand() % (monsters[i].maxAttack - monsters[i].minAttack + 1) + monsters[i].minAttack;
            player.health -= damage;
            SetColor(4, 0); //cervena
            //std::cout << (player.isBlind ? "nekdo" : monsters[i].name) << " na tebe zautocil za " << damage << "!\n"; - mozna tento cout vyuziju jinak
            SetColor(7, 0); //bila
            clearScreen();
        }
        if (player.health <= 0) {
            if (rand() % 100 < player.blessingChance) {
                clearScreen();
                SetColor(14, 0);
                std::cout << "---BUH SE SLITOVAL, BYL JSI SPASEN---\n";
                SetColor(6, 0);
                printAsciiArt("blessed");
                SetColor(7, 0);
                system("pause");
                player.health = player.maxHealth;
                player.energy = player.maxEnergy;
                player.blessingChance /= 2;
                clearScreen();
            }
            else{
            SetColor(4, 0); //cervena
            clearScreen();
            printAsciiArt("smrt");
            SetColor(7, 0); //bila
            system("pause");
            exit(0);}
        }
    }
}