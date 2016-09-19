/*******************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 3

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11
for example:
macuser$ g++ main.cpp -std=c++11

This program ...  
*******************************/

#include <iostream>
#include <fstream>
#include <vector>

void ConvertToXML ();
bool OpenFile (std::string &);
void GetDataFromCSV (const std::string &);

int main ()
{

    ConvertToXML ();
    
    return 0;
}

void ConvertToXML ()
{
    std::string fileName;

    if (OpenFile (fileName))
    {
        GetDataFromCSV (fileName);
    }
}

/*
    This method handles opening the csv file
*/
bool OpenFile (std::string &fileName)
{   
    // prompt user for the CSV file
    std::cout << "Enter the CSV file to be converted to XML: ";
    
    // get the file name from the user
    std::getline (std::cin, fileName);

    // open the file
    std::fstream file(fileName.c_str(), std::ios::in);

    // if the file cannot be opened
    if (!file)
    {
        // inform user of error, using c-error output
        std::cerr << "There was a problem with opening the file\n";
        // return false
        return false;
    }

    // close the file and return true
    file.close ();
    return true;

}

void GetDataFromCSV (const std::string &fileName)
{
    // char to hold separator
    char sep;
    while (true)
    {
        // prompt user for the separator
        std::cout << "Enter the separator used in the CSV file: ";
        std::cin >> sep;

        if (!std::cin)
        {
            std::cin.clear ();
            std::string error;
            std::cin >> error;
        }
        else if (sep >= 65 && sep <= 122)
        {
            std::cout << "\tWARNING: letters shouldn't be used as a separator\n";
        }
        else
        {
            break;
        }
    }
    
    std::vector<std::string> data;
    std::string token;
    std::fstream file (fileName.c_str(), std::ios::in);
    while (std::getline (file, token, sep))
        data.push_back (token);

    for (auto d : data)
        std::cout << d << std::endl;
}