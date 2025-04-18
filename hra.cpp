#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <limits>
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
struct Character {
    std::string name;
    int maxHealth;
    int health;
    int attack;
    int energy;
    int maxEnergy;
    int gold;
    bool isBlind;
    bool isVegetarian;
    bool isUndead;
    bool gamble;
    bool dodge;
};

Character chooseClass() {
    int choice;
    Character player;
    while (true) {
        std::cout << "Vyber si classu:\n1 - Slepec\n2 - Vegetarian\n3 - Kostlivec\n4 - Gambler\n5 - Zlodej\n";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            continue;
        }
        if (choice == 1) {
            player = {"Slepec", 8, 8, 5, 4, 4, 25, true, false, false, false, false};
        } else if (choice == 2) {
            player = {"Vegetarian", 9, 9, 3, 6, 6, 25, false, true, false, false, false};
        } else if (choice == 3) {
            player = {"Kostlivec", 7, 7, 5, 5, 5, 25, false, false, true, false, false};
        } else if (choice == 4) {
            player = {"Gambler", 6, 6, 3, 5, 5, 0, false, false, false, true, false};
        } else if (choice == 5) {
            player = {"Zlodej", 5, 5, 4, 3, 3, 25, false, false, false, false, true};
        } else {
            clearScreen();
            continue;
        }
        clearScreen();
        std::cout << "Jste si jisti, ze chcete hrat za " << player.name << "? (y/n): \n";
        char confirmation;
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "\n";
        SetColor(5, 0);
        std::cout << "Utok: " << player.attack << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
        SetColor(6, 0);
        std::cout << "Zlato: " << player.gold << "\n";
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
            std::cout << "Vyhral jsi!\n";
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
        std::cout << "Chces zautocit (1), pouzit kouzlo (2) nebo leceni (3)? ";
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
                std::cout << "Na ktere monstrum utocis (1-" << aliveCount << ")? ";
                int pick;
                std::cin >> pick;
                pick--;
                if (pick < 0 || pick >= aliveCount) {
                    std::cout << "Neplatny cil!\n";
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
            std::cout << "Zautocil jsi na " << (player.isBlind ? "nekoho" : monsters[target].name)  << " za " << damage << " poskozeni!\n";
            if (player.energy < player.maxEnergy) player.energy++;
            SetColor(7, 0);
        } else if (choice == 2) {
            if (player.energy >= 3) {
                player.energy -= 3;
                int spellDamage = player.attack * 2;
                monsters[target].health -= spellDamage;
                SetColor(5, 0);
                std::cout << "Pouzil jsi kouzlo na " << (player.isBlind ? "nekoho" : monsters[target].name)  << " za " << spellDamage << " poskozeni!\n";
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
                int healAmount = player.isVegetarian ? 6 : 3;
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

            if (!player.dodge && rand() % 100 < 25 ||player.dodge && rand() % 100 < 45) {
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
            SetColor(4, 0); //cervena
            clearScreen();
            std::cout << R"(
                             ,--.
                            {    }
                            K,   }
                           /  ~Y`
                      ,   /   /
                     {_'-K.__/
                       `/-.__L._
                       /  ' /`\_}
                      /  ' /
              ____   /  ' /
       ,-'~~~~    ~~/  ' /_
     ,'             ``~~~  ',
    (                        Y
   {                         I
  {      -                    `,
  |       ',                   )
  |        |   ,..__      __. Y
  |    .,_./  Y ' / ^Y   J   )|
  \           |' /   |   |   ||
   \          L_/    . _ (_,.'(
    \,   ,      ^^""' / |      )
      \_  \          /,L]     /
        '-_~-,       ` `   ./`
           `'{_            )
    _________  ^^\..___,.--` __
   /   _____/ _____________/  |_
   \_____  \ /     \_  __ \   __\
   /        \  Y Y  \  | \/|  |
  /_______  /__|_|  /__|   |__|
          \/      \/            )" << '\n';
            SetColor(7, 0); //bila
            system("pause");
            exit(0);
        }
    }
}
void village(Character &player) {
    std::cout << R"(
pred tebou je vesnice.

                  MMM                                                           
                MMUUUMM                                                         
              MMUUUUUUUMMM                                                      
            MMUUUUUUUUUUUUMM                                                    
         MMMUUUUUUUUUUUUUUUUMMM                                                 
       MMUUUUUUUUUUUUUUUUUUUUUUMM                                               
     MMUUUUUUUUUUUUUUUUUUUUUUUUUUMMM                                            
   MM  XXXXXXXXXXXXXXXXXXXXXXXXXXXX MM                          MMMM            
      X___________________________X                           MMUUUUMM          
      X___________________________X                         MMUUUUUUUUMM        
      X___________________________X                       MMUUUUUUUUUUUUMM      
      X_____________OOOOOOOOOO____X                     MMXXXXXXXXXXXXXXXXMM    
      X_____________O________O____X                    M X_______________X  M   
      X_____________O________O____X                      X_______________X      
      X_____________O________O____X                      X____OOOOOOO____X      
      X_____________OOOOOOOOOO____X                      X____O_____O____X      
 M^M  M^M__M^M__M^M__M^M__M^M__M^MX M^M             M^M  M^M__M^M__M^M__M^M  M^M
=|^|==|^|==|^|==|^|==|^|==|^|==|^|==|^|   .  .....  |^|==|^|==|^|==|^|==|^|==|^|
 | |  | |  | |  | |  | |  | |  | |  | | ........... | |  | |  | |  | |  | |  | |
-|o|--|o|--|o|--|o|--|o|--|o|--|o|--|o| ........... |o|--|o|--|o|--|o|--|o|--|o|
 | |  | |  | |  | |  | |  | |  | |  | |.............| |  | |  | |  | |  | |  | |
=| |==| |==| |==| |==| |==| |==| |==| |.............| |==| |==| |==| |==| |==| |
                                       .............                            
                                      ..............                            
                                      ..............     
   )" << '\n';
   system("pause");

    if (player.gamble == true) {
        player.gold = rand() % 101;
    }
    while (true) {
        clearScreen();
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
        SetColor(5, 0);
        std::cout << "Utok: " << player.attack << "\n";
        SetColor(6, 0);
        std::cout << "Zlato: " << player.gold << "\n";
        SetColor(7, 0);
        std::cout << "1. Vylepsit zivoty o 5 (15 zlata)\n";
        std::cout << "2. Vylepsit energii o 5 (20 zlata)\n";
        std::cout << "3. Vylepsit utok o 2 (20 zlata)\n";
        std::cout << "4. Odejit z vesnice\n";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            continue;
        }
        if (choice == 1) {
            if (player.gold >= 15) {
                player.gold -= 15;
                player.maxHealth += 5;
                player.health = player.maxHealth;
                clearScreen();
            } else {
                std::cout << "Nemas dostatek zlata!\n";
                system("pause");
                clearScreen();
            }
        } else if (choice == 2) {
            if (player.gold >= 20) {
                player.gold -= 20;
                player.maxEnergy += 5;
                player.energy = player.maxEnergy;
                clearScreen();
            } else {
                std::cout << "Nemas dostatek zlata!\n";
                system("pause");
                clearScreen();
            }
        } else if (choice == 3) {
            if (player.gold >= 20) {
                player.gold -= 20;
                player.attack += 2;
                clearScreen();
            } else {
                std::cout << "Nemas dostatek zlata!\n";
                system("pause");
                clearScreen();
            }
        } else if (choice == 4) {
            clearScreen();
            std::cout << "Opustil jsi vesnici.\n";
            break;
            system("pause");
            clearScreen();
        } else {
            //neplatny vstup uz nechci ohlasovat, to se proste nedela >:(
            clearScreen();
        }
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
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
        {"Goblin", 8 + rand() % 6, 1, 3},
    };
    fight(player, boj1, 1);

//nemrtvy
    Monster boj2[1] = {
        {"Nemrtvy", 9 + rand() % 4, 2, 3},
    };
    fight(player, boj2, 1);

//2 goblini
    Monster boj3[2] = {
        {"Goblin", 8 + rand() % 6, 1, 3},
        {"Maly goblin", 5 + rand() % 6, 1, 3}
    };
    fight(player, boj3, 2);
    SetColor(10, 0); //zelena
    if(player.isBlind == false)std::cout << R"(
---pred tebou se zjevil mini boss!---

⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⠻⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡀⠈⠛⢿⣿⣿⣁⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⠟⠛⠛⠿⡄⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⡟⢡⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⠀⣀⣉⣤⣤⣤⡀⣤⣄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠻⣿⣦⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⣠⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⣽⣿⣷⣄⠀⠀⠀⠀
⠀⠀⣾⡿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣿⣿⣿⣿⣷⣄⠀⠀
⠀⢀⣼⡀⠀⠀⠈⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⣿⠃⠀
⠀⠘⠟⠁⠀⠀⠀⠀⢿⣿⣿⡿⠟⠋⠉⠉⠉⠙⢿⣿⣿⣿⡟⠁⠀⠀⢠⣿⡄⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⢿⣿⣿⡦⠀⠙⠻⠟⠁⠀⠀⠀⠈⠛⠃⠀)" << '\n';
SetColor(7, 0); //bila
//MB sliz
    Monster MB1[1] = {
        {"Obrovsky sliz", 12 + rand() % 8, 3, 4},
    };
    fight(player, MB1, 1);
        village(player);
}