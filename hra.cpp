#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> //https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Barvicky
void SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) + textColor);
}

// Struktura pro postavu
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

// Funkce pro vytvoreni postavy
Character chooseClass() {
    int choice;
    Character player;
    while (true) {
        std::cout << "Vyber si classu:\n1 - Slepec\n2 - Vegetarian\n3 - Kostlivec\n4 - Gambler\n5 - Zlodej\n";
        std::cin >> choice;

        if (choice == 1) {
            player = {"Slepec", 8, 8, 6, 4, 4, 25, true, false, false, false, false};
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
SetColor(10, 0);//zelena
std::cout << "Zivoty: " << player.health << "\n";
SetColor(5, 0); //fialova
    std::cout << "Utok: " << player.attack << "\n";
SetColor(1, 0); //modra
    std::cout << "Energie: " << player.energy << "\n";
SetColor(6, 0); //zluta
    std::cout << "Zlato: " << player.gold << "\n";
SetColor(7, 0); //bila
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y') {
            return player;
        } else {
            std::cout << "Vyberte si classu znovu.\n";
        }
    }
}

// Struktura pro monstra
struct Monster {
    std::string name;
    int health;
    int minAttack;
    int maxAttack;
};

// Funkce pro boj s monstry
void fight(Character &player, Monster monster) {
    SetColor(0, 4);//cervene
    std::cout << "Pred tebou se zjevil " << (player.isBlind ? "nekdo" : monster.name) << "!\n";
    SetColor(7, 0);//bila
    while (player.health > 0 && monster.health > 0) {

        if (!player.isBlind) {
            std::cout << monster.name << " ma " << monster.health << " zivotu.\n";
        }

        std::cout << "Chces zautocit (1), pouzit kouzlo (2) nebo leceni (3)? ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int damage = player.attack;
            if (player.isUndead) {
                damage *= 1.2;
            }
            monster.health -= damage;
            SetColor(5, 0); //fialova
            std::cout << "Zautocil jsi za " << damage << " poskozeni!\n";
            SetColor(7, 0); //bila
        } else if (choice == 2) {
            if (player.energy >= 3) {
                player.energy -= 3;
                int spellDamage = player.attack * 2;
                monster.health -= spellDamage;
                SetColor(1, 0); //modra
                std::cout << "Pouzil jsi magicky utok ktery udelal " << spellDamage << " poskozeni!\n";
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
                if (player.isUndead) {
                    healAmount /= 2;
                }
                player.health = std::min(player.maxHealth, player.health + healAmount);
                std::cout << "Vylecil ses o " << healAmount << " zivotu.\n";
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

        if (monster.health > 0) {
            if (player.dodge && rand() % 100 < 30) {
                SetColor(4, 0); //červená
                std::cout << (player.isBlind ? "nekdo" : monster.name) << " minul!\n";
                SetColor(7, 0); //bílá
            } else {
                int monsterDamage = rand() % (monster.maxAttack - monster.minAttack + 1) + monster.minAttack;
                player.health -= monsterDamage;
                SetColor(4, 0); //červená
                std::cout << (player.isBlind ? "nekdo" : monster.name) << " na tebe zautocil za " << monsterDamage << "!\n";
                SetColor(1, 0); //modrá
                std::cout << "Energie: " << player.energy << "\n";
                SetColor(10, 0); //zelená
                std::cout << "Tve zivoty: " << player.health << "\n";
                SetColor(7, 0); //bílá
            }
        }

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
            return;
        }
    }

    if (player.health > 0) {
        std::cout << "Porazil jsi " << (player.isBlind ? "nekoho" : monster.name) << "!\n";
        if (rand() % 100 < 50) {

        player.gold += rand() % 20 + 1;
        std::cout << "Ziskal jsi zlato. Mas " << player.gold << " zlata.\n";
    }
    }
}
void village(Character &player) {
    if (player.gamble == true) {
        player.gold = rand() % 101;
    }
    while (true) {
        SetColor(6, 0); //zluta
    std::cout << "Zlato: " << player.gold << "\n";
        SetColor(7, 0); //bila
        std::cout << "1. Vylepsit zivoty o 5 (15 zlata)\n";
        std::cout << "2. Vylepsit energii o 5 (20 zlata)\n";
        std::cout << "3. Vylepsit utok o 2 (20 zlata)\n";
        std::cout << "4. Odejit z vesnice\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            if (player.gold >= 15) {
                player.gold -= 15;
                player.maxHealth += 5;
                player.health = player.maxHealth;
                std::cout << "Vylepsil jsi zivoty o 5. Mas " << player.maxHealth << " zivotu.\n";
            } else {
                std::cout << "Nemas dostatek zlata na vylepseni zivotu!\n";
            }
        } else if (choice == 2) {
            if (player.gold >= 20) {
                player.gold -= 20;
                player.maxEnergy += 5;
                player.energy = player.maxEnergy;
                player.energy = std::min(player.maxEnergy, player.energy + 3);
                std::cout << "Vylepsil jsi energii o 5. Mas " << player.maxEnergy << " energie.\n";
            } else {
                std::cout << "Nemas dostatek zlata na vylepseni energie!\n";
            }
        } else if (choice == 3) {
            if (player.gold >= 20) {
                player.gold -= 20;
                player.attack += 2;
                std::cout << "Vylepsil jsi utok o 2. Mas utok: " << player.attack << "\n";
            } else {
                std::cout << "Nemas dostatek zlata na vylepseni utoku!\n";
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

    std::cout << "\nVybral sis classu: " << player.name << "\n";
SetColor(10, 0); //zelena
std::cout << "Zivoty: " << player.health << "\n";
SetColor(5, 0); //fialova
    std::cout << "Utok: " << player.attack << "\n";
SetColor(1, 0); //modra
    std::cout << "Energie: " << player.energy << "\n";
SetColor(6, 0); //zluta
    std::cout << "Zlato: " << player.gold << "\n";
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
    Monster goblin = {"Goblin", 10 + rand() % 6, 1, player.health/3};
    fight(player, goblin);
    fight(player, goblin);
    std::cout << "Ve vesnici sis vylecil vsechny zivoty\n";
village(player);
}