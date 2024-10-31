// file_io_test.cpp
#include <gtest/gtest.h>
extern "C" {
    #include "lexer_test.h"  // Include your C code
    #include "scanner.h"
    int write_in_file(const char *filename, const char *content_of_file);
}
#include <stdlib.h>

int write_in_file(const char *filename, const char *content_of_file)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return ERR;
    }

    fprintf(file, "%s", content_of_file);
    fclose(file);

    // Re-open the file in read mode for the lexer
    file = fopen(filename, "r");
    if (file == NULL)
    {
        return ERR;
    }

    // Use the lexer function to set the file as the input source
    set_source_file(file); // Assuming this tells the lexer which file to read

    return SUCC;
}


TEST(FileIOTest, WriteInFile) {
    const char* filename = "testfile.txt";
    const char* content = "Hello, world!";

    // Test writing to the file
    EXPECT_EQ(write_in_file(filename, content), 0);

    // Open the file and check the content
    FILE *file = fopen(filename, "r");
    ASSERT_NE(file, nullptr);  // Check if the file was created
    char buffer[50];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    // Assert that the content matches
    EXPECT_STREQ(buffer, content);
    
    // Clean up
    remove(filename);
}

TEST(Expresion, Addition)
{
    const char* filename = "testfile.txt";
    const char* content = "var a = 5;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // var
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);          // 5
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;

    remove(filename);

} 


TEST(Expresion, IntAditionFail)
{
    const char* filename = "testfile.txt";
    const char* content = "int a = 0.5; ";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // int
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_INT);          // 0.5
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;


    remove(filename);

}

TEST(Expresion, IntAditionFail1)
{
    const char* filename = "testfile.txt";
    const char* content = "i32  = 0.5; ";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);   // int
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_IDENTIFIER);   // Missing a
    

    remove(filename);

}

TEST(Expresion, FloatAditionFail)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 5; ";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // f64
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_FLOAT);        // 5
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;

    remove(filename);

}


TEST(Expresion, ComplexExpresion1)
{
const char* filename = "testfile.txt";
    const char* content = "const result = (a + b) * 10 - 2;";  

    int check = write_in_file(filename, content);

    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // result
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET); //  (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);         // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET);// )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MUL);          //  *
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);          // 10
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MINUS);        // -
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);          // 2
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;

    remove(filename);

}

TEST(Expresion, ComplexExpresion2)
{
const char* filename = "testfile.txt";
    const char* content = "const result = (a * b) / (10*3) - 2;";  

    int check = write_in_file(filename, content);
    
    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // result
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET); //  (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MUL);          // *
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET);// )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_DIV);          // /
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET); //  (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);          // 10
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MUL);          // *
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);          // 3
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET);// )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MINUS);        // -
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);          // 2
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;

    remove(filename);

}


TEST(Expresion, MissingSemicolon)
{
    const char* filename = "testfile.txt";
    const char* content = "const a = 5";  // Missing semicolon at the end of the statement

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // = 
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 5
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_SEMICOLON);   //Error due to missing ;

    remove(filename);
}


TEST(Expresion, FloatAdition)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 0.075;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // f64
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_FLOAT);        // 0.075
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;


    remove(filename);

}



TEST(Expresion, FloatAditionExponent1)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 1e3;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      //f64
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_FLOAT);        // 1e3
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;


    remove(filename);

}

TEST(Expresion, FloatAditionExponent2)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 1.504e3;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);      // f64
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);       // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_FLOAT);        // 1.504e3 
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);    // ;


    remove(filename);

}

TEST(Expresion, MultipleVariableDeclaration)
{
    const char* filename = "testfile.txt";
    const char* content = "const x = 1, y = 2, z = 3;";  

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // x
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 1
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_COMMA);       // ,
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // y
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 2
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_COMMA);       // ,
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // z
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 3
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}


TEST(Strings, SimpleStringAssignment)
{
    const char* filename = "testfile.txt";
    const char* content = "const message = \"Hello, world!\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // message
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "Hello, world!"
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}


