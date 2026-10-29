template <typename Type, int Ord>
class BTree
{
    Node<Type, Ord> *root;

private:
    void splitChild(Node<Type, Ord> *parent, int childIdx);

    void insertRecursive(Node<Type, Ord> *node, const Type &value);
    void merge(Node<Type, Ord> *parent,int indx);
    void insert(Type key);
    void remove(Type key);
    pair<Node<Type,Ord>*,int> search(Type key);
    void display(); // inorder
    void borrowLeft(Node<Type,Ord> *parent, int indx);
    void borrowRight(Node<Type,Ord> *parent, int indx);
};
