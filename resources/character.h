#pragma once
#include <string>
struct Character {
    std::string name;
    int maxHealth;
    int health;
    int attack;
    int energy;
    int maxEnergy;
    int gold;
    int blessingChance;
    int charisma;
    bool isBlind;
    bool vampire;
    bool gamble;
    bool dodge;
    int xp = 0;
    int lvl = 1;
    int infectionStacks = 0;
    int roundsSinceLastHeal = 0;
    bool mercy = true;
    std::vector<std::string> inventory;
};

Character chooseClass();
