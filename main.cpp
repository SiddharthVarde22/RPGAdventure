#include <iostream>
using namespace std;
#include <vector>
#include<cstdlib>
#include<ctime>

// parent class that Player, enemy and BossEnemy will inherit from
class Human {
  int health;      // health of entity(Player or enemy)
  int meleeDamage; // Amount of normal damage
  int maxHealth;   // maximum health an entity can have

public:
  // constructor to initialize values
  Human(int health, int meleeDamage, int maxHealth) {
    this->health = health;
    this->meleeDamage = meleeDamage;
    this->maxHealth = maxHealth;
  }
  // Setter for health
  void SetHealth(int value) { health = value; }
  // methods to increase player strenght when he goes to the next level
  void IncreaseMaxHealth(int value) { maxHealth += value; }
  void IncreaseMeleeDamage(int value) { meleeDamage += value; }
  // Getters for health, meleedamage, and maxHealth
  int GetHealth() { return health; }
  int GetMaxHealth() { return maxHealth; }
  int GetMeleeDamage() { return meleeDamage; }
  // General take damage function
  virtual void TakeDamage(int value) {
    // cout << "Human Take damage\n";
    health -= value;
    if (health <= 0) {
      health = 0;
    }
  }
  // General Attack function
  virtual void Attack(Human *other) { other->TakeDamage(meleeDamage); }
  // General heal function
  virtual void Heal() {
    int randomValue = (rand() % 16) + 5;

    if (health + randomValue < maxHealth) {
      health += randomValue;
    } else {
      health += (maxHealth - health);
    }
  }
  // Destructor
  virtual ~Human() {}
};

// Player class - functionalities of player
class Player : public Human {
  int defence;       // amount of defence that player has
  int rangedDamage;  // amount of damage that player can deal with ranged attack
  bool isInvinsible; // if player is invinsible or not
  vector<string> specialAbility; // List of special abilities of player
  vector<string> items;          // list of special items of player

public:
  // Constructor
  Player(int meleeDamage, int maxHealth, int defence, int rangedDamage)
      : Human(maxHealth, meleeDamage, maxHealth) {

    this->defence = defence;
    this->rangedDamage = rangedDamage;
    isInvinsible = false;
  }
  // method to write the player stats
  void Introduce() {
    cout << "Health - " << GetHealth() << endl
         << "Melee Damage - " << GetMeleeDamage() << endl
         << "Defence - " << defence << endl
         << "Ranged Damage - " << rangedDamage << endl;

    PrintSpecialItems();
    PrintSpecialAbilities();
  }
  // method to increase the ampunt of defence when player goes to next level
  void IncreaseDefence(int value) { defence += value; }

  // increases the stats of player when player goes to the next level
  void IncreasePlayerStats() {
    Human::IncreaseMaxHealth(50);
    Human::IncreaseMeleeDamage(2);
    IncreaseDefence(2);

    SetHealth(GetMaxHealth());
  }
  // Adds the special item to the list
  void GainSpecialItem(string nameOfItem) { items.push_back(nameOfItem); }
  // Adds the special ability to the list
  void GainSpecialAbility(string nameOfAbility) {
    specialAbility.push_back(nameOfAbility);
  }
  // Show the list of special items
  void PrintSpecialItems() {
    cout << "Special Items gained : ";
    if (items.size() == 0) {
      cout << "None\n";
    }

    for (int i = 0; i < items.size(); i++) {
      cout << items.at(i) << " ,";

      if (i == (items.size() - 1)) {
        cout << endl;
      }
    }
  }
  // Show the list of special Abilities
  void PrintSpecialAbilities() {
    cout << "Special Abilities gained : ";

    if (specialAbility.size() == 0) {
      cout << "None\n";
    }

    for (int i = 0; i < specialAbility.size(); i++) {
      cout << specialAbility.at(i) << " ,";

      if (i == (specialAbility.size() - 1)) {
        cout << endl;
      }
    }
  }
  // Critical Hit - Special ability
  void PerformCriticalHit(Human *other) {
    int damageBoost = rand() % 11 + 5;
    cout << "Player perfomed Special Ability : Critical Hit\n";
    other->TakeDamage(GetMeleeDamage() + damageBoost);
  }
  // Blocker - Special Ability
  void PerformBlocker() {
    cout << "Player performed Special Ability : Blocker\n";
  }
  // Life Steal - Special ability
  void PerformLifeSteal(Human *other) {
    cout << "Player Perfomed Special Ability : LifeSteal\n";
    other->TakeDamage(GetMeleeDamage());
    Human::Heal();
    cout << "Player's health is : " << GetHealth() << endl;
  }
  // Ranged Attack - special ability
  void PerformRangedAttack(Human *other) {
    cout << "Player Performed Special Ability : Ranged Attack\n";
    other->TakeDamage(rangedDamage);
    isInvinsible = true;
  }
  // Take damage function of player
  // he can peform blocker or will not take damage if he performed ranged attack
  void TakeDamage(int value) override {
    // cout << "Player take damage\n"
    int randomValue = (rand() % 100);
    // cout << "random value is " << randomValue << endl;
    if (((randomValue % 10) == 0) && specialAbility.size() >= 2 &&
        isInvinsible == false) {
      PerformBlocker();

    } else if (isInvinsible == true) {
      cout << "Player did not took any damage because of the safe  distance\n";
      isInvinsible = false;
    } else {
      int randomDefence = rand() % defence;
      // cout << randomDefence << endl;
      if ((value - randomDefence) <= 0) {
        randomDefence = 0;
      }
      Human::TakeDamage(value - (randomDefence));
      cout << "Player's health is " << GetHealth() << endl;
    }
  }
  // Attack function of player
  // 10% chance to perform special abilities if they are unlocked
  void Attack(Human *other) override {
    int randomValue = (rand() % 100);
    if (((randomValue % 10) == 0) && specialAbility.size() > 0) {

      int abilityToPerform = rand() % specialAbility.size();
      // cout << "Random value for ability is" << abilityToPerform << endl;

      switch (abilityToPerform) {
      case 0:
      case 1:
        PerformCriticalHit(other);
        break;
      case 2:
        PerformLifeSteal(other);
        break;
      case 3:
        PerformRangedAttack(other);
        break;
      }
    } else {
      Human::Attack(other);
    }
  }
  // Heals the player
  void Heal() override {
    Human::Heal();
    cout << "Player healed himself\n"
         << "Player's health is " << GetHealth() << endl;
  }
  // Destructor
  ~Player() {}
};

