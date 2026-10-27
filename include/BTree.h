

template <typename Type, int Ord>
class BTree
{
    Node<Type, Ord> *root;

private:
    void splitChild(Node<Type, Ord> *parent, int childIdx);

    void insertRecursive(Node<Type, Ord> *node, const Type &value);
    void merge();
    void insert(Type key);
    void remove(Type key);
    bool search(Type key);
    void display(); // inorder
};
