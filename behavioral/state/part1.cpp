#include <iostream>

/*
  Assume a car system as below:
  - The car can be in one of three states: Off, Normal, Eco.
  - The car has 6 operations: Start, Stop, Accelerate, Decelerate, SwitchToEco, SwitchToNormal.

  Question: Try adding a new state Sport mode to the car system.
  How many different places do you need to change? all 6 methods (Start, Stop, Accelerate,
  Decelerate, SwitchToEco, SwitchToNormal)
  Answer: 6 different places.
*/

enum class CarState {
  Off,
  Normal,
  Eco,
};

class Car {
 public:
  Car() : state_(CarState::Off), speed_(0) {}

  void Start() {
    if (state_ == CarState::Off) {
      state_ = CarState::Normal;
      std::cout << "Car started in Normal mode." << std::endl;
    } else if (state_ == CarState::Normal) {
      std::cout << "Car is already running in Normal mode." << std::endl;
    } else if (state_ == CarState::Eco) {
      std::cout << "Car is already running in Eco mode." << std::endl;
    }
  }

  void Stop() {
    if (state_ == CarState::Off) {
      std::cout << "Car is already off." << std::endl;
    } else if (state_ == CarState::Normal) {
      if (speed_ > 0) {
        std::cout << "Cannot stop. The car is still moving." << std::endl;
      } else {
        state_ = CarState::Off;
        std::cout << "Car stopped." << std::endl;
      }
    } else if (state_ == CarState::Eco) {
      if (speed_ > 0) {
        std::cout << "Cannot stop. The car is still moving." << std::endl;
      } else {
        state_ = CarState::Off;
        std::cout << "Car stopped." << std::endl;
      }
    }
  }

  void Accelerate() {
    if (state_ == CarState::Off) {
      std::cout << "Cannot accelerate. The car is off." << std::endl;
    } else if (state_ == CarState::Normal) {
      speed_ += 10;
      std::cout << "Car accelerated to " << speed_ << " km/h in Normal mode." << std::endl;
    } else if (state_ == CarState::Eco) {
      speed_ += 5;
      std::cout << "Car accelerated to " << speed_ << " km/h in Eco mode." << std::endl;
    }
  }

  void Decelerate() {
    if (state_ == CarState::Off) {
      std::cout << "Cannot decelerate. The car is off." << std::endl;
    } else if (state_ == CarState::Normal) {
      if (speed_ > 0) {
        speed_ -= 10;
        if (speed_ < 0) speed_ = 0;
        std::cout << "Car decelerated to " << speed_ << " km/h in Normal mode." << std::endl;
      } else {
        std::cout << "Car is already at a standstill." << std::endl;
      }
    } else if (state_ == CarState::Eco) {
      if (speed_ > 0) {
        speed_ -= 5;
        if (speed_ < 0) speed_ = 0;
        std::cout << "Car decelerated to " << speed_ << " km/h in Eco mode." << std::endl;
      } else {
        std::cout << "Car is already at a standstill." << std::endl;
      }
    }
  }

  void LowerDriveMode() {
    if (state_ == CarState::Off) {
      std::cout << "Cannot switch to Eco mode. The car is off." << std::endl;
    } else if (state_ == CarState::Normal) {
      state_ = CarState::Eco;
      std::cout << "Car switched to Eco mode." << std::endl;
    } else if (state_ == CarState::Eco) {
      std::cout << "Car is already in Lowest mode." << std::endl;
    }
  }

  void HigherDriveMode() {
    if (state_ == CarState::Off) {
      std::cout << "Cannot switch to Normal mode. The car is off." << std::endl;
    } else if (state_ == CarState::Eco) {
      state_ = CarState::Normal;
      std::cout << "Car switched to Normal mode." << std::endl;
    } else if (state_ == CarState::Normal) {
      std::cout << "Car is already in Highest mode." << std::endl;
    }
  }

 private:
  CarState state_;
  int speed_;
};

int main() {
  Car my_car;

  my_car.Start();
  for (int i = 0; i < 10; ++i) {
    my_car.Accelerate();
  }
  // car speed should be 100 km/h in Normal mode
  my_car.Decelerate();
  // car speed should be 90 km/h in Normal mode
  my_car.Stop();  // cannot stop because the car is still moving
  my_car.LowerDriveMode();
  // car is now in Eco mode
  my_car.Decelerate();
  // car speed should be 85 km/h in Eco mode
  my_car.Stop();  // cannot stop because the car is still moving
  my_car.Decelerate();
  // car speed should be 80 km/h in Eco mode
  my_car.Accelerate();
  // car speed should be 85 km/h in Eco mode
  my_car.Stop();  // cannot stop because the car is still moving
  for (int i = 0; i < 17; ++i) {
    my_car.Decelerate();
  }
  // car speed should be 0 km/h in Eco mode
  my_car.Stop();  // car stopped

  return 0;
}