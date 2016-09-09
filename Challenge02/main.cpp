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
            cout << "\tWARNING: the file " << fileName << " cannot be opened." << endl << endl;
    }

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

    // open the xml file, with write mode
    std::fstream file (xmlFile.c_str (), std::ios::out);

    // if file is opened
    if (file)
    {
        // ouput the xml 
        file << "<? xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
        // output the comment
        file << "<!--\n";
        file << "\tXML Converter by\n";
        file << "\tWe-Code-Stuff, LLC\n";
        file << "\tDeveloper: Travis Avey\n";
        file << "-->\n\n";

        // output the root document node
        file << "<accounts>\n";

        // TODO: call a method to parse the data

    }

    // close the file
    file.close ();
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

    // prompt user for the XML file name
    cout << "Please enter the XML file name: ";
    // get the line from standard cin passing to the string 
    getline (std::cin, file);
    // output new lines
    cout << endl << endl;

    // if the file name doesn't end in 'xml', append to the end of the string
    if (file.substr(file.length() - 4, file.length()) != ".xml")
        file += ".xml";
    
    // debug file name out
    cout << file << endl;

    // return the file string
    return file;

}
