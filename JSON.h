#ifndef JSON_H
#define JSON_H

#include <iostream>

using std::string;

namespace JSON
{
    class JSONStructure
    {
        public:
            JSONStructure(string &jsonText);
            ~JSONStructure();
        private:
            string &jsonText;
    };
}

#endif // JSON_H
