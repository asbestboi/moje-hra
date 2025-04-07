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
            std::cout << "Neplatny vstup, zadej cislo.\n";
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
            std::cout << "Neplatna volba, zkuste znovu.\n";
            continue;
        }

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
            return player;
        } else {
            std::cout << "Vyberte si classu znovu.\n";
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
    std::cout << "\n---Pred tebou stoji " << monsterCount << " nepratel!---\n";
    SetColor(7, 0); //bila
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
            return;
        }

        // Vypis zivych monster
        std::cout << "Ziva monstra:\n";
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
            std::cout << "Neplatny vstup, zadej cislo.\n";
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
                std::cout << "Vylecil ses o " << healAmount << " zivotu.\n\n";
                SetColor(10, 0);
                std::cout << "Zivoty: " << player.health << "\n";
                SetColor(1, 0);
                std::cout << "Energie: " << player.energy << "\n";
                SetColor(7, 0);
                continue;
            } else {
                SetColor(4, 0); //cervena
                std::cout << "Nemas dost energie na leceni!\n";
                SetColor(7, 0); //bila
                continue;
            }
        } else {
            SetColor(4, 0); //cervena
            std::cout << "Neplatna volba!\n";
            SetColor(7, 0); //bila
            continue;
        }

        // Kazde zive monstrum zautoci
        for (int i = 0; i < monsterCount; ++i) {
            if (monsters[i].health <= 0) continue;

            if (player.dodge && rand() % 100 < 30) {
                SetColor(4, 0); //cervena
                std::cout << (player.isBlind ? "nekdo" : monsters[i].name) << " minul!\n";
                SetColor(7, 0); //bila
                continue;
            }

            int damage = rand() % (monsters[i].maxAttack - monsters[i].minAttack + 1) + monsters[i].minAttack;
            player.health -= damage;
            SetColor(4, 0); //cervena
            std::cout << (player.isBlind ? "nekdo" : monsters[i].name) << " na tebe zautocil za " << damage << "!\n";
            SetColor(7, 0); //bila
        }
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
        SetColor(7, 0);
        if (player.health <= 0) {
            SetColor(4, 0); //cervena
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
            std::cin >> choice;
            exit(0);
        }
    }
}
void village(Character &player) {
    if (player.gamble == true) {
        player.gold = rand() % 101;
    }
    while (true) {
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
            std::cout << "Neplatny vstup, zadej cislo.\n";
            continue;
        }
        if (choice == 1) {
            if (player.gold >= 15) {
                player.gold -= 15;
                player.maxHealth += 5;
                player.health = player.maxHealth;
                std::cout << "Vylepsil jsi zivoty o 5. Mas " << player.maxHealth << " zivotu.\n";
            } else {
                std::cout << "Nemas dostatek zlata!\n";
            }
        } else if (choice == 2) {
            if (player.gold >= 20) {
                player.gold -= 20;
                player.maxEnergy += 5;
                player.energy = player.maxEnergy;
                std::cout << "Vylepsil jsi energii o 5. Mas " << player.maxEnergy << " energie.\n";
            } else {
                std::cout << "Nemas dostatek zlata!\n";
            }
        } else if (choice == 3) {
            if (player.gold >= 20) {
                player.gold -= 20;
                player.attack += 2;
                std::cout << "Vylepsil jsi utok o 2. Mas utok: " << player.attack << "\n";
            } else {
                std::cout << "Nemas dostatek zlata!\n";
            }
        } else if (choice == 4) {
            std::cout << "Opoustis vesnici.\n";
            break;
        } else {
            std::cout << "Neplatna volba. Zkus to znovu.\n";
        }
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    Character player = chooseClass();

    SetColor(10, 0);
    std::cout << "Zivoty: " << player.health << "\n";
    SetColor(5, 0);
    std::cout << "Utok: " << player.attack << "\n";
    SetColor(1, 0);
    std::cout << "Energie: " << player.energy << "\n";
    SetColor(6, 0);
    std::cout << "Zlato: " << player.gold << "\n";
    SetColor(7, 0);
    SetColor(10, 0); //zelena
    std::cout << R"(
  ____             _       _
 |_  /__ _ __ __ _| |_ ___| |__
  / // _` / _/ _` |  _/ -_) / /
 /___\__,_\__\__,_|\__\___|_\_\
                           )" << '\n';
SetColor(7, 0); //bila
    std::cout << "nachazis se ve vesnici.\n";

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
    pred tebou se zjevil mini boss!

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



        std::cout << "Ve vesnici sis vylecil vsechny zivoty a doplnil energii\n";
        player.health = player.maxHealth;
        player.energy = player.maxEnergy;
        village(player);
}