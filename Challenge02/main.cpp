/*******************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 2

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11
for example:
macuser$ g++ main.cpp -std=c++11

This program does ...
*******************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::vector;

bool CheckFile (string &);
string GetFileName ();
vector<string> OpenFile ();
void ConvertToXML (const vector<string> &);
string GetXMLFileName ();


int main ()
{
    cout << "--- Welcome to We-Code-Stuff, LLC's Data to XML converter ---" << endl;

    vector<string> data = OpenFile ();
    
    ConvertToXML (data);

    return 0;
}

bool CheckFile (string &fileName)
{
    bool canOpen = false;

    std::fstream file (fileName.c_str( ), std::ios::in);
    if (file)
        canOpen = true;

    file.close ();

    return canOpen;    
}

string GetFileName ()
{
    string fileName;
    while (true)
    {
        cout << "Enter the file to be converted to XML: ";
        std::getline (std::cin, fileName);
        if (CheckFile (fileName))
            break;
        else
            cout << "\tWARNING: the file " << fileName << " cannot be opened." << endl << endl;
    }
    return fileName;
}

vector<string> OpenFile ()
{
    string fileName = GetFileName ();
    vector<string> data;
    std::fstream file (fileName.c_str (), std::ios::in);
    string line;
    while (std::getline (file, line))
        data.push_back (line);
    
    file.close ();
    return data;
}

void ConvertToXML (const vector<string> &data)
{
    string xmlFile = GetXMLFileName ();
}

string GetXMLFileName ()
{

    string file;

    cout << "Please enter the XML file name: ";
    getline (std::cin, file);
    cout << endl << endl;

    if (file.substr(file.length() - 4, file.length()) != ".xml")
        file += ".xml";
    
    cout << file << endl;
    return file;

}