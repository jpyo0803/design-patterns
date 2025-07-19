#include <iostream>
#include <vector>

/*
  Dummy tree sprite class

  This size of this class is 28 bytes:
  - 1 byte for pixel representation
  - 27 bytes of metadata
*/
class TreeSprite {
 public:
  // Assume 'T' represents a tree sprite
  // and metadata is some additional information about the sprite
  TreeSprite() : pixel_('T') {}

  char pixel() const {
    return pixel_;
  }

 private:
  char pixel_;                     // 1 byte for pixel representation
  std::array<char, 27> metadata_;  // 27 bytes of metadata
};

class Tree {
 public:
  Tree(int x, int y, TreeSprite* sprite) : coordinate_(x, y), sprite_(sprite) {}

  std::pair<int, int> coordinate() const {
    return coordinate_;
  }

  char pixel() const {
    return sprite_->pixel();
  }

 private:
  std::pair<int, int> coordinate_;  // x, y coordinates

  /*
    Tree points to a shared TreeSprite instance instead of having its own copy.
    This reduces memory usage from 28 bytes to 8 bytes for the sprite. (Pointer size is 8 bytes on
    64-bit systems)
  */
  TreeSprite* sprite_;
};

class TreeFactory {
 public:
  Tree CreateTree(int x, int y) {
    return Tree(x, y, &sprite_);
  }

 private:
  /*
    Only one instance of TreeSprite is created and reused for all trees.
    This reduces memory usage significantly as we avoid creating multiple
    instances of the same sprite.
  */
  TreeSprite sprite_;
};

class Forest {
 public:
  Forest(int grid_size = 10) : grid_size_(grid_size) {}

  void AddTree(int x, int y) {
    Tree tree = tree_factory_.CreateTree(x, y);
    trees_.push_back(tree);
  }

  void Draw() const {
    std::vector<std::vector<char>> grid(grid_size_,
                                        std::vector<char>(grid_size_, '.'));  // 10x10 grid

    for (const auto& tree : trees_) {
      int x, y;
      std::tie(x, y) = tree.coordinate();

      grid[x][y] = tree.pixel();
    }

    for (const auto& row : grid) {
      for (const auto& cell : row) {
        std::cout << cell << ' ';
      }
      std::cout << std::endl;
    }

    // Calculate total memory usage
    int num_trees = trees_.size();
    int memory_for_coordinates = num_trees * sizeof(std::pair<int, int>);
    int memory_for_sprites = sizeof(TreeSprite);
    int memory_for_sprite_ptrs = num_trees * sizeof(TreeSprite*);
    int total_memory = memory_for_coordinates + memory_for_sprites + memory_for_sprite_ptrs;

    int actual_total_memory = 0;

    for (const auto& tree : trees_) {
      actual_total_memory += sizeof(tree);
    }

    actual_total_memory += sizeof(tree_factory_);


    std::cout << "Total memory for sprites: " << memory_for_sprites << " bytes, Percentage: "
              << (static_cast<double>(memory_for_sprites) / total_memory * 100) << "%" << std::endl;
    std::cout << "Total memory for sprite pointers: " << memory_for_sprite_ptrs
              << " bytes, Percentage: "
              << (static_cast<double>(memory_for_sprite_ptrs) / total_memory * 100) << "%"
              << std::endl;
    std::cout << "Total memory for coordinates: " << memory_for_coordinates
              << " bytes, Percentage: "
              << (static_cast<double>(memory_for_coordinates) / total_memory * 100) << "%"
              << std::endl;
    std::cout << "Total memory usage: " << total_memory << " bytes" << std::endl;
    std::cout << "Actual total memory usage: " << actual_total_memory << " bytes" << std::endl;
  }

  int grid_size() const {
    return grid_size_;
  }

 private:
  std::vector<Tree> trees_;
  int grid_size_;

  TreeFactory tree_factory_;  // Factory to create tree sprites
};

int main() {
  int grid_size = 30;
  Forest forest(grid_size);

  int num_trees = 0;

  for (int i = 0; i < grid_size; ++i) {
    for (int j = 0; j < grid_size; ++j) {
      if ((i + j) % 3 == 0) {  // Add a tree at every third cell
        num_trees++;
        forest.AddTree(i, j);
      }
    }
  }
  std::cout << "Number of trees added: " << num_trees << std::endl;
  std::cout << "---- Forest Representation ----" << std::endl;
  forest.Draw();
  return 0;
}