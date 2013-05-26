#include "JSON.h"

using namespace std;
using namespace JSON;

JSON::JSON::JSON(wstring jsonText) :
    jsonText(jsonText)
{
    Tokenizer tokenizer(this->jsonText);
    Parse(tokenizer, headObject);
}

JSON::JSON::JSON(string jsonText)
{
    this->jsonText = wstring(jsonText.begin(), jsonText.end());
    Tokenizer tokenizer(this->jsonText);
    Parse(tokenizer, headObject);
}

JSON::JSON::~JSON()
{

}

void JSON::JSON::Parse(Tokenizer &tokenizer, map<wstring, Value> &object, bool skipFirstCheck)
{
    Token t = { TokenType::Symbol, L"" };
    // we only want to read the next token if we're going to check it
    // the whole point of that bool passed in is to skip the read,
    // since that token would have already been read in when called
    // from the ParseValue function
    if (!skipFirstCheck)
    {
        t = tokenizer.GetNextToken();
        if (t.tokenType != TokenType::L_CurlyBrace)
            throw Exception("Object must begin with '{' token.");
    }

    // multiple pairs can exist in the same object, they're seperated with a comma
    // object -> { members } -> { pair, members }
    while (t.tokenType != TokenType::R_CurlyBrace)
    {
        // pairs must look like this 'string : value'
        if ((t = tokenizer.GetNextToken()).tokenType != TokenType::String)
            throw Exception("Pair must begin with string token.");

        wstring str = ParseString(t.value);

        // pairs must look like this 'string : value'
        if ((t = tokenizer.GetNextToken()).tokenType != TokenType::Colon)
            throw Exception("In pair ':' token expected.");

        object[str] = ParseValue(tokenizer.GetNextToken(), tokenizer);

        t = tokenizer.GetNextToken();
        if (t.tokenType != TokenType::Comma && t.tokenType != TokenType::R_CurlyBrace)
            throw Exception("End of object reached. '}' token expected.");
    }
}

Value JSON::JSON::ParseValue(Token valueToken, Tokenizer &tokenizer)
{
    Value toReturn;

    if (valueToken.tokenType == TokenType::String)
    {
        toReturn.type = ValueType::String;
        toReturn.str = ParseString(valueToken.value);
    }
    else if (valueToken.tokenType == TokenType::Symbol)
    {
        if (valueToken.value == L"true")
        {
            toReturn.type = ValueType::Boolean;
            toReturn.b = true;
        }
        else if (valueToken.value == L"false")
        {
            toReturn.type = ValueType::Boolean;
            toReturn.b = true;
        }
        else if (valueToken.value == L"null")
        {
            toReturn.type = ValueType::Null;
        }
        else
        {
            toReturn.type = ValueType::Number;
            toReturn.num = ParseNumber(valueToken.value);
        }
    }
    else if (valueToken.tokenType == TokenType::L_CurlyBrace)
    {
        toReturn.type = ValueType::Object;
        map<wstring, Value> object;
        toReturn.obj = object;

        // parse the object structure, but skip the check for { because it's already been done
        Parse(tokenizer, toReturn.obj, true);
    }
    else if (valueToken.tokenType == TokenType::L_Bracket)
    {
        toReturn.type = ValueType::Array;

        Token t = { TokenType::Symbol, L"" };
        while (t.tokenType != TokenType::R_Bracket)
        {
            // parse the value in the array
            t = tokenizer.GetNextToken();
            toReturn.arr.push_back(ParseValue(t, tokenizer));

            t = tokenizer.GetNextToken();
            if (t.tokenType != TokenType::Comma && t.tokenType != TokenType::R_Bracket)
                throw Exception("End of array reached. ']' token expected.");
        }
    }

    return toReturn;
}

wstring JSON::JSON::ParseString(wstring s)
{
    wstring toReturn = L"";

    // build a new string with all of the escape characters escaped
    for (int i = 1; i < s.size() && s.at(i) != '"';)
    {
        // if it's an escape character, then we need to escape it
        if (s.at(i) == '\\' && (i + 2) < s.size())
        {
            i++;
            switch (s.at(i))
            {
                case '"':
                case '\\':
                case '/':
                    toReturn += s.at(i);
                    i++;
                    break;
                case 'b':
                    toReturn += '\b';
                    i++;
                    break;
                case 'f':
                    toReturn += '\f';
                    i++;
                    break;
                case 'n':
                    toReturn += '\n';
                    i++;
                    break;
                case 'r':
                    toReturn += '\r';
                    i++;
                    break;
                case 't':
                    toReturn += '\t';
                    i++;
                    break;
                case 'u':
                {
                    // incase you're not familiar with this escape character, the format is as follows:
                    // \u  hex-digit   hex-digit   hex-digit   hex-digit

                    if (i + 6 > s.size())
                        throw Exception("Not enough characters remaining in the string value for use of \\u escape.");

                    wstringstream wss;
                    wchar_t usrChar;

                    wss << s.substr(i + 1, 4);
                    wss >> usrChar;
                    toReturn += usrChar;

                    i += 5;
                }

            }
        }
        else
        {
            toReturn += s.at(i);
            i++;
        }
    }

    return toReturn;
}

double JSON::JSON::ParseNumber(wstring s)
{
    double toReturn;
    if (swscanf(s.c_str(), L"%lf", &toReturn) == 0)
        throw Exception("Invalid syntax for value type Number.");
    return toReturn;
}
