#include <iostream>
#include <string>

class Cpu {
 public:
  virtual std::string GetCpuInfo() const = 0;
  virtual ~Cpu() = default;
};

class IntelCpu : public Cpu {
 public:
  std::string GetCpuInfo() const override {
    return "Intel CPU";
  }
};

class AmdCpu : public Cpu {
 public:
  std::string GetCpuInfo() const override {
    return "AMD CPU";
  }
};

class Gpu {
 public:
  virtual std::string GetGpuInfo() const = 0;
  virtual ~Gpu() = default;
};

class NvidiaGpu : public Gpu {
 public:
  std::string GetGpuInfo() const override {
    return "Nvidia GPU";
  }
};

class AmdGpu : public Gpu {
 public:
  std::string GetGpuInfo() const override {
    return "AMD GPU";
  }
};

/*
  The Computer class serves as an abstraction that uses the Cpu and Gpu interfaces.
  This allows for different combinations of CPU and GPU implementations without
  modifying the Computer class itself, adhering to the Bridge pattern principles.
*/
class Computer {
 public:
  Computer(Cpu* cpu, Gpu* gpu) : cpu_(cpu), gpu_(gpu) {}

  virtual ~Computer() {
    delete cpu_;
    delete gpu_;
  }

  Computer(const Computer&) = delete;
  Computer& operator=(const Computer&) = delete;

  virtual void PrintSystemInfo() const = 0;

 protected:
  Cpu* cpu_;
  Gpu* gpu_;
};

/*
  Desktop class inherits from Computer and implements the PrintSystemInfo method.

  This does not depend on the specific CPU and GPU implementations, allowing for
  flexibility in the system design. New computer systems can be added without
  modifying the existing code, adhering to the Open/Closed Principle.
*/
class Desktop : public Computer {
 public:
  Desktop(Cpu* cpu, Gpu* gpu) : Computer(cpu, gpu) {}

  void PrintSystemInfo() const override {
    std::cout << "Desktop with " << cpu_->GetCpuInfo() << " and " << gpu_->GetGpuInfo()
              << std::endl;
  }
};

class Laptop : public Computer {
 public:
  Laptop(Cpu* cpu, Gpu* gpu) : Computer(cpu, gpu) {}

  void PrintSystemInfo() const override {
    std::cout << "Laptop with " << cpu_->GetCpuInfo() << " and " << gpu_->GetGpuInfo() << std::endl;
  }
};

int main() {
  Computer* desktop = new Desktop(new IntelCpu(), new NvidiaGpu());
  desktop->PrintSystemInfo();
  delete desktop;

  Computer* laptop = new Laptop(new AmdCpu(), new AmdGpu());
  laptop->PrintSystemInfo();
  delete laptop;

  Computer* laptop2 = new Laptop(new IntelCpu(), new NvidiaGpu());
  laptop2->PrintSystemInfo();
  delete laptop2;

  Computer* desktop2 = new Desktop(new AmdCpu(), new AmdGpu());
  desktop2->PrintSystemInfo();
  delete desktop2;

  return 0;
}