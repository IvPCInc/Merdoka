#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <Windows.h>

using namespace std;

struct Sword {
    string name;
    int damage;
};

struct Armor {
    string name;
    int defense;
};

struct Buy {
    int money;
};

struct PlayerStat {
    string name;
    int health;
    int money;
    int experience;
    int strength;
    Sword sword;
    Armor armor;
    Buy res;
};

struct Enemy {
    string name;
    int health;
    int strength;
    int defense;
};

void initGoblin(Enemy& goblin) {
    goblin.name = "Гоблин";
    goblin.health = 100;
    goblin.strength = 25;
    goblin.defense = 2;
}

void initTroll(Enemy& troll) {
    troll.name = "Троль";
    troll.health = 250;
    troll.strength = 25;
    troll.defense = 10; 
}

void DisplayStat(PlayerStat& charStat) {
    cout << "Имя: " << charStat.name << endl;
    cout << "Здоровье: " << charStat.health << endl;
    cout << "Сила: " << charStat.strength << endl;
    cout << "Защита: " << charStat.armor.defense << endl;
    cout << "Деньги: " << charStat.money << endl;
    cout << "Опыт: " << charStat.experience << endl;
}

void levelUpStat(PlayerStat& charStats, string statName, int amount) {
    if (statName == "health") {
        charStats.health += amount;
    }
    else if (statName == "strength") {
        charStats.strength += amount;
    }
    else if (statName == "experience") {
        charStats.experience += amount;
    }
    else {
        cout << "Invalid stat name!" << endl;
    }
}

void playerAttack(Enemy& enemy, PlayerStat& player) {
    int damage = (player.strength + rand() % 20) - enemy.defense;
    if (damage < 0) {
        damage = 0;
    }
    enemy.health -= damage;
    cout << "Вы нанесли " << damage << " урона!" << endl;
    cout << "У врага осталось " << enemy.health << " здоровья.\n" << endl;
}

void enemyAttack(PlayerStat& player, Enemy& enemy) {
    int damage = enemy.strength - player.armor.defense;
    if (damage <= 0) {
        damage = 0;
        if (player.armor.defense > 0) {
            player.armor.defense -= rand() % 3;
        }
    }
    player.health -= damage;
    cout << "Враг нанес вам " << damage << " урона!" << endl;
    cout << "У вас осталось " << player.health << " здоровья.\n" << endl;
}

void fightGoblin(PlayerStat& player, Enemy& goblin) {
    cout << "\nВы встретили гоблина! Начинается бой!\n";

    while (player.health > 0 && goblin.health > 0) {
        cout << "Нажмите 'A', чтобы атаковать, или 'E', чтобы убежать.\n";
        char action = _getch();

        if (action == 'A' || action == 'a') {
            playerAttack(goblin, player);

            if (goblin.health > 0) {
                enemyAttack(player, goblin);
            }
        }
        else if (action == 'E' || action == 'e') {
            cout << "Вы сбежали с боя!" << endl;
            return;
        }
        else {
            cout << "Неверное действие!" << endl;
        }
    }

    if (player.health > 0) {
        cout << "\nВы победили гоблина!\n" << endl;
        player.experience += 10;
        player.money += rand() % 20 + 10;
        player.health += 5;
    }
    else {
        cout << "\nВы проиграли бой...\n" << endl;
    }
}

void fightTroll(PlayerStat& player, Enemy& troll) {
    cout << "\nВы встретили троля! Начинается бой!\n";

    while (player.health > 0 && troll.health > 0) {
        cout << "Нажмите 'A', чтобы атаковать, или 'E', чтобы убежать.\n";
        char action = _getch();

        if (action == 'A' || action == 'a') {
            playerAttack(troll, player);

            if (troll.health > 0) {
                enemyAttack(player, troll);
            }
        }
        else if (action == 'E' || action == 'e') {
            cout << "Вы сбежали с боя!" << endl;
            return;
        }
        else {
            cout << "Неверное действие!" << endl;
        }
    }

    if (player.health > 0) {
        cout << "\nВы победили троля!\n" << endl;
        player.experience += 20;
        player.money += rand() % 30 + 10;
        player.health += 10;
    }
    else {
        cout << "\nВы проиграли бой...\n" << endl;
    }
}


