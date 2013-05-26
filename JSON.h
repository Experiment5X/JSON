#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "JSONException.h"
#include "JSONTokenizer.h"

using std::wstring;
using std::map;
using std::vector;
using std::wstringstream;
using std::string;

namespace JSON
{
    enum class ValueType
    {
        String,
        Number,
        Object,
        Array,
        Boolean,
        Null
    };

    struct Value;

    // really this should be a union, but i was getting a bunch of compile
    // errors and don't feel like looking into it right now
    struct ValueFields
    {
        wstring str;
        double num;
        map<wstring, Value> obj;
        vector<Value> arr;
        bool b;
    };

    struct Value
    {
        ValueType type;
        ValueFields value;
    };

    class JSON
    {
        public:
            JSON(wstring jsonText);
            JSON(string jsonText);
            ~JSON();
        //private:
            wstring jsonText;
            map<wstring, Value> headObject;

            void Parse(Tokenizer &tokenizer, map<wstring, Value> &object, bool skipFirstCheck = false);
            Value ParseValue(Token valueToken, Tokenizer &tokenizer);
            wstring ParseString(wstring s);
            double ParseNumber(wstring s);
    };
}

#endif // JSON_H
