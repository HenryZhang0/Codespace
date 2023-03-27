#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;
// Heap implementation class.
class CS240MaxHeap
{
    int k;
    int num_items;
    std::vector<std::vector<int>> heap;

private:
    int getLastNodeKeyLength()
    {
        if (num_items == 0)
            return 0;
        return heap.back().size();
    }

    void sortNode(vector<int> &node)
    {
        int len = node.size() - 1;
        while (len > 0)
        {
            if (node[len] > node[len - 1]) //
                break;
            swap(node[len], node[len - 1]);
            len--;
        }
    }

public:
    CS240MaxHeap(const int k) : k(k), num_items(0){};
    void insert(int key)
    {
        if (getLastNodeKeyLength() % k == 0)
        {
            vector<int> new_node;
            new_node.push_back(key);
            heap.push_back(new_node);
            num_items++;
        }
        else
        {
            heap.back().push_back(key);
        }
        sortNode(heap.back());
        if (num_items == 1) {
            return;
        }

        //fixdown
        int child = num_items - 1;
        int par = (child - 1) / 2;

        while ((par >= 0) && heap[child].back() > (heap[par])[0])
        {
            swap(heap[child].back(), (heap[par])[0]); // swap the first element of the parent node with the last element of the child node
            heap[par].push_back((heap[par])[0]);      // push the first element of the parent node to the end of the parent node
            heap[par].erase(heap[par].begin());       // erase the first element of the parent node
            sortNode(heap[par]);                      // sort the parent node

            if (par == 0)
                break;
            child = par;
            par = (child - 1) / 2;
        }
        sortNode(heap[0]);
    }

    int delete_max() {
        if (num_items == 0) // if the heap is empty, return -1
            return -1;
        int result = heap[0].back();
        swap(heap[0].back(), heap[num_items - 1].front());
        heap[num_items - 1].erase(heap[num_items - 1].begin());
        // cout << "OOGA BOOGA" << endl;
        // print();
        if (heap[num_items - 1].size() == 0)
        {
            heap.erase(heap.end() - 1);
            num_items--;
        }

        // cout << "OOGA BOOGA" << endl;
        // print();
        // cout << "OOGA BOOGA" << endl;

        if (num_items == 0)
            return result;

        sortNode(heap[0]);
        //fixup
        int par = 0;
        int child = par * 2 + 1;
        while (child < num_items)
        {
            if (child + 1 < num_items && heap[child + 1].back() > heap[child].back()) // gets the larger child
                child++;
            if (heap[par][0] > heap[child].back())
                break;
            // swap(heap[0].back(), heap[num_items - 1].front());
            swap(heap[par].front(), heap[child].back());
            // heap[child].push_back(heap[child].back());
            // heap[child].erase(heap[child].begin());
            sortNode(heap[par]);
            sortNode(heap[child]);
            par = child;
            child = par * 2 + 1;
        }
        sortNode(heap[0]);
        return result;
    }

    int delete_max_5()
    {
        stack<int> s;
        for (int i = 0; i < 5; i++)
            s.push(delete_max());
        int result = s.top();
        s.pop();
        for (int i = 0; i < 4; i++)
        {
            insert(s.top());
            s.pop();
        }
        return result;
    }
    void print()
    {
        if (num_items == 0)
            return;
        for (int i = 0; i < num_items - 1; i++)
        {
            vector<int> node = heap.at(i);
            // for (int j = 0; j < k; j++)
            //     cout << node.at(j) << ' ';
            for (auto j : node)
                cout << j << ' ';
            cout << '\n';
        }
        vector<int> node = heap.at(num_items - 1);
        for (auto i : node)
            cout << i << ' ';
        cout << '\n';
    }
    void printLeftPath()
    {
        if (num_items == 0)
            return;
        int idx = 0;
        while (idx < num_items)
        {
            vector<int> node = heap.at(idx);
            int a = (idx == num_items - 1) ? getLastNodeKeyLength() : k;
            for (int i = 0; i < a; i++)
            {
                cout << node.at(i) << ' ';
            }
            cout << '\n';
            idx = idx * 2 + 1;
        }
    }
    void printRightPath()
    {
        if (num_items == 0)
            return;
        int idx = 0;
        while (idx < num_items)
        {
            vector<int> node = heap.at(idx);
            int a = (idx == num_items - 1) ? getLastNodeKeyLength() : k;
            for (int i = a - 1; i >= 0; i--)
            {
                cout << node.at(i) << ' ';
            }
            cout << '\n';
            idx = idx * 2 + 2;
        }
    }
    int crazy_clean()
    {
        int keys = size();
        if (keys == 0)
            return 0;

        while (keys > 1)
        {
            int e1 = delete_max();
            int e2 = delete_max();
            int insert_key = e1 - e2;
            insert(insert_key);
            --keys;
        }
        return delete_max();
    }
    int size()
    {
        if (num_items == 0)
            return 0;
        return (num_items - 1) * k + getLastNodeKeyLength();
    }
};

int main()
{
    CS240MaxHeap *pq = nullptr;
    int k;
    int total_items = 0;

    while (true)
    {
        std::string command;
        std::cin >> command;

        if (command == "r")
        { // initializes heap with the max_capacity read from the input
            std::cin >> k;

            pq = new CS240MaxHeap(k);
        }
        else if (command == "x")
        { // finishes the program and cleans up
            delete pq;
            return 0;
        }
        else if (command == "i")
        { // inserts item read from std
            int item;
            std::cin >> item;
            pq->insert(item);
            total_items++;
        }
        else if (command == "d")
        { // deletes and prints item read from std
            if (pq->size() != 0)
            {
                total_items--;
                std::cout << pq->delete_max() << std::endl;
            }
            else
            {
                std::cout << "Empty heap!" << std::endl;
            }
        }
        else if (command == "d_")
        { // deletes and doesn't not print item read from std
            if (pq->size() != 0)
            {
                total_items--;
                pq->delete_max();
            }
            else
            {
                std::cout << "Empty heap!" << std::endl;
            }
        }
        else if (command == "d5")
        { // deletes and prints 5th largest item
            if (pq->size() >= 5)
            {
                total_items--;
                cout << pq->delete_max_5() << endl;
            }
            else
            {
                std::cout << "Heap has less than 5 elements" << std::endl;
            }
        }
        else if (command == "d_5")
        { // deletes and does not print 5th largest item
            if (pq->size() >= 5)
            {
                total_items--;
                pq->delete_max_5();
            }
            else
            {
                std::cout << "Heap has less than 5 elements" << std::endl;
            }
        }
        else if (command == "pa")
        { // prints the heap, including total number of items and k
            if (pq->size() != 0)
            {
                cout << total_items << endl;
                cout << k << endl;
                pq->print();
            }
        }
        else if (command == "pl")
        { // prints the left path, including total number of items and k
            if (pq->size() != 0)
            {
                cout << total_items << endl;
                cout << k << endl;
                pq->printLeftPath();
            }
        }
        else if (command == "pr")
        { // prints the right path,  including total number of items and k
            if (pq->size() != 0)
            {
                cout << total_items << endl;
                cout << k << endl;
                pq->printRightPath();
            }
        }
        else if (command == "es")
        {
            int sum = pq->crazy_clean();
            std::cout << sum << endl;
        }
    }
}

