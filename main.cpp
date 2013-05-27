#include <iostream>
#include <fstream>

#include "JSONTokenizer.h"
#include "JSON.h"

using namespace std;

int main()
{
    ifstream jsonFile("/home/adam/Desktop/mods.txt");
    string jsonText = "";
    string temp;

    while (getline(jsonFile, temp))
        jsonText += temp;
    jsonFile.close();

    JSON::JSON myJson(jsonText);

    cout << myJson.ToString();

    for (JSON::Value v : myJson.startValue.arr)
    {
        wcout << v.obj[L"commit"].obj[L"author"].obj[L"name"].str << L": " << endl;
        wcout << v.obj[L"commit"].obj[L"message"].str << endl << endl;
    }

    return 0;
}