TEST(Strings, MultilineString)
{
    const char* filename = "testfile.txt";
    const char* content = "i32 a = \"hello\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // escaped
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // Multiline string token
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;


    remove(filename);
}

TEST(Strings, StringWithEscapeCharacters)
{
    const char* filename = "testfile.txt";
    const char* content = "const escaped = \"This is a \\\"quote\\\".\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // escaped
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // String with escape characters
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, EmptyString)
{
    const char* filename = "testfile.txt";
    const char* content = "const empty = \"\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // empty
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // Empty string
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, StringWithSpecialCharacters)
{
    const char* filename = "testfile.txt";
    const char* content = "const special = \"!@#$%^&*()\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // special
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // Special character string
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, MultilineDeclaration)
{
    const char* filename = "testfile.txt";
    const char* content = "const a = 5;\nconst b = 10;\nconst c = a + b;";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    // Line 1
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 5
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;
    // Line 2
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 10
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;
    // Line 3
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // c
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, Empty) {
    const char* filename = "testfile.txt";
    const char* content = "";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_EOF);       // 
    

    remove(filename);
}


TEST(Strings, ComplexStringExpression)
{
    const char* filename = "testfile.txt";
    const char* content = "const msg = \"Hello\" + \" \" + \"World\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // msg
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "Hello"
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // " "
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "World"
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, StringConcatenationWithVariables)
{
    const char* filename = "testfile.txt";
    const char* content = "const greeting = \"Hello\" + name + \"!\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // greeting
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "Hello"
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // name
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "

    remove(filename);
}


TEST(SpecialStrings, SpecialCharactersAndNewline)
{
    const char* filename = "testfile.txt";
    const char* content = "Hellow \" \\x22";
    
    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    FILE* file = fopen(filename, "r");
    ASSERT_TRUE(file != NULL);  // Ensure file was opened successfully

    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    ASSERT_STREQ(buffer, "Hellow \" \\x22");  // Verify correct file content

    remove(filename);
}

TEST(Funkcion, BuildInFunkcionCorrect)
{
    const char* filename = "testfile.txt";
    const char* content = " ifj . write";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // ifj
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_DOT);          // .
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INTERN);       // write

    remove(filename);
}

TEST(Identifier, ValidIdentifiers) {
    const char* filename = "testfile.txt";
    const char* content = "myVar _internalVar i32max";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // myVar
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // _internalVar
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);   // i32max

    remove(filename);
}

TEST(Identifier, InvalidIdentifiers1) {
    const char* filename = "testfile.txt";
    const char* content = "3var var-123";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_IDENTIFIER);   // 3var

    remove(filename);
}


TEST(Identifier, InvalidIdentifiers2) {
    const char* filename = "testfile.txt";
    const char* content = "var-123";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_IDENTIFIER);   // var-123

    remove(filename);
}


TEST(Identifier, KeywordAsIdentifier) {
    const char* filename = "testfile.txt";
    const char* content = "fn var return";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // fn
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // var
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // return

    remove(filename);
}

TEST(TypeIdentifier, OptionalAndComplexTypes) {
    const char* filename = "testfile.txt";
    const char* content = "?i32 []u8 ?[ifj]u8";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_QUESTION); // ?i32
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // []u8
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD); // ?[ifj]u8

    remove(filename);
}



TEST(ErrorCases, MalformedIdentifiersAndLiterals) {
    const char* filename = "testfile.txt";
    const char* content = "3foo 1.23.4"; //?i?32" ;

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);  ASSERT_NE(token.type, TOKEN_TYPE_IDENTIFIER);   // 3foo (invalid identifier)
    get_token(&token);  ASSERT_NE(token.type, TOKEN_TYPE_FLOAT);        // 1.23.4 (invalid float literal
    //get_token(&token);  ASSERT_EQ(token.type, TOKEN_TYPE_QUESTION);     // ?i?32 (invalid type syntax)

    remove(filename);
}

TEST(KeywordRecognition, BasicKeywords) {
    const char* filename = "testfile.txt";
    const char* content = "const else fn if i32 f64 null pub return u8 var void while";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // const
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // else
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // fn
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // i32
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // f64
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // null
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // pub
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // u8
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // var
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // void
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  // while

    remove(filename);
}

