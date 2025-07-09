#include <iostream>
#include <string>

/*
  Abstract class for 'Apple' and 'Orange'
*/
class Fruit {
 public:
  Fruit(std::string name, int unit_price, int quantity) :
      name_(name), unit_price_(unit_price), quantity_(quantity) {}

  virtual ~Fruit() = default;

  std::string name() const {
    return name_;
  }

  int unit_price() const {
    return unit_price_;
  }

  int quantity() const {
    return quantity_;
  }

 private:
  std::string name_;

  int unit_price_;
  int quantity_;
};

class Apple : public Fruit {
 public:
  Apple() : Fruit("Apple", 100, 50) {}
};

class Orange : public Fruit {
 public:
  Orange() : Fruit("Orange", 75, 100) {}
};

class Company {
 public:
  /*
    Unlike Part 2, this Show method can be commonly used for
    both 'Apple' and 'Orange'
  */

  void ShowFruitInfo() const {
    Fruit* fruit = CreateFruit();

    std::string name = fruit->name();
    int unit_price = fruit->unit_price();
    int quantity = fruit->quantity();

    std::cout << "Fruit Name: " << name << std::endl;
    std::cout << "Unit Price: $" << unit_price << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Total Price: $" << unit_price * quantity << std::endl;

    delete fruit;
  }

 private:
  virtual Fruit* CreateFruit() const = 0;
};

class AppleCompany : public Company {
 private:
  Apple* CreateFruit() const override {
    return new Apple();
  }
};

class OrangeCompany : public Company {
 private:
  Orange* CreateFruit() const override {
    return new Orange();
  }
};

int main() {
  AppleCompany* ac = new AppleCompany();
  ac->ShowFruitInfo();
  delete ac;

  std::cout << "-------------------" << std::endl;

  OrangeCompany* oc = new OrangeCompany();
  oc->ShowFruitInfo();
  delete oc;

  return 0;
}