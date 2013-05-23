#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <map>

using std::string;
using std::map;

namespace JSON
{
    enum class ValueType
    {
        String,
        Number,
        Object,
        Array,
        True,
        False,
        Null
    };

    template <class T>
    class Value
    {
        ValueType type;
        T value;
    };

    class JSON
    {
        public:
            JSON(string &jsonText);
            ~JSON();
        private:
            string &jsonText;
            map<string, Value<string>> head;
    };
}

#endif // JSON_H
