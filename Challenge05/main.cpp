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
    bool seenCityName;
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
        seenCityName    = false;
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

// the startElement method
void SaxHandler::startElement (const XMLCh * const uri,
                               const XMLCh * const localName,
                               const XMLCh * const qname,
                               const Attributes &attrs)
{
    // get the tag using the XMLString class 
    char *tag = XMLString::transcode (localName);

     // if the tag is country
    if (std::strcmp (tag, "country") == 0)
        inCountry = true;  
    else if (std::strcmp (tag, "name") == 0 && inCountry)
        inCountryName = true;
    else if (std::strcmp (tag, "city") == 0 && inCountry)
        inCity = true;
    else if (std::strcmp (tag, "name") == 0 && inCity)
        inCityName = true;
    else if (std::strcmp (tag, "religions") == 0)
        inReligion = true;
    XMLString::release (&tag);


/*
    // if the tag is country
    if (std::strcmp (tag, "country") == 0)
        inCountry = true;  
    else if (std::strcmp (tag, "name") == 0 && inCountry)
        inCountryName = true;
    else if (std::strcmp (tag, "city") == 0)
        inCity = true;
    else if (std::strcmp (tag, "name") == 0 && inCity)
        inCityName = true;
    else if (std::strcmp (tag, "religions") == 0 && inCountry)
        inReligion = true;

        */
    XMLString::release (&tag);
}

void SaxHandler::characters (const XMLCh * const chars, const XMLSize_t length)
{
    char *data = new char[length+1];
    XMLString::transcode (chars, data, length);
    if (inCountry && inCountryName && !addedCountry)
    {
        AddCountry (data);
        addedCountry = true;
    }
    else if (inCountry && inCity && inCityName && !seenCityName)
        AddCity (data);
    else if (inReligion && inCountry)
        AddReligion (data);

}

void SaxHandler::endElement (const XMLCh * const uri,
                             const XMLCh * const localName,
                             const XMLCh * const qname)
{
    auto tag = XMLString::transcode (localName);

    if (inCountry && std::strcmp (tag, "country") == 0)
    {
        inCountry = false;
        seenCityName = false;
        addedCountry = false;
    }
    else if (inCountry && inCountryName && std::strcmp (tag, "name") == 0)
        inCountryName = false;
    else if (inCityName && std::strcmp (tag, "name") == 0)
    {
        inCityName = false;
        seenCityName = true;
    }
    if (inCity && std::strcmp (tag, "city") == 0)
        inCity = false;
    else if (inReligion && std::strcmp (tag, "religions") == 0)
        inReligion = false;

        /*
    if (inCity && inCityName && std::strcmp (tag, "name") == 0)
    {
        inCityName = false;
    }
    else if (inCity && std::strcmp (tag, "city") == 0)
        inCity = false;
    else if (inReligion && std::strcmp (tag, "religions") == 0)
        inReligion = false;
    else if (inCountryName && std::strcmp (tag, "name") == 0)
        inCountryName = false;
    else if (inCountry && std::strcmp (tag, "country") == 0)
    {
        inCountry = false;
        addedCountry = false;
    } */
    XMLString::release (&tag);
}

void SaxHandler::fatalError (const SAXParseException &e)
{
    char *message = XMLString::transcode (e.getMessage ());
    std::cout << "XML ERROR OCCURED: " << message << std::endl;
}

void SaxHandler::AddCountry (const char *country)
{
    currentCountry = country;
    std::cout << "currentCountry " << currentCountry << std::endl;
    mCountries.push_back (country);
}

void SaxHandler::AddCity (const char *city)
{
    
    mCities[currentCountry].push_back (city);
}

void SaxHandler::AddReligion (const char *religion)
{
    mReligions[currentCountry].push_back(religion);
}

/*
    To avoid being wet, this method outputs neccessary
    data in the header of each XML file
*/
void SaxHandler::XMlHeader (std::fstream &file)
{
    // output the xml tag
    file << "<?xml version=\"1.0\"?>" << std::endl;

    // output the required comment
    file << "<!--" << std::endl;
    file << "Travis Avey" << std::endl;
    file << "CSCI 3020 Section W1" << std::endl;
    file << "Fall 2016" << std::endl;
    file << "Assignent 5" << std::endl;
    file << "-->" << std::endl;

}

void SaxHandler::CreateCountryXML ()
{
    std::fstream countryXML ("country.xml", std::ios::out);

    XMlHeader (countryXML);

    countryXML << "<countries>\n";
    
    for (auto country : mCountries)
        countryXML << "\t<country>" << country << "</country>\n";

    countryXML << "</countries>\n";
}

void SaxHandler::CreateCitiesXML ()
{
    std::fstream cityXML ("cities.xml", std::ios::out);

    XMlHeader (cityXML);

    cityXML << "<cities>\n";

    for (const auto &city : mCities)
    {
        
        
        std::string country = city.first;
        for (int i=0; i<mCities[country].size (); ++i)
        {
            cityXML << "\t<city>\n";
            cityXML << "\t\t<city-name>" << mCities[country][i] << "</city-name>\n";
            cityXML << "\t\t<country-name>" << country << "</country-name>\n";
            cityXML << "\t</city>\n";
        }
    }

    cityXML << "</cities>\n";
}

void SaxHandler::CreateReligionsXML ()
{
    std::fstream religionXML ("religions.xml", std::ios::out);

    XMlHeader (religionXML);

    religionXML << "<religions>\n";

    for (const auto &religion : mReligions)
    {
        std::string country = religion.first;

        religionXML << "\t<country>\n";
        religionXML << "\t\t<country-name>" << country << "</country-name>\n";
        religionXML << "\t\t<religion-list>\n";

        for (int i=0; i<mReligions[country].size (); ++i)
            religionXML << "\t\t\t<religion>" << mReligions[country][i] << "</religion>\n";

        religionXML << "\t\t</religion-list>\n";
        religionXML << "\t<country>\n";


    }
}

bool Init (SaxHandler *);

int main ()
{
    auto *handler = new SaxHandler ();

    if (!Init(handler))
    {
        delete handler;
        return 1;
    }

    handler->CreateCountryXML ();
    handler->CreateCitiesXML ();
    handler->CreateReligionsXML ();
    
    delete handler;
    return 0;
}

bool Init (SaxHandler *handler)
{
    try
    {
        XMLPlatformUtils::Initialize();
    } catch (const XMLException &e)
    {
        std::cout << "ERROR: Unable to initialize the parser" << std::endl;
        return false;
    }

    SAX2XMLReader *parser = XMLReaderFactory::createXMLReader ();
    parser->setFeature (XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature (XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setContentHandler (handler);
    parser->setErrorHandler (handler);

    std::string file = "mondial-3.0.xml";

    try
    {
        parser->parse (file.c_str ());
    } 
    catch (const XMLException &e)
    {
        char *message = XMLString::transcode (e.getMessage ());
        std::cout << "XML ERROR: " << message << std::endl;
        XMLString::release (&message);
        return false;
    }
    catch (const SAXParseException &e)
    {
        char *message = XMLString::transcode (e.getMessage ());
        std::cout << "PARSE ERROR: " << message << std::endl;
        XMLString::release (&message);
        return false;
    }
    catch (...)
    {
        std::cout << "UNEXEPECTED ERROR" << std::endl;
        return false;
    }

    return true;
}