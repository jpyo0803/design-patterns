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

class GasImportInterface {
 public:
  virtual ~GasImportInterface() = default;

  /*
    Buy in korean money unit (Won)
  */
  virtual double Buy(double won) const = 0;
};

class BuyGasFromUsaAdapter : public GasImportInterface {
  /*
    Assuming 1 gallon = 3.78541 liters for conversion
    Convert gallons to liters.
  */
  constexpr static double kLitersToGallon = 3.78541;  // Conversion factor from gallons to liters

 public:
  explicit BuyGasFromUsaAdapter(double exchange_rate) :
      gas_in_usa_(new GasInUsa()), exchange_rate_(exchange_rate) {}

  /*
    NOTE(jpyo0803): Deleted default constructor as exchange rate is required.
    Deleted copy constructor and assignment operator to prevent copying,
    as shallow copy of GasInUsa could lead to double free issues.
  */
  BuyGasFromUsaAdapter() = delete;
  BuyGasFromUsaAdapter(const BuyGasFromUsaAdapter&) = delete;
  BuyGasFromUsaAdapter& operator=(const BuyGasFromUsaAdapter&) = delete;

  ~BuyGasFromUsaAdapter() {
    delete gas_in_usa_;
  }

  double Buy(double won) const override {
    if (won < 0) {
      std::cerr << "Error: Cannot buy with negative won." << std::endl;
      return 0;
    }
    double dollars = won / exchange_rate_;
    double gas_in_gallon = gas_in_usa_->Buy(dollars);

    double gas_in_liters = gas_in_gallon * kLitersToGallon;
    return gas_in_liters;
  }

 private:
  const double exchange_rate_;  // 1 USD = 1200 KRW

  GasInUsa* gas_in_usa_;
};

int main() {
  /*
    Assuming 1 USD = 1200 KRW for conversion.
    Convert won to dollars and then buy gas in gallons.
  */

  constexpr double exchange_rate = 1200.0;  // 1 USD = 1200 KRW
  GasImportInterface* gas_adapter = new BuyGasFromUsaAdapter(exchange_rate);

  double won = 36000;  // Example amount in Korean Won
  double liters = gas_adapter->Buy(won);

  std::cout << "With " << won << " Won, you can buy " << liters << " liters of gas." << std::endl;

  delete gas_adapter;
  return 0;
}