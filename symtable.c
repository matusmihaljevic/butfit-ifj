/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file symtable.c
 * @brief Implementation of Red Black Tree structures and operations.
 *
 * @author Adam Bojnanský
 * @date 2024-10-23
 */

#include "symtable.h"

// Function to create a new node
RBNode* createNode(char* name, int type, ASTNode** data) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    if (newNode == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    int ret = DString_init(&newNode->name);
	if(ret > 0) {
		print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
	}
    DString_concat(&newNode->name, name, NULL);
    newNode->type = type;
    newNode->data = data;
    newNode->color = RED; // New nodes are red by default
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Function to initialize the Red-Black Tree
RedBlackTree* createTree(void) {
    RedBlackTree* tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    if (tree == NULL){
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    tree->NIL = (RBNode*)malloc(sizeof(RBNode)); // Create the NIL node
    if (tree->NIL == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    int ret = DString_init(&tree->NIL->name);
    if (ret > 0) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    tree->NIL->type = -1;
    tree->NIL->data = NULL;
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->root = tree->NIL;
    return tree;
}

// Left Rotate
void leftRotate(RedBlackTree* tree, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left != tree->NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NULL) {
        tree->root = y; // Update root if x is root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Right Rotate
void rightRotate(RedBlackTree* tree, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right != tree->NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NULL) {
        tree->root = x; // Update root if y is root
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

// Fix the Red-Black Tree after insertion
void fixViolation(RedBlackTree* tree, RBNode* z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right; // Uncle

            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move up the tree
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left; // Uncle

            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move up the tree
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK; // Ensure the root is always black
}

// Insertion
void insert(RedBlackTree* tree, char* name, int type, ASTNode** data) {
    RBNode* newNode = createNode(name, type, data);
    newNode->left = tree->NIL;
    newNode->right = tree->NIL;

    RBNode* y = NULL; // Parent
    RBNode* x = tree->root;

    while (x != tree->NIL) {
        y = x;
        if (strcmp(newNode->name.data, x->name.data) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    newNode->parent = y;

    if (y == NULL) {
        tree->root = newNode; // Tree was empty
    } else if (strcmp(newNode->name.data, y->name.data) < 0) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

    // Fix the Red-Black Tree properties
    fixViolation(tree, newNode);
}

// Find the minimum node in the subtree
RBNode* treeMinimum(RedBlackTree* tree, RBNode* node) {
    while (node->left != tree->NIL) {
        node = node->left;
    }
    return node;
}

// Fix the Red-Black Tree after deletion
void fixDeletion(RedBlackTree* tree, RBNode* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right; // Sibling

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root; // Exit loop
            }
        } else {
            RBNode* w = x->parent->left; // Sibling

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root; // Exit loop
            }
        }
    }
    x->color = BLACK; // Ensure the node is black
}

// Delete a node from the tree
void deleteNode(RedBlackTree* tree, char* name) {
    RBNode* z = tree->root;
    RBNode* x, *y;

    // Find the node to be deleted
    while (z != tree->NIL) {
        if (strcmp(name, z->name.data) == 0) {
            break;
        } else if (strcmp(name, z->name.data) < 0) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    // Node not found
    if (z == tree->NIL) {
        printf("Node with name %s not found.\n", name);
        return;
    }

    y = z; // Node to be deleted
    Color originalColor = y->color;

    // If the node has two children
    if (z->left == tree->NIL) {
        x = z->right;
        if (z->parent == NULL) {
            tree->root = x;
        } else if (z == z->parent->left) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
        x->parent = z->parent;
    } else if (z->right == tree->NIL) {
        x = z->left;
        if (z->parent == NULL) {
            tree->root = x;
        } else if (z == z->parent->left) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
        x->parent = z->parent;
    } else {
        y = treeMinimum(tree, z->right); // Get successor
        originalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            if (y->right != tree->NIL) {
                y->right->parent = y->parent;
            }
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        }

        if (z->parent == NULL) {
            tree->root = y; // Update root
        } else if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }

        y->parent = z->parent;
        y->left = z->left;
        z->left->parent = y;
        y->color = z->color;
    }

    DString_free(&z->name);
    free(z); // Free the node

    if (originalColor == BLACK) {
        fixDeletion(tree, x);
    }
}

// In-Order Traversal (for testing)
void inOrderTraversal(RBNode* node, RBNode* NIL) {
    if (node != NIL) {
        inOrderTraversal(node->left, NIL);
        printf("(%d: %s | %s) ", node->type, node->name.data, (node->color == RED) ? "RED" : "BLACK");
        inOrderTraversal(node->right, NIL);
    }
}

// Free Memory
void freeTree(RBNode* node, RBNode* NIL) {
    if (node != NIL) {
        freeTree(node->left, NIL);
        freeTree(node->right, NIL);
        DString_free(&node->name);
        free(node);
    }
}
