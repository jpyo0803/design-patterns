#include <iostream>
#include <string>

/*
  This example demonstrates a situation where visitor pattern is not used.

  Assume you need to add a feature to print information about different types of animals.

  LandAnimal, WaterAnimal, and Bird classes must be modified to include a PrintInfo method.

  This violates the Open/Closed Principle, as you need to modify existing classes
  to add new functionality instead of extending them.

  In addition, this violates the Single Responsibility Principle,
  as each animal class now has to handle both its own properties and the printing logic.
*/

class Animal {
 public:
  Animal(std::string species, int age, int size) : species_(species), age_(age), size_(size) {}

  virtual void PrintInfo() const = 0;

  virtual ~Animal() = default;

  std::string species() const {
    return species_;
  }

  int age() const {
    return age_;
  }

  int size() const {
    return size_;
  }

 private:
  std::string species_;
  int age_;
  int size_;
};

class LandAnimal : public Animal {
 public:
  LandAnimal(std::string species, int age, int size, int sprint_speed) :
      Animal(species, age, size), sprint_speed_(sprint_speed) {}

  void PrintInfo() const override {
    std::cout << "Land Animal: Species = " << species() << ", Age = " << age()
              << ", Size = " << size() << ", Sprint Speed = " << sprint_speed_ << std::endl;
  }

 private:
  int sprint_speed_;
};

class WaterAnimal : public Animal {
 public:
  WaterAnimal(std::string species, int age, int size, int swim_speed) :
      Animal(species, age, size), swim_speed_(swim_speed) {}

  void PrintInfo() const override {
    std::cout << "Water Animal: Species = " << species() << ", Age = " << age()
              << ", Size = " << size() << ", Swim Speed = " << swim_speed_ << std::endl;
  }

 private:
  int swim_speed_;
};

class Bird : public Animal {
 public:
  Bird(std::string species, int age, int size, int fly_speed) :
      Animal(species, age, size), fly_speed_(fly_speed) {}

  void PrintInfo() const override {
    std::cout << "Bird: Species = " << species() << ", Age = " << age() << ", Size = " << size()
              << ", Fly Speed = " << fly_speed_ << std::endl;
  }

 private:
  int fly_speed_;
};

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
  Dog dog(5, 30, 60);
  dog.PrintInfo();

  Cat cat(3, 10, 50);
  cat.PrintInfo();

  Shark shark(8, 200, 40);
  shark.PrintInfo();

  Dolphin dolphin(6, 150, 30);
  dolphin.PrintInfo();

  Eagle eagle(4, 5, 100);
  eagle.PrintInfo();

  Sparrow sparrow(2, 1, 20);
  sparrow.PrintInfo();
  return 0;
}