

template <typename Type, int Ord>
class BTree
{
    Node<Type, Ord> *root;

    void splitChild(BTreeNode<T, ORDER> *x, int i);
    void merge();
    void insert(Type key);
    void remove(Type key);
    bool search(Type key);
    void display(); // inorder
};
