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
    Tokenizer tokenizer(jsonText);
    startValue = ParseValue(tokenizer.GetNextToken(), tokenizer);
}

void JSON::JSON::ParseObject(Tokenizer &tokenizer, map<wstring, Value> &object)
{
    Token t = { TokenType::Symbol, L"" };

    // multiple pairs can exist in the same object, they're seperated with a comma
    // object -> { members } -> { pair, members }
    while (t.tokenType != TokenType::R_CurlyBrace)
    {
        t = tokenizer.GetNextToken();
        // this will only be true if the object is empty, so { }
        if (t.tokenType == TokenType::R_CurlyBrace)
            return;
        // pairs must look like this 'string : value'
        else if (t.tokenType != TokenType::String)
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
        ParseObject(tokenizer, toReturn.obj);
    }
    else if (valueToken.tokenType == TokenType::L_Bracket)
    {
        toReturn.type = ValueType::Array;

        Token t = { TokenType::Symbol, L"" };
        while (t.tokenType != TokenType::R_Bracket)
        {
            // parse the value in the array
            t = tokenizer.GetNextToken();

            // this will only be true if the array is empty
            if (t.tokenType == TokenType::R_Bracket)
                return toReturn;

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

string JSON::JSON::ToString()
{
    wstring wJson = ToWString();
    string toReturn(wJson.begin(), wJson.end());
    return toReturn;
}

wstring JSON::JSON::ToWString()
{
    wstring toReturn = L"";
    ValueToWString(startValue, toReturn, 0);
    return toReturn;
}

void JSON::JSON::ObjectToWString(map<wstring, Value> &obj, wstring &outStr, int tabCount)
{
    outStr += L"{ ";
    if (tabCount != 0 && obj.size() > 1)
    {
        outStr += L"\r\n";
    }

    for (pair<wstring, Value> p : obj)
    {
        if (obj.size() > 1)
            AddTabs(outStr, tabCount);

        // string
        outStr += '"';
        outStr += p.first;
        outStr += '"';

        outStr += L": ";

        ValueToWString(p.second, outStr, tabCount + 1);

        outStr += L", \r\n";
    }

    // there will be an extra ", \r\n" every time, unless the object is empty
    if (obj.size() != 0)
        outStr.erase(outStr.end() - 4, outStr.end());

    if (tabCount == 0 || obj.size() <= 1)
    {
        outStr += '}';
    }
    else
    {
        outStr += L"\r\n";
        AddTabs(outStr, tabCount - 1);
        outStr += '}';
    }
}

void JSON::JSON::ValueToWString(Value &value, wstring &outStr, int tabCount)
{
    switch (value.type)
    {
        case ValueType::String:
            outStr += '"';
            outStr += value.str;
            outStr += '"';
            break;
        case ValueType::Number:
        {
            wstringstream ss;
            ss << value.num;
            outStr += ss.str();
            break;
        }
        case ValueType::Object:
            ObjectToWString(value.obj, outStr, tabCount);
            break;
        case ValueType::Array:
            outStr += L"[ ";
            for (Value v : value.arr)
            {
                ValueToWString(v, outStr, tabCount + 1);
                outStr += L", \r\n";
                AddTabs(outStr, tabCount + 1);
            }
            if (value.arr.size() != 0)
                outStr.erase(outStr.end() - 2, outStr.end());
            outStr += L" ]";
            break;
        case ValueType::Boolean:
            outStr += (value.b) ? L"true" : L"false";
            break;
        case ValueType::Null:
            outStr += L"Null";
            break;
    }
}

void JSON::JSON::AddTabs(wstring &outStr, int tabCount)
{
    for (int i = 0; i < tabCount; i++)
        outStr += L"\t";
}
