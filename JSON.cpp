#include "JSON.h"

using namespace std;
using namespace JSON;

JSON::JSON::JSON(wstring jsonText) :
    jsonText(jsonText)
{
    Parse();
}

JSON::JSON::JSON(string jsonText)
{
    this->jsonText = wstring(jsonText.begin(), jsonText.end());
    Parse();
}

JSON::JSON::~JSON()
{

}

void JSON::JSON::Parse()
{
    /*Tokenizer tokenizer(jsonText);

    Token t = tokenizer.GetNextToken();
    if (t.tokenType != TokenType::L_CurlyBrace)
        throw Exception("Objects must start with '{' token.");

    t = tokenizer.GetNextToken();
    if (t.tokenType != TokenType::String)
        throw Exception("Invalid pair. Pair notation is 'string : value'.");*/

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
