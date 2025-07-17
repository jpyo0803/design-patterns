#include <iostream>

enum class AttackType { Physical, Magic };

class Attack {
 public:
  explicit Attack(double damage) : damage_(damage) {}

  virtual ~Attack() = default;  // Virtual destructor for proper cleanup

  double damage() const {
    return damage_;
  }

  double Weaken(double amount) {
    damage_ -= amount;
    if (damage_ < 0) {
      damage_ = 0;  // Ensure damage does not go below zero
    }
    return damage_;
  }

  virtual AttackType GetType() const = 0;  // Pure virtual function to get attack type

 private:
  double damage_;
};

class PhysicalAttack : public Attack {
 public:
  explicit PhysicalAttack(double damage) : Attack(damage) {}

  AttackType GetType() const override {
    return AttackType::Physical;
  }
};

class MagicAttack : public Attack {
 public:
  explicit MagicAttack(double damage) : Attack(damage) {}

  AttackType GetType() const override {
    return AttackType::Magic;
  }
};

class Player {
 public:
  Player() : hp_(100.0) {}

  virtual void DoDamage(Attack* attack) {
    hp_ = std::max(0.0, hp_ - attack->damage());
  }

  double hp() const {
    return hp_;
  }

  bool IsAlive() const {
    return hp_ > 0;
  }

 private:
  double hp_;  // Initial HP of the player
};

/*
  Assume
  - LeatherArmor weakens physical attacks by 10.
  - SteelArmor weakens physical attacks by 30.
  - MagicShield weakens magic attacks by 20.
*/

/*
  Creating a subclass for each new combination of armor and shield will cause explosion of
  subclasses.

  For example, if the number of different armor types is 3 and the number of different shield types
  is 2, we would need to create 3 * 2 = 6 subclasses to cover all combinations.

  Make things even worse, if new protective category is added, we would need to create even more
  subclasses. For example, if we add a helmet category and 2 different helmet types, we would need
  to create 3 (armor) * 2 (shield) * 2 (helmet) = 12 subclasses.

  Instead, we use the Decorator pattern to dynamically add functionality.
*/

class LeatherArmorPlayer : public Player {
 public:
  void DoDamage(Attack* attack) override {
    if (attack->GetType() == AttackType::Physical) {
      attack->Weaken(10.0);  // Weaken physical attacks by 10
    }
    Player::DoDamage(attack);
  }
};

class LeatherArmorMagicShieldPlayer : public Player {
 public:
  void DoDamage(Attack* attack) override {
    if (attack->GetType() == AttackType::Physical) {
      attack->Weaken(10.0);  // Weaken physical attacks by 10
    } else if (attack->GetType() == AttackType::Magic) {
      attack->Weaken(20.0);  // Weaken magic attacks by 20
    }
    Player::DoDamage(attack);
  }
};

class SteelArmorMagicShieldPlayer : public Player {
 public:
  void DoDamage(Attack* attack) override {
    if (attack->GetType() == AttackType::Physical) {
      attack->Weaken(30.0);  // Weaken physical attacks by 30
    } else if (attack->GetType() == AttackType::Magic) {
      attack->Weaken(20.0);  // Weaken magic attacks by 20
    }
    Player::DoDamage(attack);
  }
};

int main() {
  {
    std::cout << "[Player]" << std::endl;
    Player* player = new Player();
    std::cout << "Initial Player HP: " << player->hp() << std::endl;

    PhysicalAttack* physicalAttack = new PhysicalAttack(60.0);
    MagicAttack* magicAttack = new MagicAttack(40.0);

    player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;
    player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;
    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;
  }

  std::cout << "---------------------------------------" << std::endl;
  {
    std::cout << "[Leather Armored Player] " << std::endl;
    LeatherArmorPlayer* player = new LeatherArmorPlayer();
    std::cout << "Initial Player HP: " << player->hp() << std::endl;

    PhysicalAttack* physicalAttack = new PhysicalAttack(60.0);
    MagicAttack* magicAttack = new MagicAttack(40.0);

    player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;
    player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;

    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;
  }

  std::cout << "---------------------------------------" << std::endl;
  {
    std::cout << "[Leather Armored and Magic Shielded Player] " << std::endl;
    LeatherArmorMagicShieldPlayer* player = new LeatherArmorMagicShieldPlayer();
    std::cout << "Initial Player HP: " << player->hp() << std::endl;

    PhysicalAttack* physicalAttack = new PhysicalAttack(60.0);
    MagicAttack* magicAttack = new MagicAttack(40.0);

    player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;
    player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;

    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;
  }

  std::cout << "---------------------------------------" << std::endl;
  {
    std::cout << "[Steel Armored and Magic Shielded Player] " << std::endl;
    SteelArmorMagicShieldPlayer* player = new SteelArmorMagicShieldPlayer();
    std::cout << "Initial Player HP: " << player->hp() << std::endl;

    PhysicalAttack* physicalAttack = new PhysicalAttack(60.0);
    MagicAttack* magicAttack = new MagicAttack(40.0);

    player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;
    player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;

    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;
  }
  return 0;
}