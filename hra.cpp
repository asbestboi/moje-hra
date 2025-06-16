#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <limits>
#include <fstream>
#include <vector>
#include <functional>
#include <ctime>
#include <conio.h>
#include "resources/barvy.h"
#include "resources/character.h"
#include "resources/backstory.h"
#include "resources/utility.h"
#include "resources/fight.cpp"
#include "resources/village.h"
//https://stackoverflow.com/questions/24776262/pause-console-in-c-program
//https://www.w3schools.com/cpp/ref_fstream_fstream.asp
//https://cplusplus.com/reference/fstream/ofstream/
Character chooseClass() {
    int choice;
    Character player;
    while (true) {
        clearScreen();
        drawHeaderLine();
        SetColor(6, 0);
        printCentered("VYBER CLASS");
        drawHeaderLine();
        SetColor(7, 0);
        std::cout << "[1] Slepec    -> Nemuze videt nepratele ani jejich zivoty ale je silnejsi\n";
        std::cout << "[2] Mnich     -> Hned na zacatku hry sance ze ho buh spasi je 100%\n";
        std::cout << "[3] Upir      -> Kdyz nekoho zabije normalnim utokem tak si vyleci ctvrtinu zivotu\n";
        std::cout << "[4] Gambler   -> Kdyz vejde do vesnice tak ma nahodny pocet penez az 200\n";
        std::cout << "[5] Zlodej    -> Lepe se vyhyba utokum a ma vetsi sanci na ziskani penez\n";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            continue;
        }
        if (choice == 1) {
            player = {"slepec", 8, 8, 5, 6, 6, 25, 20, 20, true, false, false, false};
        } else if (choice == 2) {
            player = {"mnich", 6, 6, 3, 6, 6, 25, 100, 35, false, false, false, false};
        } else if (choice == 3) {
            player = {"upir", 9, 9, 6, 5, 5, 25, 0, 25, false, true, false, false};
        } else if (choice == 4) {
            player = {"gambler", 6, 6, 3, 5, 5, 0, 0, 35, false, false, true, false};
        } else if (choice == 5) {
            player = {"zlodej", 6, 6, 4, 3, 3, 25, 0, 30, false, false, false, true};
        } else {
            clearScreen();
            continue;
        }
        clearScreen();
        drawHeaderLine();
        SetColor(6, 0);
        printCentered("VYBER CLASS");
        drawHeaderLine();
        std::cout << "(Tyto staty se navysi az se dozvis svuj pribeh)\n";
        char confirmation;
        SetColor(10, 0);
        std::cout << "Zivoty: " << player.health << "\n";
        SetColor(5, 0);
        std::cout << "Utok: " << player.attack << "\n";
        SetColor(1, 0);
        std::cout << "Energie: " << player.energy << "\n";
        SetColor(6, 0);
        std::cout << "Zlato: "; if (player.gamble) std::cout << "nahodne"; else std::cout << player.gold; std::cout << "\n";
        SetColor(14, 0);
        std::cout << "Charisma: " << player.charisma << "\n";
        SetColor(7, 0);
        std::cout << "Opravdu chces byt " << player.name << "? [y/n]: ";
        std::cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y') {
            clearScreen();
            return player;
        } else {
            clearScreen();
        }
    }
}
int main() {
    PlaySound("resources/choose.wav", NULL, SND_LOOP | SND_ASYNC);
    srand(time(0));
    //std::cin.get();
    Character player = chooseClass();
    generateBackstory(player);
    SetColor(10, 0); //zelena
    player.health = player.maxHealth;
    player.energy = player.maxEnergy;
    std::cout << R"(
  ____             _       _
 |_  /__ _ __ __ _| |_ ___| |__
  / // _` / _/ _` |  _/ -_) / /
 /___\__,_\__\__,_|\__\___|_\_\
                           )" << '\n';
    waitForKeyPress();
    clearScreen();
    PlaySound(NULL, NULL, 0);
    SetColor(7, 0); //bila
    village(player);
//goblin
    Monster boj1[1] = {
        {"Goblin", 8 + rand() % 6, 1, 3},
    };
    fight(player, boj1, 1);

//nemrtvy
    Monster boj2[1] = {
        {"Nemrtvy", 9 + rand() % 4, 2, 4},
    };
    fight(player, boj2, 1);

//2 goblini
    Monster boj3[2] = {
        {"Goblin", 8 + rand() % 6, 1, 3},
        {"Maly goblin", 4 + rand() % 6, 1, 3}
    };
    fight(player, boj3, 2);
    SetColor(7, 0); //bila
    std::cout << "---pred tebou se zjevil mini boss!---\n";
    Sleep(1000);
    SetColor(10, 0); //zelena
    if(player.isBlind == false) printAsciiArt("MB1");
    Sleep(500);
    waitForKeyPress();
