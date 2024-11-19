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

#define IMPLICIT_RETYPE DString_concat(&Output,                         \
                   "#ignore----implicit-retyping-----\n"                \
                   "POPS GF@GF_OPERAND1\n"                              \
                   "POPS GF@GF_OPERAND2\n"                              \
                   "TYPE GF@GF_RESULT GF@GF_OPERAND2\n"                 \
                   "PUSHS GF@GF_OPERAND2\n"                             \
                   "TYPE GF@GF_OPERAND2 GF@GF_OPERAND1\n"               \
                   "CONCAT GF@GF_RESULT GF@GF_OPERAND2 GF@GF_RESULT\n"  \
                   "JUMPIFEQ $floatint_",&curr_id," GF@GF_RESULT string@floatint\n"  \
                   "JUMPIFEQ $intfloat_",&curr_id," GF@GF_RESULT string@intfloat\n"  \
                   "JUMP $pushback_",&curr_id,"\n",  \
                   "LABEL $intfloat_",&curr_id,"\n", \
                   "PUSHS GF@GF_OPERAND1\n",         \
                   "INT2FLOATS\n",                   \
                   "JUMP $end_",&curr_id,"\n",       \
                   "LABEL $floatint_",&curr_id,"\n", \
                   "INT2FLOATS\n",                   \
                   "PUSHS GF@GF_OPERAND1\n",         \
                   "JUMP $end_",&curr_id,"\n"        \
                   "LABEL $pushback_",&curr_id,"\n"   \
                   "PUSHS GF@GF_OPERAND1\n"          \
                   "LABEL $end_",&curr_id,"\n",      \
                   "#ignore-------------------------\n",               \
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

#define INTERN_FN_STRCMP char* curr_id = id++;                                                    \
                        DString_concat(&Output,"POPS GF@GF_OPERAND2\nPOPS GF@GF_OPERAND1\n"       \
                                               "JUMPIFEQ $str_eq_",&curr_id," GF@GF_OPERAND1 GF@GF_OPERAND2\n"                  \
                                               "LT GF@GF_RESULT GF@GF_OPERAND1 GF@GF_OPERAND2\n"                                \
                                               "JUMPIFEQ $str_less_",&curr_id," GF@GF_RESULT bool@true\n"                       \
                                               "PUSHS int@1\nJUMP $str_cmp_end_",&curr_id,"\n"                                  \
                                               "LABEL $str_less_",&curr_id,"\nPUSHS int@-1\nJUMP $str_cmp_end_",&curr_id,"\n"   \
                                               "LABEL $str_eq_",&curr_id,"\nPUSHS int@0\nLABEL $str_cmp_end_",&curr_id,"\n",    \
                                               NULL); 


#define INTERN_FN_SUBSTR    char* curr_id = id++;                                    \
                            DString_concat(&Output,"POPS GF@GF_OPERAND2\n"           \
                                        "POPS GF@GF_OPERAND1\n"                      \
                                        "LT GF@GF_RESULT GF@GF_OPERAND1 int@0\n"     \
                                        "JUMPIFEQ $ret_nil_",&curr_id," GF@GF_RESULT bool@true\n"    \
                                        "LT GF@GF_RESULT GF@GF_OPERAND2 int@0\n"              \
                                        "JUMPIFEQ $ret_nil_",&curr_id," GF@GF_RESULT bool@true\n"    \
                                        "GT GF@GF_RESULT GF@GF_OPERAND1 GF@GF_OPERAND2\n"     \
                                        "JUMPIFEQ $ret_nil_",&curr_id," GF@GF_RESULT bool@true\n"    \
                                        "POPS GF@GF_SUBSTR\n"                           \ 
                                        "STRLEN GF@GF_RESULT GF@GF_SUBSTR\n"   \
                                        "LT GF@GF_RESULT GF@GF_RESULT GF@GF_OPERAND2\n"              \
                                        "JUMPIFEQ $ret_nil_",&curr_id," GF@GF_RESULT bool@true\n"    \
                                        "STRLEN GF@GF_RESULT GF@GF_SUBSTR\n"                         \
                                        "GT GF@GF_RESULT GF@GF_RESULT GF@GF_OPERAND1\n"              \
                                        "NOT GF@GF_RESULT GF@GF_RESULT\n"                            \
                                        "JUMPIFEQ $ret_nil_",&curr_id," GF@GF_RESULT bool@true\n"    \
                                        "MOVE GF@GF_SBSTR_O string@\n"                  \
                                        "LABEL $while_substr_",&curr_id,"\n"            \
                                        "JUMPIFEQ $end_while_substr_",&curr_id," GF@GF_OPERAND1 GF@GF_OPERAND2\n"    \
                                        "GETCHAR GF@GF_RESULT GF@GF_SUBSTR GF@GF_OPERAND1\n"                       \
                                        "CONCAT GF@GF_SBSTR_O GF@GF_SBSTR_O GF@GF_RESULT\n"                        \
                                        "ADD GF@GF_OPERAND1 GF@GF_OPERAND1 int@1\n"     \
                                        "JUMP $while_substr_",&curr_id,"\n"             \
                                        "LABEL $end_while_substr_",&curr_id,"\n"        \
                                        "PUSHS GF@GF_SBSTR_O\n"                         \
                                        "JUMP $end_substr_",&curr_id,"\n"               \
                                        "LABEL $ret_nil_",&curr_id,"\n"                 \
                                        "PUSHS nil@nil\n"                               \
                                        "LABEL $end_substr_",&curr_id,"\n",             \
                                        NULL);

#define INTERN_FN_ORD   DString_concat(&Output,"POPS GF@GF_OPERAND1\nPOPS GF@GF_RESULT\n",        \
                                       "STRI2INT GF@GF_RESULT GF@GF_RESULT GF@GF_OPERAND1\n",     \
                                       "PUSHS GF@GF_RESULT\n",NULL);

#define INTERN_FN_CHR   DString_concat(&Output,"INT2CHARS\n",NULL);
#define INTERN_FN_I2F   DString_concat(&Output,"INT2FLOATS\n",NULL);
#define INTERN_FN_F2I   DString_concat(&Output,"FLOAT2INTS\n",NULL);
