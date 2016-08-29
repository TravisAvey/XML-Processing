/****************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 1

Programmed on Mac OSX (El Capitan) using G++ 4.2.1

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
    std::fstream file;
    std::vector<int> inputNumbers;
public:
    XmlGenerator (std::string fileName)
    {
        file = std::fstream (fileName);
    }
};

std::string GetInput ();
std::string CreateFile ();
bool CheckFileName (std::string);

int main ()
{
    std::cout << "--- Welcome to Travis Avey's Number Conversion Answer Generator ---" << std::endl;

   
    while (true)
    {
        CreateFile ();
    }
    

    while (true)
    {
        std::string input = GetInput ();
        if (input == "DONE")
            break;
        else
        {
            try
            {
                int number = std::stoi (input);
                if (number < 0)
                    std::cout << "\tWARNING: " << input << " is less than 0. Will not be accepted" << std::endl;
            } catch (std::invalid_argument e)
            {
                std::cout << "\tWARNING: " << input << " cannot be converted to an integer." << std::endl;
            }
        }
    }


    return EXIT_SUCCESS;
}

std::string GetInput ()
{
    std::string input;
    std::cout << "Enter an inter, or DONE to stop: ";
    std::getline (std::cin, input);
    return input;
}

std::string CreateFile ()
{
    std::cout << "Enter the filename for the data: ";
    std::string fileName;
    std::getline (std::cin, fileName);

    CheckFileName (fileName);
    
    return fileName;

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