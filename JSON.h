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

    struct Value
    {
        ValueType type;

        // really this should be a union, but i was getting a bunch of compile
        // errors and don't feel like looking into it right now
        wstring str;
        double num;
        map<wstring, Value> obj;
        vector<Value> arr;
        bool b;
    };

    class JSON
    {
        public:
            map<wstring, Value> headObject;

            JSON(wstring jsonText);
            JSON(string jsonText);
            ~JSON();

            string ToString();
            wstring ToWString();
        private:
            wstring jsonText;

            void Parse(Tokenizer &tokenizer, map<wstring, Value> &object, bool skipFirstCheck = false);
            Value ParseValue(Token valueToken, Tokenizer &tokenizer);
            wstring ParseString(wstring s);
            double ParseNumber(wstring s);

            void ObjectToWString(map<wstring, Value> &obj, wstring &outStr, int tabCount);
            void ValueToWString(Value &value, wstring &outStr, int tabCount);

            void AddTabs(wstring &outStr, int tabCount);
    };
}

#endif // JSON_H
