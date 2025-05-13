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
};

Character chooseClass();
