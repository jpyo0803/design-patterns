/*
  This is an example where Mediator pattern is not used.
  The code is not scalable and not maintainable.

  Try add more stores (e.g. Costco), and you will see how complicated the code becomes.
  This is a good example to understand why Mediator pattern is useful.
*/

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Wallmart {
 public:
  Wallmart() : name_("Wallmart") {
    inventory_.insert({"Steak", std::make_pair(200, 100)});
    inventory_.insert({"Fish", std::make_pair(400, 5)});
  }

  std::tuple<int, int, std::string> GetPriceAndQuantity(std::string product) const {
    auto it = inventory_.find(product);
    if (it != inventory_.end()) {
      return std::make_tuple(it->second.first, it->second.second,
                             name_);  // Return price, quantity, and store name
    }
    return {-1, 0, ""};  // Not found
  }

  bool Sell(std::string product, int quantity) {
    auto it = inventory_.find(product);
    if (it != inventory_.end() && it->second.second >= quantity) {
      it->second.second -= quantity;
      return true;
    } else {
      return false;
    }
  }

 private:
  std::map<std::string, std::pair<int, int>> inventory_;  // product name, (price, quantity)
  std::string name_;
};

class Kroger {
 public:
  Kroger() : name_("Kroger") {
    inventory_.insert({"Steak", std::make_pair(100, 50)});
    inventory_.insert({"Fish", std::make_pair(200, 50)});
  }

  std::tuple<int, int, std::string> GetPriceAndQuantity(std::string product) const {
    auto it = inventory_.find(product);
    if (it != inventory_.end()) {
      return std::make_tuple(it->second.first, it->second.second,
                             name_);  // Return price, quantity, and store name
    }
    return {-1, 0, ""};  // Not found
  }

  bool Sell(std::string product, int quantity) {
    auto it = inventory_.find(product);
    if (it != inventory_.end() && it->second.second >= quantity) {
      it->second.second -= quantity;
      return true;
    } else {
      return false;
    }
  }

 private:
  std::map<std::string, std::pair<int, int>> inventory_;  // product name, (price, quantity)
  std::string name_;
};

class Publix {
 public:
  Publix() : name_("Publix") {
    inventory_.insert({"Steak", std::make_pair(5, 5)});
    inventory_.insert({"Fish", std::make_pair(10, 5)});
  }

  std::tuple<int, int, std::string> GetPriceAndQuantity(std::string product) const {
    auto it = inventory_.find(product);
    if (it != inventory_.end()) {
      return {it->second.first, it->second.second,
              name_};  // Return price, quantity, and store name
    }
    return {-1, 0, ""};  // Not found
  }

  bool Sell(std::string product, int quantity) {
    auto it = inventory_.find(product);
    if (it != inventory_.end() && it->second.second >= quantity) {
      it->second.second -= quantity;
      return true;
    } else {
      return false;
    }
  }

 private:
  std::map<std::string, std::pair<int, int>> inventory_;  // product name, (price, quantity)
  std::string name_;
};

class SteakRestaurant {
 public:
  SteakRestaurant(Wallmart* wallmart, Kroger* kroger, Publix* publix) :
      wallmart_(wallmart), kroger_(kroger), publix_(publix) {}

  void BuyCheapestSteak(int quantity) {
    /*
      Buy cheapest steak from the stores.

      Logic is very complicated and not scalable. (Consider adding more stores)
    */

    std::vector<std::tuple<int, int, std::string>> catalog_vec;
    catalog_vec.push_back(wallmart_->GetPriceAndQuantity("Steak"));
    catalog_vec.push_back(kroger_->GetPriceAndQuantity("Steak"));
    catalog_vec.push_back(publix_->GetPriceAndQuantity("Steak"));

    /*
      Sort the vector by price in ascending order.
      This is a simple way to find the cheapest steak.
      In a real-world scenario, you might want to use a more efficient data structure or algorithm.
    */
    std::sort(
        catalog_vec.begin(), catalog_vec.end(),
        [](const std::tuple<int, int, std::string>& a, const std::tuple<int, int, std::string>& b) {
          return std::get<0>(a) < std::get<0>(b);
        });

    std::vector<std::tuple<int, int, std::string>> purchase_vec;  // price, quantity, store name

    int remaining_quantity = quantity;

    /*
      Greedily buy the cheapest steak from the stores until the required quantity is met.
    */
    for (const auto& item : catalog_vec) {
      int price, quantity;
      std::string store_name;
      std::tie(price, quantity, store_name) = item;

      if (quantity > 0) {
        int quantity_to_buy = std::min(remaining_quantity, quantity);
        if (store_name == "Wallmart") {
          if (wallmart_->Sell("Steak", quantity_to_buy)) {
            purchase_vec.push_back(std::make_tuple(price, quantity_to_buy, store_name));
            remaining_quantity -= quantity_to_buy;
          }
        } else if (store_name == "Kroger") {
          if (kroger_->Sell("Steak", quantity_to_buy)) {
            purchase_vec.push_back(std::make_tuple(price, quantity_to_buy, store_name));
            remaining_quantity -= quantity_to_buy;
          }
        } else if (store_name == "Publix") {
          if (publix_->Sell("Steak", quantity_to_buy)) {
            purchase_vec.push_back(std::make_tuple(price, quantity_to_buy, store_name));
            remaining_quantity -= quantity_to_buy;
          }
        }
        if (remaining_quantity <= 0) {
          break;
        }
      }
    }

    // Print the purchase details
    std::cout << "SteakRestaurant Successfully bought " << quantity << " steaks from:\n";
    for (const auto& purchase : purchase_vec) {
      int price, quantity;
      std::string store_name;

      std::tie(price, quantity, store_name) = purchase;
      std::cout << "- " << quantity << " steaks at $" << price << " from " << store_name << "\n";
    }
  }

