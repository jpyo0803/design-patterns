#include <iostream>
#include <string>

class BaseClass {
 public:
  explicit BaseClass(std::string name) : name_(name) {}

  std::string name() const {
    return name_;
  }

 private:
  std::string name_;
};

class Restaurant : public BaseClass {
 public:
  Restaurant(std::string name) : BaseClass(name) {}
};

class KoreanRestaurant : public Restaurant {
 public:
  KoreanRestaurant() : Restaurant("Korean Restaurant"){};
};

class ItalianRestaurant : public Restaurant {
 public:
  ItalianRestaurant() : Restaurant("Italian Restaurant"){};
};

class Food : public BaseClass {
 public:
  Food(std::string name) : BaseClass(name) {}
};

class Kimchi : public Food {
 public:
  Kimchi() : Food("Kimchi"){};
};

class Pizza : public Food {
 public:
  Pizza() : Food("Pizza"){};
};

class Silverware : public BaseClass {
 public:
  Silverware(std::string name) : BaseClass(name) {}
};

class Chopsticks : public Silverware {
 public:
  Chopsticks() : Silverware("Chopsticks"){};
};

class Fork : public Silverware {
 public:
  Fork() : Silverware("Fork"){};
};

class Guest {
 public:
  Guest() {
    /*
      Objects that are not compatible are created.
      This will result in an undesired behavior.
    */

    restaurant_ = new KoreanRestaurant();
    food_ = new Pizza();
    silverware_ = new Chopsticks();
  }

  ~Guest() {
    if (restaurant_) delete restaurant_;
    if (food_) delete food_;
    if (silverware_) delete silverware_;
  }

  void DoAction() const {
    std::cout << "Guest eats \'" << food_->name() << "\' with \'" << silverware_->name()
              << "\' in a \'" << restaurant_->name() << "\'\n";
  }

 private:
  Restaurant* restaurant_;
  Food* food_;
  Silverware* silverware_;
};

int main() {
  Guest guest;

  /*
    This will output
    "Guest eats 'Pizza' with 'Chopsticks' in a 'Korean Restaurant'"
    , which is not what we want!
  */
  guest.DoAction();

  return 0;
}