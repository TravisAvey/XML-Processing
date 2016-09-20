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
#include <sstream>


/*
    This class Handles converting the data to an XML file
*/
class ConvertXML
{
private:
    // Private member variables
    std::vector<std::string> mRawData;
    std::string mInputFile;
    std::string mXmlFile;

    // private helper classes
    char separator;
    void GetDataFromCSV ();
    void GetSeparator ();
    void GetXMLFile ();
    void WriteXML ();

public:
    // constructor
    ConvertXML (std::string);

    // public interface to call to convert the data to XML
    void Convert ();

};

bool GoodInputFile (std::string &);

int main ()
{
    std::string fileName;
    if (GoodInputFile (fileName))
    {
        ConvertXML convert (fileName);
    
        convert.Convert ();
    }
    return 0;
}

bool GoodInputFile (std::string &inputFile)
{
    // prompt user for the CSV file
    std::cout << "Enter the CSV file to be converted to XML: ";
    
    // get the file name from the user
    std::getline (std::cin, inputFile);

    // open the file
    std::fstream file(inputFile.c_str(), std::ios::in);

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

ConvertXML::ConvertXML (std::string fileName)
{
    mInputFile = fileName;
}

void ConvertXML::Convert ()
{
    GetDataFromCSV ();
    GetSeparator ();
    GetXMLFile ();
    WriteXML ();
}

void ConvertXML::GetSeparator ()
{

    // keep looping until user inputs good data
    while (true)
    {
        // prompt user for the separator
        std::cout << "Enter the separator used in the CSV file: ";
        // get the separator from user, using the no skip white space flag
        // to get spaces and tabs (and new lines)
        std::cin >> std::noskipws >> separator;
        std::cin.ignore (256, '\n');

        // check if cin has an eror
        if (!std::cin)
        {
            // output feedback to the user
            std::cout << "\tWARNING: " << separator << " is not acceptable input\n\n";
            // clear cin
            std::cin.clear ();
            // catch the data that could still be hanging out in cin
            std::string error;
            std::cin >> error;
        }
        // if user inputs a newline, warn user that new lines don't work
        else if (separator == '\n')
            std::cout << "\tWARNING: Newlines are not acceptable for CSV file separators\n\n";
        // if get here, use the separator (user could input anything?)
        else
            break;
    }

}

void ConvertXML::GetDataFromCSV ()
{
    // open the file using the user supplied input file name
    std::fstream file (mInputFile.c_str(), std::ios::in);

    // declare a string to hold each line from the file
    std::string line;

    // while we can a line from the file, store the data in the token using the sep char
    // to split up the line
    while (std::getline (file, line))
        // add the line to the vector
        mRawData.push_back (line);
    
}

void ConvertXML::GetXMLFile ()
{
    while (true)
    {
        std::cout << "Enter the XML file name: ";

        try
        {
            std::getline (std::cin, mXmlFile);
        }
        catch (const std::exception &e)
        {
            std::cout << "\tWARNING: File name is not acceptable\n";
            continue;
        }
        break;
    }

    if (mXmlFile[0] == '.')
        mXmlFile = mXmlFile.substr (1);

    auto len = mXmlFile.length ();

    if (len <= 4 || mXmlFile.substr (len -4, len) != ".xml")
        mXmlFile += ".xml";
    
}

void ConvertXML::WriteXML ()
{
    std::fstream xmlFile (mXmlFile.c_str(), std::ios::out);

    if (xmlFile)
    {
        std::cout << "Wrting XML file\n";

        xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

        xmlFile << "<!-- Processed by Travis Avey's converter -->\n";

        xmlFile << "<countries>\n";

        for (auto data : mRawData)
        {
            std::stringstream ss (data);
            std::string token;

            while (std::getline (ss, token, separator))
                std::cout << token << std::endl;
        }

        xmlFile << "</countries>\n";

    }
}