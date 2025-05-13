#pragma once
#include <string>
#include "utility.h"
#include "character.h"  // Odkazujeme relativně, protože fight.h je v resources/

struct Monster {
    std::string name;
    int health;
    int minAttack;
    int maxAttack;
};

extern const char* attackPhrases[];
extern const char* spellPhrases[];

void fight(Character& player, Monster monsters[], int monsterCount);
