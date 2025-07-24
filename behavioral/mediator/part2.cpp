#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Store {
 public:
  Store(std::string name) : name_(name) {}

  virtual ~Store() = default;

  void AddProduct(const std::string& name, int price, int quantity) {
    inventory_[name] = std::make_pair(price, quantity);
  }

  std::tuple<int, int, std::string> GetPriceAndQuantity(const std::string& name) const {
    auto it = inventory_.find(name);
    if (it != inventory_.end()) {
      return std::make_tuple(it->second.first, it->second.second, name_);
    }
    return {-1, 0, ""};  // Not found
  }

  bool SellProduct(const std::string& name, int quantity) {
    auto it = inventory_.find(name);
    if (it != inventory_.end() && it->second.second >= quantity) {
      it->second.second -= quantity;
      return true;
    } else {
      return false;
    }
  }

  std::string name() const {
    return name_;
  }

 private:
  std::map<std::string, std::pair<int, int>> inventory_;  // product name, (price, quantity)
  std::string name_;
};

class Wallmart : public Store {
 public:
  Wallmart() : Store("Wallmart") {
    AddProduct("Steak", 200, 100);
    AddProduct("Fish", 400, 5);
  }
};

class Kroger : public Store {
 public:
  Kroger() : Store("Kroger") {
    AddProduct("Steak", 100, 50);
    AddProduct("Fish", 200, 50);
  }
};

class Publix : public Store {
 public:
  Publix() : Store("Publix") {
    AddProduct("Steak", 5, 5);
    AddProduct("Fish", 10, 5);
  }
};

/*
  Add Costco
*/

class Costco : public Store {
 public:
  Costco() : Store("Costco") {
    AddProduct("Steak", 1, 1000);
    AddProduct("Fish", 1, 1000);
  }
};

class ShoppingMediator {
 public:
  ShoppingMediator(std::vector<Store*> stores) : stores_(stores) {}

  std::vector<std::tuple<int, int, std::string>> BuyCheapestSteak(int quantity);

  std::vector<std::tuple<int, int, std::string>> BuyMostExpensiveFish(int quantity);

 private:
  std::vector<Store*> stores_;
};

class Restaurant {
 public:
  Restaurant(std::string name) : name_(name) {}

  virtual ~Restaurant() = default;

  std::string name() const {
    return name_;
  }

 private:
  std::string name_;
};

class SteakRestaurant : public Restaurant {
 public:
  SteakRestaurant(ShoppingMediator* mediator) :
      Restaurant("SteakRestaurant"), mediator_(mediator) {}

  void BuyCheapestSteak(int quantity) {
    auto purchase_vec = mediator_->BuyCheapestSteak(quantity);

    std::cout << "SteakRestaurant Successfully bought " << quantity << " steaks from:\n";
    for (const auto& purchase : purchase_vec) {
      std::cout << " - " << std::get<1>(purchase) << " steaks at $" << std::get<0>(purchase)
                << " from " << std::get<2>(purchase) << "\n";
    }
  }

 private:
  ShoppingMediator* mediator_;
};

class SushiRestaurant : public Restaurant {
 public:
  SushiRestaurant(ShoppingMediator* mediator) :
      Restaurant("SushiRestaurant"), mediator_(mediator) {}

  void BuyMostExpensiveFish(int quantity) {
    auto purchase_vec = mediator_->BuyMostExpensiveFish(quantity);

    std::cout << "SushiRestaurant Successfully bought " << quantity << " fish from:\n";
    for (const auto& purchase : purchase_vec) {
      std::cout << " - " << std::get<1>(purchase) << " fish at $" << std::get<0>(purchase)
                << " from " << std::get<2>(purchase) << "\n";
    }
  }

 private:
  ShoppingMediator* mediator_;
};

std::vector<std::tuple<int, int, std::string>> ShoppingMediator::BuyCheapestSteak(int quantity) {
  std::vector<std::tuple<int, int, Store*>> catalog_vec;  // price, quantity, store name
  for (const auto& store : stores_) {
    auto catalog = store->GetPriceAndQuantity("Steak");
    catalog_vec.push_back(std::make_tuple(std::get<0>(catalog), std::get<1>(catalog), store));
  }

  // Find the cheapest option
  std::sort(catalog_vec.begin(), catalog_vec.end(),
            [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });

  std::vector<std::tuple<int, int, std::string>> purchase_vec;  // price, quantity, store name

  int remaining_quantity = quantity;

  for (const auto& item : catalog_vec) {
    int price, quantity;
    Store* store;
    std::tie(price, quantity, store) = item;

    if (quantity > 0 && remaining_quantity > 0) {
      int purchase_quantity = std::min(quantity, remaining_quantity);
      if (store->SellProduct("Steak", purchase_quantity)) {
        purchase_vec.push_back(std::make_tuple(price, purchase_quantity, store->name()));
        remaining_quantity -= purchase_quantity;
      }
    }
    if (remaining_quantity <= 0) {
      break;
    }
  }

  return purchase_vec;  // Return the purchase details
}

std::vector<std::tuple<int, int, std::string>> ShoppingMediator::BuyMostExpensiveFish(
    int quantity) {
  std::vector<std::tuple<int, int, Store*>> catalog_vec;  // price, quantity, store name
  for (const auto& store : stores_) {
    auto catalog = store->GetPriceAndQuantity("Fish");
    catalog_vec.push_back(std::make_tuple(std::get<0>(catalog), std::get<1>(catalog), store));
  }

  // Find the most expensive option
  std::sort(catalog_vec.begin(), catalog_vec.end(),
            [](const auto& a, const auto& b) { return std::get<0>(a) > std::get<0>(b); });

  std::vector<std::tuple<int, int, std::string>> purchase_vec;  // price, quantity, store name

  int remaining_quantity = quantity;

  for (const auto& item : catalog_vec) {
    int price, quantity;
    Store* store;
    std::tie(price, quantity, store) = item;

    if (quantity > 0 && remaining_quantity > 0) {
      int purchase_quantity = std::min(quantity, remaining_quantity);
      if (store->SellProduct("Fish", purchase_quantity)) {
        purchase_vec.push_back(std::make_tuple(price, purchase_quantity, store->name()));
        remaining_quantity -= purchase_quantity;
      }
    }
    if (remaining_quantity <= 0) {
      break;
    }
  }

  return purchase_vec;  // Return the purchase details
}

int main() {
  {
    Wallmart wallmart;
    Kroger kroger;
    Publix publix;

    ShoppingMediator mediator({&wallmart, &kroger, &publix});

    SteakRestaurant steak_restaurant(&mediator);
    steak_restaurant.BuyCheapestSteak(10);

    SushiRestaurant sushi_restaurant(&mediator);
    sushi_restaurant.BuyMostExpensiveFish(10);
  }

  {
    Wallmart wallmart;
    Kroger kroger;
    Publix publix;
    Costco costco;

    ShoppingMediator mediator({&wallmart, &kroger, &publix, &costco});

    SteakRestaurant steak_restaurant(&mediator);
    steak_restaurant.BuyCheapestSteak(10);

    SushiRestaurant sushi_restaurant(&mediator);
    sushi_restaurant.BuyMostExpensiveFish(10);
  }
  return 0;
}