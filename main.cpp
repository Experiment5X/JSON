#include <iostream>

#include "JSONTokenizer.h"
#include "JSON.h"

using namespace std;

int main()
{
    JSON::JSON myJson(L"{ \"Adam\" : { \"Age\" : 17, \"E-Mail\" : \"xMeAdamx@gmail.com\", \"Grades\" : [ 94, 93, 89, 99 ] } }");
    cout << myJson.headObject[L"Adam"].value.obj[L"Grades"].value.arr.at(3).value.num << endl;

    return 0;
}
