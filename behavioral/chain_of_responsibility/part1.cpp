#include <iostream>
#include <map>
#include <string>
#include <utility>

/*
  FruitShop keeps track of fruits and their quantities and prices
*/
class FruitShop {
 public:
  FruitShop() {
    // Initialize some fruits with prices and quantities
    fruits_["Apple"] = {1.0, 100};
    fruits_["Banana"] = {0.5, 200};
    fruits_["Cherry"] = {2.0, 50};
  }

  void SellFruit(std::string fruit, int quantity) {
    if (fruits_.find(fruit) == fruits_.end()) {
      std::cout << "Fruit not available." << std::endl;
      return;
    }

    auto& [price, quantity_available] = fruits_[fruit];
    if (quantity > quantity_available) {
      std::cout << "Not enough " << fruit << " available." << std::endl;
      return;
    }
    quantity_available -= quantity;
    std::cout << "Sold " << quantity << " " << fruit << "(s) at $" << price << " each. Total: $"
              << price * quantity << std::endl;
  }

  bool IsFruitExists(std::string fruit) const {
    return fruits_.find(fruit) != fruits_.end();
  }

  double GetPrice(std::string fruit) const {
    if (fruits_.find(fruit) == fruits_.end()) {
      std::cout << "Fruit not available." << std::endl;
      return 0.0;
    }
    return fruits_.at(fruit).first;
  }

  int GetQuantity(std::string fruit) const {
    if (fruits_.find(fruit) == fruits_.end()) {
      std::cout << "Fruit not available." << std::endl;
      return 0;
    }
    return fruits_.at(fruit).second;
  }

 private:
  std::map<std::string, std::pair<double, int>> fruits_;  // fruit name to (price, quantity) mapping
};

class Order {
 public:
  Order(FruitShop* fruit_shop, std::string fruit, int quantity, double pay_amount) :
      fruit_shop_(fruit_shop), fruit_(fruit), quantity_(quantity), pay_amount_(pay_amount) {}

  FruitShop* fruit_shop() const {
    return fruit_shop_;
  }

  std::string fruit() const {
    return fruit_;
  }

  int quantity() const {
    return quantity_;
  }

  double pay_amount() const {
    return pay_amount_;
  }

 private:
  FruitShop* fruit_shop_;  // Pointer to the FruitShop instance

  std::string fruit_;  // Name of the fruit
  int quantity_;       // Quantity of the fruit ordered
  double pay_amount_;  // Amount to be paid for the order
};

class IHandler {
 public:
  virtual ~IHandler() = default;  // Virtual destructor for proper cleanup

  virtual bool Process(Order* order) = 0;
  virtual void SetNext(IHandler* handler) = 0;
};

class BaseHandler : public IHandler {
 public:
  explicit BaseHandler() : next_handler_(nullptr) {}

  void SetNext(IHandler* handler) override {
    next_handler_ = handler;
  }

  bool Process(Order* order) override {
    if (Handle(order)) {
      if (next_handler_) {
        return next_handler_->Process(order);
      } else {
        return true;  // If this is the last handler, return true
      }
    }
    return false;  // If this handler could not process the order
  }

 protected:
  virtual bool Handle(Order* order) = 0;

  IHandler* next_handler_;  // Pointer to the next handler in the chain
};

class IsFruitExistsHandler : public BaseHandler {
 public:
  bool Handle(Order* order) override {
    if (order->fruit_shop()->IsFruitExists(order->fruit())) {
      std::cout << "Fruit exists in the shop." << std::endl;
      return true;  // Continue to the next handler
    }

    std::cout << "Fruit does not exist in the shop." << std::endl;
    return false;
  }
};

class PriceCheckHandler : public BaseHandler {
 public:
  bool Handle(Order* order) override {
    double price = order->fruit_shop()->GetPrice(order->fruit());
    if (price > 0) {
      std::cout << "Price of " << order->fruit() << " is $" << price << std::endl;
      return true;  // Continue to the next handler
    }

    std::cout << "Price check failed." << std::endl;
    return false;
  }
};

class QuantityCheckHandler : public BaseHandler {
 public:
  bool Handle(Order* order) override {
    int quantity_available = order->fruit_shop()->GetQuantity(order->fruit());
    if (quantity_available >= order->quantity()) {
      std::cout << "Sufficient quantity available." << std::endl;
      return true;  // Continue to the next handler
    }

    std::cout << "Insufficient quantity available." << std::endl;
    return false;
  }
};

