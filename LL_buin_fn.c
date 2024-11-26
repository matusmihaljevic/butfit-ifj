/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 *
 * @file LL_buin_fc.c
 * @brief Linked list which holds built in functions
 *
 * This file creates data into a LL of built in functions
 *
 * @author Jaroslav Podmajerský
 */

#include "LL_buin_fn.h"

Built_in_node* createNode(char *name, TypeProperties ret_type,
                 TypeProperties *parameters, size_t param_count) {
    Built_in_node *new_node = (Built_in_node *)malloc(sizeof(Built_in_node));
    if (!new_node) {
        print_error(COMPILER_ERROR_INTERNAL,0,"malloc failed!");
        exit(COMPILER_ERROR_INTERNAL);
    }

    new_node->name = name;
    new_node->ret_type = ret_type;

    new_node->parameters = (TypeProperties *)malloc(param_count * sizeof(TypeProperties));
    if (!new_node->parameters) {
        print_error(COMPILER_ERROR_INTERNAL,0,"malloc failed!");
        free(new_node->name);
        free(new_node);
        exit(COMPILER_ERROR_INTERNAL);
    }
    for (size_t i = 0; i < param_count; ++i) {
        new_node->parameters[i] = parameters[i];
    }

    new_node->next = NULL;
    return new_node;
}

void addNode(Built_in_node **head, Built_in_node *new_node) {
    new_node->next = *head;
    *head = new_node;
}

void freeList(Built_in_node *head) {
    while (head) {
        Built_in_node *temp = head;
        head = head->next;

        free(temp->name);
        free(temp->parameters);
        free(temp);
    }
}

int init_linked_list(Built_in_node **head) {
    //ifj.readstr ifj.readi32 ifj.readf64
    TypeProperties readstr_ret = {true, true, false, U8};
    Built_in_node *readstr_node = createNode("readstr", readstr_ret, NULL, 0);

    TypeProperties readi32_ret = {true, true, false, INT};
    Built_in_node *readi32_node = createNode("readi32", readi32_ret, NULL, 0);

    TypeProperties readf64_ret = {true, true, false, FLOAT};
    Built_in_node *readf64_node = createNode("readf64", readf64_ret, NULL, 0);

    //ifj.write
    TypeProperties write_ret = {true, false, false, VOID};
    TypeProperties write_param[1] = {{true, true, false, ALL}};
    Built_in_node *write_node = createNode("write", write_ret, write_param, 1);

    //ifj.i2f ifj.f2i
    TypeProperties i2f_ret = {true, false, false, FLOAT};
    TypeProperties i2f_param[1] = {{true, false, false, INT}};
    Built_in_node *i2f_node = createNode("i2f", i2f_ret, i2f_param, 1);

    TypeProperties f2i_ret = {true, false, false, INT};
    TypeProperties f2i_param[1] = {{true, false, false, FLOAT}};
    Built_in_node *f2i_node = createNode("f2i", f2i_ret, f2i_param, 1);

    //ifj.string
    TypeProperties string_ret = {true, false, false, U8};
    TypeProperties string_param[1] = {{true, true, false, ALL}};
    Built_in_node *string_node = createNode("string", string_ret, string_param, 1);

    //ifj.lenght
    TypeProperties length_ret = {true, false, false, INT};
    TypeProperties length_param[1] = {{true, false, false, U8}};
    Built_in_node *length_node = createNode("length", length_ret, length_param, 1);

    //ifj.concat
    TypeProperties concat_ret = {true, false, false, U8};
    TypeProperties concat_param[2] = {{true, false, false, U8},{true, false, false, U8}};
    Built_in_node *concat_node = createNode("concat", concat_ret, concat_param, 2);

    //ifj.substring
    TypeProperties substr_ret = {true, true, false, U8};
    TypeProperties substr_param[3] = {{true, false, false, U8},{true, false, false, INT},{true, false, false, INT}};
    Built_in_node *substr_node = createNode("substring", substr_ret, substr_param, 3);

    //ifj.strcmp
    TypeProperties strcmp_ret = {true, false, false, INT};
    TypeProperties strcmp_param[2] = {{true, false, false, U8},{true, false, false, U8}};
    Built_in_node *strcmp_node = createNode("strcmp", strcmp_ret, strcmp_param, 2);

    //ifj.ord
    TypeProperties ord_ret = {true, false, false, INT};
    TypeProperties ord_param[2] = {{true, false, false, U8},{true, false, false, INT}};
    Built_in_node *ord_node = createNode("ord", ord_ret, ord_param, 2);

    //ifj.chr
    TypeProperties chr_ret = {true, false, false, U8};
    TypeProperties chr_param[1] = {{true, false, false, INT}};
    Built_in_node *chr_node = createNode("chr", chr_ret, chr_param, 1);


    addNode(head, readstr_node);
    addNode(head, readi32_node);
    addNode(head, readf64_node);

    addNode(head, write_node);

    addNode(head, i2f_node);
    addNode(head, f2i_node);

    addNode(head, string_node);
    addNode(head, length_node);
    addNode(head, concat_node);
    addNode(head, substr_node);
    addNode(head, strcmp_node);
    addNode(head, ord_node);
    addNode(head, chr_node);
    return 0;
}