 private:
  Wallmart* wallmart_;
  Kroger* kroger_;
  Publix* publix_;
};

class SushiRestaurant {
 public:
  SushiRestaurant(Wallmart* wallmart, Kroger* kroger, Publix* publix) :
      wallmart_(wallmart), kroger_(kroger), publix_(publix) {}

  void BuyMostExpensiveFish(int quantity) {
    /*
      Buy most expensive fish from the stores.

      Logic is very complicated and not scalable. (Consider adding more stores)
    */

    std::vector<std::tuple<int, int, std::string>> catalog_vec;
    catalog_vec.push_back(wallmart_->GetPriceAndQuantity("Fish"));
    catalog_vec.push_back(kroger_->GetPriceAndQuantity("Fish"));
    catalog_vec.push_back(publix_->GetPriceAndQuantity("Fish"));

    /*
      Sort the vector by price in descending order.
      This is a simple way to find the most expensive fish.
      In a real-world scenario, you might want to use a more efficient data structure or algorithm.
    */
    std::sort(
        catalog_vec.begin(), catalog_vec.end(),
        [](const std::tuple<int, int, std::string>& a, const std::tuple<int, int, std::string>& b) {
          return std::get<0>(a) > std::get<0>(b);
        });

    std::vector<std::tuple<int, int, std::string>> purchase_vec;  // price, quantity, store name

    int remaining_quantity = quantity;

    /*
      Greedily buy the most expensive fish from the stores until the required quantity is met.
    */
    for (const auto& price_and_quantity : catalog_vec) {
      int price, quantity;
      std::string store_name;
      std::tie(price, quantity, store_name) = price_and_quantity;

      if (quantity > 0) {
        int quantity_to_buy = std::min(remaining_quantity, quantity);
        if (store_name == "Wallmart") {
          if (wallmart_->Sell("Fish", quantity_to_buy)) {
            purchase_vec.push_back(std::make_tuple(price, quantity_to_buy, store_name));
            remaining_quantity -= quantity_to_buy;
          }
        } else if (store_name == "Kroger") {
          if (kroger_->Sell("Fish", quantity_to_buy)) {
            purchase_vec.push_back(std::make_tuple(price, quantity_to_buy, store_name));
            remaining_quantity -= quantity_to_buy;
          }
        } else if (store_name == "Publix") {
          if (publix_->Sell("Fish", quantity_to_buy)) {
            purchase_vec.push_back(std::make_tuple(price, quantity_to_buy, store_name));
            remaining_quantity -= quantity_to_buy;
          }
        }
        if (remaining_quantity <= 0) {
          break;
        }
      }
    }

    // Print the purchase details
    std::cout << "SushiRestaurant Successfully bought " << quantity << " fish from:\n";
    for (const auto& purchase : purchase_vec) {
      int price, quantity;
      std::string store_name;

      std::tie(price, quantity, store_name) = purchase;
      std::cout << "- " << quantity << " fish at $" << price << " from " << store_name << "\n";
    }
  }

 private:
  Wallmart* wallmart_;
  Kroger* kroger_;
  Publix* publix_;
};

int main() {
  {
    Wallmart wallmart;
    Kroger kroger;
    Publix publix;

    SteakRestaurant steak_restaurant(&wallmart, &kroger, &publix);
    steak_restaurant.BuyCheapestSteak(10);

    SushiRestaurant sushi_restaurant(&wallmart, &kroger, &publix);
    sushi_restaurant.BuyMostExpensiveFish(10);
  }
  return 0;
}