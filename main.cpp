#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Item {
public:
    string name;
    int healAmount;

    Item(string n, int h) {
        name = n;
        healAmount = h;
    }
};

class Inventory {
private:
    vector<Item> items;

public:
    void addItem(Item item) {
        items.push_back(item);
    }

    void showItems() {
        if (items.empty()) {
            cout << "  Inventory is empty" << endl;
            return;
        }
        for (int i = 0; i < items.size(); i++) {
            cout << "  " << i + 1 << ". " << items[i].name << " (+" << items[i].healAmount << " HP)" << endl;
        }
    }

    int useFirst() {
        if (items.empty()) return 0;
        int heal = items[0].healAmount;
        cout << "  Used " << items[0].name << endl;
        items.erase(items.begin());
        return heal;
    }

    Inventory operator+(Inventory other) {
        Inventory merged;
        for (int i = 0; i < items.size(); i++)
            merged.addItem(items[i]);
        for (int i = 0; i < other.items.size(); i++)
            merged.addItem(other.items[i]);
        return merged;
    }
};

class Character {
protected:
    string name;
    int hp;
    int maxHp;
    int atk;

public:
    Character(string n, int h, int a) {
        name = n;
        hp = h;
        maxHp = h;
        atk = a;
    }

    virtual ~Character() {}

    virtual int attack() = 0;
    virtual string getClassName() = 0;

    string getName() { return name; }
    int getHp() { return hp; }
    int getMaxHp() { return maxHp; }
    bool isAlive() { return hp > 0; }

    void takeDamage(int dmg) {
        hp = hp - dmg;
        if (hp < 0) hp = 0;
        cout << "  " << name << " takes " << dmg << " damage! (" << hp << "/" << maxHp << " HP)" << endl;
    }

    void heal(int amt) {
        hp = hp + amt;
        if (hp > maxHp) hp = maxHp;
        cout << "  " << name << " heals " << amt << " HP! (" << hp << "/" << maxHp << " HP)" << endl;
    }
};

class Warrior : public Character {
public:
    Warrior(string n) : Character(n, 120, 18) {}

    string getClassName() { return "Warrior"; }

    int attack() {
        int dmg = atk + rand() % 6;
        cout << "  " << name << " swings a greatsword!" << endl;
        return dmg;
    }
};

class Mage : public Character {
public:
    Mage(string n) : Character(n, 80, 25) {}

    string getClassName() { return "Mage"; }

    int attack() {
        int dmg = atk + rand() % 8;
        cout << "  " << name << " casts a fireball!" << endl;
        return dmg;
    }
};

class Archer : public Character {
public:
    Archer(string n) : Character(n, 90, 20) {}

    string getClassName() { return "Archer"; }

    int attack() {
        int dmg = atk + rand() % 7;
        cout << "  " << name << " fires an arrow!" << endl;
        return dmg;
    }
};

class Enemy : public Character {
    string type;
public:
    Enemy(string n, string t, int h, int a) : Character(n, h, a) {
        type = t;
    }

    string getClassName() { return type; }

    int attack() {
        int dmg = atk + rand() % 5;
        cout << "  " << name << " attacks!" << endl;
        return dmg;
    }
};

void combat(Character* player, Enemy& enemy, Inventory& inv) {
    cout << endl << "===== " << enemy.getName() << " appears! =====" << endl;

    while (player->isAlive() && enemy.isAlive()) {
        cout << endl;
        cout << player->getClassName() << " " << player->getName()
             << " [" << player->getHp() << "/" << player->getMaxHp() << " HP]";
        cout << "  vs  ";
        cout << enemy.getClassName() << " " << enemy.getName()
             << " [" << enemy.getHp() << "/" << enemy.getMaxHp() << " HP]" << endl;

        cout << "  1) Attack  2) Use Item" << endl << "> ";
        int choice;
        cin >> choice;

        if (choice == 2) {
            int h = inv.useFirst();
            if (h > 0)
                player->heal(h);
            else
                cout << "  No items!" << endl;
        } else {
            int dmg = player->attack();
            enemy.takeDamage(dmg);
        }

        if (enemy.isAlive()) {
            int edmg = enemy.attack();
            player->takeDamage(edmg);
        }
    }

    if (player->isAlive())
        cout << endl << "  >> Victory! " << enemy.getName() << " defeated!" << endl;
    else
        cout << endl << "  >> You have fallen..." << endl;
}

int main() {
    srand(time(0));

    cout << "========================================" << endl;
    cout << "         OOP Quest - Text RPG           " << endl;
    cout << "========================================" << endl;

    cout << "Enter your name: ";
    string name;
    cin >> name;

    cout << endl << "Choose your class:" << endl;
    cout << "  1. Warrior (High HP, strong melee)" << endl;
    cout << "  2. Mage (Low HP, high damage)" << endl;
    cout << "  3. Archer (Balanced)" << endl;
    cout << "Choice: ";
    int choice;
    cin >> choice;

    Character* player;
    if (choice == 1)
        player = new Warrior(name);
    else if (choice == 2)
        player = new Mage(name);
    else
        player = new Archer(name);

    Inventory inv;
    inv.addItem(Item("Health Potion", 30));
    inv.addItem(Item("Elixir", 50));

    Inventory loot;
    loot.addItem(Item("Herb", 20));
    loot.addItem(Item("Greater Potion", 40));

    cout << endl << "--- Your inventory ---" << endl;
    inv.showItems();

    cout << endl << "You enter a dark forest..." << endl;
    Enemy goblin("Grak", "Goblin", 50, 10);
    combat(player, goblin, inv);
    if (!player->isAlive()) { cout << "GAME OVER" << endl; delete player; return 0; }

    cout << endl << "Bones rattle in the shadows..." << endl;
    Enemy skeleton("Rattles", "Skeleton", 45, 12);
    combat(player, skeleton, inv);
    if (!player->isAlive()) { cout << "GAME OVER" << endl; delete player; return 0; }

    cout << endl << "You find a loot bag!" << endl;
    inv = inv + loot;
    inv.showItems();

    cout << endl << "A cave looms ahead..." << endl;
    Enemy troll("Thog", "Troll", 90, 16);
    combat(player, troll, inv);
    if (!player->isAlive()) { cout << "GAME OVER" << endl; delete player; return 0; }

    cout << endl << "The Dragon's lair!" << endl;
    Enemy dragon("Ignarius", "Dragon", 150, 22);
    combat(player, dragon, inv);

    if (player->isAlive()) {
        cout << endl << "==============================" << endl;
        cout << "  CONGRATULATIONS, " << player->getName() << "!" << endl;
        cout << "  You conquered OOP Quest!" << endl;
        cout << "==============================" << endl;
    } else {
        cout << endl << "GAME OVER. Try again!" << endl;
    }

    delete player;
    return 0;
}
