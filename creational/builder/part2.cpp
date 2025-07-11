#include <iostream>
#include <string>

class Pizza {
 public:
  void set_dough(const std::string& dough) {
    dough_ = dough;
  }

  void set_sauce(const std::string& sauce) {
    sauce_ = sauce;
  }

  void set_cheese(const std::string& cheese) {
    cheese_ = cheese;
  }

  void set_topping(const std::string& topping) {
    topping_ = topping;
  }

  void ShowPizzaInfo() const {
    std::cout << "Pizza Details:" << std::endl;
    std::cout << "Dough: " << dough_ << std::endl;
    std::cout << "Sauce: " << sauce_ << std::endl;
    std::cout << "Cheese: " << cheese_ << std::endl;
    std::cout << "Topping: " << topping_ << std::endl;
    std::cout << "-----------------------" << std::endl;
  }

 private:
  std::string dough_;
  std::string sauce_;
  std::string cheese_;
  std::string topping_;
};

class PizzaBuilder {
 public:
  PizzaBuilder() {
    Reset();
  }

  void Reset() {
    pizza_ = Pizza();
  }

  virtual void SetDough() = 0;

  virtual void SetSauce() = 0;

  virtual void SetCheese() = 0;

  virtual void SetTopping() = 0;

  Pizza Build() {
    auto ret = pizza_;
    Reset();  // Reset the builder for future use
    return ret;
  }

 protected:
  Pizza pizza_;
};

class ItalianPizzaBuilder : public PizzaBuilder {
 public:
  void SetDough() override {
    pizza_.set_dough("Thin Crust");
  }

  void SetSauce() override {
    pizza_.set_sauce("Tomato Sauce");
  }

  void SetCheese() override {
    pizza_.set_cheese("Mozzarella Cheese");
  }

  void SetTopping() override {
    pizza_.set_topping("Pepperoni");
  }
};

class AmericanPizzaBuilder : public PizzaBuilder {
 public:
  void SetDough() override {
    pizza_.set_dough("Thick Crust");
  }

  void SetSauce() override {
    pizza_.set_sauce("Barbecue Sauce");
  }

  void SetCheese() override {
    pizza_.set_cheese("Cheddar Cheese");
  }

  void SetTopping() override {
    pizza_.set_topping("Sausage and Peppers");
  }
};

int main() {
  ItalianPizzaBuilder italian_builder;
  italian_builder.SetDough();
  italian_builder.SetSauce();
  italian_builder.SetCheese();
  italian_builder.SetTopping();
  Pizza italian_pizza = italian_builder.Build();
  italian_pizza.ShowPizzaInfo();

  AmericanPizzaBuilder american_builder;
  american_builder.SetDough();
  american_builder.SetSauce();
  american_builder.SetCheese();
  american_builder.SetTopping();
  Pizza american_pizza = american_builder.Build();
  american_pizza.ShowPizzaInfo();

  return 0;
}
