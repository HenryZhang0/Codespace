#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <optional>
#include <memory>
#include <ctime>
#include <climits>
#include <cstdlib>

using std::vector;
using std::string;
using kvp = std::pair<string, int>;
using std::unique_ptr;

/*********************************************************************/
/* CS240KatieMap
 * -------------
 *
 * Implement this class. See the assignment for more details.
 *********************************************************************/

struct Node {
  kvp point;
  std::pair<string, string> key_range = {std::string(16, '\0'), std::string(16, '\0')};
  std::pair<int, int> value_range = {INT_MIN, INT_MAX};
  bool is_leaf;
  bool is_vertical;
  Node *left = nullptr;
  Node *right = nullptr;
};

class CS240KatieMap {
  private:
    vector<kvp> points;
    Node *tree;

    // Helper functions
    int partition(vector<kvp>& points, int pivot, bool is_vertical) {
      int size = points.size();
      std::swap(points[size - 1], points[pivot]);
      int i = -1;
      int j = size - 1;

      string key;
      int value;
      if (is_vertical) {
        key = points[size - 1].first;
      } else {
        value = points[size - 1].second;
      }

      while (true) {
        do {
          i += 1;
        } while (is_vertical ? points[i].first < key : points[i].second < value);
        do {
          j -= 1;
        } while (j >= i && (is_vertical ? points[j].first > key : points[j].second > value));
        if (i >= j) {
          break;
        } else {
          std::swap(points[i], points[j]);
        }
      }
      std::swap(points[size - 1], points[i]);
      return i;
    }
    
    kvp quickSelect(vector<kvp> points, int k, bool is_vertical) {
      int pivot = rand() % points.size();
      // std::cout << "pivot: " << pivot << std::endl;
      int index = partition(points, pivot, is_vertical); // i

      if (index == k) {
        return points[index];
      } else if (index > k) {
        vector<kvp> subVec(points.begin(), points.begin() + index);
        return quickSelect(subVec, k, is_vertical);
      } else {
        vector<kvp> subVec(points.begin() + (index + 1), points.end());
        return quickSelect(subVec, k - (index + 1), is_vertical);
      } 
    }

    void constructTree(Node* node, const vector<kvp>& points, bool is_vertical) {
      if (points.size() == 0) return;

      if (points.size() == 1) {
        node->point = points[0];
        node->is_leaf = true;
        node->is_vertical = is_vertical;
        return;
      }

      kvp median = quickSelect(points, (int)points.size()/2, is_vertical);
      node->point = median;
      node->is_leaf = false;
      node->is_vertical = is_vertical;

      vector<kvp> left_points;
      vector<kvp> right_points;
      for (int i = 0; i < points.size(); i++) {
        if (is_vertical) {
          if (points[i].first < median.first) {
            left_points.push_back(points[i]);
          } else {
            right_points.push_back(points[i]);
          }
        } else {
          if (points[i].second < median.second) {
            left_points.push_back(points[i]);
          } else {
            right_points.push_back(points[i]);
          }
        }
      }

      is_vertical = !is_vertical;
      Node *left = new Node();
      node->left = left;
      constructTree(left, left_points, is_vertical);
      Node *right = new Node();
      node->right = right;
      constructTree(right, right_points, is_vertical);
    }
    
    void preorder (Node* node, vector<kvp>& points) {
      if (node->is_leaf) {
        points.push_back(node->point);
        return;
      }

      preorder(node->left, points);
      preorder(node->right, points);
    }

    void splits(vector<string>& retKey, vector<int>& retVal, Node *node, bool key, bool is_vertical) {
      if (node->is_leaf) return;

      if(is_vertical && key) retKey.emplace_back(node->point.first);
      if(!is_vertical && !key) retVal.emplace_back(node->point.second);

      splits(retKey, retVal, node->left, key, !is_vertical);
      splits(retKey, retVal, node->right, key, !is_vertical);
    }

    void updateBounds(Node *node) {
      if (node->is_leaf) {
        return;
      }
      updateBounds(node->left);
      updateBounds(node->right);
      if (node->is_vertical) {
        node->key_range.first = node->left->key_range.first;
        node->key_range.second = node->right->key_range.second;
        node->value_range.first = node->left->value_range.first;
        node->value_range.second = node->right->value_range.second;
      } else {
        node->key_range.first = node->left->key_range.first;
        node->key_range.second = node->right->key_range.second;
        node->value_range.first = node->left->value_range.first;
        node->value_range.second = node->right->value_range.second;
      }
    }

    void rangeSearch(Node *node, kvp start, kvp end, vector<kvp>& points) {
      if (node->is_leaf) {
        if (node->point.first >= start.first && node->point.first < end.first && node->point.second >= start.second && node->point.second < end.second) {
          points.push_back(node->point);
        }
        return;
      }
      if (node->is_vertical) {
        if (node->point.first >= start.first && node->point.first < end.first) {
          rangeSearch(node->left, start, end, points);
          rangeSearch(node->right, start, end, points);
        } else if (node->point.first < start.first) {
          rangeSearch(node->right, start, end, points);
        } else {
          rangeSearch(node->left, start, end, points);
        }
      } else {
        if (node->point.second >= start.second && node->point.second < end.second) {
          rangeSearch(node->left, start, end, points);
          rangeSearch(node->right, start, end, points);
        } else if (node->point.second < start.second) {
          rangeSearch(node->right, start, end, points);
        } else {
          rangeSearch(node->left, start, end, points);
        }
      }
    }

