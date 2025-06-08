#pragma once
#include "character.h"
#include "utility.h"
void generateBackstory(Character &player) {

    struct moznost {
        std::string text;
        std::function<void(Character&)> applyStat;
    };

    std::vector<moznost> childhood = {
        {"zabavne dite se kterym se nikdo nenudil.", [](Character& p){p.charisma += 5;}},
        {"problemove dite.", [](Character& p){p.attack += 1;}},
        {"hodne dite.", [](Character& p){ p.maxHealth += 1;}},
        {"samostatne dite.", [](Character& p){p.gold += 10;}},
        {"hloupe dite.", [](Character&){}}, //nepouzity parametr
        {"genialni a velice nadane dite, ve vsem jsi vynikal.", [](Character& p){ p.maxHealth += 2;p.maxEnergy += 2;}}
    };

    std::vector<moznost> lifePath = {
        {"venoval zahradniceni.", [](Character& p){p.health += 1;}},
        {"venoval zenam.", [](Character& p){p.charisma += 5;}},
        {"venoval obchodovani.", [](Character& p){p.gold += 15;}},
        {"venoval branenim sve materske vesnice.", [](Character& p){p.attack += 2;}},
        {"venoval bojovem jezdeni na koni.", [](Character& p){p.attack += 2;}},
        {"venoval sbiranim hub.", [](Character& p){p.health += 2;}},
        {"valel v posteli, bylo tezke se zvednout.", [](Character&){}},//nepouzity parametr
        {"venoval magii.", [](Character& p){p.maxEnergy += 1;}}
    };

    std::vector<moznost> reason = {
        {"Odesel jsi z domu, protoze mas hlad.", [](Character& p){ p.maxHealth += 1;}},
        {"Odesel jsi z domu, protoze jsi mel zly sen o tom jak nepratele napadnou tvoji vesnici", [](Character& p){p.maxEnergy += 2;p.maxHealth += 1;}},
        {"Odesel jsi z domu, protoze te boli bricho a potrebujes na zachod.", [](Character&){}},//nepouzity parametr
        {"Odesel jsi z domu aby ses vydal na vypravu.", [](Character& p){p.attack += 1;}},
        {"Odesel jsi z domu jelikoz... Uz nevis proc jsi odesel z domu.", [](Character& p){p.maxEnergy += 2;}},
        {"Odesel jsi, protoze se chces naucit carovat.", [](Character& p){p.energy += 1;}}
    };

    moznost c = childhood[rand() % childhood.size()];
    moznost l = lifePath[rand() % lifePath.size()];
    moznost r = reason[rand() % reason.size()];
    drawHeaderLine();
    SetColor(14, 0);
    printCentered("TVUJ PRIBEH");
    drawHeaderLine();
    SetColor(7, 0);
    std::cout << "(tohle bude mit dopad na tve staty)\n";
    Sleep(1200);
    std::cout << "Byl jsi " << c.text << "\n";
    Sleep(1000);
    std::cout << "Cely zivot ses " << l.text << "\n";
    Sleep(1000);
    std::cout << "Nakonec se z tebe stal " << player.name << " \n";
    Sleep(1000);
    std::cout << r.text << "\n";
    Sleep(1000);
    std::cout << "Opoustis svuj dum.\n";
    c.applyStat(player);
    l.applyStat(player);
    r.applyStat(player);

    waitForKeyPress();
    clearScreen();
}
