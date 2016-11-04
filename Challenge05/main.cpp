/**************************
Travis Avey
CSCI 3020 Section W1
Fall 2016
Assignment 5

Programmed on Mac OSX (10.11.6) using g++ 4.2.1 with flag -std=c++11
for example:
macuser$ g++  main.cpp -O2 -I ./xerces/include/ -bind_at_load -Wl 
./xerces/lib/libxerces-c.dylib -std=c++11

This program uses the Xerces C++ library by Apache and converts an XML into 3
separate XML files.  It parses an XML file 'mondial-3.0.xml' for Country names,
the cities belonging to those countries, and finally religions also belonging
to those countries.  During parsing it places the Country Names in a vector,
the cities in a map <country, city>, and the religions in a map <country, religion>.
Upon success of parsing, the program calls three methods that creates each of the xml
files from the vector and 2 maps
**************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace xercesc;

class SaxHandler : public DefaultHandler
{
private:
    // the data structures to hold the extracted information
    std::vector<std::string> mCountries;
    std::map<std::string, std::vector<std::string>> mCities;
    std::map<std::string, std::vector<std::string>> mReligions;
    // a string to hold the current country
    std::string currentCountry;

    // boolean values to keep track of where we are parsing
    bool inCountry;
    bool inCountryName;
    bool addedCountry;
    bool inCity;
    bool inCityName;
    bool inReligion;

    // private helper methods
    void AddCountry (const char *);
    void AddCity (const char *);
    void AddReligion (const char *);

    void XMlHeader (std::fstream &);
    
public:
    // public constructor
    SaxHandler ()
    {
        // set all the boolean values initally
        // to false
        inCountry       = false;
        inCountryName   = false;
        addedCountry    = false;
        inCity          = false;
        inCityName      = false;
        inReligion      = false;
    }

    // override the startElement method
    void startElement (const XMLCh * const uri,
                       const XMLCh * const localName,
                       const XMLCh * const qname,
                       const Attributes &attrs);
    
    // override the endElement method
    void endElement (const XMLCh * const uri,
                     const XMLCh * const localName,
                     const XMLCh * const qname);

    // override the characters method
    void characters (const XMLCh * const chars, const XMLSize_t length);

    // override the fatalError method
    void fatalError (const SAXParseException &);

    // methods to call when parsing complete
    void CreateCountryXML ();
    void CreateCitiesXML ();
    void CreateReligionsXML ();
};

/*
    The startElement method checks the start element tag and sets
    appropriate boolean values to keep track of which XML elements
    the parser is in
*/
void SaxHandler::startElement (const XMLCh * const uri,
                               const XMLCh * const localName,
                               const XMLCh * const qname,
                               const Attributes &attrs)
{
    // get the tag using the XMLString class 
    char *tag = XMLString::transcode (localName);

     // if the tag is country, we are now in the country xml ele
    if (std::strcmp (tag, "country") == 0)
        inCountry = true;  
    // if the tag is name and inCountry is true, but not in the city ele
    else if (std::strcmp (tag, "name") == 0 && inCountry && !inCity)
        // we are now in the country name element
        inCountryName = true;
    // if the tag is city and parser is in the country element
    else if (std::strcmp (tag, "city") == 0 && inCountry)
        // we are now in the city element
        inCity = true;
    // if the tag is name and parser is in the city element
    else if (std::strcmp (tag, "name") == 0 && inCity)
        // we are now in the city name element
        inCityName = true;
    // if the tag is religions and within the country element
    else if (std::strcmp (tag, "religions") == 0 && inCountry)
        // the parser is now in the religions tag
        inReligion = true;
    // done with the tag, free up memory
    XMLString::release (&tag);

}

/*
    The characters method grabs the data in between tags and calls appropriate methods
    based on where the parser is (or does nothing if we aren't in a tag that we care about)
*/
void SaxHandler::characters (const XMLCh * const chars, const XMLSize_t length)
{
    // create a new char array to hold the data
    char *data = new char[length+1];
    // pass the characters to the data char array
    XMLString::transcode (chars, data, length);

    // if parser is in country tag and in name, but we haven't added the country name
    if (inCountry && inCountryName && !addedCountry)
    {
        // call method to add the data
        AddCountry (data);
        // set boolean to true, we now have added (the first) country name
        addedCountry = true;
    }
    // if parser is in the country, city, and name tag
    else if (inCountry && inCity && inCityName)
        // call method to add the city
        AddCity (data);
    // if parser is in religion and in country tag
    else if (inReligion && inCountry)
        // call method to add the religion
        AddReligion (data);
    
    // clean up memory
    delete [] data;

}