    void deleteTree(Node *node) {
      if (node->is_leaf) {
        delete node;
        return;
      }
      deleteTree(node->left);
      deleteTree(node->right);
      delete node;
    }

  public:
    CS240KatieMap(const vector<kvp>& kvps) {  // Constructor: Builds a new tree on the KVPs provided.
      tree = new Node();
      constructTree(tree, kvps, true);
      if (!tree->is_leaf) {
        updateBounds(tree);
      }
    }
    // all_points: Returns all KVPs in the tree, in the order they are
    //             visited in preorder traversal.
    vector<kvp> all_points() {
      vector<kvp> points;
      preorder(tree, points);
      return points;
    }

    // get: Returns the value, if any, stored at the given key. 
    //      Returns std::nullopt if there is no such key.
    //      Runs in O(sqrt(n)) time.
    std::optional<int> get(string key, Node *node = nullptr) {
      if (node == nullptr) {
        node = tree;
      }
      if (node->is_leaf) {
        return (node->point.first == key) ? (std::optional<int>) node->point.second : (std::optional<int>) std::nullopt;
      } else if (node->is_vertical) {
        return key < node->point.first ? get(key, node->left) : get(key, node->right);
      } else {
        std::optional<int> left = get(key, node->left);
        return left.has_value() ? left : get(key, node->right);
      }
    }

    // range: Query points in the "rectangle" defined by start and end.
    //        Specifically, if start = (key_start, value_start) and
    //                         end = (key_end, value_end)
    //        returns all points in the range
    //              [key_start, key_end) x [value_start, value_end)
    //        Runs in O(sqrt(n) + s) time
    //              where s is the size of the returned vector.
    vector<kvp> range(kvp start, kvp end) {
      vector<kvp> points;
      rangeSearch(tree, start, end, points);
      return points;
    }

    // key_splits: Return all keys (strings) that are used for splits,
    //             in the order they are visited in preorder traversal.
    //             Runs in O(n) time.
    vector<string> key_splits() {
      vector<string> keys;
      vector<int> values;
      splits(keys, values, tree, true, true);
      return keys;
    }

    vector<int> value_splits() {
      vector<string> keys;
      vector<int> values;
      splits(keys, values, tree, false, true);
      return values;
    }

    ~CS240KatieMap() { // Destructor: Frees all memory
      deleteTree(tree);
    }
    
    void printTreee(Node *node, int depth = 0) {
      for (int i = 0; i < depth * 2 - 2; i++) {
        std::cout << (i != depth * 2 - 4 ? " ": "|");
      }
      std::cout << "|--";
      if (node->is_leaf) {
        std::cout << node->point.first << " " << node->point.second << std::endl;
        return;
      }
      std::cout << node->point.first << " " << node->point.second << std::endl;
      printTreee(node->left, depth + 1);
      printTreee(node->right, depth + 1);
    }
    void printTree() {
      printTreee(tree, 1);
    }
};

/*********************************************************************/
/* main
 * ----
 *
 * Do not modify any code below this comment.
 *********************************************************************/

int main() {
  unique_ptr<CS240KatieMap> km;
  std::srand(std::time(NULL));

  while (true) {
    string command;
    std::cin >> command;

    if (command == "n") {
      // Construct a new kd-tree->
      vector<kvp> points;
      int n;
      for (std::cin >> n; n > 0; --n) {
        string key;
 	int value;
        std::cin >> key >> value;
        points.push_back({key, value});
      }

      km = std::make_unique<CS240KatieMap>(points);
    } else if (command == "g") {
      // Lookup a value by its key.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      string key;
      std::cin >> key;
      std::optional<int> value = km->get(key);
      if (value == std::nullopt) {
        std::cout << "Not found" << std::endl;
      } else {
        std::cout << value.value() << std::endl;
      }
    } else if (command == "p") {
      // Print all elements in the tree->
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      vector<kvp> points = km->all_points();
      for (auto p = points.begin(); p != points.end(); ++p) {
        std::cout << p->first << " " << p->second << std::endl;
      }
    } else if (command == "r") {
      // Range search
      string k1, k2;
      int v1, v2;
      std::cin >> k1 >> v1 >> k2 >> v2;
      vector<kvp> points = km->range(std::make_pair(k1, v1), std::make_pair(k2, v2));
      for (auto p = points.begin(); p != points.end(); ++p) {
        std::cout << p->first << " " << p->second << std::endl;
      }
    } else if (command == "k") {
      // Print the string splits.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      vector<string> keys = km->key_splits();
      for (auto k = keys.begin(); k != keys.end(); ++k) {
        std::cout << *k << std::endl;
      }
    } else if (command == "v") {
      // Print the integer splits.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      vector<int> values = km->value_splits();
      for (auto v = values.begin(); v != values.end(); ++v) {
        std::cout << *v << std::endl;
      }
    } else if (command == "q" || std::cin.eof()) {
      // Quit.
      break;
    } else {
      std::cerr << "Error: invalid command" << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
