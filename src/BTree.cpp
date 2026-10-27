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