#include <chrono>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <thread>

namespace {
void SleepMS(int milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
}  // namespace
/*
  This is a File class that represents a simple file with a filename and content.
*/
class File {
 public:
  File(std::string filename, std::string content) : filename_(filename), content_(content) {}

  std::string filename() const {
    return filename_;
  }

  std::string content() const {
    return content_;
  }

 private:
  std::string filename_;
  std::string content_;
};
/*

*/
class RemoteFileStorage {
 public:
  void Connect() {
    SleepMS(1000);  // Simulate network delay

    std::cout << "Connected to remote file storage." << std::endl;
    storage_.clear();
  }

  void Store(File file) {
    SleepMS(100);  // Simulate network delay

    storage_.insert({file.filename(), file});
  }

  File Load(std::string filename) const {
    SleepMS(100);  // Simulate network delay

    auto it = storage_.find(filename);
    if (it != storage_.end()) {
      return it->second;
    }
    throw std::runtime_error("File not found");
  }

  void Disconnect() {
    SleepMS(1000);  // Simulate network delay

    std::cout << "Disconnected from remote file storage." << std::endl;
    storage_.clear();
  }

 private:
  std::map<std::string, File> storage_;
};

class IFileStorageProxy {
 public:
  virtual ~IFileStorageProxy() = default;
  virtual void Store(File file) = 0;
  virtual File Load(std::string filename) = 0;
};

class FileStorageService : public IFileStorageProxy {
 public:
  FileStorageService() : remote_storage_(new RemoteFileStorage()) {}

  ~FileStorageService() {
    if (remote_storage_) {
      delete remote_storage_;
    }
  }

  void Store(File file) override {
    remote_storage_->Store(file);
  }

  File Load(std::string filename) override {
    return remote_storage_->Load(filename);
  }

  void Connect() {
    remote_storage_->Connect();
  }

  void Disconnect() {
    remote_storage_->Disconnect();
  }

 private:
  RemoteFileStorage* remote_storage_;
};

/*
  FileStorageProxy is a proxy class that manages the connection to the remote file storage service.
  It allows for caching of loaded files to improve performance.
  It also ensures that the connection is established only once and disconnected when no longer
  needed.
*/
class FileStorageProxy : public IFileStorageProxy {
  static int instance_count_;  // Not thread-safe, but sufficient for this example

 public:
  FileStorageProxy(FileStorageService* file_storage_service, bool caching_enabled = false) :
      file_storage_service_(file_storage_service), caching_enabled_(caching_enabled) {
    instance_count_++;

    // Connect if this is the first instance
    if (instance_count_ == 1) {
      file_storage_service_->Connect();
    }
  }
  ~FileStorageProxy() {
    instance_count_--;

    // Disconnect if this is the last instance
    if (instance_count_ == 0) {
      file_storage_service_->Disconnect();
    }
  }

  void Store(File file) override {
    // We only have load caching for simplicity
    file_storage_service_->Store(file);
  }

  File Load(std::string filename) override {
    if (caching_enabled_) {
      // Check if the file is in the cache
      auto it = cache_.find(filename);
      if (it != cache_.end()) {
        return it->second;
      }
    }

    auto file = file_storage_service_->Load(filename);
    if (caching_enabled_) {
      cache_.insert({filename, file});  // Store in cache
    }
    return file;
  }

 private:
  FileStorageService* file_storage_service_;

  bool caching_enabled_;
  std::map<std::string, File> cache_;
};

int FileStorageProxy::instance_count_ = 0;

int main() {
  {
    File file1("example.txt", "This is an example file content.");
    File file2("example2.txt", "This is another example file content.");

    FileStorageService file_storage_service;

    auto start_time = std::chrono::high_resolution_clock::now();

    IFileStorageProxy* file_storage_proxy1 = new FileStorageProxy(&file_storage_service, false);
    IFileStorageProxy* file_storage_proxy2 = new FileStorageProxy(&file_storage_service, false);

    file_storage_proxy1->Store(file1);
    file_storage_proxy2->Store(file2);

    File loaded_file1 = file_storage_proxy2->Load("example.txt");
    File loaded_file2 = file_storage_proxy1->Load("example2.txt");

    /*
      To demonstrate the non-caching effect, we load the files several times.
      This will add extra 800 ms to the expected time taken.
    */
    for (int i = 0; i < 9; ++i) {
      loaded_file1 = file_storage_proxy1->Load("example.txt");
      loaded_file2 = file_storage_proxy2->Load("example2.txt");
    }

    delete file_storage_proxy1;
    delete file_storage_proxy2;

    auto end_time = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded file1: " << loaded_file1.filename()
              << ", Content: " << loaded_file1.content() << std::endl;
    std::cout << "Loaded file2: " << loaded_file2.filename()
              << ", Content: " << loaded_file2.content() << std::endl;

    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    int expected_time = 1000;   // Connection time
    expected_time += 100 * 2;   // Store time for each file, stored total 2 times
    expected_time += 100 * 20;  // Load time for each file, loaded total 20 times
    expected_time += 1000;      // Disconnection time

    std::cout << "Expected Time taken to store and load files: " << expected_time << " ms"
              << std::endl;
    std::cout << "Actual Time taken to store and load files: " << duration.count() << " ms"
              << std::endl;
  }

  std::cout << "----------------------------------------" << std::endl;
  {
    File file1("example.txt", "This is an example file content.");
    File file2("example2.txt", "This is another example file content.");

    FileStorageService file_storage_service;

    auto start_time = std::chrono::high_resolution_clock::now();

    IFileStorageProxy* file_storage_proxy1 = new FileStorageProxy(&file_storage_service, true);
    IFileStorageProxy* file_storage_proxy2 = new FileStorageProxy(&file_storage_service, true);

    file_storage_proxy1->Store(file1);
    file_storage_proxy2->Store(file2);

    File loaded_file1 = file_storage_proxy2->Load("example.txt");
    File loaded_file2 = file_storage_proxy1->Load("example2.txt");

    /*
      To demonstrate the non-caching effect, we load the files several times.
      This will add extra 800 ms to the expected time taken.
    */
    for (int i = 0; i < 9; ++i) {
      loaded_file1 = file_storage_proxy1->Load("example.txt");
      loaded_file2 = file_storage_proxy2->Load("example2.txt");
    }

    delete file_storage_proxy1;
    delete file_storage_proxy2;

    auto end_time = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded file1: " << loaded_file1.filename()
              << ", Content: " << loaded_file1.content() << std::endl;
    std::cout << "Loaded file2: " << loaded_file2.filename()
              << ", Content: " << loaded_file2.content() << std::endl;

    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    int expected_time = 1000;  // Connection time
    expected_time += 100 * 2;  // Store time for each file, stored total 2 times
    expected_time += 100 * 2;  // Load time for each file, loaded total 2 times
    expected_time += 1000;     // Disconnection time

    std::cout << "Expected Time taken to store and load files: " << expected_time << " + (cache-related overhead) ms"
              << std::endl;
    std::cout << "Actual Time taken to store and load files: " << duration.count() << " ms"
              << std::endl;
  }
  return 0;
}