// Enemy class - functionalities of normal enemy
class Enemy : public Human {
public:
  // Constructor
  Enemy(int meleeDamage, int maxHealth)
      : Human(maxHealth, meleeDamage, maxHealth) {}
  // Randomly choses to attack or heal himself
  void ChoseAttackOrHeal(Human *other) {

    int randomValue = (rand() % 11) + 1;

    if ((randomValue % 2) == 0) {
      Heal();
    } else {
      Attack(other);
    }
  }
  // Takes damage
  void TakeDamage(int value) override {
    Human::TakeDamage(value);
    cout << "Enemy's health is " << GetHealth() << endl;
  }
  // Attacks on player
  virtual void Attack(Human *other) override {
    cout << "Enemy is Attacking\n";
    other->TakeDamage(GetMeleeDamage());
  }
  // Heals himself
  virtual void Heal() override {
    Human::Heal();
    cout << "Enemy Healed him self\n";
    cout << "Enemy's health is " << GetHealth() << endl;
  }
  // Destructor
  ~Enemy() {}
};

// Boss Enemy class - enemy to fight in last level
class BossEnemy : public Enemy {
  int attackBoost; // Amount of extra damage that BossEnemy can deal when
                   // attacking player with Heavy Attack

public:
  // Constructor
  BossEnemy(int attackBoost, int meleeDamage, int maxHealth)
      : Enemy(meleeDamage, maxHealth) {
    this->attackBoost = attackBoost;
  }
  // randomly choooses to perform normal attack or Heavy attack
  void Attack(Human *other) override {

    int randomValue = rand() % 11;

    if ((randomValue % 2) == 0) {
      int boostValue = (rand() % (attackBoost - 4)) + 5;
      cout << "Merlock perfomed a Heavy Attack\n";
      other->TakeDamage(GetMeleeDamage() + boostValue);
    } else {
      cout << "Merlock Perfomed an Attack\n";
      other->TakeDamage(GetMeleeDamage());
    }
  }
  // Heals himself
  void Heal() override {
    Human::Heal();
    cout << "Merlock Healed himself\n";
    cout << "Merlock's health is " << GetHealth() << endl;
  }
  // Takes damage
  void TakeDamage(int value) override {
    Human::TakeDamage(value);
    cout << "Merlock's health is " << GetHealth() << endl;
  }
  // Destructor
  ~BossEnemy() {}
};

