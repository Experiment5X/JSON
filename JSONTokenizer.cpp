#include "JSONTokenizer.h"

using namespace JSON;

Tokenizer::Tokenizer(wstring jsonText) :
    jsonText(jsonText)
{

}

Tokenizer::~Tokenizer()
{

}

Token Tokenizer::GetNextToken()
{
    Token toReturn = { TokenType::End_of_File, L"" };

    // loop while there's whitespace, we can ignore that
    while ((unsigned int)currentIndex < jsonText.size() && isspace(jsonText.at(currentIndex)))
    {
        currentIndex++;
    }

    if (currentIndex == jsonText.size())
        return toReturn;


    // get the next token from the text stream
    toReturn.value = jsonText.at(currentIndex);
    switch (jsonText.at(currentIndex))
    {
        case '{':
            toReturn.tokenType = TokenType::L_CurlyBrace;
            goto Return;
        case '}':
            toReturn.tokenType = TokenType::R_CurlyBrace;
            goto Return;
        case '[':
            toReturn.tokenType = TokenType::L_Bracket;
            goto Return;
        case ']':
            toReturn.tokenType = TokenType::R_Bracket;
            goto Return;
        case ':':
            toReturn.tokenType = TokenType::Colon;
            goto Return;
        case ',':
            toReturn.tokenType = TokenType::Comma;
            goto Return;
        case '"':
            toReturn.tokenType = TokenType::String;

            // keep getting the characters in the string until it ends
            while ((unsigned int)currentIndex < jsonText.size() && jsonText.at(++currentIndex) != '"')
                toReturn.value += jsonText.at(currentIndex);
            if (jsonText.at(currentIndex) == '"')
                toReturn.value += '"';

            goto Return;

        default:
            toReturn.tokenType = TokenType::Symbol;
            toReturn.value = L"";

            while (isalpha(jsonText.at(currentIndex)) || isdigit(jsonText.at(currentIndex)) || jsonText.at(currentIndex) == '-' || jsonText.at(currentIndex) == '+')
            {
                toReturn.value += jsonText.at(currentIndex);
                currentIndex++;
            }

            if (toReturn.value == L"")
                throw Exception("Invalid token.");

            return toReturn;

        Return:
            currentIndex++;
            return toReturn;
    }
}
