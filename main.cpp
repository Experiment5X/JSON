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

   // wcout << myJson.headObject[L"web-app"].obj[L"servlet"].arr.at(0).obj[L"servlet-name"].str << endl;
    cout << myJson.ToString() << endl;

    return 0;
}
