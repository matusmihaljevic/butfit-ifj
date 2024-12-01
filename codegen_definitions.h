/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 *
 * @file codegen_definitions.h
 * @brief Header file for code definitions used in the program.
 *
 * This file provides all definitions used in codegen.c
 *
 * @author Jaroslav Podmajerský
 */

#define PROLOGUE   DString_concat(&Output,      \
                   ".IFJcode24\n"               \
                   "DEFVAR GF@GF_RESULT\n"      \
                   "DEFVAR GF@_\n"              \
                   "DEFVAR GF@GF_SUBSTR\n"      \
                   "DEFVAR GF@GF_SBSTR_O\n"     \
                   "DEFVAR GF@GF_OPERAND1\n"    \
                   "DEFVAR GF@GF_OPERAND2\n"    \
                   "JUMP $main\n",              \
                   NULL);

#define INTERN_FN_STRING  ;

#define INTERN_FN_WRITE   DString_concat(&Output,"POPS GF@GF_RESULT\n",           \
                                        "WRITE GF@GF_RESULT\n",NULL);

#define INTERN_FN_READI32 DString_concat(&Output,"READ GF@GF_RESULT int\n",       \
                                        "PUSHS GF@GF_RESULT\n",NULL);

#define INTERN_FN_READF64 DString_concat(&Output,"READ GF@GF_RESULT float\n",     \
                                        "PUSHS GF@GF_RESULT\n",NULL);

#define INTERN_FN_READSTR DString_concat(&Output,"READ GF@GF_RESULT string\n",    \
                                        "PUSHS GF@GF_RESULT\n",NULL);

#define INTERN_FN_LENGHT DString_concat(&Output,"POPS GF@GF_RESULT",                   \
                                        "\nSTRLEN GF@GF_RESULT GF@GF_RESULT",          \
                                        "\nPUSHS GF@GF_RESULT\n",NULL);

#define INTERN_FN_CONCAT DString_concat(&Output,"POPS GF@GF_OPERAND2\nPOPS GF@GF_OPERAND1\n"            \
                                                "CONCAT GF@GF_RESULT GF@GF_OPERAND1 GF@GF_OPERAND2\n"   \
                                                "PUSHS GF@GF_RESULT\n",NULL);

#define INTERN_FN_STRCMP DString_concat(&Output,"POPS GF@GF_OPERAND2\nPOPS GF@GF_OPERAND1\n"       \
                                               "JUMPIFEQ $str_eq_",get_CB_hash(built_in_fn_node)," GF@GF_OPERAND1 GF@GF_OPERAND2\n"                  \
                                               "LT GF@GF_RESULT GF@GF_OPERAND1 GF@GF_OPERAND2\n"                                					 \
                                               "JUMPIFEQ $str_less_",get_CB_hash(built_in_fn_node)," GF@GF_RESULT bool@true\n"                       \
                                               "PUSHS int@1\nJUMP $str_cmp_end_",get_CB_hash(built_in_fn_node),"\n"                                  				      \
                                               "LABEL $str_less_",get_CB_hash(built_in_fn_node),"\nPUSHS int@-1\nJUMP $str_cmp_end_",get_CB_hash(built_in_fn_node),"\n"   \
                                               "LABEL $str_eq_",get_CB_hash(built_in_fn_node),"\nPUSHS int@0\nLABEL $str_cmp_end_",get_CB_hash(built_in_fn_node),"\n",    \
                                               NULL);

