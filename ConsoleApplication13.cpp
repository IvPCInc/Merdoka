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

enum CharacterClass {
    WARRIOR,
    MAGE,
    SHAMAN
};

struct Sword {
    string name;
    int damage;
};

struct Armor {
    string name;
    int defense;
};

struct Buy {
    int money = 0; 
};

struct PlayerStat {
    string name;
    int health;
    int money;
    int experience;
    int strength;
    int level;
    Sword sword;
    Armor armor;
    Buy res;
    CharacterClass characterClass;
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
    cout << "Уровень: " << charStat.level << endl;
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
        cout << "Неверное имя статистики!" << endl;
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
        outfile << player.level << endl;
        outfile << static_cast<int>(player.characterClass) << endl;
        outfile.close();
    }
    else {
        cout << "Ошибка при открытии файла для сохранения." << endl;
    }
}

void loadStats(PlayerStat& player) {
    ifstream infile("player_stats.txt");
    if (infile.is_open()) {
        getline(infile, player.name);
        infile >> player.health;
        infile >> player.strength;
        infile >> player.armor.defense;
        infile >> player.money;
        infile >> player.experience;
        infile >> player.level;

        int classIndex;
        infile >> classIndex;
        player.characterClass = static_cast<CharacterClass>(classIndex);

        infile.close();
    }
    else {
        cout << "Файл player_stats.txt не найден. Используются стандартные настройки." << endl;
    }
}

void playerInventory(PlayerStat& player) {
    player.sword.name = "Меч Бога";
    player.sword.damage = 200; 
    player.strength += player.sword.damage; 
    cout << "Инвентарь игрока:" << endl;
    cout << "1. Меч: " << player.sword.name << " (Урон: " << player.sword.damage << ")" << endl;
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

    cout << endl << "Вы добыли " << coal << " - угля, " << iron << " - железа, " << gold << " - золота, " << diamond << " - алмазов." << endl;
    player.res.money += (coal * 10) + (iron * 25) + (gold * 1000) + (diamond * 15000);
    cout << "\nТеперь у вас " << player.res.money << " денег от продажи ресурсов." << endl;
    player.health += 10; 
    cout << player.health << " - Ваше здоровье восстановлено на 10" << endl;
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

void GameShop(PlayerStat& player) {
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_BLUE);
    cout << "\nВы зашли в лавку!" << endl;
    cout << "  ____||____  \n ///////////\\ \n///////////  \\\n|    _    |  |\n|[] | | []|[]|\n|   | |   |  |";

    int playerbuy;
    cout << "\n(1) Зелье регенерации(100 ХП) = 45000 МР\n(2) Зелье силы(50 ЕД.) = 60000 МР\n(3) Зелье опыта(100 ЕД.) = 100000 МР\nВвод : ";
    cin >> playerbuy;

    switch (playerbuy) {
    case 1:
        if (player.money >= 45000) {
            player.money -= 45000;
            player.health += 100;
            cout << "Вы купили Зелье регенерации!" << endl;
        }
        else {
            cout << "Недостаточно денег!" << endl;
        }
        break;
    case 2:
        if (player.money >= 60000) {
            player.money -= 60000;
            player.strength += 50;
            cout << "Вы купили Зелье силы!" << endl;
        }
        else {
            cout << "Недостаточно денег!" << endl;
        }
        break;
    case 3:
        if (player.money >= 100000) {
            player.money -= 100000;
            player.experience += 100;
            cout << "Вы купили Зелье опыта!" << endl;
        }
        else {
            cout << "Недостаточно денег!" << endl;
        }
        break;
    case 0:
        cout << "Вы вышли из магазина." << endl;
        break;
    default:
        cout << "Недействительный выбор." << endl;
        break;
    }
    system("color 0F");
}

void Blacksmith(PlayerStat& player) {
    cout << "\nВы зашли к кузнецу!" << endl;
    cout << "Кузнец предлагает создать броню за 50000 МР." << endl;
    cout << "У вас " << player.money << " денег." << endl;

    cout << "Если вы хотите создать броню, введите '1', если хотите выйти, введите '0': ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        if (player.money >= 50000) {
            player.money -= 50000;
            player.armor.name = "Улучшенная броня";
            player.armor.defense += 20; 
            cout << "Вы создали улучшенную броню! Защита: " << player.armor.defense << endl;
        }
        else {
            cout << "Недостаточно денег!" << endl;
        }
    }
    else {
        cout << "Вы вышли из кузницы." << endl;
    }
}

