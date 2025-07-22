#include <cassert>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <vector>

class Unit {
  static int id_counter_;

 public:
  Unit(int hp) : id_(id_counter_++), hp_(hp) {}
  virtual ~Unit() = default;

  int id() const {
    return id_;
  }

  int hp() const {
    return hp_;
  }

  bool IsAlive() const {
    return hp_ > 0;
  }

  void IncreaseHpBy(int heal) {
    hp_ += heal;
  }

  void DecreaseHpBy(int damage) {
    hp_ -= damage;
  }

 private:
  int id_;  // Unique identifier for the unit
  int hp_;
};

int Unit::id_counter_ = 0;

class Attackable : public Unit {
 public:
  Attackable(int hp, int damage) : Unit(hp), damage_(damage) {}

  virtual void DoAttack(Unit* target) {
    target->DecreaseHpBy(damage_);
  }

  int damage() const {
    return damage_;
  }

 private:
  int damage_;
};

class Warrior : public Attackable {
 public:
  Warrior(int hp, int damage) : Attackable(hp, damage) {}

  void DoAttack(Unit* target) override {
    Attackable::DoAttack(target);
    std::cout << "Warrior " << id() << " attacked unit " << target->id() << " for " << damage()
              << " damage." << std::endl;
  }
};

class Orc : public Attackable {
 public:
  Orc(int hp, int damage) : Attackable(hp, damage) {}

  void DoAttack(Unit* target) override {
    Attackable::DoAttack(target);
    std::cout << "Orc " << id() << " attacked unit " << target->id() << " for " << damage()
              << " damage." << std::endl;
  }
};

class Cleric : public Unit {
 public:
  Cleric(int hp, int heal) : Unit(hp), heal_(heal) {}

  void DoHeal(Unit* target) {
    target->IncreaseHpBy(heal_);
    std::cout << "Cleric " << id() << " healed unit " << target->id() << " for " << heal_
              << " health." << std::endl;
  }

  int heal() const {
    return heal_;
  }

 private:
  int heal_;
};

class Game {
 public:
  void AddWarrior(int hp, int damage) {
    warriors_.emplace_back(std::make_unique<Warrior>(hp, damage));
  }
  void AddCleric(int hp, int heal_amt) {
    clerics_.emplace_back(std::make_unique<Cleric>(hp, heal_amt));
  }
  void AddOrc(int hp, int damage) {
    orcs_.emplace_back(std::make_unique<Orc>(hp, damage));
  }

 public:
  const std::vector<std::unique_ptr<Warrior>>& GetWarriors() const {
    return warriors_;
  }

  const std::vector<std::unique_ptr<Cleric>>& GetClerics() const {
    return clerics_;
  }

  const std::vector<std::unique_ptr<Orc>>& GetOrcs() const {
    return orcs_;
  }

  const std::vector<Unit*> GetAllUnits() const {
    std::vector<Unit*> all_units;
    all_units.reserve(warriors_.size() + clerics_.size() + orcs_.size());

    // As each element is wrapped in a unique_ptr, we can only access via reference
    for (const auto& warrior : warriors_) {
      // Use get() to access the raw pointer. This does not transfer ownership.
      all_units.push_back(warrior.get());
    }
    for (const auto& cleric : clerics_) {
      all_units.push_back(cleric.get());
    }
    for (const auto& orc : orcs_) {
      all_units.push_back(orc.get());
    }
    return all_units;
  }

  bool IsAllWarriorsDead() const {
    for (const auto& warrior : warriors_) {
      if (warrior->IsAlive()) {
        return false;  // At least one warrior is alive
      }
    }
    return true;  // All warriors are dead
  }

  bool IsAllClericsDead() const {
    for (const auto& cleric : clerics_) {
      if (cleric->IsAlive()) {
        return false;  // At least one cleric is alive
      }
    }
    return true;  // All clerics are dead
  }

  bool IsAllOrcsDead() const {
    for (const auto& orc : orcs_) {
      if (orc->IsAlive()) {
        return false;  // At least one orc is alive
      }
    }
    return true;  // All orcs are dead
  }

  void ShowUnitStatus() const {
    std::cout << "[Unit Status]" << std::endl;
    for (const auto& warrior : warriors_) {
      std::cout << "Warrior ID: " << warrior->id() << ", HP: " << warrior->hp() << std::endl;
    }
    std::cout << std::endl;
    for (const auto& cleric : clerics_) {
      std::cout << "Cleric ID: " << cleric->id() << ", HP: " << cleric->hp() << std::endl;
    }
    std::cout << std::endl;
    for (const auto& orc : orcs_) {
      std::cout << "Orc ID: " << orc->id() << ", HP: " << orc->hp() << std::endl;
    }
  }