// Level class
// To Store the data of every level
class Level {
  int numberOfEnemies;     // Number of enemies that a level has
  vector<Enemy *> enemies; // List of enemies that level has

protected:
  Player *player; // Refrence of player in every level

public:
  // Constructor - prints the player stats
  // initializes the enemies for player to fight
  Level(int numberOfEnemies, Player *player) {
    this->numberOfEnemies = numberOfEnemies;
    this->player = player;

    if (numberOfEnemies == 0) {
      cout << "Last Level\n";
    } else {
      cout << "Level " << numberOfEnemies << endl;
    }
    if (numberOfEnemies != 1) {
      player->IncreasePlayerStats();
    }
    player->Introduce();

    for (int i = 0; i < numberOfEnemies; i++) {
      enemies.push_back(new Enemy(10, 100));
    }
  }
  // takes niput from player to attack or heal untill player provides
  // correct-input
  char TakePlayerInput() {
    char input;
    cout << "enter a to attack OR enter h to heal\t";
    cin >> input;

    switch (input) {
    case 'a':
    case 'A':
      return 'a';
      break;

    case 'h':
    case 'H':
      return 'h';
      break;

    default:
      cout << "Enter valid input\n";
      return TakePlayerInput();
      break;
    }
  }
  // Starts the fight of enemies and player one by one untill all
  // enemies are dead or player is dead
  virtual void StartFight() {
    for (int i = 0; i < enemies.size(); i++) {

      while (enemies.at(i)->GetHealth() > 0 && player->GetHealth() > 0) {
        char playerInput = TakePlayerInput();

        switch (playerInput) {
        case 'a':
          player->Attack(enemies.at(i));
          break;
        case 'h':
          player->Heal();
          break;
        }
        // cout << "Enemy's turn\n" << enemies.size() << endl;

        if (enemies.at(i)->GetHealth() > 0) {
          enemies.at(i)->ChoseAttackOrHeal(player);
        }
      }

      if (player->GetHealth() <= 0) {
        cout << "\n\nYou Died!\nGameOver.\n\n";
        break;
      }
    }
  }
  // Destructor - deallocates all enemies and checks if player is dead or not
  // and provides rewards
  ~Level() {
    if (player->GetHealth() > 0 && numberOfEnemies != 0) {
      cout << "You have cleared the level " << numberOfEnemies << endl;
    }

    for (int i = 0; i < enemies.size(); i++) {
      if (enemies.at(i) != nullptr) {
        delete enemies.at(i);
        enemies.at(i) = nullptr;
      }
    }
  }
};

// Last level class- Player fights Merlock (boss Enemy)
class LastLevel : public Level {
  Enemy *merlock;

public:
  // Initializes the boss enemy
  LastLevel(Player *player) : Level(0, player) {
    merlock = new BossEnemy(20, 20, 500);
  }
  // Starts the fight of merlock and player
  void StartFight() override {

    while (merlock->GetHealth() > 0 && player->GetHealth() > 0) {
      char playerInput = TakePlayerInput();

      switch (playerInput) {
      case 'a':
        player->Attack(merlock);
        break;
      case 'h':
        player->Heal();
        break;
      }

      if (merlock->GetHealth() > 0) {
        merlock->ChoseAttackOrHeal(player);
      }

      if (player->GetHealth() <= 0) {
        cout << "\n\nYou Died!\nGameOver.\n\n";
        break;
      }
    }
  }
  // Destructor - deallocates merlock anf checks if player is alive
  ~LastLevel() {
    delete merlock;

    if (player->GetHealth() > 0) {
      cout << "You have cleared Last level\n";
    }
  }
};

// Game class that acts as a game manager and keeps the track all the refrences
class Game {
  Player *player; // Player object

public:
  // Initializes the player
  Game() { this->player = new Player(15, 100, 5, 25); }
  // introduces player with the story and start all the levels and provides the
  // rewards
  void Play() {

    cout << "Your village is captured by Merlock\n"
         << "find the map for magical items and"
         << "\ncollect those magical items\n"
         << "they will help you to defeat Merlock\n\n\n";

    for (int i = 1; i <= 5; i++) {
      Level level(i, player);
      level.StartFight();

      switch (i) {
      case 1:
        cout << "\nYou got map\n\n\n\n";
        player->GainSpecialItem("map");
        break;
      case 2:
        cout << "\nYou got Sword\n";
        player->GainSpecialItem("Sword");
        cout << "Ypu got Special ability : CriticalHits\n\n\n\n";
        player->GainSpecialAbility("CriticalHits");
        break;
      case 3:
        cout << "\nYou got shield\n";
        player->GainSpecialItem("Shield");
        cout << "Ypu got Special ability : Blocker\n\n\n\n";
        player->GainSpecialAbility("Blocker");
        break;
      case 4:
        cout << "\nYou got Armour\n";
        player->GainSpecialItem("Armour");
        cout << "Ypu got Special ability : LifeSteal\n\n\n\n";
        player->GainSpecialAbility("LifeSteal");
        break;
      case 5:
        cout << "\nYou got Bow\n";
        player->GainSpecialItem("Bow");
        cout << "Ypu got Special ability : RangedAttack\n\n\n\n";
        player->GainSpecialAbility("RangedAttack");
        break;
      }
    }

    LastLevel bossFight(player);
    bossFight.StartFight();
  }
  // Checks if player is alive or not and deallocatres player
  ~Game() {
    if (player->GetHealth() > 0) {
      cout << "\n\n\nYou have Won the Game\n\n\n";
    }
    delete player;
  }
};

int main() {
  srand(time(0));
  Game rpgGame;
  rpgGame.Play();
}
