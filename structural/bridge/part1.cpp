#include <iostream>
#include <string>

class IntelCpu {
 public:
  std::string GetIntelCpuInfo() const {
    return "Intel CPU";
  }
};

class AmdCpu {
 public:
  std::string GetAmdCpuInfo() const {
    return "AMD CPU";
  }
};

class ArmCpu {
 public:
  std::string GetArmCpuInfo() const {
    return "ARM CPU";
  }
};

class NvidiaGpu {
 public:
  std::string GetNvidiaGpuInfo() const {
    return "Nvidia GPU";
  }
};

class AmdGpu {
 public:
  std::string GetAmdGpuInfo() const {
    return "AMD GPU";
  }
};

/*
  Each computer system(here, Desktop and Laptop) depends on the details of the specific CPU and GPU
  implementations.

  This tight coupling prevents each computer system from independent development.

  Also, upon a new computer system with different CPU and GPU requirements, we need to add a new
  class that inherits from both CPU and GPU classes. This results in a combinatorial explosion of
  classes, making the codebase difficult to maintain and extend.

  To solve this issue, we can use the Bridge pattern to decouple the abstraction (Computer) from
  the implementation (CPU and GPU). This allows us to create new computer systems without modifying
  the existing code, adhering to the Open/Closed Principle.
*/

class IntelCpuNvidiaGpuDesktop : public IntelCpu, public NvidiaGpu {
 public:
  void PrintDesktopInfo() const {
    std::cout << "Desktop with " << GetIntelCpuInfo() << " and " << GetNvidiaGpuInfo() << std::endl;
  }
};

class ArmCpuAmdGpuLaptop : public ArmCpu, public AmdGpu {
 public:
  void PrintLaptopInfo() const {
    std::cout << "Laptop with " << GetArmCpuInfo() << " and " << GetAmdGpuInfo() << std::endl;
  }
};

int main() {
  IntelCpuNvidiaGpuDesktop desktop;
  desktop.PrintDesktopInfo();

  ArmCpuAmdGpuLaptop laptop;
  laptop.PrintLaptopInfo();

  return 0;
}