 private:
  std::vector<std::unique_ptr<Warrior>> warriors_;
  std::vector<std::unique_ptr<Cleric>> clerics_;
  std::vector<std::unique_ptr<Orc>> orcs_;
};

class Command {
 public:
  explicit Command(Game* game) : game_(game) {}

  virtual ~Command() = default;

  virtual void Execute() = 0;

  virtual void Undo() = 0;

 protected:
  Game* game_;  // Pointer to the game instance
};

class AttackCommand : public Command {
 public:
  AttackCommand(Game* game, Attackable* attacker, Unit* target) :
      Command(game), attacker_(attacker), target_(target) {}

  void Execute() override {
    attacker_->DoAttack(target_);
  }

  void Undo() override {
    target_->IncreaseHpBy(attacker_->damage());
  }

 private:
  Attackable* attacker_;
  Unit* target_;
};

class HealCommand : public Command {
 public:
  HealCommand(Game* game, Cleric* healer, Unit* target) :
      Command(game), healer_(healer), target_(target) {}

  void Execute() override {
    healer_->DoHeal(target_);
  }

  void Undo() override {
    target_->DecreaseHpBy(healer_->heal());
  }

 private:
  Cleric* healer_;
  Unit* target_;
};

void Simulate(Game& game) {
  std::stack<std::unique_ptr<Command>> command_history;

  // Simulate a battle until either all warriors or all orcs are dead
  while (!game.IsAllWarriorsDead() && !game.IsAllOrcsDead()) {
    std::queue<std::unique_ptr<Command>> commands;

    std::cout << "----------------------------------------" << std::endl;
    game.ShowUnitStatus();
    std::cout << "----------------------------------------" << std::endl;

    // Warriors attack orcs with lowest HP first
    for (const auto& warrior : game.GetWarriors()) {
      if (!warrior->IsAlive()) continue;  // Skip dead warriors
      Unit* target = nullptr;
      for (const auto& orc : game.GetOrcs()) {
        if (orc->IsAlive() && (target == nullptr || orc->hp() < target->hp())) {
          target = orc.get();  // Find the orc with the lowest HP
        }
      }
      if (target != nullptr) {
        // Create and execute attack command
        commands.push(std::make_unique<AttackCommand>(&game, warrior.get(), target));
      }
    }

    // Orcs attack warriors with lowest HP first
    for (const auto& orc : game.GetOrcs()) {
      if (!orc->IsAlive()) continue;  // Skip dead orcs
      Unit* target = nullptr;
      for (const auto& warrior : game.GetWarriors()) {
        if (warrior->IsAlive() && (target == nullptr || warrior->hp() < target->hp())) {
          target = warrior.get();  // Find the warrior with the lowest HP
        }
      }
      if (target != nullptr) {
        // Create and execute attack command
        commands.push(std::make_unique<AttackCommand>(&game, orc.get(), target));
      }
    }

    // Clerics heal warriors with lowest HP first
    for (const auto& cleric : game.GetClerics()) {
      if (!cleric->IsAlive()) continue;  // Skip dead clerics
      Unit* target = nullptr;
      for (const auto& warrior : game.GetWarriors()) {
        if (warrior->IsAlive() && (target == nullptr || warrior->hp() < target->hp())) {
          target = warrior.get();  // Find the warrior with the lowest HP
        }
      }
      if (target != nullptr) {
        // Create and execute heal command
        commands.push(std::make_unique<HealCommand>(&game, cleric.get(), target));
      }
    }

    // Execute all commands in the queue
    while (!commands.empty()) {
      commands.front()->Execute();
      command_history.push(std::move(commands.front()));
      commands.pop();
    }
  }

  if (game.IsAllOrcsDead()) {
    std::cout << "All orcs are dead. Warriors win!" << std::endl;
  } else {
    std::cout << "All warriors are dead. Orcs win!" << std::endl;
  }

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "After battle ..." << std::endl;
  game.ShowUnitStatus();

  // Rollback commands
  while (!command_history.empty()) {
    command_history.top()->Undo();
    command_history.pop();
  }

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "After rollback ..." << std::endl;
  game.ShowUnitStatus();
}

int main() {
  Game game;

  // Create some units
  game.AddWarrior(100, 20);
  game.AddWarrior(100, 20);
  game.AddCleric(80, 10);
  game.AddOrc(200, 30);

  Simulate(game);

  return 0;
}