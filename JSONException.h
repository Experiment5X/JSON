#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H

#include <iostream>

using std::string;
using std::exception;

namespace JSON
{

    class Exception : public exception
    {
        public:
            Exception(const string &message);
            const char* what() const throw();
        private:
            const string &message;
    };
}

#endif // JSONEXCEPTION_H
