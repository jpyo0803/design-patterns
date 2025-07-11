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

/*
  Director can encapsulate the construction process.
  It can use different builders to create different types of pizzas.
  Users do not need to know the details of how pizzas are built.
  They just need to set the builder and call the construction method.
  This allows for a clear separation of concerns and makes the code more maintainable.
*/

class PizzaDirector {
 public:
  void SetBuilder(PizzaBuilder* builder) {
    builder_ = builder;
  }

  void ConstructPizza() {
    if (!builder_) {
      std::cerr << "Builder is not set!" << std::endl;
      return;
    }

    // Director can control the order of building steps
    builder_->SetDough();
    builder_->SetSauce();
    builder_->SetCheese();
    builder_->SetTopping();
  }

 private:
  PizzaBuilder* builder_;
};

int main() {
  PizzaDirector director;

  ItalianPizzaBuilder italian_builder;
  director.SetBuilder(&italian_builder);
  director.ConstructPizza();
  Pizza italian_pizza = italian_builder.Build();
  italian_pizza.ShowPizzaInfo();

  AmericanPizzaBuilder american_builder;
  director.SetBuilder(&american_builder);
  director.ConstructPizza();
  Pizza american_pizza = american_builder.Build();
  american_pizza.ShowPizzaInfo();

  return 0;
}