void StatsFile(PlayerStat& player) {
    ifstream file("player_stats.txt");
    cout << "-----------------------------------------------------------------------------------------------------\n";
    cout << "                                       ▄▄                             \n▀████▄     ▄███▀                     ▀███          ▀███               \n  ████    ████                         ██            ██               \n  █ ██   ▄█ ██   ▄▄█▀██▀███▄███   ▄█▀▀███   ▄██▀██▄  ██  ▄██▀ ▄█▀██▄  \n  █  ██  █▀ ██  ▄█▀   ██ ██▀ ▀▀ ▄██    ██  ██▀   ▀██ ██ ▄█   ██   ██  \n  █  ██▄█▀  ██  ██▀▀▀▀▀▀ ██     ███    ██  ██     ██ ██▄██    ▄█████  \n  █  ▀██▀   ██  ██▄    ▄ ██     ▀██    ██  ██▄   ▄██ ██ ▀██▄ ██   ██  \n▄███▄ ▀▀  ▄████▄ ▀█████▀████▄    ▀████▀███▄ ▀█████▀▄████▄ ██▄▄████▀██▄\n";
    cout << "\n";

    if (file.is_open()) {
        string line1, line2, line3, line4, line5, line6, line7, line8;

        getline(file, line1);
        getline(file, line2);
        getline(file, line3);
        getline(file, line4);
        getline(file, line5);
        getline(file, line6);
        getline(file, line7);
        getline(file, line8);

        cout << "Имя: " << line1 << endl;
        cout << "Здоровье: " << line2 << endl;
        cout << "Сила: " << line3 << endl;
        cout << "Защита: " << line4 << endl;
        cout << "Деньги: " << line5 << endl;
        cout << "Опыт: " << line6 << endl;
        cout << "Уровень: " << line7 << endl;

        if (player.characterClass == WARRIOR) cout << "Ваш класс: Воин" << endl;
        else if (player.characterClass == MAGE) cout << "Ваш класс: Маг" << endl;
        else if (player.characterClass == SHAMAN) cout << "Ваш класс: Шаман" << endl;

        file.close();
    }
    else {
        cout << "Ошибка открытия файла." << endl;
    }
    cout << "-----------------------------------------------------------------------------------------------------\n";
}

void levelUp(PlayerStat& player) {
    while (player.experience >= 100) {
        player.experience -= 100;
        player.level += 1;
        player.health += 20 + 10 * (player.level - 1); 
        player.strength += 5;

        cout << "Поздравляем! Вы подняли уровень до " << player.level << "!" << endl;
        cout << "Теперь у вас " << player.health << " здоровья и " << player.strength << " силы." << endl;
    }
}

void chooseCharacterClass(PlayerStat& player) {
    if (player.characterClass == WARRIOR || player.characterClass == MAGE || player.characterClass == SHAMAN) {
        cout << "Класс персонажа загружен: ";
        if (player.characterClass == WARRIOR) cout << "Воин";
        else if (player.characterClass == MAGE) cout << "Маг";
        else cout << "Шаман";
        cout << endl;
    }
    else {
        cout << "\nВыберите класс персонажа:\n1. Воин\n2. Маг\n3. Шаман\nВвод : ";
        int classChoice;
        cin >> classChoice;

        switch (classChoice) {
        case 1:
            player.characterClass = WARRIOR;
            player.health += 20;
            player.strength += 5;
            player.armor.defense += 5;
            break;
        case 2:
            player.characterClass = MAGE;
            player.health = 80;
            player.strength += 2;
            break;
        case 3:
            player.characterClass = SHAMAN;
            player.health = 90;
            player.strength += 4;
            break;
        default:
            cout << "Неверный выбор! Используется класс по умолчанию (воин)." << endl;
            player.characterClass = WARRIOR;
            player.health += 20;
            player.strength += 5;
            break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int user, random_u;
    srand(static_cast<unsigned int>(time(0)));

    PlayerStat myCharacter;

    loadStats(myCharacter); 
    chooseCharacterClass(myCharacter);  

    cout << "\n";

    Enemy goblin, troll;

    while (true) {
        cout << "\n1. Поиск ресурсов\n2. Бой\n3. Посмотреть статистику\n4. Инвентарь\n5. Продавать ресурсы\n6. Магазин\n7. Кузнец\n8. Выход\nВвод : ";
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
            }
            else {
                initTroll(troll);
                fightTroll(myCharacter, troll);
            }
            levelUp(myCharacter);
            saveStats(myCharacter);
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
            Blacksmith(myCharacter);
        }
        else if (user == 8) {
            break;
        }
        else {
            cout << "Неверный выбор." << endl;
        }
    }

    return 0;
}