#include <iostream>
#include <fstream>
#include <sstream>

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

    /* An example output for this section:

        Adam Spindler:
        Changed to support unicode JSON

        Adam Spindler:
        Removed unnecessary code

        Adam Spindler:
        Changed class name to JSON

        Adam Spindler:
        Added project to repo

        Adam Spindler:
        Added readme

    */


    // It is also possible to create/edit JSON structures. To create a
    // a new one, just go ahead and pass "{ }" in through the JSON::JSON
    // constructor.
    JSON::JSON newJson("{ }");

    // Now, we can just start adding to it. We'll create a JSON structure
    // that will hold a person's information.
    for (int i = 0; i < 3; i++)
    {
        // This personInfo is an object. In this library, objects
        // are maps with of type wstring and JSON::Value
        map<wstring, JSON::Value> personInfo;

        // The strings in the JSON structure must be wstrings
        wstring name;
        int age = 0;

        wcout << L"Name: ";
        wcin >> name;
        personInfo[L"Name"].type = JSON::ValueType::String;
        personInfo[L"Name"].str = name;

        wcout << L"Age: ";
        wcin >> age;
        personInfo[L"Age"].type = JSON::ValueType::Number;
        // all numbers must be doubles
        personInfo[L"Age"].num = (double)age;

        wstringstream wss;
        wss << i;
        wstring personId = wss.str();

        // Create a value, would look like this in JSON:
        // { "Name" : "NAME_USER_ENTERED", "Age" : AGE_USER_ENTERED }
        JSON::Value newPerson;
        newPerson.type = JSON::ValueType::Object;
        newPerson.obj = personInfo;

        // Add the value into the JSON structure
        newJson.startValue.obj[personId] = newPerson;

        wcout << endl;
    }

    // Print out the JSON structure that was created. Another version
    // of this function exists, called ToString, which will return a
    // std::string instead of a std::wstring
    wcout << newJson.ToWString() << endl;

    /* An example output for this section:

        { "0": {
            "Age": 17,
            "Name": "Adam"
        },
        "1": {
            "Age": 18,
            "Name": "Jenna"
        },
        "2": {
            "Age": 22,
            "Name": "Tom"
        }}
    */

    return 0;
}
