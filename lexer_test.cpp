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

    Token *token;

    get_token(token);
    ASSERT_EQ(token->type, TOKEN_TYPE_KEYWORD);

    get_token(token);
    ASSERT_EQ(token->type, TOKEN_TYPE_IDENTIFIER);

    get_token(token);
    ASSERT_EQ(token->type, TOKEN_TYPE_ASSIGN);

    get_token(token);
    ASSERT_EQ(token->type, TOKEN_TYPE_INT);

    get_token(token);
    ASSERT_EQ(token->type, TOKEN_TYPE_SEMICOLON);

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