#define INTERN_FN_SUBSTR DString_concat(&Output,"POPS GF@GF_OPERAND2\n"           \
                                        "POPS GF@GF_OPERAND1\n"                      \
                                        "LT GF@GF_RESULT GF@GF_OPERAND1 int@0\n"     \
                                        "JUMPIFEQ $ret_nil_",get_CB_hash(built_in_fn_node)," GF@GF_RESULT bool@true\n"    \
                                        "LT GF@GF_RESULT GF@GF_OPERAND2 int@0\n"                     \
                                        "JUMPIFEQ $ret_nil_",get_CB_hash(built_in_fn_node)," GF@GF_RESULT bool@true\n"    \
                                        "GT GF@GF_RESULT GF@GF_OPERAND1 GF@GF_OPERAND2\n"            \
                                        "JUMPIFEQ $ret_nil_",get_CB_hash(built_in_fn_node)," GF@GF_RESULT bool@true\n"    \
                                        "POPS GF@GF_SUBSTR\n"        \
                                        "STRLEN GF@GF_RESULT GF@GF_SUBSTR\n"    \
                                        "LT GF@GF_RESULT GF@GF_RESULT GF@GF_OPERAND2\n"              \
                                        "JUMPIFEQ $ret_nil_",get_CB_hash(built_in_fn_node)," GF@GF_RESULT bool@true\n"    \
                                        "STRLEN GF@GF_RESULT GF@GF_SUBSTR\n"                         \
                                        "GT GF@GF_RESULT GF@GF_RESULT GF@GF_OPERAND1\n"              \
                                        "NOT GF@GF_RESULT GF@GF_RESULT\n"                            \
                                        "JUMPIFEQ $ret_nil_",get_CB_hash(built_in_fn_node)," GF@GF_RESULT bool@true\n"    \
                                        "MOVE GF@GF_SBSTR_O string@\n"                  \
                                        "LABEL $while_substr_",get_CB_hash(built_in_fn_node),"\n"            \
                                        "JUMPIFEQ $end_while_substr_",get_CB_hash(built_in_fn_node)," GF@GF_OPERAND1 GF@GF_OPERAND2\n"    \
                                        "GETCHAR GF@GF_RESULT GF@GF_SUBSTR GF@GF_OPERAND1\n"                       \
                                        "CONCAT GF@GF_SBSTR_O GF@GF_SBSTR_O GF@GF_RESULT\n"                        \
                                        "ADD GF@GF_OPERAND1 GF@GF_OPERAND1 int@1\n"     \
                                        "JUMP $while_substr_",get_CB_hash(built_in_fn_node),"\n"             \
                                        "LABEL $end_while_substr_",get_CB_hash(built_in_fn_node),"\n"        \
                                        "PUSHS GF@GF_SBSTR_O\n"                         \
                                        "JUMP $end_substr_",get_CB_hash(built_in_fn_node),"\n"               \
                                        "LABEL $ret_nil_",get_CB_hash(built_in_fn_node),"\n"                 \
                                        "PUSHS nil@nil\n"                               					 \
                                        "LABEL $end_substr_",get_CB_hash(built_in_fn_node),"\n",             \
                                        NULL);

#define INTERN_FN_ORD DString_concat(&Output,"POPS GF@GF_OPERAND1\nPOPS GF@GF_RESULT\n",        \
									   "LT GF@GF_OPERAND2 GF@GF_OPERAND1 int@0\n"				  \
									   "JUMPIFEQ $ord_null_",get_CB_hash(built_in_fn_node)," GF@GF_OPERAND2 bool@true\n"	\
									   "STRLEN GF@GF_OPERAND2 GF@GF_RESULT\n"			\
									   "SUB GF@GF_OPERAND2 GF@GF_OPERAND2 int@1\n"			\
									   "GT GF@GF_OPERAND2 GF@GF_OPERAND1 GF@GF_OPERAND2\n"		\
									   "JUMPIFEQ $ord_null_",get_CB_hash(built_in_fn_node)," GF@GF_OPERAND2 bool@true\n"	\
                                       "STRI2INT GF@GF_RESULT GF@GF_RESULT GF@GF_OPERAND1\n",     \
                                       "PUSHS GF@GF_RESULT\nJUMP $ord_end_",get_CB_hash(built_in_fn_node),"\n",	\
									   "LABEL $ord_null_",get_CB_hash(built_in_fn_node),"\n"	\
									   "PUSHS int@0\nLABEL $ord_end_",get_CB_hash(built_in_fn_node),"\n",NULL);

#define INTERN_FN_CHR   DString_concat(&Output,"INT2CHARS\n",NULL);
#define INTERN_FN_I2F   DString_concat(&Output,"INT2FLOATS\n",NULL);
#define INTERN_FN_F2I   DString_concat(&Output,"FLOAT2INTS\n",NULL);