class HasEnoughMoneyHandler : public BaseHandler {
 public:
  bool Handle(Order* order) override {
    double price = order->fruit_shop()->GetPrice(order->fruit());
    if (price * order->quantity() <= order->pay_amount()) {
      std::cout << "Customer has enough money." << std::endl;
      return true;  // Order can be processed
    }

    std::cout << "Customer does not have enough money." << std::endl;
    return false;
  }
};

int main() {
  {
    FruitShop fruit_shop;

    IsFruitExistsHandler is_fruit_exists_handler;
    PriceCheckHandler price_check_handler;
    QuantityCheckHandler quantity_check_handler;
    HasEnoughMoneyHandler has_enough_money_handler;

    is_fruit_exists_handler.SetNext(&price_check_handler);
    price_check_handler.SetNext(&quantity_check_handler);
    quantity_check_handler.SetNext(&has_enough_money_handler);

    Order order(&fruit_shop, "Apple", 5, 5.0);

    if (is_fruit_exists_handler.Process(&order)) {
      fruit_shop.SellFruit(order.fruit(), order.quantity());
    } else {
      std::cout << "Order could not be processed." << std::endl;
    }
  }

  std::cout << "----------------------------------------" << std::endl;
  {
    FruitShop fruit_shop;

    IsFruitExistsHandler is_fruit_exists_handler;
    PriceCheckHandler price_check_handler;
    QuantityCheckHandler quantity_check_handler;
    HasEnoughMoneyHandler has_enough_money_handler;

    is_fruit_exists_handler.SetNext(&price_check_handler);
    price_check_handler.SetNext(&quantity_check_handler);
    quantity_check_handler.SetNext(&has_enough_money_handler);

    Order order(&fruit_shop, "Apple", 100, 99.0);

    if (is_fruit_exists_handler.Process(&order)) {
      fruit_shop.SellFruit(order.fruit(), order.quantity());
    } else {
      std::cout << "Order could not be processed." << std::endl;
    }
  }

  std::cout << "----------------------------------------" << std::endl;
  {
    FruitShop fruit_shop;

    IsFruitExistsHandler is_fruit_exists_handler;
    PriceCheckHandler price_check_handler;
    QuantityCheckHandler quantity_check_handler;
    HasEnoughMoneyHandler has_enough_money_handler;

    is_fruit_exists_handler.SetNext(&price_check_handler);
    price_check_handler.SetNext(&quantity_check_handler);
    quantity_check_handler.SetNext(&has_enough_money_handler);

    Order order(&fruit_shop, "Banana", 10, 4.0);

    if (is_fruit_exists_handler.Process(&order)) {
      fruit_shop.SellFruit(order.fruit(), order.quantity());
    } else {
      std::cout << "Order could not be processed." << std::endl;
    }
  }

  std::cout << "----------------------------------------" << std::endl;
  {
    FruitShop fruit_shop;

    IsFruitExistsHandler is_fruit_exists_handler;
    PriceCheckHandler price_check_handler;
    QuantityCheckHandler quantity_check_handler;
    HasEnoughMoneyHandler has_enough_money_handler;

    is_fruit_exists_handler.SetNext(&price_check_handler);
    price_check_handler.SetNext(&quantity_check_handler);
    quantity_check_handler.SetNext(&has_enough_money_handler);

    Order order(&fruit_shop, "Cherry", 51, 100000.0);

    if (is_fruit_exists_handler.Process(&order)) {
      fruit_shop.SellFruit(order.fruit(), order.quantity());
    } else {
      std::cout << "Order could not be processed." << std::endl;
    }
  }

  std::cout << "----------------------------------------" << std::endl;
  {
    FruitShop fruit_shop;

    IsFruitExistsHandler is_fruit_exists_handler;
    PriceCheckHandler price_check_handler;
    QuantityCheckHandler quantity_check_handler;
    HasEnoughMoneyHandler has_enough_money_handler;

    is_fruit_exists_handler.SetNext(&price_check_handler);
    price_check_handler.SetNext(&quantity_check_handler);
    quantity_check_handler.SetNext(&has_enough_money_handler);

    Order order(&fruit_shop, "Mango", 5, 10.0);

    if (is_fruit_exists_handler.Process(&order)) {
      fruit_shop.SellFruit(order.fruit(), order.quantity());
    } else {
      std::cout << "Order could not be processed." << std::endl;
    }
  }

  return 0;
}