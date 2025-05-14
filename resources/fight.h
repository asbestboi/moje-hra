#pragma once
#include <string>
#include "utility.h"
#include "character.h"

struct Monster {
    std::string name;
    int health;
    int minAttack;
    int maxAttack;
    int isBoss = false;
};

extern const char* attackPhrases[];
extern const char* spellPhrases[];

void fight(Character& player, Monster monsters[], int monsterCount);
