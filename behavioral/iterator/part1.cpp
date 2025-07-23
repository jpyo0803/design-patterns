#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

class Iterator {
 public:
  virtual ~Iterator() = default;

  virtual const int& GetNext() = 0;
  virtual bool IsDone() const = 0;
};

class IterableCollection {
 public:
  virtual ~IterableCollection() = default;

  virtual Iterator* CreateForwardOrderIterator() const = 0;

  virtual Iterator* CreateReverseOrderIterator() const = 0;

  virtual Iterator* CreateRandomOrderIterator() const = 0;
};

class MyVector : public IterableCollection {
 public:
  MyVector(int size) : size_(size), data_(size) {}

  int size() const {
    return size_;
  }

  int& operator[](int index) {
    return data_[index];
  }

  const int& operator[](int index) const {
    return data_[index];
  }

  // Implementation will be provided after ForwardOrderIterator, ReverseOrderIterator, and
  // RandomOrderIterator classes are defined.
  Iterator* CreateForwardOrderIterator() const override;

  Iterator* CreateReverseOrderIterator() const override;

  Iterator* CreateRandomOrderIterator() const override;

 private:
  int size_;
  std::vector<int> data_;
};

class ForwardOrderIterator : public Iterator {
 public:
  ForwardOrderIterator(const MyVector* collection) : collection_(collection), current_index_(0) {}

  const int& GetNext() override {
    if (IsDone()) {
      throw std::out_of_range("Iterator has reached the end of the collection.");
    }
    return (*collection_)[current_index_++];
  }

  bool IsDone() const override {
    return current_index_ >= collection_->size();
  }

 private:
  const MyVector* collection_;
  int current_index_;
};

class ReverseOrderIterator : public Iterator {
 public:
  ReverseOrderIterator(const MyVector* collection) :
      collection_(collection), current_index_(collection->size() - 1) {}

  const int& GetNext() override {
    if (IsDone()) {
      throw std::out_of_range("Iterator has reached the beginning of the collection.");
    }
    return (*collection_)[current_index_--];
  }

  bool IsDone() const override {
    return current_index_ < 0;
  }

 private:
  const MyVector* collection_;
  int current_index_;
};

class RandomOrderIterator : public Iterator {
 public:
  RandomOrderIterator(const MyVector* collection) : collection_(collection) {
    // Initialize random indices or any other logic needed for random access
    for (int i = 0; i < collection->size(); ++i) {
      random_indices_.push_back(i);
    }

    std::shuffle(random_indices_.begin(), random_indices_.end(),
                 std::mt19937{std::random_device{}()});
  }

  const int& GetNext() override {
    if (IsDone()) {
      throw std::out_of_range("Iterator has reached the end of the collection.");
    }
    return (*collection_)[random_indices_[current_index_++]];
  }

  bool IsDone() const override {
    return current_index_ >= random_indices_.size();
  }

 private:
  const MyVector* collection_;

  std::vector<int> random_indices_;  // Store random indices if needed
  int current_index_ = 0;            // Current index in the random order
};

/*
  Implementations of the iterator creation methods must be provided
  after the iterator classes are defined.
*/
Iterator* MyVector::CreateForwardOrderIterator() const {
  return new ForwardOrderIterator(this);
}

Iterator* MyVector::CreateReverseOrderIterator() const {
  return new ReverseOrderIterator(this);
}

Iterator* MyVector::CreateRandomOrderIterator() const {
  return new RandomOrderIterator(this);
}

int main() {
  MyVector vec(10);
  for (int i = 0; i < 10; ++i) {
    vec[i] = (i + 1) * 10;
  }
  {
    std::cout << "Forward Order:" << std::endl;
    Iterator* forward_iterator = vec.CreateForwardOrderIterator();
    while (!forward_iterator->IsDone()) {
      std::cout << forward_iterator->GetNext() << " ";
    }
    std::cout << std::endl;
    delete forward_iterator;
  }

  {
    std::cout << "Reverse Order:" << std::endl;
    Iterator* reverse_iterator = vec.CreateReverseOrderIterator();
    while (!reverse_iterator->IsDone()) {
      std::cout << reverse_iterator->GetNext() << " ";
    }
    std::cout << std::endl;
    delete reverse_iterator;
  }

  {
    std::cout << "Random Order:" << std::endl;
    Iterator* random_iterator = vec.CreateRandomOrderIterator();
    while (!random_iterator->IsDone()) {
      std::cout << random_iterator->GetNext() << " ";
    }
    std::cout << std::endl;
    delete random_iterator;
  }

  return 0;
}