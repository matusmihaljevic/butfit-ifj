/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file symtable.c
 * @brief Implementation of operations with Red-Black Tree.
 *
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 */

#include "symtable.h"

RBNode* create_RBNode(char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    if (newNode == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    newNode->data = (RBNodeData*)malloc(sizeof(RBNodeData));
    if (newNode->data == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        free(newNode);
        return NULL;
    }
    int ret = DString_init(&newNode->name);
    if (ret > 0) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        free(newNode->data);
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
    newNode->color = RED;
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
        tree->root = y;
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
        tree->root = x;
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
            RBNode* y = z->parent->parent->right;

            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
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
            RBNode* y = z->parent->parent->left;

            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
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
    tree->root->color = BLACK;
}

int insert_RBNode(RedBlackTree* tree, char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr) {
    RBNode* newNode = create_RBNode(name, nodeType, varType, nullable, changed, ptr);
    if (newNode == NULL)
        return COMPILER_ERROR_INTERNAL;

    newNode->left = tree->NIL;
    newNode->right = tree->NIL;

    RBNode* y = NULL;
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
        tree->root = newNode;
    } else if (strcmp(newNode->name.data, y->name.data) < 0) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

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
        if (x->parent == NULL) {
            break;
        }

        if (x == x->parent->left) {
            RBNode* w = x->parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }

            if ((w->left == tree->NIL || w->left->color == BLACK) &&
                (w->right == tree->NIL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right == tree->NIL || w->right->color == BLACK) {
                    if (w->left != tree->NIL) {
                        w->left->color = BLACK;
                    }
                    w->color = RED;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != tree->NIL) {
                    w->right->color = BLACK;
                }
                left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode* w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }

            if ((w->right == tree->NIL || w->right->color == BLACK) &&
                (w->left == tree->NIL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left == tree->NIL || w->left->color == BLACK) {
                    if (w->right != tree->NIL) {
                        w->right->color = BLACK;
                    }
                    w->color = RED;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != tree->NIL) {
                    w->left->color = BLACK;
                }
                right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

RBNode* find_RBNode(RBNode* root, char* name) {
    if (root == NULL || strcmp(root->name.data, name) == 0)
        return root;
    if (strcmp(name, root->name.data) < 0)
        return find_RBNode(root->left, name);
    else
        return find_RBNode(root->right, name);
}

void transplant(RedBlackTree* tree, RBNode* u, RBNode* v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != tree->NIL) {
        v->parent = u->parent;
    }
}

void delete_RBNode(RedBlackTree* tree, RBNode* nodeToDelete) {
    RBNode* z = nodeToDelete;
    RBNode* x, *y;

    if (z == tree->NIL) {
        return;
    }

    y = z;
    Color originalColor = y->color;

    if (z->left == tree->NIL) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->NIL) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = minimum_RBTree(tree, z->right);
        originalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != tree->NIL) {
                x->parent = y;
            }
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    DString_free(&z->name);
    free(z->data);
    free(z);

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
