/*******************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 2

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11
for example:
macuser$ g++ main.cpp -std=c++11

This program converts old records that are in a text format to
an XML format.  This program accepts data files in the text format,
then parses all the data in the text file, then writes out to an xml
file of the users choosing.  
*******************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// declare std namespace for most commonly
using std::cout;
using std::endl;
using std::string;
using std::vector;

// method declarations
bool CheckFile (string &);
string GetFileName ();
vector<string> OpenFile ();
void ConvertToXML (const vector<string> &);
string GetXMLFileName ();
void ParseData (const vector<string> &, int &);
vector<string> ParseInfo (string &);
void SplitString(string &, char);

int main ()
{
    // output the title of the program
    cout << "--- Welcome to We-Code-Stuff, LLC's Data to XML converter ---" << endl;

    // get the data from the file
    vector<string> data = OpenFile ();
    
    // convert the data to the xml file
    ConvertToXML (data);

    return 0;
}

/*
    This method checks if the file can be opened or not
    returns true if so, else returns false
*/
bool CheckFile (string &fileName)
{
    // init a bool if we can open the file
    bool canOpen = false;

    // open the file
    std::fstream file (fileName.c_str( ), std::ios::in);

    // if the file is opened, set canOpen to true
    if (file)
        canOpen = true;

    // close the file
    file.close ();

    // return the bool
    return canOpen;    
}

/*
    This method gets the file name from the user
    it will keep looping until the user enters
    a correct file name
*/
string GetFileName ()
{
    // declare a string to hold the file name
    string fileName;

    // keep looping until we get a good file
    while (true)
    {
        // prompt the user for the file name
        cout << "Enter the file to be converted to XML: ";
        // get the line using standard cin and pass to the string
        std::getline (std::cin, fileName);

        // if we get a successful check on the file name, break from loop
        if (CheckFile (fileName))
            break;
        // else we output a warning that the file can't be opened
        else
            cout << "\tWARNING: the file " << fileName << " cannot be opened." << endl;
    }
    cout << endl;

    // return the file name
    return fileName;
}

/*
    This method is responsible for getting the file opened
    and putting the data from that file into a vector
*/
vector<string> OpenFile ()
{
    // call the method that gets the file name
    string fileName = GetFileName ();
    // declare a vector to hold the data
    vector<string> data;
    // open the file with read mode
    std::fstream file (fileName.c_str (), std::ios::in);
    // declare a string to hold each line from the file
    string line;
    // while we can get a line from the file, place into the string
    while (std::getline (file, line))
        // push that string into the vector
        data.push_back (line);
    
    // close the file
    file.close ();

    // return the vector of data
    return data;
}

