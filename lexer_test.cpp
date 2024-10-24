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

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);

    remove(filename);

} 


TEST(Expresion, IntAditionFail)
{
    const char* filename = "testfile.txt";
    const char* content = "int a = 0.5; ";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);

    get_token(&token);
    ASSERT_NE(token.type, TOKEN_TYPE_INT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);


    remove(filename);

}

TEST(Expresion, ComplexExpresion)
{
const char* filename = "testfile.txt";
    const char* content = "const result = (a + b) * 10 - 2;";  

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_LEFT_BRACKET);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_RIGHT_BRACKET);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_MUL);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_MINUS);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);

    remove(filename);

}


TEST(Expresion, MissingSemicolon)
{
    const char* filename = "testfile.txt";
    const char* content = "const a = 5";  // Missing semicolon at the end of the statement

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);         

    get_token(&token);
    ASSERT_NE(token.type, TOKEN_TYPE_SEMICOLON);   

    remove(filename);
}


TEST(Expresion, FloatAdition)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 0.075;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_FLOAT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);


    remove(filename);

}



TEST(Expresion, FloatAditionExponent1)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 1e3;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_FLOAT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);


    remove(filename);

}

TEST(Expresion, FloatAditionExponent2)
{
    const char* filename = "testfile.txt";
    const char* content = "f64 a = 1.504e3;";

    int check = write_in_file(filename,content);

    ASSERT_EQ(check, 0); // "Failed to write content to file";

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);  

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_FLOAT);

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);


    remove(filename);

}

TEST(Expresion, MultipleVariableDeclaration)
{
    const char* filename = "testfile.txt";
    const char* content = "const x = 1, y = 2, z = 3;";  

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // x

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 1

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_COMMA);       // ,

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // y

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 2

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_COMMA);       // ,

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // z

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 3

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}


TEST(Strings, SimpleStringAssignment)
{
    const char* filename = "testfile.txt";
    const char* content = "const message = \"Hello, world!\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // message

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "Hello, world!"

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}


TEST(Strings, MultilineString)
{
    const char* filename = "testfile.txt";
    const char* content = "i32 a = \"hello\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // escaped

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // Multiline string token

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;


    remove(filename);
}

TEST(Strings, StringWithEscapeCharacters)
{
    const char* filename = "testfile.txt";
    const char* content = "const escaped = \"This is a \\\"quote\\\".\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // escaped

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // String with escape characters

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, EmptyString)
{
    const char* filename = "testfile.txt";
    const char* content = "const empty = \"\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // empty

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // Empty string

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, StringWithSpecialCharacters)
{
    const char* filename = "testfile.txt";
    const char* content = "const special = \"!@#$%^&*()\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // special

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // Special character string

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

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
    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // a

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 5

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    // Line 2
    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // b

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_INT);         // 10

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    // Line 3
    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // c

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // a

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // b

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, ComplexStringExpression)
{
    const char* filename = "testfile.txt";
    const char* content = "const msg = \"Hello\" + \" \" + \"World\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // msg

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "Hello"

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // " "

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "World"

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_SEMICOLON);   // ;

    remove(filename);
}

TEST(Strings, StringConcatenationWithVariables)
{
    const char* filename = "testfile.txt";
    const char* content = "const greeting = \"Hello\" + name + \"!\";";

    int check = write_in_file(filename, content);
    ASSERT_EQ(check, 0);  // Ensure file was written successfully

    Token token;

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_KEYWORD);     // const

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // greeting

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_ASSIGN);      // =

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "Hello"

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_IDENTIFIER);  // name

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_PLUS);        // +

    get_token(&token);
    ASSERT_EQ(token.type, TOKEN_TYPE_STRING);      // "

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
