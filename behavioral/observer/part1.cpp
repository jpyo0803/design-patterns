#include <chrono>
#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <vector>

std::string GetCurrentDateTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  auto time_string = std::string(std::ctime(&now_time));
  // Remove newline character added by ctime
  time_string.erase(time_string.length() - 1);
  return time_string;
}

class EventInfo {
 public:
  explicit EventInfo(std::string name) : event_name(name), event_time(GetCurrentDateTime()) {}

  std::string GetName() const {
    return event_name;
  }

  std::string GetTime() const {
    return event_time;
  }

 private:
  std::string event_name;
  std::string event_time;
};

class IObserver {
 public:
  virtual ~IObserver() = default;
  virtual void Update(EventInfo event_info) = 0;
};

class EventNameTracker : public IObserver {
 public:
  void Update(EventInfo event_info) override {
    std::cout << "EventNameTracker: Event '" << event_info.GetName() << "' occurred at "
              << event_info.GetTime() << std::endl;

    event_names_.push_back(event_info.GetName());
  }

  std::vector<std::string> GetEventNames() const {
    return event_names_;
  }

 private:
  std::vector<std::string> event_names_;
};

class EventTimeTracker : public IObserver {
 public:
  void Update(EventInfo event_info) override {
    std::cout << "EventTimeTracker: Event '" << event_info.GetName() << "' occurred at "
              << event_info.GetTime() << std::endl;

    event_times_.push_back(event_info.GetTime());
  }

  std::vector<std::string> GetEventTimes() const {
    return event_times_;
  }

 private:
  std::vector<std::string> event_times_;
};

class LogCounter : public IObserver {
 public:
  LogCounter() : count_(0) {}

  void Update(EventInfo event_info) override {
    std::cout << "LogCounter: Event '" << event_info.GetName() << "' occurred at "
              << event_info.GetTime() << std::endl;

    count_++;
  }

  int count() const {
    return count_;
  }

 private:
  int count_;
};

class Publiser {
 public:
  void Subscribe(IObserver* observer) {
    observers_.insert(observer);
  }

  void Unsubscribe(IObserver* observer) {
    observers_.erase(observer);
  }

  void Notify(EventInfo event_info) {
    for (IObserver* observer : observers_) {
      observer->Update(event_info);
    }
  }

 private:
  std::set<IObserver*> observers_;
};

int main() {
  Publiser publisher;
  EventNameTracker name_tracker;
  EventTimeTracker time_tracker;
  LogCounter log_counter;

  publisher.Subscribe(&name_tracker);
  publisher.Subscribe(&time_tracker);
  publisher.Subscribe(&log_counter);

  publisher.Notify(EventInfo("Event 1"));
  publisher.Notify(EventInfo("Event 2"));

  std::cout << "\nEvent Names:\n";
  for (const auto& name : name_tracker.GetEventNames()) {
    std::cout << "- " << name << "\n";
  }
  std::cout << "\nEvent Times:\n";
  for (const auto& time : time_tracker.GetEventTimes()) {
    std::cout << "- " << time << "\n";
  }
  std::cout << "\nTotal Events Logged: " << log_counter.count() << "\n";

  publisher.Unsubscribe(&log_counter);
  publisher.Notify(EventInfo("Event 3"));

  std::cout << "\nTotal Events Logged after unsubscribe: " << log_counter.count() << "\n";
  std::cout << "Event Names after unsubscribe:\n";
  for (const auto& name : name_tracker.GetEventNames()) {
    std::cout << "- " << name << "\n";
  }
  std::cout << "Event Times after unsubscribe:\n";
  for (const auto& time : time_tracker.GetEventTimes()) {
    std::cout << "- " << time << "\n";
  }

  // This should remain unchanged as the log_counter is no longer subscribed.
  std::cout << "\nTotal Events Logged after unsubscribe: " << log_counter.count() << "\n";

  return 0;
}