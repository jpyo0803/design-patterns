#include <iostream>

/*
  Originally, the 'AppleCompany' only sells 'Apple' product.
*/
class Apple {
 public:
  Apple() : unit_price_(100), quantity_(50) {}

  int unit_price() const {
    return unit_price_;
  }

  int quantity() const {
    return quantity_;
  }

 private:
  int unit_price_;
  int quantity_;
};

class AppleCompany {
 public:
  void ShowAppleInfo() const {
    Apple* apple = new Apple();

    int unit_price = apple->unit_price();
    int quantity = apple->quantity();

    std::cout << "Fruit Name: Apple" << std::endl;
    std::cout << "Unit Price: $" << unit_price << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Total Price: $" << unit_price * quantity << std::endl;

    delete apple;
  }
};

int main() {
  AppleCompany* ac = new AppleCompany();
  ac->ShowAppleInfo();

  return 0;
}