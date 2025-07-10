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

class AbstractFactory {
 public:
  virtual ~AbstractFactory() = default;

  virtual Restaurant* CreateRestaurant() const = 0;

  virtual Food* CreateFood() const = 0;

  virtual Silverware* CreateSilverware() const = 0;
};

class KoreanFactory : public AbstractFactory {
 public:
  KoreanRestaurant* CreateRestaurant() const override {
    return new KoreanRestaurant();
  }

  Kimchi* CreateFood() const override {
    return new Kimchi();
  }

  Chopsticks* CreateSilverware() const override {
    return new Chopsticks();
  }
};

class ItalianFactory : public AbstractFactory {
 public:
  ItalianRestaurant* CreateRestaurant() const override {
    return new ItalianRestaurant();
  }

  Pizza* CreateFood() const override {
    return new Pizza();
  }

  Fork* CreateSilverware() const override {
    return new Fork();
  }
};

class Guest {
 public:
  Guest(AbstractFactory* factory) : factory_(factory) {
    /*
      Force guests to use the abstract factory,
      which guarantees the creations of compatible objects.
      This also decouples specific restaurants, food, silverwares from guest
    */

    restaurant_ = factory_->CreateRestaurant();
    food_ = factory_->CreateFood();
    silverware_ = factory_->CreateSilverware();
  }

  ~Guest() {
    if (factory_) delete factory_;
    if (restaurant_) delete restaurant_;
    if (food_) delete food_;
    if (silverware_) delete silverware_;
  }

  void DoAction() const {
    std::cout << "Guest eats \'" << food_->name() << "\' with \'" << silverware_->name()
              << "\' in a \'" << restaurant_->name() << "\'\n";
  }

 private:
  AbstractFactory* factory_;

  Restaurant* restaurant_;
  Food* food_;
  Silverware* silverware_;
};

int main() {
  Guest korean_guest(new KoreanFactory());
  korean_guest.DoAction();

  std::cout << "-----------------------" << std::endl;

  Guest italian_guest(new ItalianFactory());
  italian_guest.DoAction();

  return 0;
}