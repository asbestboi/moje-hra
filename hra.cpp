#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#include <windows.h>

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
};

// Funkce pro vytvoreni postavy
Character chooseClass() {
    int choice;
    Character player;
    while (true) {
        std::cout << "Vyber si classu:\n1 - Slepec\n2 - Vegetarian\n3 - Kostlivec\n4 - Gambler\n";
        std::cin >> choice;

        if (choice == 1) {
            player = {"Slepec", 8, 8, 6, 4, 4, 0, true, false, false};
        } else if (choice == 2) {
            player = {"Vegetarian", 9, 9, 3, 6, 6, 0, false, true, false};
        } else if (choice == 3) {
            player = {"Kostlivec", 7, 7, 5, 5, 5, 0, false, false, true};
        } else if (choice == 4) {
            player = {"Gambler", 6, 6, 3, 5, 5, rand() % 101, false, false, false};
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
    std::cout << "Pred tebou se zjevil " << (player.isBlind ? "nekdo" : monster.name) << "!\n";
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
                std::cout << "Nemas dost energie na kouzlo!\n";
            }
        } else if (choice == 3) {
            if (player.energy >= 2) {
                player.energy -= 2;
                int healAmount = player.isVegetarian ? 6 : 3;
                player.health = std::min(player.maxHealth, player.health + healAmount);
                std::cout << "Vylecil ses o " << healAmount << " zivotu.\n";
            } else {
                std::cout << "Nemas dost energie na leceni!\n";
            }
        } else {
            SetColor(4, 0); //cervena
            std::cout << "Neplatna volba!\n";
            SetColor(7, 0); //bila
            continue;
        }

        if (monster.health > 0) {
            int monsterDamage = rand() % (monster.maxAttack - monster.minAttack + 1) + monster.minAttack;
            player.health -= monsterDamage;
            SetColor(4, 0); //cervena
            std::cout << monster.name << " na tebe zautocil za " << monsterDamage << "!\n";
            SetColor(1, 0); //modra
            std::cout << "Energie: " << player.energy << "\n";
            SetColor(10, 0); //zelena
        std::cout << "Tve zivoty: " << player.health << "\n";
            SetColor(7, 0); //bila
        }

        if (player.health <= 0) {
            SetColor(4, 6); //velice viditelne
            std::cout << "Zemrel jsi! Konec hry.";
            SetColor(7, 0); //bila
            return;
        }
    }

    if (player.health > 0) {
        std::cout << "Porazil jsi " << (player.isBlind ? "nekoho" : monster.name) << "!\n";
        
        player.gold += rand() % 20 + 1;
        std::cout << "Ziskal jsi zlato. Mas " << player.gold << " zlata.\n";
    }
}

int main() {
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
SetColor(7, 0); //bila

    Monster goblin = {"Goblin", 5, 1, player.health/3};
    fight(player, goblin);

    return 0;
}