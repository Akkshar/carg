 1. Item class (lines 9-18) — Composition

  class Item {
  public:
      string name;
      int healAmount;
      Item(string n, int h) { name = n; healAmount = h; }
  };

  This is the simplest class. It just holds two pieces of data: the item's name and how much HP it restores. The constructor takes
   both values and assigns them. Items don't do anything on their own — they exist to be stored inside an Inventory. This
  relationship (Inventory has Items) is called composition — one object is made up of other objects.

  ---
  2. Inventory class (lines 20-55) — Encapsulation + Operator Overloading

  class Inventory {
  private:
      vector<Item> items;

  The items vector is private. Nothing outside this class can directly touch it. This is encapsulation — hiding internal data and
  only exposing it through controlled methods.

  addItem() — pushes an Item into the vector.

  showItems() — loops through the vector and prints each item with a number. If empty, says so.

  useFirst() — grabs the heal amount from the first item, prints which item was used, removes it from the vector with erase(), and
   returns the heal value. Returns 0 if empty.

  operator+ — this is operator overloading. It lets you write inv = inv + loot as if Inventory objects were numbers. What it does:
  1. Creates a new empty Inventory called merged
  2. Copies all items from this (the left side) into merged
  3. Copies all items from other (the right side) into merged
  4. Returns the combined inventory

  ---
  3. Character class (lines 57-93) — Abstraction + Inheritance base

  class Character {
  protected:
      string name;
      int hp, maxHp, atk;

  Members are protected — this means subclasses (Warrior, Mage, etc.) can access them directly, but code outside the class
  hierarchy cannot.

  Constructor — takes a name, HP, and attack value. Sets maxHp equal to hp so we know the cap.

  virtual ~Character() {} — a virtual destructor. This is needed because we use Character* pointers to hold Warrior/Mage/Archer
  objects. Without this, delete player wouldn't properly clean up the subclass.

  Pure virtual functions:
  virtual int attack() = 0;
  virtual string getClassName() = 0;
  The = 0 makes these pure virtual, which means:
  - Character is now an abstract class — you cannot create a Character object directly
  - Every subclass must provide its own implementation of attack() and getClassName()
  - This is abstraction — defining what something must do without saying how

  takeDamage(int dmg) — subtracts damage from HP, clamps it to 0 minimum so HP never goes negative, prints the result.

  heal(int amt) — adds HP but caps it at maxHp so you can't overheal.

  isAlive() — returns true if hp > 0.

  ---
  4. Warrior, Mage, Archer (lines 95-132) — Inheritance + Polymorphism

  class Warrior : public Character {
  public:
      Warrior(string n) : Character(n, 120, 18) {}

  Each class inherits from Character using : public Character. They get all of Character's members and methods for free.

  The constructor uses an initializer list (: Character(n, 120, 18)) to call the parent constructor with hardcoded stats:

  ┌─────────┬─────┬─────┐
  │  Class  │ HP  │ ATK │
  ├─────────┼─────┼─────┤
  │ Warrior │ 120 │ 18  │
  ├─────────┼─────┼─────┤
  │ Mage    │ 80  │ 25  │
  ├─────────┼─────┼─────┤
  │ Archer  │ 90  │ 20  │
  └─────────┴─────┴─────┘

  Each class provides its own attack() — this is polymorphism. The same function name does different things depending on which
  class it belongs to:

  - Warrior: atk + rand() % 6 → damage ranges from atk to atk+5, prints "swings a greatsword"
  - Mage: atk + rand() % 8 → wider range (0-7 bonus), prints "casts a fireball"
  - Archer: atk + rand() % 7 → middle range (0-6 bonus), prints "fires an arrow"

  rand() % N gives a random number from 0 to N-1.

  Why polymorphism matters here: In the combat() function, player->attack() is called on a Character* pointer. C++ doesn't know at
   compile time whether it's a Warrior, Mage, or Archer — it figures it out at runtime because the function is virtual. The
  correct version of attack() gets called automatically.

  ---
  5. Enemy class (lines 134-148) — Inheritance

  Same idea as the player classes. Inherits from Character, has its own attack() implementation. The extra type string lets each
  enemy have a different class name ("Goblin", "Skeleton", "Dragon", etc.) rather than hardcoding it.

  ---
  6. combat() function (lines 150-186) — Game loop

  void combat(Character* player, Enemy& enemy, Inventory& inv)

  Takes a pointer to the player, a reference to the enemy, and a reference to the inventory.

  The while loop runs as long as both are alive:
  1. Prints both characters' stats
  2. Asks the player to choose: attack or use item
  3. If use item: calls inv.useFirst() which returns the heal amount. If > 0, heals the player. If 0 (empty inventory), prints "No
   items!"
  4. If attack: calls player->attack() (polymorphic — runs the correct subclass version), gets the damage back, and applies it to
  the enemy with takeDamage()
  5. If the enemy is still alive after the player's turn, the enemy attacks back

  After the loop, it checks who won and prints accordingly.

  ---
  7. main() (lines 188-260) — Game flow

  srand(time(0)) — seeds the random number generator with the current time so rand() gives different numbers each run.

  Class selection — reads an int, uses new to create the right subclass on the heap, stores it in a Character* pointer. This is
  where polymorphism starts — player is typed as Character* but actually points to a Warrior, Mage, or Archer.

  Two inventories are set up:
  - inv — player's starting bag (Health Potion +30, Elixir +50)
  - loot — found later (Herb +20, Greater Potion +40)

  The game runs 4 encounters in order:
  1. Goblin (50 HP, 10 ATK)
  2. Skeleton (45 HP, 12 ATK)
  3. After skeleton: inv = inv + loot merges the loot bag in using operator+
  4. Troll (90 HP, 16 ATK)
  5. Dragon (150 HP, 22 ATK) — final boss

  After each fight, if (!player->isAlive()) checks if you died. If so, prints GAME OVER, calls delete player to free the heap
  memory, and exits.

  delete player at the end (line 258) frees the memory allocated by new. This is why the virtual destructor exists — it ensures
  the correct subclass destructor runs, not just the base class one.

  ---
  OOP concepts summary

  ┌──────────────────────┬──────────────────────────────────────────────────────────────────┐
  │       Concept        │                         Where it appears                         │
  ├──────────────────────┼──────────────────────────────────────────────────────────────────┤
  │ Encapsulation        │ Inventory's private vector, Character's protected members        │
  ├──────────────────────┼──────────────────────────────────────────────────────────────────┤
  │ Abstraction          │ Character is abstract (pure virtual attack() and getClassName()) │
  ├──────────────────────┼──────────────────────────────────────────────────────────────────┤
  │ Inheritance          │ Warrior/Mage/Archer/Enemy all extend Character                   │
  ├──────────────────────┼──────────────────────────────────────────────────────────────────┤
  │ Polymorphism         │ player->attack() calls the right subclass version at runtime     │
  ├──────────────────────┼──────────────────────────────────────────────────────────────────┤
  │ Composition          │ Inventory contains a vector of Item objects                      │
  ├──────────────────────┼──────────────────────────────────────────────────────────────────┤
  │ Operator Overloading │ operator+ merges two Inventory objects                           │
  └──────────────────────┴──────────────────────────────────────────────────────────────────┘
