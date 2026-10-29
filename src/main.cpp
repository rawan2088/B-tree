#include <iostream>
#include "../include/BTree.h"
using namespace std;

int main()
{
    BTree<int, 4> tree;

    cout << "Empty tree: ";
    tree.display();

    // Test insert
    cout << "Inserting values..." << endl;
    int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int v : values)
        tree.insert(v);

    cout << "Inorder:  ";
    tree.display();
    cout << "Expected: 5 6 7 10 12 17 20 30" << endl;

    // Test search
    cout << "\nTesting search:" << endl;

    auto result = tree.search(12);
    cout << "Search 12: " << (result.first != nullptr ? "Found" : "Not found") << endl;

    result = tree.search(100);
    cout << "Search 100: " << (result.first != nullptr ? "Found" : "Not found") << endl;

    // Test remove
    cout << "\nTesting remove:" << endl;

    tree.remove(6);
    result = tree.search(6);
    cout << "Search 6 after removal: " << (result.first != nullptr ? "Found" : "Not found") << endl;

    tree.remove(12);
    result = tree.search(12);
    cout << "Search 12 after removal: " << (result.first != nullptr ? "Found" : "Not found") << endl;

    cout << "Inorder:  ";
    tree.display();
    cout << "Expected: 5 7 10 17 20 30" << endl;

    return 0;
}