/****************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 1

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11
for example:
macuser$ g++ main.cpp -std=c++11

This program generates an XML file that contains number conversions.
These conversions are decimal, binary, octal, and hexidecimal.
It will only take in positive numbers, and 0, and will only successfully
write an xml file if the user chooses a correct file name.
For each number the output into the xml file will be:
<question assign-num="1">
    <decimal> 1 </decimal>
    <binary> 0b1 </binary>
    <octal> 01 </octal>
    <hexadecimal> 0x1 </hexadecimal>
</question>

where the root of the document will be <assignment></assignment>
****************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/*
    This class handles generating an xml file and
    parsing the numbers into binary, octal, and hexidecimal
*/
class XmlGenerator
{
private:
    // the string to hold the name of the file we are working with
    std::string fileName;
    // the vector to hold all the input numbers
    std::vector<int> inputNumbers;

    /*
        This method converts the integer to a binary number,
        returns the value as a string.

        Format: 0b1001, removes all leading 0s
    */
    std::string ConvertBinary (const int &number)
    {
        // if the number is 0, short circuit and just return 0b0
        if (number == 0)
            return "0b0";
        
        // init a string to hold the binary value
        std::string binary = "0b";
        
        // get the binary number and store as a string
        std::string binStr = std::bitset<16> (number).to_string ();
        
        // declare the index i
        int i;
        // loop through the binary string
        for (i = 0; i<binStr.length (); ++i)
        {
            // find the first 1
            if (binStr[i] == '1')
                break;
        }
        
        // from the i position to the end, append the 1s and 0s to the binary string
        for (int j = i; j<binStr.length (); ++j)
            binary += binStr[j];
        
        // return the binary string
        return binary;
    }

    /*
        This method converts the number to an Octal number
        returns a string of the Octal number
    */
    std::string ConvertOctal (const int &number)
    {
        // initiallize a string stream
        std::stringstream ss;
        // use the string stream to create the octal number:
        // start with a 0, then append the number as an oct to the string stream
        ss << "0" << std::oct << number;

        // return the stringstream as a string
        return ss.str ();
    }

    /*
        This method converts the number to a hexadecimal number
    */
    std::string ConvertHex (const int &number)
    {
        // intiliaze a string stream
        std::stringstream ss;
        // on the stringstream, start with the 0x, then append the number as a hexadecimal
        ss << "0x" << std::hex << number;
        // return the stingstream as a string
        return ss.str ();
    }

    // Helper method to output a cerr, that something went wrong with file
    void FileError ()
    {
        // output an error that something is wrong with the file
        std::cerr << "Something went wrong with the file!\n";
    }

public:
    /*
        Public consructor.
        sets the file name
    */
    XmlGenerator  (const std::string &file) : fileName (file) {}

    /*
        Pulbic method to add the number to be converted
    */
    void AddNumber (int number)
    {
        inputNumbers.push_back (number);
    }

    /*
        This method writes the data to the xml file when user is done
        inputing numbers
    */
    void WriteFile ()
    {
       // open the file in append mode
        std::ofstream file (fileName.c_str(), std::ios::out);

        // assign-num counter
        int assignNum = 1;
        if (file)
        {
            // output to the file the xml tag
            file << "<?xml version=\"1.1\" encoding=\"utf-8\" ?>" << std::endl;
            // write to the file the root node
            file << "<assignment>" << std::endl;

            // for each number in the input number vector
            for (auto n : inputNumbers)
            {
                // ouput question element with the attribute with correct value
                file << "\t<question assign-num=\"" << assignNum++ << "\">\n";

                // output the decimal
                file << "\t\t<decimal> " << n << " </decimal>\n";

                // write to the file the binary number
                file << "\t\t<binary> " << ConvertBinary (n) << " </binary>\n";

                // write to the file the octal number
                file << "\t\t<octal> " << ConvertOctal (n) << " </octal>\n";

                // write to the file the hexadecimal number
                file << "\t\t<hexadecimal> " << ConvertHex (n) << " </hexadecimal>\n";

                // output to the file the closing quesion element
                file << "\t</question>\n";
            }

            // write out to the file the closing root node
            file << "</assignment>" << std::endl;
        }
        else
            // we have a issue with the file
            FileError ();

        // close the file
        file.close ();
    }

