#include "JSONException.h"

using namespace JSON;

JSON::Exception::Exception(const string &message) :
    message(message)
{

}

const char* JSON::Exception::what() const throw()
{
    return string("JSON: " + message).c_str();
}
