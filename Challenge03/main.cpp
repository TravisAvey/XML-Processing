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
#include <map>
#include <sstream>


/*
    This class Handles converting the data to an XML file
*/
class ConvertXML
{
private:
    // Private member variables

    // the raw data from the file 
    std::vector<std::string> mRawData;
    // the list of countries already inputed in the XML file
    std::vector<std::string> mCountries;

    std::map<std::string, std::vector<std::string> > mData;

    // the input file name
    std::string mInputFile;
    // the XML file name
    std::string mXmlFile;
    // the separator for the csv file
    char separator;

    // private helper classes
    void GetDataFromCSV ();
    void GetSeparator ();
    void GetXMLFile ();
    void WriteXML ();
    void ExtractData ();

    bool CountryExists (const std::string &);

public:
    // constructor
    ConvertXML (std::string);

    // public interface to call to convert the data to XML
    void Convert ();

};

bool IsGoodInputFile (std::string &);

int main ()
{
    std::string fileName;
    if (IsGoodInputFile (fileName))
    {
        ConvertXML convert (fileName);
    
        convert.Convert ();
    }
    return 0;
}

bool IsGoodInputFile (std::string &inputFile)
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
    ExtractData ();
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


        for (const auto &data : mData)
        {
            std::string country = data.first;
            xmlFile << "\t<country>\n";

            xmlFile << "\t\t<name>" << mData[country][0] << "</name>\n";
            xmlFile << "\t\t<region>" << mData[country][1] << "</region>\n";
            xmlFile << "\t\t<code>" << mData[country][2] << "</code>\n"; 
            
            // loop from 3 to end of this list
            for (int i=3; i<mData[country].size (); ++i)
            {
                xmlFile << "\t\t\t<>" << mData[country][i] << "</>\n";
            }

            xmlFile << "\t</country>\n";
        }   

        xmlFile << "</countries>\n";

    }
}

bool ConvertXML::CountryExists (const std::string &data)
{
    return mData.find (data) == mData.end() ? false : true;
}

void ConvertXML::ExtractData ()
{
    for (auto data : mRawData)
    {
        std::stringstream ss (data);
        std::string token;
        std::vector<std::string> line;
        while (std::getline (ss, token, separator))
            line.push_back (token);

        // data is for each line:
        /*
            0 : Country
            1 : Region
            2 : Code
            3 : City
            4 : District
            5 : Population
        */
        std::string currentCountry = line[0];
        
        if (!CountryExists (currentCountry))
            mData[currentCountry] = line;
        else
            for (int i=3; i<=5; ++i)
                mData[currentCountry].push_back (line[i]);
    }
}
