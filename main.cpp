#include <iostream>

#include "JSONTokenizer.h"

using namespace std;

int main()
{
    JSON::Tokenizer tokenizer("{ \"Adam\":[ { \"ddog\" : 5, \"boost\" : 7 } ] }");
    JSON::Token token;

    while ((token = tokenizer.GetNextToken()).tokenType != JSON::TokenType::End_of_File)
        cout << "Value: " << token.value << endl << "Type: " << (int)token.tokenType << endl << endl;
    return 0;
}
