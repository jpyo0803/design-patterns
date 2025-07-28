#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

class Map {
 public:
  void AddWalkRoute(std::string x, std::string y) {
    walk_adjacency_list_[x].insert(y);
    walk_adjacency_list_[y].insert(x);
  }

  void AddDriveRoute(std::string x, std::string y) {
    drive_adjacency_list_[x].insert(y);
    drive_adjacency_list_[y].insert(x);
  }

  const std::map<std::string, std::set<std::string>>& GetWalkAdjList() const {
    return walk_adjacency_list_;
  }

  const std::map<std::string, std::set<std::string>>& GetDriveAdjList() const {
    return drive_adjacency_list_;
  }

 private:
  std::map<std::string, std::set<std::string>>
      walk_adjacency_list_;  // Adjacency list representation of the map
  std::map<std::string, std::set<std::string>>
      drive_adjacency_list_;  // Adjacency list for driving routes
};

class NavigatorStrategy {
 public:
  virtual ~NavigatorStrategy() = default;
  virtual std::pair<bool, std::vector<std::string>> Navigate(const Map& map, std::string from,
                                                             std::string to) const = 0;

  static std::vector<std::string> FindShortestPath(
      const std::map<std::string, std::set<std::string>>& adjacency_list, std::string from,
      std::string to) {
    std::map<std::string, std::string> visited; // also store visited from where
    
    std::queue<std::string> queue;
    queue.push(from);
    visited.insert({from, ""});

    bool found = false;
    
    while (!queue.empty()) {
      std::string current = queue.front();
      queue.pop();
      
      if (current == to) {
        found = true;
        break;
      }
      
      for (const auto& neighbor : adjacency_list.at(current)) {
        if (visited.find(neighbor) == visited.end()) {
          visited.insert({neighbor, current});
          queue.push(neighbor);
        }
      }
    }

    if (!found) {
      return {};  // Return empty vector if destination is not reachable
    }
    std::vector<std::string> route;

    // Backtrack to find the route
    for (std::string at = to; !at.empty(); at = visited[at]) {
      route.push_back(at);
    }
    std::reverse(route.begin(), route.end());
    return route;
  }
};

class WalkNavigator : public NavigatorStrategy {
 public:
  std::pair<bool, std::vector<std::string>> Navigate(const Map& map, std::string from,
                                                     std::string to) const override {
    const auto& walk_adj_list = map.GetWalkAdjList();

    std::vector<std::string> route = FindShortestPath(walk_adj_list, from, to);

    bool found = route.empty() == false;

    return {found, route};
  }
};

class DriveNavigator : public NavigatorStrategy {
 public:
  std::pair<bool, std::vector<std::string>> Navigate(const Map& map, std::string from,
                                                     std::string to) const override {
    const auto& drive_adj_list = map.GetDriveAdjList();

    std::vector<std::string> route = FindShortestPath(drive_adj_list, from, to);

    bool found = route.empty() == false;

    return {found, route};
  }
};

class Traveler {
 public:
  Traveler(Map map) : map_(map), strategy_(nullptr) {}

  void set_strategy(NavigatorStrategy* strategy) {
    strategy_ = strategy;
  }

  std::pair<bool, std::vector<std::string>> Navigate(const Map& map, std::string from,
                                                     std::string to,
                                                     std::string stopover = std::string()) const {
    assert(strategy_ != nullptr && "Strategy must be set before navigating");

    std::vector<std::string> complete_route;
    if (stopover.empty()) {
      auto [found, route] = strategy_->Navigate(map, from, to);
      if (found) {
        complete_route = route;
      }
    } else {
      auto [found1, route1] = strategy_->Navigate(map, from, stopover);
      auto [found2, route2] = strategy_->Navigate(map, stopover, to);

      if (found1 && found2) {
        complete_route = route1;
        complete_route.insert(complete_route.end(), route2.begin(), route2.end());
      }
    }

    return {complete_route.empty() == false, complete_route};
  }

 private:
  Map map_;                      // The map to navigate
  NavigatorStrategy* strategy_;  // Strategy for navigation
};

int main() {
  /*
    Initialize the map with walk and drive routes.

    Walk routes:
    A -- B -- C    H
    |         |    |
    D ------- E    F -- G

    Drive routes:
    A -- B -- C -- H
    |              |
    D ------- E    F -- G

    Note that Walk routes have 2 components:
    1. A, B, C, D, E
    2. F, G, H
    So, it is not possible to walk from A to G.
    Drive routes have 1 component:
    A, B, C, H, D, E, F, G

    For example, it is not possible to walk from A to G
  */

  Map map;

  // Construct walk routes
  map.AddWalkRoute("A", "B");
  map.AddWalkRoute("A", "D");
  map.AddWalkRoute("B", "C");
  map.AddWalkRoute("C", "E");
  map.AddWalkRoute("D", "E");
  map.AddWalkRoute("F", "G");
  map.AddWalkRoute("F", "H");

  // Construct drive routes
  map.AddDriveRoute("A", "B");
  map.AddDriveRoute("B", "C");
  map.AddDriveRoute("C", "H");
  map.AddDriveRoute("A", "D");
  map.AddDriveRoute("D", "E");
  map.AddDriveRoute("F", "G");
  map.AddDriveRoute("F", "H");

  Traveler traveler(map);

  WalkNavigator walk_navigator;
  DriveNavigator drive_navigator;

  traveler.set_strategy(&walk_navigator);

  // Walk from A to E
  auto [found, route] = traveler.Navigate(map, "A", "E");
  if (found) {
    std::cout << "Walk route from A to E: ";
    for (const auto& stop : route) {
      std::cout << stop << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "No walk route found from A to D." << std::endl;
  }

  // Walk from A to G (should not be found)
  auto [found2, route2] = traveler.Navigate(map, "A", "G");
  if (found2) {
    std::cout << "Walk route from A to G: ";
    for (const auto& stop : route2) {
      std::cout << stop << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "No walk route found from A to G." << std::endl;
  }

  // Drive from A to G
  traveler.set_strategy(&drive_navigator);
  auto [found3, route3] = traveler.Navigate(map, "A", "G");
  if (found3) {
    std::cout << "Drive route from A to G: ";
    for (const auto& stop : route3) {
      std::cout << stop << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "No drive route found from A to G." << std::endl;
  }

  return 0;
}