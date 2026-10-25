# B-Tree Implementation in C++

An implementation of a **B-Tree data structure in C++** for the Advanced Data Structures course at the Faculty of Computers and Artificial Intelligence, Cairo University.

The project focuses on implementing the fundamental B-Tree operations while following the class structure, templates, inheritance, and coding style specified in the assignment.

---

## 📌 Project Overview

A B-Tree is a self-balancing search tree in which each node can contain multiple keys and have multiple children. Unlike a binary search tree, a B-Tree reduces the tree's height by storing multiple keys in each node.

The tree maintains its properties through node splitting during insertion and borrowing or merging during deletion.

### Objectives

- Implement a generic B-Tree using C++ templates.
- Apply object-oriented programming principles through inheritance.
- Maintain the B-Tree properties after every insertion and deletion.
- Support efficient searching and depth-first traversal.
- Organize the implementation into reusable and maintainable components.

---

## ✨ Features

The program supports the following operations:

- **Insertion:** Insert a key while maintaining the B-Tree properties.
- **Deletion:** Delete a key and restore the tree properties when necessary.
- **Searching:** Determine whether a key exists in the tree.
- **Display All (Depth First):** Traverse and display the tree using depth-first traversal.

---

## 🏗️ Project Structure

```text
BTree-Assignment/
│
├── include/
│   ├── Node.h
│   ├── LeafNode.h
│   ├── InternalNode.h
│   └── BTree.h
│
├── src/
│   └── main.cpp
│
├── .gitignore
└── README.md
```

### Component Responsibilities

| Component        | Responsibility                                               |
| ---------------- | ------------------------------------------------------------ |
| `Node.h`         | Defines the base `Node` template and shared node properties. |
| `LeafNode.h`     | Defines leaf nodes that store keys without child nodes.      |
| `InternalNode.h` | Defines internal nodes that store keys and child pointers.   |
| `BTree.h`        | Manages the root and implements the tree's operations.       |
| `main.cpp`       | Provides the program entry point and interactive menu.       |

The implementation follows the required class hierarchy:

```text
                  Node<Type, Ord>
                  /             \
        LeafNode<Type, Ord>   InternalNode<Type, Ord>

                   BTree<Type, Ord>
                           |
                          root
```

The exact class relationships and interfaces follow the assignment specification.

---

## ⚙️ Requirements

To build and run the project, you need:

- A C++ compiler supporting the selected C++ standard.
- GCC with C++17 support (recommended).
- A terminal or command prompt.
- Git (optional, for version control and collaboration).

---

---

## 🖥️ Available Operations

The program provides the following operations:

| Operation                 | Description                                    |
| ------------------------- | ---------------------------------------------- |
| Insert                    | Adds a key to the B-Tree.                      |
| Delete                    | Removes a key from the B-Tree.                 |
| Search                    | Searches for a specific key.                   |
| Display All (Depth First) | Displays the tree using depth-first traversal. |

The interactive menu and output format are defined in `src/main.cpp`.

---

## 🧠 B-Tree Properties

The implementation must preserve the following properties:

1. Keys within each node are maintained in ascending order.
2. An internal node with `k` keys has `k + 1` children.
3. All leaf nodes remain at the same depth.
4. Insertion preserves the tree properties through splitting and key promotion.
5. Deletion preserves the tree properties through borrowing, merging, and root adjustment.
6. Searching follows the key ranges represented by each internal node.

The maximum and minimum numbers of keys and children depend on the chosen B-Tree order and the conventions specified in the assignment.

---

## 🧪 Testing

The implementation should be tested against normal cases, boundary cases, and operations that change the tree's structure.

### Insertion

- Insert into an empty tree.
- Insert keys in ascending and descending order.
- Trigger a leaf split.
- Trigger splits at multiple levels.
- Insert duplicate keys according to the chosen duplicate-handling policy.

### Deletion

- Delete an existing key from a leaf.
- Delete a key from an internal node.
- Trigger borrowing from either sibling.
- Trigger node merging.
- Trigger root adjustment.
- Delete the final key.
- Attempt to delete a nonexistent key.

### Searching

- Search for an existing key.
- Search for a nonexistent key.
- Search in an empty tree.

### Depth-First Traversal

- Display an empty tree.
- Display a tree containing only the root.
- Display a tree with multiple levels.
- Display the tree after insertion and deletion operations.

Test results and expected outputs can be documented in `docs/test_cases.md`.

---

## 👥 Team Members

| No. | Name       | Student ID   | Responsibilities                              |
| --- | ---------- | ------------ | --------------------------------------------- |
| 1   | [Member 1] | [Student ID] | Node classes and class architecture           |
| 2   | [Member 2] | [Student ID] | Insertion                                     |
| 3   | [Member 3] | [Student ID] | Deletion                                      |
| 4   | [Member 4] | [Student ID] | Searching and depth-first traversal           |
| 5   | [Member 5] | [Student ID] | B-Tree integration, main program, and testing |

---

## 📚 Course Information

- **University:** Cairo University
- **Faculty:** Faculty of Computers and Artificial Intelligence
- **Department:** Computer Science
- **Course:** Advanced Data Structures
- **Assignment:** Assignment 1 — B-Tree
- **Instructor:** Dr. Amin Allam
- **Academic Year:** 2026

---

## 📝 Implementation Notes

- The implementation follows the class structure specified in the assignment.
- C++ templates are used to support generic key types.
- The tree order is represented by the `Ord` template parameter, according to the agreed implementation.
- Template definitions are kept in header files to ensure they are available during compilation.
- The implementation should follow a consistent coding style and the assignment's formatting requirements.

---

## 📄 License

This project was developed for educational purposes as part of the Advanced Data Structures course at Cairo University.
