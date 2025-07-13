#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

namespace {
// Function to generate a random number
int64_t GenerateRandomNumber() {
  std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int64_t> distribution(1, 1e15);
  return distribution(generator);
}
}  // namespace

class Singleton {
 public:
  /*
    GetInstance method provides a global point of access to the Singleton instance.
  */

  static Singleton& GetInstance() {
    /*
      You can also use a static local variable instead of a static pointer.

      e.g.)
      static Singleton& GetInstance() {
        static Singleton instance;
        return instance;
      }
    */
    if (instance_ == nullptr) {
      instance_ = new Singleton();
    }
    return *instance_;
  }

  int64_t random_number() const {
    return random_number_;
  }

 private:
  Singleton() : random_number_(GenerateRandomNumber()) {
    std::cout << "Singleton instance created with random number: " << random_number_ << std::endl;
  }

  // Delete copy constructor and assignment operator to prevent copying
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

  static Singleton* instance_;
  const int64_t random_number_;
};

Singleton* Singleton::instance_ = nullptr;

int main() {
  // Access the Singleton instance
  Singleton& singleton_instance = Singleton::GetInstance();

  Singleton& another_instance = Singleton::GetInstance();
  if (&singleton_instance == &another_instance) {
    std::cout << "Both instances are the same." << std::endl;
  } else {
    std::cout << "Instances are different." << std::endl;
  }

  std::cout << "Random number from singleton instance: " << singleton_instance.random_number()
            << std::endl;
  std::cout << "Random number from another instance: " << another_instance.random_number()
            << std::endl;

  return 0;
}