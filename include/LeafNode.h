#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include "Node.h"

template <typename Type, int Ord>
struct LeafNode : public Node<Type, Ord>
{
    LeafNode()
        : Node<Type, Ord>(true)
    {
    }
};

#endif