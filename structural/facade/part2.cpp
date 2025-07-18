#include <iostream>
#include <string>

/*
  How to make a executable from source code in C++

  1. Preprocessing (handles directives like #include, #define)
  2. Compilation (translates source code to assembly code)
  3. Assembly (translates assembly code to machine code)
  4. Linking (combines object files and libraries to create an executable)
*/

std::string Prepocess(std::string source_code) {
  auto pos = source_code.find(".cpp");
  if (pos == std::string::npos) {
    std::cerr << "Error: Source code must contain a .cpp file extension." << std::endl;
    return "";
  }
  // Simulate preprocessing by returning the source code unchanged
  return "preproc_" + source_code;
}

std::string Compile(std::string preprocessed_code) {
  auto pos = preprocessed_code.find("preproc_");
  if (pos == std::string::npos) {
    std::cerr << "Error: Preprocessed code must start with 'preprocessed_'." << std::endl;
    return "";
  }

  // Remove the "preprocessed_" prefix to simulate compilation
  preprocessed_code.erase(0, std::string("preproc_").length());

  // Remove the ".cpp" extension to simulate assembly
  pos = preprocessed_code.find(".cpp");
  if (pos == std::string::npos) {
    std::cerr << "Error: Source code must contain a .cpp file extension." << std::endl;
    return "";
  }
  preprocessed_code.erase(pos);

  // Simulate compilation by returning the preprocessed code unchanged
  return preprocessed_code + ".asm";
}

std::string Assemble(std::string compiled_code) {
  auto pos = compiled_code.find(".asm");
  if (pos == std::string::npos) {
    std::cerr << "Error: Compiled code must end with '.asm'." << std::endl;
    return "";
  }

  // Remove the ".asm" extension to simulate assembly
  compiled_code.erase(pos);

  // Simulate assembly by returning the compiled code unchanged
  return compiled_code + ".obj";
}

std::string Link(std::string object_code) {
  auto pos = object_code.find(".obj");
  if (pos == std::string::npos) {
    std::cerr << "Error: Object code must end with '.obj'." << std::endl;
    return "";
  }

  // Remove the ".obj" extension to simulate linking
  object_code.erase(pos);

  // Simulate linking by returning the object code unchanged
  return object_code + ".exe";
}

std::string BuildExecutableFacade(std::string source_code) {
  // Step 1: Preprocess the source code
  std::string preprocessed_code = Prepocess(source_code);
  if (preprocessed_code.empty()) {
    return "";  // Exit if preprocessing failed
  }

  // Step 2: Compile the preprocessed code
  std::string compiled_code = Compile(preprocessed_code);
  if (compiled_code.empty()) {
    return "";  // Exit if compilation failed
  }

  // Step 3: Assemble the compiled code
  std::string assembled_code = Assemble(compiled_code);
  if (assembled_code.empty()) {
    return "";  // Exit if assembly failed
  }

  // Step 4: Link the assembled code
  std::string linked_code = Link(assembled_code);
  if (linked_code.empty()) {
    return "";  // Exit if linking failed
  }

  return linked_code;  // Return the final executable code
}

int main() {
  /*
    This example demonstrates how to create an executable from source code using a facade pattern.
    The BuildExecutableFacade function encapsulates the steps of preprocessing, compiling,
    assembling, and linking, providing a simple interface for the user to create an executable from
    a source code file. The user only needs to call this function with the source code file name,
    and it will handle all the steps internally.
  */

  std::string source_code = "app.cpp";
  std::string executable = BuildExecutableFacade(source_code);
  if (!executable.empty()) {
    std::cout << "Executable created successfully: " << executable << std::endl;
  } else {
    std::cerr << "Failed to create executable." << std::endl;
  }

  return 0;
}