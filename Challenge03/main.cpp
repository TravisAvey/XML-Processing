/*******************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 3

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11
for example:
macuser$ g++ main.cpp -std=c++11

This program will convert a CSV file to an XML file.
It will prompt the user for the CSV file name, then prompt the
user for the separator used in the CSV file.  Finally it will prompt
the user for the XML file name to write to.  It then will write the
data to the XML file, using the namespaces for country and city for each
element.  Upon completion of writing the XML file it will inform the user
that the XML conversion is complete. 
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
    std::map<std::string, std::vector<std::string> > mCountriesMap;

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
    void Validate (std::string &);

public:
    /*
        Public constructor for the ConvertXML class
        pass in the file name to open 
    */
    ConvertXML (const std::string &file) : mInputFile (file) {};

    // public interface to call to convert the data to XML
    void Convert ();

    std::string GetXMLFileName () const;

};

// method declaration
bool IsGoodInputFile (std::string &);

int main ()
{
    // output the title of the program
    std::cout << "--- Welcome to Travis Avey's CSV to XML Converter ---\n\n";

    // declare a string to hold the file name
    std::string fileName;

    // check if we have a good csv file name
    if (IsGoodInputFile (fileName))
    {
        // Create a ConvertXML object
        ConvertXML convert (fileName);
        // call method to convert the csv file to xml
        convert.Convert ();

        // output feedback to user that the conversion to xml is complete
        std::cout << "Finished writing to \"" << convert.GetXMLFileName () 
                  << "\".\nThank you for using Travis Avey's Converter\n\n";
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
        std::cerr << "\nERROR: There was a problem trying to open \"" << inputFile 
                  << "\".\nCheck file name and try again\n.";
        // return false
        return false;
    }

    std::cout << std::endl;

    // close the file and return true
    file.close ();
    return true;
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

    std::cout << std::endl;

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

    std::cout << std::endl;

    // if user entered a . at the begining
    if (mXmlFile[0] == '.')
        // create a substring removing that .
        mXmlFile = mXmlFile.substr (1);

    // get the length of the xml file name
    auto len = mXmlFile.length ();

    // if the length is less than 4, or doesn't end in .xml
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

    // if the xml file is opened
    if (xmlFile)
    {
        // set the locale to the machine's locale for number formatting
        xmlFile.imbue(std::locale(""));

        // inform user we are writing to the XML file
        std::cout << "Wrting XML file\n\n";

        // write the xml prolog
        xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

        // write the comment
        xmlFile << "<!-- Processed by Travis Avey's converter -->\n";

        // init the namespace values for country and city
        std::string countryNS  = "country";
        std::string cityNS     = "city";

        // write out the root tag, including the namespaces
        xmlFile << "<" << countryNS << ":countries" <<
            "\n\txmlns:country=\"http://library.smalltown.usa/country\"" <<
            "\n\txmlns:city=\"http://library.smalltown.usa/city\">\n";
        
        // for each country in the data map
        for (const auto &data : mCountriesMap)
        {
            // store the country from the map
            std::string country = data.first;
            std::string name    = mCountriesMap[country][0];
            std::string region  = mCountriesMap[country][1];
            std::string code    = mCountriesMap[country][2];
            // write the country tag
            xmlFile << "\t<" << countryNS << ":country>\n";

            // write the country name, region, and code
            xmlFile << "\t\t<" << countryNS << ":name>" << name << "</" << countryNS <<":name>\n";
            xmlFile << "\t\t<" << countryNS << ":region>" << region << "</" << countryNS << ":region>\n";
            xmlFile << "\t\t<" << countryNS << ":code>" << code << "</" << countryNS << ":code>\n"; 

            // write the cities opening tag
            xmlFile << "\t\t<" << cityNS << ":cities>\n";

            // init a counter, start at 3 for the city information
            int i = 3;
            // loop through the rest of the country data
            while (i <mCountriesMap[country].size ())
            {
                // store the city, district, and population for the current city
                std::string city       = mCountriesMap[country][i++];
                std::string district   = mCountriesMap[country][i++];
                int population = std::stoi (mCountriesMap[country][i++]);
                // validate the city && district
                Validate (city);
                Validate (district);

                // write out the city opening tag
                xmlFile << "\t\t\t<" << cityNS << ":city>\n";
                // write out the city name, district, and population
                xmlFile << "\t\t\t\t<" << cityNS << ":name>" << city << "</" << cityNS << ":name>\n";
                xmlFile << "\t\t\t\t<" << cityNS << ":district>" << district << "</" << cityNS << ":district>\n";
                xmlFile << "\t\t\t\t<" << cityNS << ":population>" << population << "</" << cityNS << ":population>\n";

                // write out the city closing tag
                xmlFile << "\t\t\t</" << cityNS << ":city>\n";
            }
            // close cities tag
            xmlFile << "\t\t</" << cityNS << ":cities>\n"; 
            // close the country tag
            xmlFile << "\t</" << countryNS << ":country>\n";
        }   
        // close the countries tag
        xmlFile << "</" << countryNS << ":countries>\n";
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
    return mCountriesMap.find (data) == mCountriesMap.end() ? false : true;
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
            mCountriesMap[currentCountry] = line;
        // else we already have this country, so just add the cities
        else
            // loop from 3 (city name) to 5 (population)
            for (int i=3; i<=5; ++i)
                // add that data to the currenty country's vector
                mCountriesMap[currentCountry].push_back (line[i]);
    }
}

/*
    This method Validates the string for XML.  It finds and
    replaces all '&'
*/
void ConvertXML::Validate (std::string &line)
{
    // init a location variable
    auto loc = 0;

    // while we can find an '&' in the string
    while ((loc = line.find('&', loc)) != std::string::npos)
    {
        // set the line to replacing the '&' with xml equivalent
        line = line.replace (loc, 1, "&amp;");
        // increment loc to begin searching past the current '&' for any others
        loc++;
    }
}

// this method returns the xml file name
std::string ConvertXML::GetXMLFileName () const { return mXmlFile; }
