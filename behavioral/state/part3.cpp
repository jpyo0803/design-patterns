#include <iostream>

/*
  In this part, State pattern is applied to the car system.

  Question: How many different places do you need to change if you want to add a new state 'Sport'
  mode to the car system?

  Answer: 2 different places (1) Addition of SportState and (2) Change in
  NormalState::HigherDriveMode
*/

class Car;

class State {
 public:
  explicit State(Car* context);
  virtual ~State() = default;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Accelerate() = 0;
  virtual void Decelerate() = 0;
  virtual void LowerDriveMode() = 0;
  virtual void HigherDriveMode() = 0;

 protected:
  Car* context_;
};

class OffState : public State {
 public:
  explicit OffState(Car* context);

  void Start() override;
  void Stop() override;
  void Accelerate() override;
  void Decelerate() override;
  void LowerDriveMode() override;
  void HigherDriveMode() override;
};

class NormalState : public State {
 public:
  explicit NormalState(Car* context);

  void Start() override;
  void Stop() override;
  void Accelerate() override;
  void Decelerate() override;
  void LowerDriveMode() override;
  void HigherDriveMode() override;
};

class EcoState : public State {
 public:
  explicit EcoState(Car* context);

  void Start() override;
  void Stop() override;
  void Accelerate() override;
  void Decelerate() override;
  void LowerDriveMode() override;
  void HigherDriveMode() override;
};

/*
  Provide declaration for SportState

  NOTE(jpyo0803): first place to change
  implementation of this declaration is also part of this change
*/
class SportState : public State {
 public:
  explicit SportState(Car* context);

  void Start() override;
  void Stop() override;
  void Accelerate() override;
  void Decelerate() override;
  void LowerDriveMode() override;
  void HigherDriveMode() override;
};

class Car {
 public:
  Car();
  ~Car();

  void ChangeState(State* new_state);
  void Start();
  void Stop();
  void Accelerate();
  void Decelerate();
  void LowerDriveMode();
  void HigherDriveMode();

  int speed() const;
  void set_speed(int speed);

 private:
  State* state_;
  int speed_;
};

State::State(Car* context) : context_(context) {}

OffState::OffState(Car* context) : State(context) {}

void OffState::Start() {
  std::cout << "Car started in Normal mode." << std::endl;
  context_->ChangeState(new NormalState(context_));
}

void OffState::Stop() {
  // Do nothing, already off
  std::cout << "Car is already off." << std::endl;
}

void OffState::Accelerate() {
  // Cannot accelerate when the car is off
  std::cout << "Cannot accelerate. The car is off." << std::endl;
}

void OffState::Decelerate() {
  // Cannot decelerate when the car is off
  std::cout << "Cannot decelerate. The car is off." << std::endl;
}

void OffState::LowerDriveMode() {
  // Cannot switch to Eco mode when the car is off
  std::cout << "Cannot switch to Eco mode. The car is off." << std::endl;
}

void OffState::HigherDriveMode() {
  // Cannot switch to Normal mode when the car is off
  std::cout << "Cannot switch to Normal mode. The car is off." << std::endl;
}

NormalState::NormalState(Car* context) : State(context) {}

void NormalState::Start() {
  // Do nothing, already in Normal mode
  std::cout << "Car is already running in Normal mode." << std::endl;
}

void NormalState::Stop() {
  if (context_->speed() > 0) {
    std::cout << "Cannot stop. The car is still moving." << std::endl;
  } else {
    std::cout << "Car stopped." << std::endl;
    context_->ChangeState(new OffState(context_));
  }
}

void NormalState::Accelerate() {
  context_->set_speed(context_->speed() + 10);
  std::cout << "Car accelerated to " << context_->speed() << " km/h in Normal mode." << std::endl;
}

void NormalState::Decelerate() {
  if (context_->speed() > 0) {
    context_->set_speed(context_->speed() - 10);
    std::cout << "Car decelerated to " << context_->speed() << " km/h in Normal mode." << std::endl;
  } else {
    std::cout << "Car is already at a standstill." << std::endl;
  }
}

void NormalState::LowerDriveMode() {
  std::cout << "Car switched to Eco mode." << std::endl;
  context_->ChangeState(new EcoState(context_));
}

void NormalState::HigherDriveMode() {
  std::cout << "Car switched to Sport mode." << std::endl;

  /*
    NOTE(jpyo0803): Second place to change.
    This violates OCP, but in reality, this much change is acceptable.
  */
  context_->ChangeState(new SportState(context_));
}

