#ifndef JSONTOKENIZER_H
#define JSONTOKENIZER_H

#include <iostream>
#include <ctype.h>
#include "JSONException.h"

using std::string;

namespace JSON
{

    enum class TokenType
    {
        L_CurlyBrace,
        R_CurlyBrace,
        L_Bracket,
        R_Bracket,
        String,
        Colon,
        Comma,
        Number,
        End_of_File
    };

    struct Token
    {
        TokenType tokenType;
        string value;
    };

    class Tokenizer
    {
        public:
            Tokenizer(string jsonText);
            ~Tokenizer();

            Token GetNextToken();

        private:
            string jsonText;
            int currentIndex = 0;

            bool IsNumberCharacter(const char c);
    };
}

#endif // JSONTOKENIZER_H