/*
    The end element method checks which xml element has just closed and
    sets appropriate boolean values to check where the parser is
*/
void SaxHandler::endElement (const XMLCh * const uri,
                             const XMLCh * const localName,
                             const XMLCh * const qname)
{
    // grab the tag name of the closing element
    auto tag = XMLString::transcode (localName);

    // if we are in the country and the tag name is country
    if (inCountry && std::strcmp (tag, "country") == 0)
    {
        // we are no longer in country, set to false
        inCountry = false;
        // reset that we have added country name to false
        addedCountry = false;
    }
    // if in country, in country name and tag is name
    else if (inCountry && inCountryName && std::strcmp (tag, "name") == 0)
        // no longer in the country name element
        inCountryName = false;
    // if in the city and tag name is name
    else if (inCityName && std::strcmp (tag, "name") == 0)
        // no longer in city name element
        inCityName = false;
    // if in the city tag and the tag name is city
    else if (inCity && std::strcmp (tag, "city") == 0)
        // no longer in the city element
        inCity = false;
    // if in religion and tag name is religions
    else if (inReligion && std::strcmp (tag, "religions") == 0)
        // no longer in religion
        inReligion = false;

    // clean up memory, release the tag
    XMLString::release (&tag);
}

/*
    The fatal error method outputs a message to the user
    that an xml error has occured
*/
void SaxHandler::fatalError (const SAXParseException &e)
{
    // get the exception message
    char *message = XMLString::transcode (e.getMessage ());
    // output message to the user
    std::cout << "XML ERROR OCCURED: " << message << std::endl;
    // clean up memory
    delete message;
}

/*
    The add country method will add the country name 
    to the countries vector data structure
*/
void SaxHandler::AddCountry (const char *country)
{
    // set the current country
    currentCountry = country;
    // add the current country
    mCountries.push_back (country);
}

/*
    The add city method adds the city to the
    cities map data structure
*/
void SaxHandler::AddCity (const char *city)
{
    // add the city using the current country
    mCities[currentCountry].push_back (city);
}

/*
    The add religion method adds the religion to
    the religions map data structure
*/
void SaxHandler::AddReligion (const char *religion)
{
    // add the religion using the current country
    mReligions[currentCountry].push_back(religion);
}

/*
    To avoid being wet, this method outputs neccessary
    data in the header of each XML file
*/
void SaxHandler::XMlHeader (std::fstream &file)
{
    // output the xml tag
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

    // output the required comment
    file << "<!--" << std::endl;
    file << "Travis Avey" << std::endl;
    file << "CSCI 3020 Section W1" << std::endl;
    file << "Fall 2016" << std::endl;
    file << "Assignent 5" << std::endl;
    file << "-->" << std::endl;

}

/*
    The create country xml method creates and opens the country.xml
    and adds the appropriate output to the xml file
*/
void SaxHandler::CreateCountryXML ()
{
    // create and open the file in output mode
    std::fstream countryXML ("country.xml", std::ios::out);

    // call helper method to output header information
    XMlHeader (countryXML);

    // output open country tag
    countryXML << "<countries>\n";
    
    // for each countr in the countries vector
    for (auto country : mCountries)
        // output the country name
        countryXML << "\t<country>" << country << "</country>\n";

    // close the countries tag
    countryXML << "</countries>\n";

    // close the file
    countryXML.close ();
}

/*
    This method creates and opens the cities.xml file
    add adds appropriate output the xml file
*/
void SaxHandler::CreateCitiesXML ()
{
    // create and open the file in output mode
    std::fstream cityXML ("cities.xml", std::ios::out);

    // call helper method to output the header information
    XMlHeader (cityXML);

    // output the cities tag
    cityXML << "<cities>\n";

    // for each country in the country vector
    for (const auto &country : mCountries)
    {
        // loop through the cities for the country
        for (int i=0; i<mCities[country].size (); ++i)
        {
            // output the city information
            cityXML << "\t<city>\n";
            cityXML << "\t\t<city-name>" << mCities[country][i] << "</city-name>\n";
            cityXML << "\t\t<country-name>" << country << "</country-name>\n";
            cityXML << "\t</city>\n";
        }
    }

    // output the closing tag
    cityXML << "</cities>\n";

    // close the file
    cityXML.close ();
}