EcoState::EcoState(Car* context) : State(context) {}

void EcoState::Start() {
  // Do nothing, already in Eco mode
  std::cout << "Car is already running in Eco mode." << std::endl;
}

void EcoState::Stop() {
  if (context_->speed() > 0) {
    std::cout << "Cannot stop. The car is still moving." << std::endl;
  } else {
    std::cout << "Car stopped." << std::endl;
    context_->ChangeState(new OffState(context_));
  }
}

void EcoState::Accelerate() {
  context_->set_speed(context_->speed() + 5);
  std::cout << "Car accelerated to " << context_->speed() << " km/h in Eco mode." << std::endl;
}

void EcoState::Decelerate() {
  if (context_->speed() > 0) {
    context_->set_speed(context_->speed() - 5);
    std::cout << "Car decelerated to " << context_->speed() << " km/h in Eco mode." << std::endl;
  } else {
    std::cout << "Car is already at a standstill." << std::endl;
  }
}

void EcoState::LowerDriveMode() {
  // Do nothing, already in Eco mode
  std::cout << "Car is already in Lowest mode." << std::endl;
}

void EcoState::HigherDriveMode() {
  std::cout << "Car switched to Normal mode." << std::endl;
  context_->ChangeState(new NormalState(context_));
}

/*
  Provide implementation for SportState
  NOTE(jpyo0803): this is part of the first change. Not counted as a separate change
*/
SportState::SportState(Car* context) : State(context) {}

void SportState::Start() {
  // Do nothing, already in Sport mode
  std::cout << "Car is already running in Sport mode." << std::endl;
}

void SportState::Stop() {
  if (context_->speed() > 0) {
    std::cout << "Cannot stop. The car is still moving." << std::endl;
  } else {
    std::cout << "Car stopped." << std::endl;
    context_->ChangeState(new OffState(context_));
  }
}

void SportState::Accelerate() {
  context_->set_speed(context_->speed() + 15);
  std::cout << "Car accelerated to " << context_->speed() << " km/h in Sport mode." << std::endl;
}

void SportState::Decelerate() {
  if (context_->speed() > 0) {
    context_->set_speed(context_->speed() - 15);
    std::cout << "Car decelerated to " << context_->speed() << " km/h in Sport mode." << std::endl;
  } else {
    std::cout << "Car is already at a standstill." << std::endl;
  }
}

void SportState::LowerDriveMode() {
  std::cout << "Car switched to Normal mode." << std::endl;
  context_->ChangeState(new NormalState(context_));
}

void SportState::HigherDriveMode() {
  // Do nothing, already in Sport mode
  std::cout << "Car is already in Highest mode." << std::endl;
}

// Provide implementation for Car methods
Car::Car() : state_(new OffState(this)), speed_(0) {}

Car::~Car() {
  delete state_;
}

void Car::ChangeState(State* new_state) {
  delete state_;
  state_ = new_state;
}

void Car::Start() {
  state_->Start();
}

void Car::Stop() {
  state_->Stop();
}

void Car::Accelerate() {
  state_->Accelerate();
}

void Car::Decelerate() {
  state_->Decelerate();
}

void Car::LowerDriveMode() {
  state_->LowerDriveMode();
}

void Car::HigherDriveMode() {
  state_->HigherDriveMode();
}

int Car::speed() const {
  return speed_;
}

void Car::set_speed(int speed) {
  speed_ = speed;
}

int main() {
  Car my_car;

  my_car.Start();

  my_car.HigherDriveMode();
  for (int i = 0; i < 10; ++i) {
    my_car.Accelerate();
  }
  // car speed should be 150 km/h in Sport mode

  my_car.Decelerate();
  // car speed should be 135 km/h in Sport mode
  my_car.Decelerate();
  // car speed should be 120 km/h in Sport mode
  my_car.Stop();  // cannot stop because the car is still moving
  my_car.LowerDriveMode();
  // car is now in Normal mode
  for (int i = 0; i < 10; ++i) {
    my_car.Decelerate();
  }
  // car speed should be 20 km/h in Normal mode
  my_car.Stop();  // cannot stop because the car is still moving
  my_car.LowerDriveMode();
  for (int i = 0; i < 4; ++i) {
    my_car.Decelerate();
  }
  // car speed should be 0 km/h in Eco mode
  my_car.Stop();  // car is now stopped

  return 0;
}