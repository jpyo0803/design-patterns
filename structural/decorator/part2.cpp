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

class Unit {
 public:
  virtual void DoDamage(Attack* attack) = 0;  // Pure virtual function
  virtual ~Unit() = default;                  // Virtual destructor for proper cleanup
};

class Player : public Unit {
 public:
  Player() : hp_(100.0) {}

  void DoDamage(Attack* attack) override {
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

class BaseDecorator : public Unit {
 public:
  explicit BaseDecorator(Unit* wrapee) : wrapee_(wrapee) {}

  void DoDamage(Attack* attack) override {
    wrapee_->DoDamage(attack);
  }

 protected:
  Unit* wrapee_;  // Pointer to the decorated unit
};

class LeatherArmor : public BaseDecorator {
 public:
  explicit LeatherArmor(Unit* wrapee) : BaseDecorator(wrapee) {}

  void DoDamage(Attack* attack) override {
    if (attack->GetType() == AttackType::Physical) {
      attack->Weaken(10.0);  // Weaken physical attacks by 10
    }
    BaseDecorator::DoDamage(attack);
  }
};

class SteelArmor : public BaseDecorator {
 public:
  explicit SteelArmor(Unit* wrapee) : BaseDecorator(wrapee) {}

  void DoDamage(Attack* attack) override {
    if (attack->GetType() == AttackType::Physical) {
      attack->Weaken(30.0);  // Weaken physical attacks by 30
    }
    BaseDecorator::DoDamage(attack);
  }
};

class MagicShield : public BaseDecorator {
 public:
  explicit MagicShield(Unit* wrapee) : BaseDecorator(wrapee) {}

  void DoDamage(Attack* attack) override {
    if (attack->GetType() == AttackType::Magic) {
      attack->Weaken(20.0);  // Weaken magic attacks by 20
    }
    BaseDecorator::DoDamage(attack);
  }
};

int main() {
  {
    Player* player = new Player();
    std::cout << "Initial Player HP: " << player->hp() << std::endl;

    Attack* physicalAttack = new PhysicalAttack(60.0);
    Attack* magicAttack = new MagicAttack(40.0);

    player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;

    player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;

    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;
  }

  {
    std::cout << "---------------------------------------" << std::endl;
    Player* player = new Player();

    /*
      Chain decorators
      1. Wrap player with LeatherArmor
      2. Wrap LeatherArmoredPlayer with MagicShield
    */
    LeatherArmor* la = new LeatherArmor(player);
    MagicShield* ms = new MagicShield(la);

    Unit* decorated_player = ms;

    std::cout << "Initial Leather Armored Player HP: " << player->hp() << std::endl;

    // Due to the leather armor, physical attack damage weakens from 60 to 50.
    Attack* physicalAttack = new PhysicalAttack(60.0);

    // Due to the magic shield, magic attack damage weakens from 40 to 20.
    Attack* magicAttack = new MagicAttack(40.0);

    decorated_player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;

    decorated_player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;

    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;

    delete la;  // Clean up LeatherArmor
    delete ms;  // Clean up MagicShield
  }

  {
    std::cout << "---------------------------------------" << std::endl;
    Player* player = new Player();

    // Steel Armor weakens physical attacks by 30.
    SteelArmor* sa = new SteelArmor(player);

    // Magic Shield weakens magic attacks by 20.
    MagicShield* ms = new MagicShield(sa);

    Unit* decorated_player = ms;

    std::cout << "Initial Steel Armored Player HP: " << player->hp() << std::endl;

    // Due to the steel armor, physical attack damage weakens from 60 to 30.
    Attack* physicalAttack = new PhysicalAttack(60.0);

    // Due to the magic shield, magic attack damage weakens from 40 to 20.
    Attack* magicAttack = new MagicAttack(40.0);

    decorated_player->DoDamage(physicalAttack);
    std::cout << "Player HP after physical attack: " << player->hp() << std::endl;

    decorated_player->DoDamage(magicAttack);
    std::cout << "Player HP after magic attack: " << player->hp() << std::endl;

    std::cout << "Is Player alive? " << (player->IsAlive() ? "Yes" : "No") << std::endl;

    delete player;
    delete physicalAttack;
    delete magicAttack;

    delete sa;  // Clean up SteelArmor
    delete ms;  // Clean up MagicShield
  }

  return 0;
}