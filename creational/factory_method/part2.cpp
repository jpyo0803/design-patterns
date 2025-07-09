#include <iostream>

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

    std::cout << "Product Name: Apple" << std::endl;
    std::cout << "Unit Price: $" << unit_price << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Total Price: $" << unit_price * quantity << std::endl;

    delete apple;
  }
};

/*
  'Orange' product is newly added
*/

class Orange {
 public:
  Orange() : unit_price_(75), quantity_(100) {}

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

/*
  'OrangeCompany' is newly added to show 'Orange' information.
*/

class OrangeCompany {
 public:
  /*
    Notice that 'ShowOrangeInfo' method is very similar to 'ShowAppleInfo'
    method. If possible, it is preferable to reuse the code.
  */
  void ShowOrangeInfo() const {
    Orange* orange = new Orange();

    int unit_price = orange->unit_price();
    int quantity = orange->quantity();

    std::cout << "Fruit Name: Orange" << std::endl;
    std::cout << "Unit Price: $" << unit_price << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Total Price: $" << unit_price * quantity << std::endl;

    delete orange;
  }
};

int main() {
  AppleCompany* ac = new AppleCompany();
  ac->ShowAppleInfo();
  delete ac;

  std::cout << "-------------------" << std::endl;

  OrangeCompany* oc = new OrangeCompany();
  oc->ShowOrangeInfo();
  delete oc;
  return 0;
}