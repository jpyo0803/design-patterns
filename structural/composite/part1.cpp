#include <iostream>
#include <string>
#include <vector>

class Component {
 public:
  virtual ~Component() = default;

  virtual double GetPrice() const = 0;
};

class Item : public Component {
 public:
  Item(const std::string& name, double price) : name_(name), price_(price) {}

  double GetPrice() const override {
    return price_;
  }

  std::string name() const {
    return name_;
  }

 private:
  std::string name_;

  double price_;
};

class Box : public Component {
 public:
  void Add(Component* component) {
    components_.push_back(component);
  }

  /*
    void Remove(Component* component) {
      // Implementation to remove a component
      // Not implemented for simplicity
    }
  */

  double GetPrice() const override {
    // Assume box costs nothing

    double total_price = 0.0;
    for (const auto& component : components_) {
      total_price += component->GetPrice();
    }

    return total_price;
  }

 private:
  std::vector<Component*> components_;  // Component can be Item or Box
};

int main() {
  // Create some items and boxes to demonstrate the Composite pattern
  Item* apple = new Item("Apple", 1.0);
  Item* banana = new Item("Banana", 0.5);
  Item* orange = new Item("Orange", 0.75);
  Item* mango = new Item("Mango", 1.5);
  Item* blueberry = new Item("Blueberry", 0.2);

  Box* main_fruit_box = new Box();
  Box* big_box = new Box();
  Box* small_box = new Box();
  Box* smallest_box = new Box();

  smallest_box->Add(blueberry);

  big_box->Add(apple);
  big_box->Add(banana);
  big_box->Add(smallest_box);

  small_box->Add(orange);

  main_fruit_box->Add(big_box);
  main_fruit_box->Add(small_box);
  main_fruit_box->Add(mango);

  /*
    The structure of the fruit box is as follows:

    (main_fruit_box)
    ├── (big_box)
    │   ├── (apple)
    │   ├── (banana)
    │   └── (smallest_box)
    │       └── (blueberry)
    ├── (small_box)
    │   └── (orange)
    └── (mango)
  */

  std::cout << "Total price of the main fruit box: $" << main_fruit_box->GetPrice() << std::endl;

  // Clean up memory
  delete main_fruit_box;
  delete big_box;
  delete small_box;
  delete smallest_box;
  delete apple;
  delete banana;
  delete orange;
  delete mango;
  delete blueberry;

  return 0;
}