/*
    This method creates and opens the religions.xml file
    and write the appropriate info to the file
*/
void SaxHandler::CreateReligionsXML ()
{
    // create and open the religions xml file in output mode
    std::fstream religionXML ("religions.xml", std::ios::out);

    // call helper method to write header info
    XMlHeader (religionXML);

    // output the open tag
    religionXML << "<religions>\n";

    // for reach country in the countries vector
    for (const auto &country : mCountries)
    {
        // output the country open tag
        religionXML << "\t<country>\n";
        // output the country namd
        religionXML << "\t\t<country-name>" << country << "</country-name>\n";
        // output the religion list tag
        religionXML << "\t\t<religion-list>\n";

        // loop through each religion using the country as a key
        for (int i=0; i<mReligions[country].size (); ++i)
            // output the current religion
            religionXML << "\t\t\t<religion>" << mReligions[country][i] << "</religion>\n";

        // close the religion list tag
        religionXML << "\t\t</religion-list>\n";
        // close the country tag
        religionXML << "\t</country>\n";
    }

    // close the religions tag
    religionXML << "</religions>\n";

    // close the file
    religionXML.close ();
}

// Declare helper method
bool Init (SaxHandler *);

int main ()
{
    // output welcome
    std::cout << "-------- Welcome to Travis Avey's Sax Parser --------\n\n";

    // create to SaxHandler object
    auto *handler = new SaxHandler ();

    // if initializing the hanlder fails
    if (!Init(handler))
    {
        // delete the handler
        delete handler;
        // return other than 0, 1
        return 1;
    }

    // output info we are writing to the files
    std::cout << "Writing parsed data from modial-3.0.xml to files:\n";
    std::cout << "\tcountry.xml\n\tcities.xml\n\treligions.xml\n\n";

    // call methods to create the xml files
    handler->CreateCountryXML ();
    handler->CreateCitiesXML ();
    handler->CreateReligionsXML ();

    // output success
    std::cout << "Writing to files completed successfully.\n";
    
    // free up memory
    delete handler;
    return 0;
}

/*
    This method handles setting up the sax parser
*/
bool Init (SaxHandler *handler)
{
    try
    {
        // try to initialize
        XMLPlatformUtils::Initialize();
    } catch (const XMLException &e)
    {
        // if we get an exception, output error and return false
        std::cout << "ERROR: Unable to initialize the parser" << std::endl;
        return false;
    }

    // boiler plate code to set up the parser
    SAX2XMLReader *parser = XMLReaderFactory::createXMLReader ();
    parser->setFeature (XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature (XMLUni::fgSAX2CoreNameSpaces, true);

    // set the content handler and the error handler,
    // that is our SaxHandler object
    parser->setContentHandler (handler);
    parser->setErrorHandler (handler);

    // the file name
    std::string file = "mondial-3.0.xml";

    try
    {
        // try to parse the file 
        parser->parse (file.c_str ());
    } 
    catch (const XMLException &e)
    {
        // if we get an XMLException 
        char *message = XMLString::transcode (e.getMessage ());
        // output the error message
        std::cout << "XML ERROR: " << message << std::endl;
        // clean up the memory, releasing the message
        XMLString::release (&message);
        // return false
        return false;
    }
    catch (const SAXParseException &e)
    {
        // if we get a SAXParse exception output the error message
        char *message = XMLString::transcode (e.getMessage ());
        // output message
        std::cout << "PARSE ERROR: " << message << std::endl;
        // free up the memory
        XMLString::release (&message);
        // return false
        return false;
    }
    catch (...)
    {
        // if we get an unexpected error
        std::cout << "UNEXEPECTED ERROR" << std::endl;
        // return false
        return false;
    }

    // free up memory
    delete parser;

    // getting to this point means everything was successful, return true
    return true;
}
