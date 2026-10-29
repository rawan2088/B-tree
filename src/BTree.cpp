#include "Node.h"
#include "InternalNode.h"
#include "LeafNode.h"
#include "BTree.h"
#include <iostream>
using namespace std;

// template <typename Type, int Ord>
// void BTree<Type, Ord>::insert(Type value)
// {

//     if (root == nullptr)
//     {
//         root = new LeafNode<Type, Ord>();
//         root->is_leaf = true;
//         root->num_keys = 0;
//     }

//     if (root->num_keys == Ord - 1)
//     {
//         InternalNode<Type, Ord> *newRoot = new InternalNode<Type, Ord>();
//         newRoot->num_keys = 0;
//         newRoot->is_leaf = false;
//         newRoot->child[0] = root;
//         splitChild(newRoot, 0);
//     }

//     Node<Type, Ord> currNode = root;
//     while (!currNode->is_leaf)
//     {
//         InternalNode<Type, Ord> internal = static_cast<InternalNode<Type, Ord> *>(currNode);
//         int i = currNode->num_keys - 1;
//         while (value < currNode->key[i])
//         {
//             i--;
//             if (i < 0)
//                 break;
//         }
//         i++;

//         if (internal->child[i]->num_keys == Ord - 1)
//         {
//             splitChild(currNode, i);
//             // to determine where the value fits after the split (i++ if the value belongs to the new right subtree)
//             if (value > currNode->key[i])
//             {
//                 i++;
//             }
//         }
//         currNode = internal->child[i];
//     }

//     int i = currNode->num_keys - 1;
//     while (value < currNode->key[i])
//     {
//         if (i < 0)
//             break;
//         currNode->key[i + 1] = currNode->key[i];
//         i--;
//     }
//     currNode->key[i + 1] = value;
//     currNode->num_keys++;
// };

template <typename Type, int Ord>
void BTree<Type, Ord>::insert(Type value)
{
    // Case 1: The tree is empty.
    if (root == nullptr)
    {
        root = new LeafNode<Type, Ord>();
        root->is_leaf = true;
        root->num_keys = 1;
        root->key[0] = value;
        return;
    }

    // Insert into the existing tree.
    insertRecursive(root, value);

    // Check after insertion if the root is overflowed, other nodes would be checked in the recursive function
    if (root->num_keys == Ord)
    {
        InternalNode<Type, Ord> *newRoot =
            new InternalNode<Type, Ord>();

        newRoot->child[0] = root;

        splitChild(newRoot, 0);

        root = newRoot;
    }
}

template <typename Type, int Ord>
void BTree<Type, Ord>::insertRecursive(
    Node<Type, Ord> *node,
    const Type &value)
{
    // Case 1: We reached a leaf.
    if (node->is_leaf)
    {
        int i = node->num_keys - 1;

        // Shift larger keys to the right.
        while (i >= 0 && value < node->key[i])
        {
            node->key[i + 1] = node->key[i];
            i--;
        }

        // Avoid inserting duplicates.
        if (i >= 0 && node->key[i] == value)
            return;

        // Insert the new key.
        node->key[i + 1] = value;
        node->num_keys++;

        return;
    }

    // Case 2: We are at an internal node.
    InternalNode<Type, Ord> *internal =
        static_cast<InternalNode<Type, Ord> *>(node);

    int childIdx = 0;

    while (childIdx < node->num_keys &&
           value >= node->key[childIdx])
    {
        childIdx++;
    }

    // Insert recursively into the selected child.
    insertRecursive(internal->child[childIdx], value);

    // If the child overflowed, split it.
    if (internal->child[childIdx]->num_keys == Ord)
    {
        splitChild(internal, childIdx);
    }
}

template <typename Type, int Ord>
void BTree<Type, Ord>::splitChild(
    Node<Type, Ord> *parent,
    int childIdx)
{
    InternalNode<Type, Ord> *internalParent =
        static_cast<InternalNode<Type, Ord> *>(parent);

    // The child that contains Ord keys.
    Node<Type, Ord> *childCopy =
        internalParent->child[childIdx];

    // The index of the middle key.
    const int middle = Ord / 2;

    // Create the new right-hand node.
    Node<Type, Ord> *upperHalf;

    if (childCopy->is_leaf)
    {
        upperHalf = new LeafNode<Type, Ord>();
    }
    else
    {
        upperHalf = new InternalNode<Type, Ord>();
    }

    upperHalf->is_leaf = childCopy->is_leaf;

    // Copy keys after the middle key.
    upperHalf->num_keys = Ord - middle - 1;

    for (int i = 0; i < upperHalf->num_keys; i++)
    {
        upperHalf->key[i] =
            childCopy->key[middle + 1 + i];
    }

    // If the child is internal, move its child pointers too.
    if (!childCopy->is_leaf)
    {
        InternalNode<Type, Ord> *oldInternal =
            static_cast<InternalNode<Type, Ord> *>(childCopy);

        InternalNode<Type, Ord> *newInternal =
            static_cast<InternalNode<Type, Ord> *>(upperHalf);

        for (int i = 0; i <= upperHalf->num_keys; i++)
        {
            newInternal->child[i] =
                oldInternal->child[middle + 1 + i];

            oldInternal->child[middle + 1 + i] = nullptr;
        }
    }

    // Save the median before changing the left node.
    Type median = childCopy->key[middle];

    // The left node keeps the keys before the median.
    childCopy->num_keys = middle;

    // Shift the parent's child pointers to make room.
    for (int i = parent->num_keys; i >= childIdx + 1; i--)
    {
        internalParent->child[i + 1] =
            internalParent->child[i];
    }

    // Attach the new right-hand node.
    internalParent->child[childIdx + 1] = upperHalf;

    // Shift the parent's keys to make room for the median.
    for (int i = parent->num_keys - 1; i >= childIdx; i--)
    {
        parent->key[i + 1] = parent->key[i];
    }

    // Promote the median into the parent.
    parent->key[childIdx] = median;
    parent->num_keys++;
}

