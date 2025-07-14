#include <iostream>

class GasInUsa {
 public:
  /*
    1 Gallon of gas costs $3.
    Buy(dollars) method returns the number of gallons that can be bought with the given dollars.
  */
  GasInUsa() : unit_price_(3) {}

  double unit_price() const {
    return unit_price_;
  }

  double Buy(double dollars) const {
    if (dollars < 0) {
      std::cerr << "Error: Cannot buy with negative dollars." << std::endl;
      return 0;
    }
    return dollars / unit_price_;
  }

 private:
  double unit_price_;
};

int main() {
  /*
    Assuming 1 USD = 1200 KRW for conversion.
    Convert won to dollars and then buy gas in gallons.
  */

  GasInUsa* gas_in_usa = new GasInUsa();
  double won = 36000;  // Korean Won

  /*
    GasInUsa::Buy method expects dollars, and returns gallons.
    Thus, providing money in Won will result in an incorrect calculation.
  */
  double liters = gas_in_usa->Buy(won); 

  /*
    The current result is wrong.
    The correct result would be approximately 37.8541 liters.
  */
  std::cout << "With " << won << " Won, you can buy " << liters << " liters of gas." << std::endl;

  delete gas_in_usa;
  return 0;
}