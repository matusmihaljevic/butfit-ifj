/**
 * @file parser.h
 * 
 * Syntaktický analyzátor (hlavičkový súbor)
 * 
 * IFJ Projekt 2024
 * 
 * @author <xbojnaa00> Adam Bojnanský
 * @author <xmihalm00> Matúš Mihaljevič
 * @author <xpribik00> Kristián Pribila
 * @author <xpodmaj00> Jaroslav Podmajerský
 */

typedef struct Error
{
    int err_num = 0;
    char* err_msg = "";
}sError;

typedef struct Token
{
    char* lexeme = "";      // lexeme of a variable, fn ...  
    int type = 0;           // variable, keyword, identifier, ...
    __uint128_t value = 0;
    int line = 0;
    sError Error;
}sToken;

#define E_LEXICAL               1  //<Chyba v rámci lexikálnej analýzy>
#define E_SYNTAX                2  //<Chyba v rámci syntaktickej analýzy>
#define E_SEMANTIC_udcl         3  //<Nedefinová funkcia alebo premenná>
#define E_SEMANTIC_ivld         4  //<Nesprávny počet/typ parametrov u volania funkcie či nepovolené zahodenie navrátovej hodnoty>
#define E_SEMANTIC_rdfn         5  //<Redefinícia premennej alebo funkcie alebo priradenie do nemodifik. premennej>
#define E_SEMANTIC_o_u_expr     6  //<Chýbajúci alebo prebývajúci výraz v príkazu návratu z funkcie>
#define E_SEMANTIC_incmpt       7  //<Nekompatibilný typ výrazu (priradenia s pretypovaním)> 
#define E_SEMANTIC_tmiss        8  //<Neni uvedený typ premennej (nie je možné ju ani odvodiť)>
#define E_SEMANTIC_nmod         9  //<Nevyužitá premenná v jej rozsahu alebo mimo jej rozsah (mod. const ...)>
#define E_SEMANTIC_else         10 //<Iná sémantická chyba>
#define E_INTERN                99 //<Chyba nespôsobená vstupom (chyba alokácie pamäte atd.)>

