#include <iostream>
#include <fstream>

#include "JSON.h"

using namespace std;

int main()
{
    // This example file is the JSON that's returned from GitHub's API
    // for all of the commits on this project. You can find that resonse
    // here: https://api.github.com/repos/Experiment5X/JSON/commits
    ifstream jsonFile("/home/adam/Desktop/example-json.txt");
    string jsonText = "";
    string temp;

    // read the entire text file
    while (getline(jsonFile, temp))
        jsonText += temp;
    jsonFile.close();

    // Create a new JSON object and pass in the JSON text. This library
    // supports both strings and wstrings, as JSON does.
    JSON::JSON myJson(jsonText);

    // In the JSON::JSON class there is a member called 'startValue'
    // that simply holds the start value for the JSON structure. Some
    // JSON structures start with objects, { }, some start with arrays, [ ],
    // like in this case. A value could also be a number, a string, a boolean,
    // or even null. This library supports all of those types. The 'Value'
    // struct contains really 2 members. The first, 'type' stores what
    // type of Value it is. The second is the value itself. Here is a table
    // that shows you the different ones and their types:
    //
    // NAME         FRIENDLY TYPE           C++ TYPE
    //  str          string                  wstring
    //  num          number                  double
    //  obj          object                  map<wstring, Value>
    //  arr          array                   vector<Value>
    //  b            boolean                 bool
    //
    // The awesome thing about this is it's recursive, so in one line of
    // code you can access exaclty what you want. There is an example of
    // this in the for loop below.

    // this will iterate through all of the commits and print out the author
    // and the message associated with the commit
    for (JSON::Value v : myJson.startValue.arr)
    {
        wcout << v.obj[L"commit"].obj[L"author"].obj[L"name"].str << L": " << endl;
        wcout << v.obj[L"commit"].obj[L"message"].str << endl << endl;
    }

    return 0;
}
