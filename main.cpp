#include <iostream>

#include "JSONTokenizer.h"
#include "JSON.h"

using namespace std;

int main()
{
    JSON::JSON myJson(L"{ \"Adam\" : { \"Age\" : 17, \"E-Mail\" : \"xMeAdamx@gmail.com\" } }");
    wcout << myJson.headObject[L"Adam"].value.obj[L"E-Mail"].value.str << endl;

    return 0;
}