//MB sliz
    Monster MB1[1] = {
        {"Obrovsky sliz", 25 + rand() % 8, 3, 4, true},
    };
    fight(player, MB1, 1);
        village(player);
    int choice = 0;
while (true) {
    clearScreen();
    PlaySound("resources/sounds/appear.wav", NULL, SND_FILENAME | SND_ASYNC);
    std::cout << (player.isBlind ? "vis ze pred tebou jsou dve cesty, nevis kam vedou\n" : "pred tebou se nachazi 2 cesty ta prvni je zarostla a vede do kopce, ta druha vede z kopce do tmy\n");
    Sleep(1000);
    std::cout << "[1] do kopce\n";
    std::cout << "[2] z kopce\n";
    Sleep(200);
    std::cout << "Vyber 1 nebo 2: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        clearScreen();
        continue;
    }

    if (choice == 1) {
        clearScreen();
        std::cout << "nachazis se ve VINES.\n";
        Sleep(1000);
        std::cout << "citis vuni prirody a " << (player.isBlind ? "slysis brouky.\n" : "vidis brouky.\n");
        Sleep(1000);
        waitForKeyPress();
        Monster boj4[1] = {
            {"Kenku", 18 + rand() % 6, 2, 6},
        };
        fight(player, boj4, 1);
        Monster boj5[3] = {
            {"Brouk", 3 + rand() % 8, 1, 3},
            {"Brouk", 3 + rand() % 8, 1, 3},
            {"Brouk", 3 + rand() % 8, 1, 3},
        };
        fight(player, boj5, 3);
                Monster boj6[3] = {
        {"Zivy listnaty strom", 15 + rand() % 8, 4, 5},
        {"Zivy listnaty strom", 15 + rand() % 8, 4, 5},
        {"Zivy jehlicnaty strom", 20 + rand() % 8, 4, 5},
        };
        fight(player, boj6, 3);
        break;
    } else if (choice == 2) {
        clearScreen();
        std::cout << "nachazis se v UNDERDARKU.\n";
        Sleep(1000);
        std::cout << "citis smrad\n";
        Sleep(1000);
        waitForKeyPress();
        Monster boj4[1] = {
            {"Grimlock", 18 + rand() % 8, 3, 6},
        };
        fight(player, boj4, 1);
        Monster boj5[3] = {
            {"Pavouk", 3 + rand() % 6, 2, 3},
            {"Pavouk", 3 + rand() % 6, 2, 3},
            {"Pavouk", 3 + rand() % 6, 2, 3},
        };
        fight(player, boj5, 3);
        Monster boj6[3] = {
        {"Temny elf", 15 + rand() % 8, 3, 5},
        {"Temny elf", 15 + rand() % 8, 3, 5},
        {"Temny mag", 20 + rand() % 8, 4, 7},
        };
        fight(player, boj6, 3);
        break;
    } else {
        clearScreen();
    }
}
        village(player);
        Monster boj7[1] = {
        {"Temny Executioner", 20 + rand() % 8, 3, 5},
        };
        fight(player, boj7, 1);
        while (true) {
    clearScreen();
    PlaySound("resources/sounds/appear.wav", NULL, SND_FILENAME | SND_ASYNC);
    std::cout << "Pred tebou ruzne domy, co udelas?\n";
    Sleep(1000);
    std::cout << "[1] prohledas ty domy a zjistis o co jde.\n";
    std::cout << "[2] vis ze tam jsou nejake budovy ale jdes dal.\n";
    Sleep(1000);
    std::cout << "Vyber 1 nebo 2: ";
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        clearScreen();
        continue;
    }

    if (choice == 1) {
        clearScreen();
        //special fight
        Monster Chlapec[1] = {
            {"Chlapec", 15 + rand() % 10, 6, 9, true},
        };
        fight(player, Chlapec, 1);
        Sleep(1000);
        break;
    } else if (choice == 2) {
        clearScreen();
        std::cout << "Jdes dal a jses stastny.\n";
        Sleep(1000);
        waitForKeyPress();
        break;
    } else {
        clearScreen();
    }
}
        Monster boj8[3] = {
            {"Bandita", 25 + rand() % 3, 4, 6},
            {"Banditka", 25 + rand() % 3, 4, 6},
            {"Bandita s paskou pres oko", 25 + rand() % 8, 5, 8},
        };
        fight(player, boj8, 3);
        village(player);
