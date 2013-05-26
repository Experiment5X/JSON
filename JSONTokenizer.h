#ifndef JSONTOKENIZER_H
#define JSONTOKENIZER_H

#include <iostream>
#include <ctype.h>
#include "JSONException.h"

using std::wstring;

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
        Symbol,
        End_of_File
    };

    struct Token
    {
        TokenType tokenType;
        wstring value;
    };

    class Tokenizer
    {
        public:
            Tokenizer(wstring jsonText);
            ~Tokenizer();

            Token GetNextToken();

        private:
            wstring jsonText;
            int currentIndex = 0;
    };
}

#endif // JSONTOKENIZER_H
