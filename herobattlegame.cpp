#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class Hero {
   protected:
       string name;
       int health;
       int damage;
       int armor;
       int mana;
       int gold;
       int experience;
       int level;


   public:
       Hero(string name, int health, int damage, int armor, int mana) : name(name), health(health), damage(damage), armor(armor), mana(mana), gold(0), experience(0), level(1) {}
       virtual ~Hero() {}

       virtual void attack(Hero &opponent) {
           int effectiveDamage = max(0, damage - opponent.armor);
          opponent.takeDamage(effectiveDamage);
          cout << name << " attacks " << opponent.getName() << " and damage " << effectiveDamage << " loss!" << endl;
       }

       virtual void specialAbility(Hero &opponent) = 0;

       void takeDamage(int amount) {
          health -= amount;
          if (health < 0) health = 0;
       }

       void heal(int amount) {
         health += amount;
         cout << name << " heal on " << amount << " health!" << endl;
       }

       bool isAlive() const {
          return health > 0;
       }

       string getName() const {
          return name;
       }

       int getHealth() const {
          return health;
       }

       void gainExperience(int amount) {
          experience += amount;
          if (experience >= level * 50) {
            experience = 0;
            level++;
            health += 10;
            damage += 2;
            cout << name << " raise a level! Now the level " << level << "!" << endl;
          }
       }

       void gainGold(int amount) {
          gold += amount;
       }

       int getGold() const {
          return gold;
       }
};

       class Warrior : public Hero {
      public:
        Warrior(string name) : Hero(name, 100, 15, 5, 20) {}

        void specialAbility(Hero &opponent) override {
          int critDamage = damage * 2;
          opponent.takeDamage(critDamage);
          cout << name << " uses " << "Powerful kick!" << " and damage " << critDamage << " loss!" << endl;
        }
       };

       class Mage : public Hero {
      public:
        Mage(string name) : Hero(name, 80, 10, 2, 50) {}

        void specialAbility(Hero &opponent) override {
           if (mana >= 20) {
            opponent.takeDamage(25);
            mana -= 20;
            cout << name << " use Fireball! loss 25 damage!" << endl;
           } else {
            cout << name << "Not enough mana for magic!" << endl;
           }
         }
       };

       class Archer : public Hero {
      public:
        Archer(string name) : Hero(name, 90, 12, 3, 30) {}

        void specialAbility(Hero &opponent) override {
          int poisonDamage = 10;
          opponent.takeDamage(poisonDamage);
          cout << name << " use Poisoned arrow! And damage " << poisonDamage << " damage!" << endl;
         }
       };

       class Orc : public Hero {
      public:
        Orc(string name) : Hero(name, 120, 20, 7, 10) {}

        void specialAbility(Hero &opponent) override {
          health += 10;
          cout << name << " use regeneration! Restore 10 health!" << endl;
         }
       };

       class Paladin : public Hero {
     public:
        Paladin(string name) : Hero(name, 110, 14, 6, 40) {}

        void specialAbility(Hero &opponent) override {
           health += 15;
           cout << name << " use Sacred healing! Restore 15 health!" << endl;
         }
       };

       Hero* createHero() {
         string name;
         int choice;
         cout << "Enter a name of hero: ";
         cin >> name;

         cout << "Choose a class of hero:\n";
         cout << "1. Warrior\n 2. Mage\n 3. Archer\n 4. Orc\n 5. Paladin\n";
         cin >> choice;

         switch (choice) {
           case 1: return new Warrior(name);
           case 2: return new Mage(name);
           case 3: return new Archer(name);
           case 4: return new Orc(name);
           case 5: return new Paladin(name);
           default: cout << "Wrong choice! By default, a warrior is chosen." << endl;
                                    return new Warrior(name);
         }
       }

       void aiTurn(Hero &ai, Hero &player) {
          if (ai.getHealth() < 30 && ai.getGold() >= 50) {
            ai.heal(20);
            ai.gainGold(-50);
          } else if (rand() % 2 == 0) {
            ai.attack(player);
          } else {
            ai.specialAbility(player);
          }
       }

       void battle(Hero &hero1, Hero &hero2, bool aiMode) {
          srand(time(0));

          while (hero1.isAlive()&& hero2.isAlive()) {
            cout << "\n" << hero1.getName() << " (Health: " << hero1.getHealth() << ") vs " << hero2.getName() << " (Health: " << hero2.getHealth() << ")\n";

            if (!aiMode || (aiMode && rand() % 2 == 0)) {
                hero1.attack(hero2);
            } else {
                aiTurn(hero1, hero2);
            }

            if (!hero2.isAlive()) {
                cout << hero2.getName() << "  defeated! " << hero1.getName() << " win!" << endl;
                hero1.gainExperience(50);
                hero1.gainGold(100);
                break;
            }

            if (!aiMode || (aiMode && rand() % 2 == 1)) {
                hero2.attack(hero1);
            } else {
                aiTurn(hero2, hero1);
            }

            if (!hero1.isAlive()) {
                cout << hero1.getName() << " defeated! " << hero2.getName() << " win!" << endl;
                hero2.gainExperience(50);
                hero2.gainGold(100);
                break;
             }
           }
         }

int main() {
   cout << "1. Play against computer\n 2. Play against friend\n";
   int mode;
   cin >> mode;

   Hero* hero1 = createHero();
   Hero* hero2 = (mode == 1) ? new Warrior("AI-opponent") : createHero();

   battle(*hero1, *hero2, mode == 1);

   delete hero1;
   delete hero2;

  return 0;

}
