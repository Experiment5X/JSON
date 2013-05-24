#include <iostream>

#include "JSONTokenizer.h"
#include "JSON.h"

using namespace std;

int main()
{
    JSON::Tokenizer tokenizer(L"{ \"Adam\":[ { \"ddog\" : 5, \"boost\" : 7 } ] }");
    JSON::Token token;

    JSON::JSON myJson(L"");
    wstring ws = myJson.ParseString(L"\"adam \u0054\\\"Experiment5X\\\" spindler\"");
    wcout << ws << endl;

    return 0;
}