template<typename Type, int Ord>
pair<Node<Type,Ord>*,int> BTree<Type, Ord>::search(Type key)
{
    Node<Type, Ord> *current = root;
    while (current != nullptr)
        {
        int indx=0;
        //gets the first key greater than the one I'm searching for
        while (indx<current->num_keys && current->key[indx]< key)
            {
            indx++;
        }

        if (indx<current->num_keys && current->key[indx] == key)
            {
            return {current, indx};
        }
        // if the key is not found
        if (current->is_leaf)
            {
            return {nullptr, -1};
        }
        current = static_cast<InternalNode<Type, Ord> *>(current)->child[indx];

    }
    return {nullptr,-1};
}

template<typename Type, int Ord>
void BTree<Type, Ord>::merge(Node<Type,Ord>* parentNode,int indx) {
    InternalNode<Type, Ord> *parent =static_cast<InternalNode<Type, Ord>*>(parentNode);
    //get the left and right children of the parent
    Node<Type, Ord> *left = parent->child[indx];
    Node<Type, Ord> *right = parent->child[indx+1];
    //merge the parent with the left child
    left->key[left->num_keys] = parent->key[indx];
    // increment the left child's num_keys
    left->num_keys++;
    int oldLeftKeys = left->num_keys;
    //copy all right child's keys into the left
    for (int i = 0; i < right->num_keys; i++)
    {
        left->key[oldLeftKeys+i] = right->key[i];
    }
    // if the right child is not a leaf copy all its children into the left key
    if (!right->is_leaf)
    {
        // if the nodes are not leaves cast them in internal node type
        InternalNode<Type, Ord> *leftNode = static_cast<InternalNode<Type, Ord> *>(left);
        InternalNode<Type, Ord> *rightNode = static_cast<InternalNode<Type, Ord> *>(right);
        for (int i = 0; i <= right->num_keys; i++)
        {
            leftNode->child[oldLeftKeys+i] = rightNode->child[i];
        }
    }
    left->num_keys = oldLeftKeys+right->num_keys;
    for (int i = indx; i < parent->num_keys-1; i++)
    {
        parent->key[i] = parent->key[i+1];
    }
    //index+1 cuz right leaf's elements are shifted to the left but children are not
    for (int i = indx+1; i < parent->num_keys; i++) {
        {
            parent->child[i] = parent->child[i+1];
        }
    }
    parent->child[parent->num_keys] = nullptr;
    parent->num_keys--;
    delete right;
}

template<typename Type, int Ord>
void BTree<Type, Ord>::borrowLeft(Node<Type, Ord> *parentNode, int indx)
{
    InternalNode<Type, Ord> *parent = static_cast<InternalNode<Type, Ord>*>(parentNode);
    Node<Type, Ord> *left = parent->child[indx];
    Node<Type, Ord> *current = parent->child[indx+1];
    //shift all elements in the current node and their children to the right one step
    for (int i = current->num_keys-1; i >=0; i--)
    {
        current->key[i+1] = current->key[i];
    }
    if (!current->is_leaf)
    {
        InternalNode<Type, Ord> *leftNode = static_cast<InternalNode<Type, Ord> *>(left);
        InternalNode<Type, Ord> *currentNode = static_cast<InternalNode<Type, Ord> *>(current);
        for (int i = current->num_keys; i >=0; i--)
        {
            currentNode->child[i+1] = currentNode->child[i];
        }
        /* assign the left node's last key's children to the first new key in the current node
        so that when we put the left key in the parent's index all its children
        are in the right place :left children to the parent in its new position */
        current->child[0] = left->child[left->num_keys];
        left->child[left->num_keys] = nullptr;
    }
    //copy the value of the parent's key into the current's first key
    //and the left's last key into the old parent's key index
    current->key[0] = parent->key[indx];
    parent->key[indx] = left->key[left->num_keys-1];
    //increment number of current nodes keys by 1 and decrement the left by 1
    current->num_keys++;
    left->num_keys--;
}

template<typename Type, int Ord>
void BTree<Type, Ord>::borrowRight(Node<Type, Ord> *parentNode, int indx)
{
    InternalNode<Type, Ord> *parent = static_cast<InternalNode<Type, Ord>*>(parentNode);
    Node<Type, Ord> *current = parent->child[indx];
    Node<Type, Ord> *right = parent->child[indx+1];
    //copy the value of the parent's key into the last key of the current node
    current->key[current->num_keys] = parent->key[indx];
    parent->key[indx] = right->key[0];
    for (int i = 0; i < current->num_keys-1; i++)
    {
        right->key[i] = right->key[i+1];
    }
    if (!right->is_leaf)
    {
        InternalNode<Type, Ord> *rightNode = static_cast<InternalNode<Type, Ord> *>(right);
        InternalNode<Type, Ord> *currentNode = static_cast<InternalNode<Type, Ord> *>(current);
        //right child of current's last node is the child of the right sibling
        currentNode->child[current->num_keys+1] = rightNode->child[0];
        for (int i =0; i<right->num_keys-1; i++) {
            rightNode->child[i] = rightNode->child[i+1];
        }
        rightNode->child[right->num_keys] = nullptr;
    }
    right->num_keys--;
    current->num_keys++;
}