if (choice == 1) {
        clearScreen();
        //check na milost
        std::cout << "Potkal jsi druheho chlapce ktery vypada podobne jak ten minuly!\n";
        if (player.mercy == true) {
            waitForKeyPress();
            clearScreen();
            std::cout << "dekuji moc ze jsi nezabil meho bratra, byl jen vystraseny a proto utocil. Tady mas! (hodil ti sacek zlataku a prastary svitek)\n";
            player.inventory.push_back("Prastary svitek");
            player.gold += 200;
            waitForKeyPress();
        } else if (player.mercy == false) {
            waitForKeyPress();
            clearScreen();
            std::cout << "ty...\n";
            waitForKeyPress();
            std::cout << "zabil jsi meho bratra...\n";
            waitForKeyPress();
            std::cout << "TED BUDES TRPET. POMSTIM HO!!!\n";
            waitForKeyPress();
            clearScreen();
            if(player.isBlind == false) printAsciiArt("chlapec");
            waitForKeyPress();
                Monster Chlapec[2] = {
            {"Silny Chlapec", 70 + rand() % 5, 10, 15, true},
            };
            fight(player, Chlapec, 1);
            std::cout << "nasel jsi hromadu zlataku ktere chlapec mel u sebe.\n";
            player.gold += 400;
            waitForKeyPress();
        }
    } else if (choice == 2) {
        //nic
    }
    village(player);
    Monster boj9[2] = {
        {"Posedla lebka", 25 + rand() % 3, 4, 5},
        {"Posedla lebka", 25 + rand() % 3, 4, 5},
    };
    fight(player, boj9, 2);
    std::cout << "---pred tebou se zjevil mini boss!---\n";
    Sleep(1000);
    if(player.isBlind == false) printAsciiArt("MB2");
    Sleep(500);
    waitForKeyPress();
    Monster MB2[2] = {
        {"Zly Rytir", 50 + rand() % 10, 8, 25, true},
        {"Drak", 100 + rand() % 10, 5, 15},
    };
    fight(player, MB2, 2);
    std::cout << "nasel jsi mesec zlataku ktere rytir mel u sebe.\n";
    player.gold += 150;
    if(player.isBlind == false) std::cout << "V dalce vidis vez. Rytir odtamtud asi priletel. Pobliz je take vesnice\n";
    waitForKeyPress();
    village(player);
    std::cout << "vchazis do veze...\n";
    Monster boj10[2] = {
        {"Prizrak s mecem", 40 + rand() % 5, 4, 5},
        {"Prizrak s kopim", 30 + rand() % 5, 3, 8},
    };
    fight(player, boj10, 2);
    Monster boj11[4] = {
        {"Nacelnik", 30 + rand() % 5, 4, 5},
        {"Berserker", 16 + rand() % 5, 3, 8},
        {"Berserker", 16 + rand() % 5, 3, 8},
        {"Berserker s lebkou na hlave", 24 + rand() % 5, 3, 8},
    };
    fight(player, boj11, 4);
    std::cout << "Dostanes se na vrchol veze.\n";
    Sleep(1000);
    std::cout << "Je ti nevolno a obloha je cerna (pociti i slepec)\n";
    waitForKeyPress();
    clearScreen();
    printAsciiArt("HB1");
    Sleep(1000);
    std::cout << "HNUSNA PRISERA JE PRED TEBOU\n";
    Sleep(1000);
    std::cout << "VIS KDO TO JE. VYPRAVELO SE O NEM DLOUHOU DOBU\n";
    Sleep(1000);
    std::cout << "Vsechny tve nemoce, uzkosti a bolest je ted pred tebou, ve forme toho tvora...\n";
    Sleep(1000);
    Monster HB1[1] = {
        {"Plagueville", 400 + rand() % 10, 4, 10, true},
    };
    fight(player, HB1, 1);
    PlaySound("resources/ending.wav", NULL, SND_LOOP | SND_ASYNC);
    std::cout << "PRISERA PADA K ZEMI...\n";
    Sleep(1000);
    std::cout << "JE STALE NA ZIVU?\n";
    Sleep(1000);
    std::cout << "NE...\n";
    Sleep(1000);
    std::cout << "PRISERA PADA K ZEMI\n";
    Sleep(1000);
    std::cout << "PRISERA PADA Z VEZE!\n";
    Sleep(1000);
    if(player.isBlind == true) std::cout << "Jako slepec... ZACINAS VIDET\n";
    Sleep(1000);
    std::cout << "Vidis jak se obloha meni. Vse je krasne\n";
    if(player.vampire == true) std::cout << "tve zuby se vrati do normalu... UZ NEJSI UPIR\n";
    waitForKeyPress();
    std::cout << "Konec...\n";
    waitForKeyPress();
}
