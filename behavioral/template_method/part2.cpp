#include <iostream>

class PizzaMaker {
 public:
  void Make() {
    MakeDough();
    PutTomatoSauce();
    PutCheese();
    PutToppings();
    Bake();
  }

 protected:
  virtual void MakeDough() {
    // Default implementation for making dough
    std::cout << "- Making dough for pizza" << std::endl;
  };
  virtual void PutToppings() {
    /*
      Default implementation for putting toppings.
      You may not put any toppings in some pizza types.
    */
  }

 private:
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

// For Cheese Pizza, we can use the default implementation
class CheesePizzaMaker : public PizzaMaker {};

class PepperoniPizzaMaker : public PizzaMaker {
 protected:
  /*
    For Pepperoni Pizza, we override the MakeDough method
    to put pepperoni on the pizza
  */
  void PutToppings() override {
    std::cout << "- Putting pepperoni on pizza" << std::endl;
  }
};

class CheeseCrustedPepperoniPizzaMaker : public PepperoniPizzaMaker {
 protected:
  /*
    For Cheese Crusted Pepperoni Pizza, we override the MakeDough method
    to make cheese-crusted dough.

    In addition, we reuse the PutToppings method from PepperoniPizzaMaker
    to put pepperoni on the pizza.
    This avoids code duplication.
  */
  void MakeDough() override {
    std::cout << "- Making cheese-crusted dough for pizza" << std::endl;
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