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

int main() {
  /*
    This is a simple C++ program that demonstrates how to create an executable
    from source code by simulating the steps of preprocessing, compiling, assembling,
    and linking. Each step is represented by a function that processes the input
    and returns the next stage of code.

    Note that this is long and complex, and it is prone to errors if the input does not follow the
    expected steps.
  */
  {
    // This is a successful example of creating an executable from source code

    std::string source_code = "app.cpp";

    // Step 1: Preprocess the source code
    std::string preprocessed_code = Prepocess(source_code);
    if (preprocessed_code.empty()) {
      return 1;  // Exit if preprocessing failed
    }
    std::cout << "Preprocessed code: " << preprocessed_code << std::endl;
    // Step 2: Compile the preprocessed code
    std::string compiled_code = Compile(preprocessed_code);
    if (compiled_code.empty()) {
      return 1;  // Exit if compilation failed
    }
    std::cout << "Compiled code: " << compiled_code << std::endl;
    // Step 3: Assemble the compiled code
    std::string assembled_code = Assemble(compiled_code);
    if (assembled_code.empty()) {
      return 1;  // Exit if assembly failed
    }
    std::cout << "Assembled code: " << assembled_code << std::endl;
    // Step 4: Link the assembled code
    std::string linked_code = Link(assembled_code);
    if (linked_code.empty()) {
      return 1;  // Exit if linking failed
    }

    std::cout << "Linked code: " << linked_code << std::endl;
    std::cout << "Executable created successfully!" << std::endl;
  }

  std::cout << "------------------------" << std::endl;

  {
    /*
      This is an unsuccessful example of creating an executable from source code.
      User accidently does the Assembly step before the Compilation step.
    */
    std::string source_code = "app.cpp";

    // Step 1: Preprocess the source code
    std::string preprocessed_code = Prepocess(source_code);
    if (preprocessed_code.empty()) {
      return 1;  // Exit if preprocessing failed
    }

    std::cout << "Preprocessed code: " << preprocessed_code << std::endl;
    // Step 2: Assemble the preprocessed code (incorrect order)
    std::string assembled_code = Assemble(preprocessed_code);
    if (assembled_code.empty()) {
      return 1;  // Exit if assembly failed
    }
    std::cout << "Assembled code: " << assembled_code << std::endl;
    // Step 3: Compile the assembled code (incorrect order)
    std::string compiled_code = Compile(assembled_code);
    if (compiled_code.empty()) {
      return 1;  // Exit if compilation failed
    }
    std::cout << "Compiled code: " << compiled_code << std::endl;
    // Step 4: Link the compiled code
    std::string linked_code = Link(compiled_code);
    if (linked_code.empty()) {
      return 1;  // Exit if linking failed
    }
    std::cout << "Linked code: " << linked_code << std::endl;
    std::cout << "Executable created successfully!" << std::endl;
    std::cout << "Note: This is an incorrect order of steps, which may lead to errors."
              << std::endl;
  }

  return 0;
}