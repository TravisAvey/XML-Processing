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
#include <locale>


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
    // the map to store all the data for each country
    // country name is the key, and a vector containing all the data
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

// method declaration
bool IsGoodInputFile (std::string &);

int main ()
{
    std::cout << "--- Welcome to Travis Avey's CSV to XML Converter ---" << std::endl;
    std::string fileName;
    if (IsGoodInputFile (fileName))
    {
        ConvertXML convert (fileName);
    
        convert.Convert ();
    }
    return 0;
}

/*
    This helper method gets the input file name from the user
    returns true if the file is good, else returns false
*/
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

/*
    Public constructor for the ConvertXML class
    pass in the file name to open 
*/
ConvertXML::ConvertXML (std::string fileName)
{
    // set the private file name
    mInputFile = fileName;
}

/*
    This method calls the methods required to convert
    the CSV to XML
*/
void ConvertXML::Convert ()
{
    GetDataFromCSV ();
    GetSeparator ();
    GetXMLFile ();
    ExtractData ();
    WriteXML ();
}

/*
    This method gets the separator from the user that
    is used in the current CSV file
*/
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

/*
    This method gets the raw data from the csv file.
    It places each line as a string into a vector
*/
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

/*
    This method gets the XML file name from the user
*/
void ConvertXML::GetXMLFile ()
{
    // loop until we get an appropriate xml file name from the user
    while (true)
    {
        // prompt user for the file name
        std::cout << "Enter the XML file name: ";

        try
        {
            // try to get the xml file name
            std::getline (std::cin, mXmlFile);
        }
        catch (const std::exception &e)
        {
            // if it fails, then inform the user and continue the loop
            std::cout << "\tWARNING: File name is not acceptable\n";
            continue;
        }
        // if we get here, good file name and break
        break;
    }

    // if user entered a . at the begining
    if (mXmlFile[0] == '.')
        // create a substring removing that .
        mXmlFile = mXmlFile.substr (1);

    // get the length of the xml file name
    auto len = mXmlFile.length ();

    // if the lenght is less than 4, or doesn't end in .xml
    if (len <= 4 || mXmlFile.substr (len -4, len) != ".xml")
        // add the .xml to the end of the file
        mXmlFile += ".xml";
}

/*
    This method writes all the data to the XML file
*/
void ConvertXML::WriteXML ()
{
    // open the xml file for writing
    std::fstream xmlFile (mXmlFile.c_str(), std::ios::out);

    // set the locale to the machine's locale for numbers
    xmlFile.imbue(std::locale(""));
    // if the xml file is opened
    if (xmlFile)
    {
        // inform user we are writing to the XML file
        std::cout << "Wrting XML file\n";

        // write the xml
        xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

        // write the comment
        xmlFile << "<!-- Processed by Travis Avey's converter -->\n";

        // write out the root tag
        xmlFile << "<countries>\n";

        // for each country in the data map
        for (const auto &data : mData)
        {
            // store the country from the map
            std::string country = data.first;

            // write the country tag
            xmlFile << "\t<country>\n";

            // write the country name, region, and code
            xmlFile << "\t\t<name>" << mData[country][0] << "</name>\n";
            xmlFile << "\t\t<region>" << mData[country][1] << "</region>\n";
            xmlFile << "\t\t<code>" << mData[country][2] << "</code>\n"; 

            // write the cities opening tag
            xmlFile << "\t\t<cities>\n";

            // init a counter, start at 3 for the city information
            int i = 3;
            // loop through the rest of the country data
            while (i <mData[country].size ())
            {
                // write out the city opening tag
                xmlFile << "\t\t\t<city>\n";
                // write out the city name, district, and population (using string to int for commas)
                xmlFile << "\t\t\t\t<name>" << mData[country][i++] << "</name>\n";
                xmlFile << "\t\t\t\t<district>" << mData[country][i++] << "</district>\n";
                xmlFile << "\t\t\t\t<population>" << std::stoi(mData[country][i++]) << "</population>\n";

                // write out the city closing tag
                xmlFile << "\t\t\t</city>\n";
            }
            // close cities tag
            xmlFile << "\t\t</cities>\n"; 
            // close the country tag
            xmlFile << "\t</country>\n";
        }   
        // close the countries tag
        xmlFile << "</countries>\n";
    }
    // close the file
    xmlFile.close ();
}

/*
    This method checks if the string already exists in the country map
    returns true if it does, false otherwise
*/
bool ConvertXML::CountryExists (const std::string &data)
{
    // ternary operator: true if we find the country, false otherwise
    return mData.find (data) == mData.end() ? false : true;
}

/*
    This method extracts the data from the raw data vector
    and adds the country data to a map
*/
void ConvertXML::ExtractData ()
{
    // for each data in the raw data vector
    for (auto data : mRawData)
    {
        // init a string stream with the data
        std::stringstream ss (data);
        // declare a string to hold the token
        std::string token;
        // declare a vector that will hold the data
        std::vector<std::string> line;
        // while we can split the data from the stringstream on the separator
        while (std::getline (ss, token, separator))
            // add the token to the vector
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
        // store the current country
        std::string currentCountry = line[0];
        
        // check if we already have this country
        if (!CountryExists (currentCountry))
            // if not, then add the vector to the country map
            mData[currentCountry] = line;
        // else we already have this country, so just add the cities
        else
            // loop from 3 (city name) to 5 (population)
            for (int i=3; i<=5; ++i)
                // add that data to the currenty country's vector
                mData[currentCountry].push_back (line[i]);
    }
}
