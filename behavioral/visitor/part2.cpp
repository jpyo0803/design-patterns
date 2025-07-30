#include <iostream>
#include <string>

class LandAnimal;
class WaterAnimal;
class Bird;

class Visitor {
 public:
  virtual ~Visitor() = default;

  virtual void Visit(const LandAnimal* land_animal) = 0;
  virtual void Visit(const WaterAnimal* water_animal) = 0;
  virtual void Visit(const Bird* bird) = 0;
};

/*
  Other than printing information in the console,
  client can easily extend the Visitor with new features (e.g., file storage).
*/
class PrintVisitor : public Visitor {
 public:
  void Visit(const LandAnimal* land_animal) override;
  void Visit(const WaterAnimal* water_animal) override;
  void Visit(const Bird* bird) override;
};

class Animal {
 public:
  Animal(std::string species, int age, int size);

  virtual ~Animal() = default;

  std::string species() const;

  int age() const;

  int size() const;

  virtual void Accept(Visitor* visitor) const = 0;

 private:
  std::string species_;
  int age_;
  int size_;
};

class LandAnimal : public Animal {
 public:
  LandAnimal(std::string species, int age, int size, int sprint_speed);

  int sprint_speed() const;

  void Accept(Visitor* visitor) const override;

 private:
  int sprint_speed_;
};

class WaterAnimal : public Animal {
 public:
  WaterAnimal(std::string species, int age, int size, int swim_speed);

  int swim_speed() const;

  void Accept(Visitor* visitor) const override;

 private:
  int swim_speed_;
};

class Bird : public Animal {
 public:
  Bird(std::string species, int age, int size, int fly_speed);

  int fly_speed() const;

  void Accept(Visitor* visitor) const override;

 private:
  int fly_speed_;
};

// Implementation
void PrintVisitor::Visit(const LandAnimal* land_animal) {
  std::cout << "Land Animal: Species = " << land_animal->species()
            << ", Age = " << land_animal->age() << ", Size = " << land_animal->size()
            << ", Sprint Speed = " << land_animal->sprint_speed() << std::endl;
}

void PrintVisitor::Visit(const WaterAnimal* water_animal) {
  std::cout << "Water Animal: Species = " << water_animal->species()
            << ", Age = " << water_animal->age() << ", Size = " << water_animal->size()
            << ", Swim Speed = " << water_animal->swim_speed() << std::endl;
}

void PrintVisitor::Visit(const Bird* bird) {
  std::cout << "Bird: Species = " << bird->species() << ", Age = " << bird->age()
            << ", Size = " << bird->size() << ", Fly Speed = " << bird->fly_speed() << std::endl;
}

Animal::Animal(std::string species, int age, int size) :
    species_(species), age_(age), size_(size) {}

std::string Animal::species() const {
  return species_;
}

int Animal::age() const {
  return age_;
}

int Animal::size() const {
  return size_;
}

LandAnimal::LandAnimal(std::string species, int age, int size, int sprint_speed) :
    Animal(species, age, size), sprint_speed_(sprint_speed) {}

int LandAnimal::sprint_speed() const {
  return sprint_speed_;
}

void LandAnimal::Accept(Visitor* visitor) const {
  visitor->Visit(this);
}

WaterAnimal::WaterAnimal(std::string species, int age, int size, int swim_speed) :
    Animal(species, age, size), swim_speed_(swim_speed) {}

int WaterAnimal::swim_speed() const {
  return swim_speed_;
}

void WaterAnimal::Accept(Visitor* visitor) const {
  visitor->Visit(this);
}

Bird::Bird(std::string species, int age, int size, int fly_speed) :
    Animal(species, age, size), fly_speed_(fly_speed) {}

int Bird::fly_speed() const {
  return fly_speed_;
}

void Bird::Accept(Visitor* visitor) const {
  visitor->Visit(this);
}

// Specific animal classes

class Dog : public LandAnimal {
 public:
  Dog(int age, int size, int sprint_speed) : LandAnimal("Dog", age, size, sprint_speed) {}
};

class Cat : public LandAnimal {
 public:
  Cat(int age, int size, int sprint_speed) : LandAnimal("Cat", age, size, sprint_speed) {}
};

class Shark : public WaterAnimal {
 public:
  Shark(int age, int size, int swim_speed) : WaterAnimal("Shark", age, size, swim_speed) {}
};

class Dolphin : public WaterAnimal {
 public:
  Dolphin(int age, int size, int swim_speed) : WaterAnimal("Dolphin", age, size, swim_speed) {}
};

class Eagle : public Bird {
 public:
  Eagle(int age, int size, int fly_speed) : Bird("Eagle", age, size, fly_speed) {}
};

class Sparrow : public Bird {
 public:
  Sparrow(int age, int size, int fly_speed) : Bird("Sparrow", age, size, fly_speed) {}
};

int main() {
  PrintVisitor print_visitor;

  Dog dog(5, 30, 60);
  dog.Accept(&print_visitor);

  Cat cat(3, 10, 50);
  cat.Accept(&print_visitor);

  Shark shark(8, 200, 40);
  shark.Accept(&print_visitor);

  Dolphin dolphin(6, 150, 30);
  dolphin.Accept(&print_visitor);

  Eagle eagle(4, 5, 100);
  eagle.Accept(&print_visitor);

  Sparrow sparrow(2, 1, 20);
  sparrow.Accept(&print_visitor);
  return 0;
}