void saveStats(PlayerStat& player) {
    ofstream outfile("player_stats.txt");
    if (outfile.is_open()) {
        outfile << player.name << endl;
        outfile << player.health << endl;
        outfile << player.strength << endl;
        outfile << player.armor.defense << endl;
        outfile << player.money << endl;
        outfile << player.experience << endl;
        outfile << player.res.money << endl;
        outfile.close();
    }
    else {
        cout << "Ошибка при открытии файла для сохранения." << endl;
    }
}

void loadStats(PlayerStat& player, Buy& shop) {
    ifstream infile("player_stats.txt");
    if (infile.is_open()) {
        getline(infile, player.name);
        infile >> player.health;
        infile >> player.strength;
        infile >> player.armor.defense;
        infile >> player.money;
        infile >> player.experience;
        infile.close();
    }
    else {
        cout << "Файл player_stats.txt не найден. Используются стандартные настройки." << endl;
    }
}

void playerInventory(PlayerStat& player) {
    player.sword.name = "Меч Бога";
    int damage = 200;
    player.strength += 200;
    cout << "Инвентарь игрока:" << endl;
    cout << "1. Меч: " << player.sword.name << " (Урон: " << damage << ")" << endl;
    cout << "2. Броня: " << player.armor.name << " (Защита: " << player.armor.defense << ")" << endl;
}

void playerClad(PlayerStat& player) {
    srand(time(0));
    int coal = rand() % 30;
    int iron = rand() % 20;
    int gold = rand() % 10;
    int diamond = rand() % 5;

    int foundArmor = rand() % 15;
    if (foundArmor == 5) {
        player.armor.name = "Щит";
        player.armor.defense = 50;
        cout << "Поздравляем! Вы нашли броню: " << player.armor.name << " (Защита: " << player.armor.defense << ")\n";
    }
    else {
        cout << "К сожалению, вы не нашли броню в этот раз.\n";
    }

    int delayInSeconds = rand() % 120;
    cout << " ( " << delayInSeconds << " ) секунд ";
    for (int i = 0; i < delayInSeconds; i++) {
        cout << ".";
        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << std::endl << "Вы добыли " << coal << " - угля, " << iron << " - железа, " << gold << " - золота, " << diamond << " - алмазов." << endl;
    player.res.money += (coal * 10) + (iron * 25) + (gold * 1000) + (diamond * 15000);
    cout << "\n";
    cout << "Теперь у вас " << player.res.money << " денег от продажи ресурсов." << endl; 
    cout << player.health + 10 << " - Ваше здоровье восстановлено на 10";
    cout << "\n";
}

void sellResources(PlayerStat& player) {
    if (player.res.money > 0) {
        int price = player.res.money; 
        cout << "Вы продали ресурсы за " << price << " денег." << endl;
        player.money += price;
        player.res.money = 0;
    }
    else {
        cout << "У вас нет ресурсов для продажи." << endl;
    }
}

void GameShop(PlayerStat& shop) {
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_BLUE );
    cout << "\nВы зашли в лавку!" << endl;
    cout << "  ____||____  \n ///////////\\ \n///////////  \\\n|    _    |  |\n|[] | | []|[]|\n|   | |   |  |";
        
    long playerbuy;
    cout << "\n\n(1) Зелье регенерации (100 ХП) = 45000 МР\n(2) Зелье силы (50 ЕД.) = 60000 МР\n(3) Зелье опыта (100 ЕД.) = 100000 МР\nВвод: ";
    cin >> playerbuy;

    if (playerbuy == 1 && shop.money >= 45000) {
        shop.money -= 45000;
        shop.health += 100;
        cout << "Вы купили Зелье регенерации!" << endl;
    }
    else if (playerbuy == 2 && shop.money >= 60000) {
        shop.money -= 60000;
        shop.strength += 50;
        cout << "Вы купили Зелье силы!" << endl;
    }
    else if (playerbuy == 3 && shop.money >= 100000) {
        shop.money -= 100000;
        shop.experience += 100;
        cout << "Вы купили Зелье опыта!" << endl;
    }
    else {
        cout << "Недостаточно денег!" << endl;
    }
    cout << "n";
    system("color 0F");
}

