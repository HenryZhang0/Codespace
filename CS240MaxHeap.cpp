#include<iostream>
#include<vector>
#include<stack>
#include<string>
using namespace std;
// Heap implementation class.
class CS240MaxHeap {
        int k;
        int num_items;
        std::vector<std::vector<int>> maxHeap;
 private:
 public:
        CS240MaxHeap(const int k) : k(k), num_items(0) {};
        void insert(int key) {
        }
        int delete_max() {
        }
        int delete_max_5() {
                stack<int> s;
                for (int i = 0; i < 5; i++)
                        s.push(delete_max());
                int result = s.pop();
                for (int i = 0; i < 4; i++) {
                        insert(s.top());
                        s.pop();
                }
                return result;
                int m1 = delete_max();
                int m2 = delete_max();
                int m3 = delete_max();
                int m4 = delete_max();
                int m5 = delete_max();
                insert(m1);
                insert(m2);
                insert(m3);
                insert(m4);
                return m5;
        }
        void print() {
                if (num_items == 0) return;
                for (int i = 0; i < num_items - 1; i++) {
                        vector<int> node = maxHeap.at(i);
                        for (int j = 0; j < k; j++) cout << node.at(j) << ' ';
                        cout << '\n';
                }
                int remain_keys = getLastNodeKeyLength();
                vector<int> node = maxHeap.at(num_items - 1);
                for (int i = 0; i < remain_keys; i++) cout << node.at(i) << ' ';
                cout << '\n';
        }
        void printLeftPath() {
                if (num_items == 0) return;
                int idx = 0;
                while (idx < num_items) {
                        vector<int> node = maxHeap.at(idx);
                        if (idx == num_items - 1) {
                                int r_keys = getLastNode
                        }
                        for (int i = 0; i < k; i++) {
                                cout << node.at(i) << ' ';
                        }
                        cout << '\n';
                        idx = idx * 2 + 1;
                }
        }
        void printRightPath() {
        }
        int crazy_clean() {
        }
};
   
int main() {
    CS240MaxHeap* pq = nullptr;
    int k;
    int total_items = 0;


    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "r") {  // initializes heap with the max_capacity read from the input
            std::cin >> k;

            pq = new CS240MaxHeap(k);
        }
        else if (command == "x") {  // finishes the program and cleans up
            delete pq;
            return 0;
        }
        else if (command == "i") { // inserts item read from std
            int item;
            std::cin >> item;
            pq->insert(item);
            total_items++;
        }
        else if (command == "d") {  // deletes and prints item read from std
            if (pq->size() != 0) {
                total_items--;
                std::cout << pq->delete_max() << std::endl;
            }
            else {
                std::cout << "Empty heap!" << std::endl;
            }
        }
        else if (command == "d_") {  // deletes and doesn't not print item read from std
            if (pq->size() != 0) {
                total_items--;
                pq->delete_max();
            }
            else {
                std::cout << "Empty heap!" << std::endl;
            }
        }
        else if (command == "d5") {  // deletes and prints 5th largest item
            if (pq->size() >= 5) {
                total_items--;
                cout << pq->delete_max_5() << endl;
            }
            else {
                std::cout << "Heap has less than 5 elements" << std::endl;
            }
        }
        else if (command == "d_5") {  // deletes and does not print 5th largest item
            if (pq->size() >= 5) {
                total_items--;
                pq->delete_max_5();
            }
            else {
                std::cout << "Heap has less than 5 elements" << std::endl;
            }
        }
        else if (command == "pa") { // prints the heap, including total number of items and k
            if (pq->size() != 0) {
                cout << total_items << endl;
                cout << k << endl;
                pq->print();
            }
        }
        else if (command == "pl") {  // prints the left path, including total number of items and k
            if (pq->size() != 0) {
                cout << total_items << endl;
                cout << k << endl;
                pq->printLeftPath();
            }
        }
        else if (command == "pr") {  // prints the right path,  including total number of items and k
            if (pq->size() != 0) {
                cout << total_items << endl;
                cout << k << endl;
                pq->printRightPath();
            }
        }
        else if (command == "es") {
            int sum = pq->crazy_clean();
            std::cout << sum << endl;
        }
    }
}