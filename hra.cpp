#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <limits>
#include <fstream>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

void SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) + textColor);
}
//https://stackoverflow.com/questions/24776262/pause-console-in-c-program
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
//https://www.w3schools.com/cpp/ref_fstream_fstream.asp
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

struct Character {
    std::string name;
    int maxHealth;
    int health;
    int attack;
    int energy;
    int maxEnergy;
    int gold;
    int maxSanity;
    int sanity;
    int blessingChance;
    bool isBlind;
    bool isUndead;
    bool gamble;
    bool dodge;
    int xp = 0;
};

Character chooseClass() {
    int choice;
    Character player;
    while (true) {
        clearScreen();
        SetColor(6, 0);
        std::cout << "---Vyber si classu:---\n";
        SetColor(7, 0);
        std::cout << "[1] Slepec    -> Nemuze videt nepratele ani jejich zivoty ale je silnejsi\n";
        std::cout << "[2] Mnich     -> Ma sanci ze ho buh spasi\n";
        std::cout << "[3] Kostlivec -> Ma mensi heal ale ma 20% ke kazdemu utoku\n";
        std::cout << "[4] Gambler   -> Kdyz vejde do vesnice tak ma nahodny pocet penez\n";
        std::cout << "[5] Zlodej    -> Lepe se vyhyba utokum\n";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            continue;
        }
        if (choice == 1) {
            player = {"Slepec", 8, 8, 5, 6, 6, 25, 100, 100, 20, true, false, false, false};
        } else if (choice == 2) {
            player = {"Mnich", 6, 6, 3, 6, 6, 25, 110, 110, 100, false, false, false, false};
        } else if (choice == 3) {
            player = {"Kostlivec", 7, 7, 5, 5, 5, 25, 75, 75, 0, false, true, false, false};
        } else if (choice == 4) {
            player = {"Gambler", 6, 6, 3, 5, 5, 0, 80, 80, 0, false, false, true, false};
        } else if (choice == 5) {
            player = {"Zlodej", 5, 5, 4, 3, 3, 25, 100, 100, 0, false, false, false, true};
        } else {
            clearScreen();
            continue;
        }
        clearScreen();
        std::cout << "Jste si jisti, ze chcete hrat za " << player.name << "? [y/n]: \n";
        char confirmation;
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "\n";
        SetColor(5, 0);
        std::cout << "Utok: " << player.attack << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
        SetColor(6, 0);
        std::cout << "Zlato: "; if (player.gamble) std::cout << "nahodne"; else std::cout << player.gold; std::cout << "\n"; //jen text
        SetColor(7, 0);
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y') {
            clearScreen();
            return player;
        } else {
            clearScreen();
        }
    }
}

struct Monster {
    std::string name;
    int health;
    int minAttack;
    int maxAttack;
};
//hlasky
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
            system("pause");
            clearScreen();
            return;
        }

        // Vypis zivych monster
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
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
            if (player.isUndead) damage *= 1.2;
            if (player.isBlind) damage *= 1.5;
            monsters[target].health -= damage;
            SetColor(5, 0);
            //toto pujde videt jen na konci souboje
            if (allDead == true){
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
                SetColor(5, 0);
                //toto pujde videt jen na konci souboje
                if (allDead == true){
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
                int healAmount = player.isUndead ? 6 : 3;
                if (player.isUndead) healAmount /= 2;
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
                std::cout << "---BUH SE SLITOVAL, BYL JSI SPASEN---\n";
                SetColor(6, 0);
                printAsciiArt("blessed");
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
void village(Character &player) {
    bool visitedChurch = false;
    printAsciiArt("vesnice");

    system("pause");

    if (player.gamble == true) {
        player.gold = rand() % 101;
    }

    while (true) {
        clearScreen();
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
        SetColor(5, 0);
        std::cout << "Utok: " << player.attack << "\n";
        SetColor(6, 0);
        std::cout << "Zlato: " << player.gold << "\n";
        SetColor(11, 0);
        std::cout << "---VESNICE---\n";
        SetColor(7, 0);
        std::cout << "[1] Jit do krcmy\n";
        std::cout << "[2] Jit do kostela\n";
        std::cout << "[3] Jit do obchodu\n";
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
            clearScreen();
            player.health = player.maxHealth;
            player.energy = player.maxEnergy;
            std::cout << "V krčmě je veselo. Dáš si pivko a chvilku odpocines. (obnovil sis zivoty a energii)\n";
            system("pause");
        } else if (mainChoice == 2) {
            clearScreen();
            if (visitedChurch) {
                std::cout << "Uz ses dnes modlil. Buh te slysel.\n";
            } else {
                if (player.blessingChance < 100) {
                    player.blessingChance += 10;
                    if (player.blessingChance > 100) player.blessingChance = 100;
                    std::cout << "Pomodlil ses v kostele. Sance ze te buh ochrani je " << player.blessingChance << "%.\n";
                } else {
                    std::cout << "Buh te miluje.\n";
                }
                visitedChurch = true;  // tady je ta oprava
            }
            system("pause");
        } else if (mainChoice == 3) {
            while (true) {
                clearScreen();
                SetColor(10, 0);
                std::cout << "Zivoty: " << player.health << "/" << player.maxHealth << "\n";
                SetColor(1, 0);
                std::cout << "Energie: " << player.energy << "\n";
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
                    break; // zpet do vesnice
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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Character player = chooseClass();
    SetColor(10, 0); //zelena
    std::cout << R"(
  ____             _       _
 |_  /__ _ __ __ _| |_ ___| |__
  / // _` / _/ _` |  _/ -_) / /
 /___\__,_\__\__,_|\__\___|_\_\
                           )" << '\n';
SetColor(7, 0); //bila
    village(player);
//goblin
    Monster boj1[1] = {
        {"Goblin", 7 + rand() % 6, 1, 3},
    };
    fight(player, boj1, 1);

//nemrtvy
    Monster boj2[1] = {
        {"Nemrtvy", 9 + rand() % 4, 2, 3},
    };
    fight(player, boj2, 1);

//2 goblini
    Monster boj3[2] = {
        {"Goblin", 7 + rand() % 6, 1, 3},
        {"Maly goblin", 4 + rand() % 6, 1, 3}
    };
    fight(player, boj3, 2);
    SetColor(7, 0); //bila
    std::cout << "---pred tebou se zjevil mini boss!---\n";
    SetColor(10, 0); //zelena
    if(player.isBlind == false) printAsciiArt("MB1");
SetColor(7, 0); //bila
//MB sliz
    Monster MB1[1] = {
        {"Obrovsky sliz", 12 + rand() % 8, 3, 4},
    };
    fight(player, MB1, 1);
        village(player);
}