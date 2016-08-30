/****************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 1

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11

This program does 
// TODO: fill this part in

****************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class XmlGenerator
{
private:
    // the string to hold the name of the file we are working with
    std::string fileName;
    // the vector to hold all the input numbers
    std::vector<int> inputNumbers;

    /*
        This method initializes the file to store the
        data in XML format
    */
    void InitFile ()
    {
        // open the file to write
        std::ofstream file (fileName.c_str());

        // if the file is opened
        if (file)
        {
            // output to the file the xml tag
            file << "<?xml version=\"1.1\" encoding=\"utf-8\" ?>" << std::endl;
            // write to the file the root node
            file << "<assignment>" << std::endl;
        }
        else
            // we have an issue with the file
            FileError ();

        // done with the file, close it    
        file.close ();
    }

    /*
        This method writes the data to the file
    */
    void WriteFile ()
    {
        // open the file in append mode
        std::ofstream file (fileName.c_str(), std::ios::app);

        // assign-num counter
        int assignNum = 1;
        if (file)
        {
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
        }
        else
            // we have a issue with the file
            FileError ();

        // close the file
        file.close ();
    }


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
    */
    std::string ConvertOctal (const int &number)
    {
        return "";
    }

    /*
        This method converts the number to a hexadecimal number
    */
    std::string ConvertHex (const int &number)
    {
        return "";
    }

    // Helper method to output a cerr, that something went wrong with file
    void FileError ()
    {
        std::cerr << "Something went wrong with the file!\n";
    }

public:
    /*
        Public consructor.
        variable file is the name of the file
    */
    XmlGenerator  (const std::string &file) : fileName (file)
    {
        // call private method to initialize the file
        InitFile ();
    }

    /*
        Pulbic method to add the number to be converted
    */
    void AddNumber (int number)
    {
        inputNumbers.push_back (number);
    }

    /*
        This method is public interface when done inputting data
        
        (Possibly change this to just be the private write file method)
    */
    void Done ()
    {

        WriteFile ();
        
        // open the file to write in append mode
        std::ofstream file (fileName.c_str(), std::ios::app);

        // if the file is opened
        if (file)
        {
            // write out to the file the closing root node
            file << "</assignment>" << std::endl;
        }
        else
            FileError ();
        
        // close file
        file.close ();
    }

    /*
        This method returns the number of items that are
        to be converted
    */
    int Count()
    {
        return inputNumbers.size ();
    }
};

std::string PromptInput ();
std::string CreateFileName ();
bool CheckFileName (std::string);
std::string GetFile ();
void GetInput (XmlGenerator &);

int main ()
{
    std::cout << "--- Welcome to Travis Avey's Number Conversion Answer Generator ---" << std::endl;

    std::string fileName = GetFile ();

    auto xmlGenerator = new XmlGenerator (fileName);

    GetInput (*xmlGenerator);
    
    xmlGenerator->Done ();

    if (xmlGenerator->Count () > 0)
        std::cout << "Finished. Data written to " << fileName << std::endl;
    else
        std::cout << "WARNING: no data to generate quiz" << std::endl;

    return EXIT_SUCCESS;
}

void GetInput (XmlGenerator &xml)
{
    while (true)
    {
        std::string input = PromptInput ();
        if (input == "DONE")
            break;
        else
        {
            try
            {
                int number = std::stoi (input);
                if (number < 0)
                    std::cout << "\tWARNING: " << input << " is less than 0. Will not be accepted" << std::endl;
                else
                    xml.AddNumber (number);
            } catch (std::invalid_argument e)
            {
                std::cout << "\tWARNING: " << input << " cannot be converted to an integer." << std::endl;
            }
        }
    }
}

std::string PromptInput ()
{
    std::string input;
    std::cout << "Enter an inter, or DONE to stop: ";
    std::getline (std::cin, input);
    return input;
}

std::string GetFile ()
{
    std::string fileName;    
    while (true)
    {
        fileName = CreateFileName ();
        if (fileName != "")
            break;
    }
    return fileName;
}

std::string CreateFileName ()
{
    std::cout << "Enter the filename for the data: ";
    std::string fileName;
    std::getline (std::cin, fileName);

    if (CheckFileName (fileName))
        return fileName;
    else
        return "";

}

bool CheckFileName (std::string file)
{
    std::size_t position = file.find (".xml");
    if (position == SIZE_T_MAX || file.length () < 5)
    {
        std::cout << "\tWARNING: " << file << "does not end in .xml or is not long enough\n";
        return false;
    }
    return true;
}