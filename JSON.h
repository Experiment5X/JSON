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

        union value
        {
            wstring str;
            double num;
            map<string, Value> obj;
            vector<Value> arr;
            bool b;
        };
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

            void Parse();
            wstring ParseString(wstring s);
    };
}

#endif // JSON_H
