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
            toReturn.tokenType = TokenType::R_CurlyBrace;
            goto Return;
        case '}':
            toReturn.tokenType = TokenType::L_CurlyBrace;
            goto Return;
        case '[':
            toReturn.tokenType = TokenType::R_Bracket;
            goto Return;
        case ']':
            toReturn.tokenType = TokenType::L_Bracket;
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
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            toReturn.tokenType = TokenType::Number;

            // This isn't going to check if it's a valid number, that will
            // be done later. Right now it will say this: -e.2123.3 is valid
            while ((unsigned int)currentIndex < jsonText.size() && IsNumberCharacter(jsonText.at(++currentIndex)))
                toReturn.value += jsonText.at(currentIndex);

            goto Return;

        default:
            throw Exception("Invalid token.");

        Return:
            currentIndex++;
            return toReturn;
    }
}

bool Tokenizer::IsNumberCharacter(const char c)
{
    return isdigit(c) || c == 'e' || c == 'E' || c == '+' || c == '-';
}