TEST(KeywordRecognition, InvalidIdentifiers) {
    const char* filename = "testfile.txt";
    const char* content = "constVar else_else fnFunction if_else i32Var f64Number nullValue pub_data returnValue u8Type var1 voider whileLoop";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // constVar
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // else_else
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // fnFunction
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // if_else
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // i32Var
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // f64Number
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // nullValue
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // pub_data
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // returnValue
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // u8Type
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // var1
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // voider
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // whileLoop

    remove(filename);
}


TEST(IfElseStatement, SimpleIfWithoutElse1) {
    const char* filename = "testfile.txt";
    const char* content = "if (a < b) { return a; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LTN);           // <
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }

    remove(filename);
}

TEST(IfElseStatement, SimpleIfWithoutElse2) {
    const char* filename = "testfile.txt";
    const char* content = "if (a != b) { return a; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_NEQ);           // !=
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }

    remove(filename);
}


TEST(IfElseStatement, SimpleIfElse) {
    const char* filename = "testfile.txt";
    const char* content = "if (x == 10) { y = 20; } else { y = 30; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // x
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_EQ);            // ==
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 10
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // y
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);        // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 20
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // else
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // y
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);        // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 30
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }

    remove(filename);
}

TEST(IfElseStatement, IfElseStatment) {
    const char* filename = "testfile.txt";
    const char* content = "if (a >= b) { return a - b; } else { return b; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MEQ);           // >=
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MINUS);         // -
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // else
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }

 remove(filename);
}

TEST(IfElseStatement, IfElseStatmentFail) {
    const char* filename = "testfile.txt";
    const char* content = "if (a <= b) { return a - b;  else { return b; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEQ);           // <=
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MINUS);         // -
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_RIGHT_BRACE);   // Error due to missing braces }
    
 remove(filename);
}

TEST(IfElseStatement, MissingParentheses) {
    const char* filename = "testfile.txt";
    const char* content = "if a > b { return a; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (

    remove(filename);
}


TEST(IfElseStatement, MissingBraces) {
    const char* filename = "testfile.txt";
    const char* content = "if (a < b) return a;";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LTN);           // <
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_LEFT_BRACE);    // Error due to missing braces {

    remove(filename);
}

TEST(IfElseStatement, MissingBrace) {
    const char* filename = "testfile.txt";
    const char* content = "if (a < b return a;";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // a
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LTN);           // <
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // b
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_RIGHT_BRACKET); // Error due to missing braces )

    remove(filename);
}


TEST(WhileStatement, SimpleWhileLoop) {
    const char* filename = "testfile.txt";
    const char* content = "while (i < 10) { i = i + 1; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // while
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // i
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LTN);           // <
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 10
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // i
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);        // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // i
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);          // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 1
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }

    remove(filename);
}


TEST(WhileStatement, MissingParentheses) {
    const char* filename = "testfile.txt";
    const char* content = "while i < 10 { i = i + 1; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_LEFT_BRACKET);  // Error due to missing (

    remove(filename);
}


TEST(WhileStatement, MissingBraces) {
    const char* filename = "testfile.txt";
    const char* content = "while (i >= 10) i = i + 1;";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // while
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // i
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_MEQ);           // >=
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 10
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_NE(token.type, TOKEN_TYPE_LEFT_BRACE);    // Error due to missing {

    remove(filename);
}


TEST(WhileStatement, NestedWhileIf) {
    const char* filename = "testfile.txt";
    const char* content = "while (i != 5) { if (j == 3) { return null; } j = j + 1; }";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);

    Token token;

    
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // while
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // i
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_NEQ);           // !=
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 5
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // if
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);  // (
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // j
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_EQ);            // ==
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 3
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET); // )
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACE);    // {
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // return
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);       // null
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // j
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);        // =
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);    // j
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);          // +
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_INT);           // 1
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);     // ;
    get_token(&token);      ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACE);   // }   
    remove(filename);   
}   