    /*
        This method returns the file name
    */
    std::string GetFileName () const
    {
        // returns the file name
        return fileName;
    }
};

// method declarations
std::string PromptInput ();
bool CheckFileName (std::string);
std::string GetFileName ();
void GetInput (XmlGenerator *);

int main ()
{
    // output the title of the program
    std::cout << "--- Welcome to Travis Avey's Number Conversion Answer Generator ---" << std::endl;

    // get the file name from the user
    std::string fileName = GetFileName ();

    // initialize a new XmlGenerator class using the file name
    auto xmlGenerator = new XmlGenerator (fileName);

    // get input from the user for the numbers
    GetInput (xmlGenerator);
    
    // done with xmlGenerator, delete it
    delete xmlGenerator;

    return 0;
}

/*
    This method Gets input from the user for the numbers
    that will be converted

    Pass in a pointer to the XmlGenerator object
*/
void GetInput (XmlGenerator *xml)
{
    // initialize a counter
    int count = 0;
    // loop untile we break
    while (true)
    {
        // get the input
        std::string input = PromptInput ();

        // if user input DONE, break
        if (input == "DONE")
            break;
        else
        {
            // use a try/catch block to convert the input to an int
            try
            {
                // try to convert the string to an int
                int number = std::stoi (input);
                // if number is less than 0, output warnging and skip this input
                if (number < 0)
                    std::cout << "\tWARNING: " << input << " is less than 0. Will not be accepted" << std::endl;
                else
                {
                    // if number is ok, then add the number to be converted
                    xml->AddNumber (number);
                    // increment counter
                    count++;
                }
            // if conversion failed, user didn't enter an int
            } catch (std::invalid_argument e)
            {
                // output warning that this input will not be accepted
                std::cout << "\tWARNING: " << input << " cannot be converted to an integer." << std::endl;
            }
        }
    }

    // if user inputed anything, then output message that data was written to the file
    if (count > 0)
    {
        // call method that completes and writes to file
        xml->WriteFile ();
        // output message to user that writing to file is complete
        std::cout << "Finished. Data written to " << xml->GetFileName () << std::endl;
    }
    else
        // else, user didn't enter anything so output warning
        std::cout << "WARNING: no data to generate quiz" << std::endl;
}

/*
    This method prompts the user for input and
    returns that input as a string
*/
std::string PromptInput ()
{
    // declare a string for the input
    std::string input;
    // prompt the user for input
    std::cout << "Enter an integer, or DONE to stop: ";
    // get the input store in the input string, using standard cin
    std::getline (std::cin, input);
    // return input
    return input;
}

/*
    This method gets the file name from the user,
    by looping until they enter a usable file name
    and returns that file name as a string
*/
std::string GetFileName ()
{
    // decalare a string to hold the file name
    std::string fileName;    
    // loop until we get a usable file name
    while (true)
    {
        // prompt the user for the file name
        std::cout << "Enter the filename for the data: ";
        
        // get the file name from the user storing in a string, using standard cin
        std::getline (std::cin, fileName);

        // check the file name, it if is ok break from loop
        if (CheckFileName (fileName))
            break;
    }
    // return file name
    return fileName;
}

/*
    This method checks the user input for the file name.
    If the supplied string isn't long enough or doesn't end
    in xml, return false, otherwise return true
*/
bool CheckFileName (std::string file)
{
    // get the size of the file name
    int fileLen = file.length ();

    if (fileLen < 5 || file.substr (fileLen -4, fileLen) != ".xml")
    {
        std::cout << "\tWARNING: " << file << " does not end in .xml or is not long enough\n";
        return false;
    }
    else
        return true;
    // get the position of the string ".xml" from the paramater file
    std::size_t position = file.find (".xml");
    // if the position is the max of size_t or if the file is less than 5
    if (position == SIZE_T_MAX || file.length () < 5)
    {
        // then we output a warning and return false
        std::cout << "\tWARNING: " << file << "does not end in .xml or is not long enough\n";
        return false;
    }
    // if we get to this point, the file name is ok. return true
    return true;
}
