/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file symtable.c
 * @brief Implementation of operations with Red Black Tree.
 *
 * @author Adam Bojnanský
 * @date 2024-10-23
 */

#include "symtable.h"

RBNode* create_RBNode(char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    if (newNode == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    // Alokácia pamäte pre RBNodeData
    newNode->data = (RBNodeData*)malloc(sizeof(RBNodeData));
    if (newNode->data == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        free(newNode); // Uvoľnenie alokovanej pamäte pre uzol
        return NULL;
    }
    int ret = DString_init(&newNode->name);
    if (ret > 0) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        free(newNode->data); // Uvoľnenie alokovanej pamäte pre RBNodeData
        free(newNode);
        return NULL;
    }
    DString_concat(&newNode->name, name, NULL);
    newNode->data->varType = varType;
    newNode->data->nodeType = nodeType;
    newNode->data->ptr = ptr;
    newNode->data->nullable = nullable;
    newNode->data->changed = changed;
    newNode->data->return_found = false;
    newNode->color = RED; // New nodes are red by default
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

RedBlackTree* create_RBTree(void) {
    RedBlackTree* tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    if (tree == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    tree->NIL = create_RBNode("", -1, -1, false, false, NULL);
    tree->NIL->color = BLACK;
    tree->root = tree->NIL;
    return tree;
}

void left_rotate(RedBlackTree* tree, RBNode* x) {
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

void right_rotate(RedBlackTree* tree, RBNode* y) {
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

void fix_violation(RedBlackTree* tree, RBNode* z) {
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
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
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
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK; // Ensure the root is always black
}

int insert_RBNode(RedBlackTree* tree, char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr) {
    RBNode* newNode = create_RBNode(name, nodeType, varType, nullable, changed, ptr);
    if (newNode == NULL)
        return COMPILER_ERROR_INTERNAL;

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
    fix_violation(tree, newNode);
    return 0;
}

RBNode* minimum_RBTree(RedBlackTree* tree, RBNode* node) {
    while (node->left != tree->NIL) {
        node = node->left;
    }
    return node;
}

void fix_deletion(RedBlackTree* tree, RBNode* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right; // Sibling

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(tree, x->parent);
                x = tree->root; // Exit loop
            }
        } else {
            RBNode* w = x->parent->left; // Sibling

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(tree, x->parent);
                x = tree->root; // Exit loop
            }
        }
    }
    x->color = BLACK; // Ensure the node is black
}

RBNode* find_RBNode(RBNode* root, char* name) {
    // Ak je strom prázdny alebo ak sme našli uzol s hľadaným názvom
    if (root == NULL || strcmp(root->name.data, name) == 0)
        return root;
    // Porovnávame názov s aktuálnym uzlom, aby sme určili, kam pokračovať
    if (strcmp(name, root->name.data) < 0)
        return find_RBNode(root->left, name);
    else
        return find_RBNode(root->right, name);
}

void delete_RBNode(RedBlackTree* tree, RBNode* nodeToDelete) {
    RBNode* z = nodeToDelete;
    RBNode* x, *y;

    // Node not found
    if (z == tree->NIL)
        return;

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
        y = minimum_RBTree(tree, z->right); // Get successor
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
    free(z->data); // Free the data
    free(z); // Free the node

    if (originalColor == BLACK) {
        fix_deletion(tree, x);
    }
}

void in_order_traversal(RBNode* node, RBNode* NIL) {
    if (node != NIL) {
        in_order_traversal(node->left, NIL);
        printf("(%s | %s) ", node->name.data, (node->color == RED) ? "RED" : "BLACK");
        in_order_traversal(node->right, NIL);
    }
}

void print_RBTree(RBNode* node, RBNode* NIL, int level) {
    if (node == NIL) return;

    print_RBTree(node->right, NIL, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("(%s - %s)\n", node->name.data, node->color == RED ? "RED" : "BLACK");
    print_RBTree(node->left, NIL, level + 1);
}

void free_RBTree(RBNode* node, RBNode* NIL) {
    if (node != NIL) {
        free_RBTree(node->left, NIL);
        free_RBTree(node->right, NIL);
        DString_free(&node->name);
        free(node->data);
        free(node);
    }
}
