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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type, int Ord>
pair<Node<Type, Ord> *, int> BTree<Type, Ord>::search(Type key)
{
    Node<Type, Ord> *current = root;
    while (current != nullptr)
    {
        int indx = 0;
        // gets the first key greater than the one I'm searching for
        while (indx < current->num_keys && current->key[indx] < key)
        {
            indx++;
        }

        if (indx < current->num_keys && current->key[indx] == key)
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
    return {nullptr, -1};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Type, int Ord>
void BTree<Type, Ord>::deleteNode(Node<Type, Ord> *node)
{
    if (node->is_leaf)
        delete static_cast<LeafNode<Type, Ord> *>(node);
    else
        delete static_cast<InternalNode<Type, Ord> *>(node);
}

template <typename Type, int Ord>
void BTree<Type, Ord>::removeFromLeaf(Node<Type, Ord> *node, int idx)
{
    for (int i = idx + 1; i < node->num_keys; i++)
    {
        node->key[i - 1] = node->key[i];
    } // move the rest of the keys one place behind <-- to fill the deleted node
    node->num_keys--;
}

template <typename Type, int Ord>
void BTree<Type, Ord>::removeFromNonLeaf(InternalNode<Type, Ord> *node, int idx)
{
    Node<Type, Ord> *left = node->child[idx];
    Node<Type, Ord> *right = node->child[idx + 1];
    if (left->num_keys > minKeys() || right->num_keys <= minKeys())
    { // FIX: <=
        // Use predecessor
        Type predecessor = getPredecessor(node, idx);
        node->key[idx] = predecessor;
        removeRecursive(node->child[idx], predecessor);

        if (node->child[idx]->num_keys < minKeys())
        {
            fill(node, idx);
        }
    }
    else
    {
        // Use Successor
        Type successor = getSuccessor(node, idx);
        node->key[idx] = successor;
        removeRecursive(node->child[idx + 1], successor);

        if (node->child[idx + 1]->num_keys < minKeys())
        { // FIX: check child idx+1
            fill(node, idx + 1);
        }
    }
}

template <typename Type, int Ord>
void BTree<Type, Ord>::merge(Node<Type, Ord> *parent, int indx)
{
    InternalNode<Type, Ord> *internalParent = static_cast<InternalNode<Type, Ord> *>(parent);
    // get the left and right children of the parent
    Node<Type, Ord> *left = internalParent->child[indx];
    Node<Type, Ord> *right = internalParent->child[indx + 1];
    // merge the parent with the left child
    left->key[left->num_keys] = internalParent->key[indx];
    // increment the left child's num_keys
    left->num_keys++;
    int oldLeftKeys = left->num_keys;
    // copy all right child's keys into the left
    for (int i = 0; i < right->num_keys; i++)
    {
        left->key[oldLeftKeys + i] = right->key[i];
    }
    // if the right child is not a leaf copy all its children into the left key
    if (!right->is_leaf)
    {
        // if the nodes are not leaves cast them in internal node type
        InternalNode<Type, Ord> *leftNode = static_cast<InternalNode<Type, Ord> *>(left);
        InternalNode<Type, Ord> *rightNode = static_cast<InternalNode<Type, Ord> *>(right);
        for (int i = 0; i <= right->num_keys; i++)
        {
            leftNode->child[oldLeftKeys + i] = rightNode->child[i];
        }
    }
    left->num_keys = oldLeftKeys + right->num_keys;
    for (int i = indx; i < internalParent->num_keys - 1; i++)
    {
        internalParent->key[i] = internalParent->key[i + 1];
    }
    // index+1 cuz right leaf's elements are shifted to the left but children are not
    for (int i = indx + 1; i < internalParent->num_keys; i++)
    {
        {
            internalParent->child[i] = internalParent->child[i + 1];
        }
    }
    internalParent->child[internalParent->num_keys] = nullptr;
    internalParent->num_keys--;
    deleteNode(right); // FIX: delete through the real type
}

template <typename Type, int Ord>
void BTree<Type, Ord>::borrowLeft(Node<Type, Ord> *parent, int indx)
{
    InternalNode<Type, Ord> *internalParent = static_cast<InternalNode<Type, Ord> *>(parent);
    Node<Type, Ord> *left = internalParent->child[indx];
    Node<Type, Ord> *current = internalParent->child[indx + 1];
    // shift all elements in the current node and their children to the right one step
    for (int i = current->num_keys - 1; i >= 0; i--)
    {
        current->key[i + 1] = current->key[i];
    }
    if (!current->is_leaf)
    {
        InternalNode<Type, Ord> *leftNode = static_cast<InternalNode<Type, Ord> *>(left);
        InternalNode<Type, Ord> *currentNode = static_cast<InternalNode<Type, Ord> *>(current);
        for (int i = current->num_keys; i >= 0; i--)
        {
            currentNode->child[i + 1] = currentNode->child[i];
        }
        /* assign the left node's last key's children to the first new key in the current node
        so that when we put the left key in the parent's index all its children
        are in the right place :left children to the parent in its new position */
        currentNode->child[0] = leftNode->child[left->num_keys]; // FIX: use the InternalNode casts
        leftNode->child[left->num_keys] = nullptr;
    }
    // copy the value of the parent's key into the current's first key
    // and the left's last key into the old parent's key index
    current->key[0] = internalParent->key[indx];
    internalParent->key[indx] = left->key[left->num_keys - 1];
    // increment number of current nodes keys by 1 and decrement the left by 1
    current->num_keys++;
    left->num_keys--;
}

template <typename Type, int Ord>
void BTree<Type, Ord>::borrowRight(Node<Type, Ord> *parent, int indx)
{
    InternalNode<Type, Ord> *internalParent = static_cast<InternalNode<Type, Ord> *>(parent);
    Node<Type, Ord> *current = internalParent->child[indx];
    Node<Type, Ord> *right = internalParent->child[indx + 1];
    // copy the value of the parent's key into the last key of the current node
    current->key[current->num_keys] = internalParent->key[indx];
    internalParent->key[indx] = right->key[0];
    for (int i = 0; i < right->num_keys - 1; i++) // FIX: loop over right's keys
    {
        right->key[i] = right->key[i + 1];
    }
    if (!right->is_leaf)
    {
        InternalNode<Type, Ord> *rightNode = static_cast<InternalNode<Type, Ord> *>(right);
        InternalNode<Type, Ord> *currentNode = static_cast<InternalNode<Type, Ord> *>(current);
        // right child of current's last node is the child of the right sibling
        currentNode->child[current->num_keys + 1] = rightNode->child[0];
        for (int i = 0; i < right->num_keys; i++)
        { // FIX: right has num_keys+1 children
            rightNode->child[i] = rightNode->child[i + 1];
        }
        rightNode->child[right->num_keys] = nullptr;
    }
    right->num_keys--;
    current->num_keys++;
}

template <typename Type, int Ord>
Type BTree<Type, Ord>::getPredecessor(Node<Type, Ord> *node, int indx)
{
    Node<Type, Ord> *current = static_cast<InternalNode<Type, Ord> *>(node)->child[indx];

    while (!current->is_leaf)
        current = static_cast<InternalNode<Type, Ord> *>(current)->child[current->num_keys];
    return current->key[current->num_keys - 1]; // last key
}

template <typename Type, int Ord>
Type BTree<Type, Ord>::getSuccessor(Node<Type, Ord> *node, int indx)
{
    Node<Type, Ord> *current = static_cast<InternalNode<Type, Ord> *>(node)->child[indx + 1];

    while (!current->is_leaf)
        current = static_cast<InternalNode<Type, Ord> *>(current)->child[0];
    return current->key[0]; // first key
}

template <typename Type, int Ord>
void BTree<Type, Ord>::fill(InternalNode<Type, Ord> *parent, int idx)
{
    if (idx > 0 && parent->child[idx - 1]->num_keys > minKeys())
        borrowLeft(parent, idx - 1); // FIX: borrowLeft takes the LEFT sibling's index
    else if (idx < parent->num_keys && parent->child[idx + 1]->num_keys > minKeys())
        borrowRight(parent, idx); // FIX: borrowRight takes the underfull child's index
    else if (idx < parent->num_keys)
        merge(parent, idx); // merge with the right sibling
    else
        merge(parent, idx - 1); // last child: merge with the left sibling
}

template <typename Type, int Ord>
void BTree<Type, Ord>::removeRecursive(Node<Type, Ord> *node, const Type &key)
{
    // find first key >= key
    int idx = 0;
    while (idx < node->num_keys && node->key[idx] < key)
        idx++;

    // Case A: key is in this node
    if (idx < node->num_keys && node->key[idx] == key)
    {
        if (node->is_leaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(static_cast<InternalNode<Type, Ord> *>(node), idx);
        return;
    }

    // Case B: key is not here
    if (node->is_leaf)
    {
        cout << "The key " << key << " is not present in the tree\n";
        return;
    }

    InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(node);

    removeRecursive(internal->child[idx], key);

    // on the way back up: if the child became too small, repair it
    if (internal->child[idx]->num_keys < minKeys())
        fill(internal, idx);
}

template <typename Type, int Ord>
void BTree<Type, Ord>::remove(Type key)
{
    if (root == nullptr)
    {
        cout << "The tree is empty\n";
        return;
    }

    removeRecursive(root, key);

    // root may have become empty: the tree shrinks by one level
    if (root->num_keys == 0)
    {
        Node<Type, Ord> *old = root;

        if (root->is_leaf)
            root = nullptr;
        else
            root = static_cast<InternalNode<Type, Ord> *>(root)->child[0];

        deleteNode(old);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Type, int Ord>
void BTree<Type, Ord>::display()
{
    if (root == nullptr)
    {
        cout << "The tree is empty\n";
        return;
    }

    displayRecursive(root);
    cout << endl;
}
template <typename Type, int Ord>
void BTree<Type, Ord>::displayRecursive(Node<Type, Ord> *node)
{
    // Base case: a leaf has no children, just print its keys.
    if (node->is_leaf)
    {
        for (int i = 0; i < node->num_keys; i++)
            cout << node->key[i] << " ";
        return;
    }

    // Internal node: child[0], key[0], child[1], key[1], ..., child[n]
    InternalNode<Type, Ord> *internal =
        static_cast<InternalNode<Type, Ord> *>(node);

    for (int i = 0; i < node->num_keys; i++)
    {
        displayRecursive(internal->child[i]); // everything smaller than key[i]
        cout << node->key[i] << " ";          // then the key itself
    }

    // The last child holds everything larger than the last key.
    displayRecursive(internal->child[node->num_keys]);
}