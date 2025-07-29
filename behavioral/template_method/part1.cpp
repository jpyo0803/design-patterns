#include <iostream>

/*
  This code demonstrates the problem of code duplication in pizza making.
  Most methods in each pizza maker class are the same, leading to redundancy.
  The solution is to use the Template Method pattern to define a skeleton of the pizza making
  process
*/

class CheesePizzaMaker {
 public:
  void Make() {
    MakeDough();
    PutTomatoSauce();
    PutCheese();
    Bake();
  }

  void MakeDough() {
    std::cout << "- Making dough for pizza" << std::endl;
  }

  void PutTomatoSauce() {
    std::cout << "- Putting tomato sauce on pizza" << std::endl;
  }

  void PutCheese() {
    std::cout << "- Putting cheese on pizza" << std::endl;
  }

  void Bake() {
    std::cout << "- Baking pizza" << std::endl;
  }
};

class PepperoniPizzaMaker {
 public:
  void Make() {
    MakeDough();
    PutTomatoSauce();
    PutCheese();
    PutPepperoni();
    Bake();
  }

  void MakeDough() {
    std::cout << "- Making dough for pizza" << std::endl;
  }

  void PutTomatoSauce() {
    std::cout << "- Putting tomato sauce on pizza" << std::endl;
  }

  void PutCheese() {
    std::cout << "- Putting cheese on pizza" << std::endl;
  }

  void PutPepperoni() {
    std::cout << "- Putting pepperoni on pizza" << std::endl;
  }

  void Bake() {
    std::cout << "- Baking pizza" << std::endl;
  }
};

class CheeseCrustedPepperoniPizzaMaker {
 public:
  void Make() {
    MakeDough();
    PutTomatoSauce();
    PutCheese();
    PutPepperoni();
    Bake();
  }

  void MakeDough() {
    std::cout << "- Making cheese-crusted dough for pizza" << std::endl;
  }

  void PutTomatoSauce() {
    std::cout << "- Putting tomato sauce on pizza" << std::endl;
  }

  void PutCheese() {
    std::cout << "- Putting cheese on pizza" << std::endl;
  }

  void PutPepperoni() {
    std::cout << "- Putting pepperoni on pizza" << std::endl;
  }

  void Bake() {
    std::cout << "- Baking pizza" << std::endl;
  }
};

int main() {
  {
    CheesePizzaMaker cheese_pizza_maker;
    std::cout << "Making Cheese Pizza:" << std::endl;
    cheese_pizza_maker.Make();
    std::cout << std::endl;
  }
  {
    PepperoniPizzaMaker pepperoni_pizza_maker;
    std::cout << "Making Pepperoni Pizza:" << std::endl;
    pepperoni_pizza_maker.Make();
    std::cout << std::endl;
  }
  {
    CheeseCrustedPepperoniPizzaMaker cheese_crusted_pepperoni_pizza_maker;
    std::cout << "Making Cheese Crusted Pepperoni Pizza:" << std::endl;
    cheese_crusted_pepperoni_pizza_maker.Make();
    std::cout << std::endl;
  }
  return 0;
}