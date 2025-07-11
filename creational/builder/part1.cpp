#include <iostream>
#include <string>

class Pizza {
 public:
  Pizza(std::string dough = "", std::string sauce = "", std::string cheese = "",
        std::string topping = "") :
      dough_(dough), sauce_(sauce), cheese_(cheese), topping_(topping) {}

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

int main() {
  Pizza italian_pizza("Thin Crust", "Tomato Sauce", "Mozzarella Cheese", "Pepperoni");
  italian_pizza.ShowPizzaInfo();

  Pizza american_pizza("Thick Crust", "Barbecue Sauce", "Cheddar Cheese", "Sausage and Peppers");
  american_pizza.ShowPizzaInfo();

  // Having too many input parameters can lead to confusion.
  // Even though some parameters are unused, they still need to be provided.
  // This is an example of a poorly designed constructor.
  // It is better to use a builder pattern or named parameters to avoid confusion.
  Pizza custom_pizza("Pickle", "Banana", "", "");
  custom_pizza.ShowPizzaInfo();

  return 0;
}