void StatsFile(PlayerStat& player) {
    ifstream file("player_stats.txt");
    cout << "-----------------------------------------------------------------------------------------------------\n";
    cout << "                                       ▄▄                             \n▀████▄     ▄███▀                     ▀███          ▀███               \n  ████    ████                         ██            ██               \n  █ ██   ▄█ ██   ▄▄█▀██▀███▄███   ▄█▀▀███   ▄██▀██▄  ██  ▄██▀ ▄█▀██▄  \n  █  ██  █▀ ██  ▄█▀   ██ ██▀ ▀▀ ▄██    ██  ██▀   ▀██ ██ ▄█   ██   ██  \n  █  ██▄█▀  ██  ██▀▀▀▀▀▀ ██     ███    ██  ██     ██ ██▄██    ▄█████  \n  █  ▀██▀   ██  ██▄    ▄ ██     ▀██    ██  ██▄   ▄██ ██ ▀██▄ ██   ██  \n▄███▄ ▀▀  ▄████▄ ▀█████▀████▄    ▀████▀███▄ ▀█████▀▄████▄ ██▄▄████▀██▄\n\n";
    if (file.is_open()) {
        string line1, line2, line3, line4, line5, line6;

        getline(file, line1);
        getline(file, line2);
        getline(file, line3);
        getline(file, line4);
        getline(file, line5);
        getline(file, line6);

        cout << "Имя: " << line1 << endl;
        cout << "Здоровье: " << line2 << endl;
        cout << "Сила: " << line3 << endl;
        cout << "Защита: " << line4 << endl;
        cout << "Деньги: " << line5 << endl;
        cout << "Опыт: " << line6 << endl;

        file.close();
    }
    else {
        cout << "Ошибка открытия файла." << endl;
    }
    cout << "-----------------------------------------------------------------------------------------------------\n";
    cout << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    int user, random_u;
    srand(static_cast<unsigned int>(time(0)));

    PlayerStat myCharacter;
    Buy shop;
    myCharacter.res.money = 0;
    loadStats(myCharacter, shop);

    cout << "\n\n";
    if (myCharacter.name.empty()) {
        myCharacter.name = "John";
        myCharacter.health = 100;
        myCharacter.money = 1000;
        myCharacter.strength = 10 + myCharacter.sword.damage;
        myCharacter.sword.name = "Меч";
        myCharacter.sword.damage = 5;
        myCharacter.experience = 0;
        myCharacter.armor.name = "";
        myCharacter.armor.defense = 0;
    }

    DisplayStat(myCharacter);
    cout << "\n";

    Enemy goblin, troll;

    while (true) {
        cout << "\n1. Поиск ресурсов\n2. Бой\n3. Посмотреть статистику\n4. Инвентарь\n5. Продавать ресурсы\n6. Магазин\n7. Выход\nВвод : ";
        cin >> user;
        cout << "\n";

        if (user == 1) {
            playerClad(myCharacter);
            saveStats(myCharacter);
        }
        else if (user == 2) {
            random_u = rand() % 2;
            if (random_u == 0) { 
                initGoblin(goblin);
                fightGoblin(myCharacter, goblin);
                saveStats(myCharacter);
            }
            else {
                initTroll(troll);
                fightTroll(myCharacter, troll);
                saveStats(myCharacter);
            }
        }
        else if (user == 3) {
            StatsFile(myCharacter);
        }
        else if (user == 4) {
            playerInventory(myCharacter);
        }
        else if (user == 5) {
            sellResources(myCharacter);
            saveStats(myCharacter);
        }
        else if (user == 6) {
            GameShop(myCharacter);
        }
        else if (user == 7) {
            break;
        }
        else {
            cout << "Неверный выбор." << endl;
        }
    }

    return 0;
}