/*
    This method converts the data from the vector to
    an XML file
*/
void ConvertToXML (const vector<string> &data)
{
    // Get the XML file name from the user
    string xmlFile = GetXMLFileName ();

    // initialize line counters to keep track of where the manager data
    // is, where the end manager data line is, and where the companies data
    // is located
    int i = 0, j = 0;

    // open the xml file, with write mode
    std::fstream file (xmlFile.c_str (), std::ios::out);

    // if file is opened
    if (file)
    {
        // tell user we are writing to the file
        cout << "Writing to " << xmlFile <<  "\n\n";

        // write the xml 
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        // write the comment
        file << "<!--\n";
        file << "\tXML Converter by\n";
        file << "\tWe-Code-Stuff, LLC\n";
        file << "\tDeveloper: Travis Avey\n";
        file << "-->\n";

        // write the root document node
        file << "<accounts>\n";

        do
        {
            // open the account tag
            file << "\t<account>\n";

            // parse the data and get the location of the end manager data
            ParseData (data, j);

            // get the first line of this data set, which will be the manager and location data
            string line = data[i++];
            // parse the manager data
            auto info = ParseInfo (line);

            // write the manager and location data
            file << "\t\t<manager employeeId=\"" << info[1] << "\" mId=\"" << info[0] << "\">\n";
            file << "\t\t\t<lastName>" << info[2] << "</lastName>\n";
            file << "\t\t\t<firstName>" << info[3] << "</firstName>\n";
            file << "\t\t</manager>\n";
            file << "\t\t<location>\n";
            file << "\t\t\t<city>" << info[4] << "</city>\n";
            file << "\t\t\t<state>" << info[5] << "</state>\n";
            file << "\t\t</location>\n";

            // the company count will be line j - line i
            file << "\t\t<companies count=\"" << j-i << "\">\n";

            // take line i+1 to j (not including) and output the companies
            for (; i<j; ++i)
            {
                // get the company
                string company = data[i];
                
                // split the company on an empty character
                SplitString (company, ' ');

                // output the company
                file << "\t\t\t<company>" << company << "</company>\n";
            }
            // close the companies tag
            file << "\t\t</companies>\n";

            // close the account tag
            file << "\t</account>\n";

            // set i and j to next line and continue loop, unless end of data
            i = ++j;
        } while (j < data.size ());

        // cloase the accounts tag
        file << "</accounts>\n";
    }

    // close the file
    file.close ();
    // inform the user writing is completed
    cout << "Done writing to " << xmlFile << endl;
}

/*
    This method gets the XML file name
    from the user.  If user doen't inlcude
    .xml at the end, this will add to the end
*/
string GetXMLFileName ()
{
    // declare a string to hold the file name
    string file;

    while (true)
    {
        // prompt user for the XML file name
        cout << "Please enter the XML file name: ";
        
        try
        {
            // get the line from standard cin passing to the string 
            getline (std::cin, file);
            
        }
        catch (const std::exception &e)
        {
            cout << "\tWARNING: file name not acceptable\n\n";
            continue;
        }
        break;
    }

        // if user accidently a file name starting with .
        if (file[0] == '.')
            // set the file name, removing the first char 
            file = file.substr (1);
        
        // if the file name doesn't end in 'xml', append to the end of the string:

        // get the length of the file name
        auto length = file.length ();

        // short circuit the length first
        // if less than 4, then filename cant end in .xml, or if the last 4 chars aren't '.xml'
        if (length <= 4 || file.substr(length - 4, length) != ".xml")
            // append .xml to the file name
            file += ".xml";

    cout << endl;

    // return the file string
    return file;

}

/*
    This method Parses the data, it finds the first line where
    the --END_MANAGER_DATA-- appears.  It will update the counter
    to this location.
*/
void ParseData (const vector<string> &data, int &j)
{
    // keep looping
    while (true)
    {
        // get the current line
        string line = data[j];

        // if the line is the end of the manager data, break
        if (line.find("--END_MANAGER_DATA--") != string::npos)
            break;
        // increment to next line
        j++;
    }
}

/*
    This method parses the admin data of the account

    returns a vector of strings:
    0 : mId
    1 : employeeId
    2 : lastName
    3 : firstName
    4 : city
    5 : state
*/
vector<string> ParseInfo (string &info)
{
    // declare a vector of strings to hold the data
    vector<string> data;
    

    // mId will always be the first in the line
    data.push_back (info.substr(0,1));

    // the rest of the data is split by an empty space,
    // call method that splits the info string
    SplitString (info, ' ');
    
    // initialize a string stream on the info
    std::stringstream ss(info);
    // declare a string to hold the token
    string token;

    // while we can get a token by a delimeter of a space
    while (std::getline(ss, token, ' '))
        // add that token to the vector
        data.push_back (token);

    // return the parsed info
    return data;
}

/*
    This helper method splits the string on the delimeter
*/
void SplitString (string &str, char delimeter)
{
    // find the position of the delimeter
    auto position = str.find(delimeter, 0);
    // change the string to be from that position +1 to the end
    str = str.substr (position+1);
}
