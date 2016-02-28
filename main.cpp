/*File:   main.cpp
 * 
 *Author: Alex Cushing
 *Email: acushing@cs.uml.edu
 *Created: 12/6/15
 *Edited 12/9
 */

#include <iostream>  // for cout and friends
#include <sstream>   // for string streams
#include <string>    // for the STL string class
#include <regex>     // for regular expressions
#include "jmhUtilities.h"   //Jesse Heines's utilities h file
#include "CreateDomDoc.h"   //Create dom doc h file


using namespace std;

// forward references

void ParseCommandString(string strUserEntry);
void ProcessAddCommand(string strUserEntry);
void ProcessAddRootCommand(string strUserEntry);
void ProcessAddElementCommand(string strUserEntry);
void ProcessAddAttributeCommand(string strUserEntry);
void ProcessRemoveCommand(string strUserEntry);
void ProcessPrintCommand(string strUserEntry);
void AddRoot(string myStringName);

//function which takes in the processed command and adds it into the DOMDoc
void AddToDOM(string strUserEntry);

//function which removes a named node
void removeNode(string strUserEntry);
//decalre doc global to make life easier for now
DOMDocument* doc = NULL;
DOMElement* rootElem = NULL;
bool aRoot = false; //this aRoot bool will check if there is a root because otherwise
                    //the program crashes if the user tries to create or run another
                    //command with no root in the dom

XMLCh* str_to_XMLCh(string str);
/*
 * Global variables for easy access from Assigmment Six for assignment 7
 */
static char* gXPathExpression = 0;
static XMLCh* gOutputEncoding = 0;
static bool gUseFilter = false;
static char* goutputfile = 0;
static bool gFormatPrettyPrint = true;
static bool gSplitCdataSections = true;
static bool gDiscardDefaultContent = true;
static bool gWriteBOM = false;

/**
 * The standard C++ main function.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 
 */
int main(int argc, char** argv) {
    // display program title
    cout << "=============================" << endl;
    cout << "     Assignment No. 7" << endl;
    cout << "=============================" << endl << endl;
    cout << "Type 'help' to see command list" << endl << endl;
    // string entered by the user in response to the command prompt
    string strUserEntry = "";

    //  // prompt the user to enter a command string
    //  // version 1 without using a regular expression
    //  while ( ! jmhUtilities::caseInsCompare( jmhUtilities::trim( strUserEntry ), "quit" ) ) {
    //    cout << "\nYour command: " ;
    //    getline( cin, strUserEntry ) ;
    //  }

    // prompt the user to enter a command string
    // version 2 using a regular expression
    regex reQuit("\\s*quit\\s*", regex::icase);
    while (!regex_match(strUserEntry, reQuit)) {
        cout << "\nYour command: ";
        getline(cin, strUserEntry);

        // if the user didn't enter 'quit', go parse the command string
        if (!regex_match(strUserEntry, reQuit)) {
            ParseCommandString(strUserEntry);
        }
    }

    return EXIT_SUCCESS;
}

/**
 * Check for a valid basic command.
 * this function will parse through the command string and
 * return the user entry, it will call specific functions with this
 * entry based on the regex for of it
 * @param strUserEntry command string entered by the user
 */
void ParseCommandString(string strUserEntry) {
    // regular expressions for basic commands
    regex reBasicAddCommand("\\s*add.*", regex::icase);
    regex reBasicPrintCommand("\\s*print.*", regex::icase);
    regex reBasicHelpCommand("\\s*help.*", regex::icase);

    //remove regular expression
    regex reBasicRemoveCommand("\\s*remove.*", regex::icase);


    // test for each basic command in turn
    if (regex_match(strUserEntry, reBasicAddCommand)) {
        ProcessAddCommand(strUserEntry);
    } else if (regex_match(strUserEntry, reBasicPrintCommand)) {
        ProcessPrintCommand(strUserEntry);
    } else if (regex_match(strUserEntry, reBasicRemoveCommand)) {
        //remove cmd still not completely implemented 
        ProcessRemoveCommand(strUserEntry);
    } else if (regex_match(strUserEntry, reBasicHelpCommand)) {
        //THE FOLLOWING IS THE HELP MENU FOR THE USER WHEN HELP IS INPUTTED
        cout << "HELP: Here are the commands:" << endl <<endl;
        cout << "-  ADD ROOT --  add root root_name" << endl;
        cout << "-  ADD ELEMENT --  add element parent_name element_name" << endl;
        cout << "-  ADD ATTRIBUTE --  add attribute parent_name attribute_name attribute_value" << endl;
        cout << "-  ADD ELEMENT TEXT --  add element text parent_name element_name inputted_text" << endl;
        cout << "-  ADD ATTRIBUTE TEXT --  add attribute text parent_name attribute_name value_name inputted_text" << endl << endl;
        cout << "  --  print" << endl;
        cout << "  --  help" << endl;
        cout << "  --  quit" << endl;
    } else{
        cout << "  Invalid command.  Acceptable commands are 'add', 'print', 'help', and 'quit'." << endl;
    }
}

/**
 * Handle an add command entered by the user
 * @param strUserEntry command string entered by the user
 */
void ProcessAddCommand(string strUserEntry) {
    // regular expressions for the second parameter in the add command
    regex reAddElementCommand("\\s*add\\s+element.*", regex::icase);
    regex reAddAttributeCommand("\\s*add\\s+attribute.*", regex::icase);
    //regular expression for the root in the add command
    regex reAddRootCommand("\\s*add\\s+root.*", regex::icase);



    // test for each possible second parameter in turn
    //check for root first
    if (regex_match(strUserEntry, reAddRootCommand)) {
        ProcessAddRootCommand(strUserEntry);
    } else if (regex_match(strUserEntry, reAddElementCommand) && aRoot == true) {
        ProcessAddElementCommand(strUserEntry);
    } else if (regex_match(strUserEntry, reAddAttributeCommand) && aRoot == true) {
        ProcessAddAttributeCommand(strUserEntry);
    } else if(aRoot==true){
        cout << "  Invalid add command: 2nd parameter must be 'root, 'element' or 'attribute'." << endl;
    }
    else{
        cout << "  Must add a root." << endl;
    }
}

/**
 * Handle an add root command entered by the user
 * @param strUserEntry command string entered by the user
 */
void ProcessRemoveCommand(string strUserEntry) {
    // the what variable is actually an array that will be populated by the regex_match function
    //    when matched groups are found
    cmatch what;
    // what[0] contains the entire matched string
    // what[1] contains the first matched group
    // what[2] contains the second matched group
    // what[3] etc.


    // regular expression to pick out the name of the parent to which the new element is to be added 
    //    and the name of the new element itself
    regex reRemoveCmd("^\\s*remove\\s*root\\s(\\w+)$", regex::icase);
    //regex reAddRootCmd( "\\s*add\\s+root.*", regex::icase ) ;

    // note that the following variant of the regex_match command requires a C string, not an STL string
    if (regex_match(strUserEntry.c_str(), what, reRemoveCmd)) {
        cout << "  You have specified that you want to remove the element under root named '" << what[1]
                << "'." << endl;
        removeNode(strUserEntry);
    } else {
        cout << "  Invalid 'remove' command." << endl;
    }
}

/**
 * Handle an add root command entered by the user
 * @param strUserEntry command string entered by the user
 */
void ProcessAddRootCommand(string strUserEntry) {
    // the what variable is actually an array that will be populated by the regex_match function
    //    when matched groups are found
    aRoot = true;
    cmatch what;//character match
    // what[0] contains the entire matched string
    // what[1] contains the first matched group
    // what[2] contains the second matched group
    // what[3] etc.

    // regular expression to pick out the name of the parent to which the new element is to be added 
    //    and the name of the new element itself
    regex reAddRootCmd("^\\s*add\\s*root\\s(\\w+)$", regex::icase);
    //regex reAddRootCmd( "\\s*add\\s+root.*", regex::icase ) ;

    // note that the following variant of the regex_match command requires a C string, not an STL string
    if (regex_match(strUserEntry.c_str(), what, reAddRootCmd)) {
        cout << "  You have specified that you want to add a new root named '" << what[1]
                << "'." << endl;
        AddToDOM(strUserEntry);
    } else {
        cout << "  Invalid 'add element' command." << endl;
        cout << "    'add element' must be followed by two more parameters:" << endl;
        cout << "      (1) the name of the parent to which the new element is to be added, and" << endl;
        cout << "      (2) the name of the new element itself." << endl;
    }
}

/**
 * Handle an add element command entered by the user
 * @param strUserEntry command string entered by the user
 */
void ProcessAddElementCommand(string strUserEntry) {
    // the what variable is actually an array that will be populated by the regex_match function
    //    when matched groups are found
    cmatch what;
    // what[0] contains the entire matched string
    // what[1] contains the first matched group
    // what[2] contains the second matched group
    // what[3] etc.

    // regular expression to pick out the name of the parent to which the new element is to be added 
    //    and the name of the new element itself
    regex reAddElementCmd("^\\s*add\\s*element\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);

    //regex command to add text to an already created element
    //example - "add element text root_name element_name desired_text"
    regex reAddTextElementCmd("^\\s*add\\s*element\\s*text\\s(\\w+)\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);
    // note that the following variant of the regex_match command requires a C string, not an STL string
    if (regex_match(strUserEntry.c_str(), what, reAddTextElementCmd)) {
        cout << "added element text";
        //cout << "  You have specified that you want to add a text: " << what[3] << " to element named '" << what[2]
         //       << "' to parent element '" << what[1] << "'." << endl;
        AddToDOM(strUserEntry);
    } else
        if (regex_match(strUserEntry.c_str(), what, reAddElementCmd)) {
        cout << "  You have specified that you want to add a new element named '" << what[2]
                << "' to parent element '" << what[1] << "'." << endl;
        AddToDOM(strUserEntry);
    } else {
        cout << "  Invalid 'add element' command." << endl;
        cout << "    'add element' must be followed by two more parameters:" << endl;
        cout << "      (1) the name of the parent to which the new element is to be added, and" << endl;
        cout << "      (2) the name of the new element itself." << endl;
    }
}

/**
 * Handle an add attribute command entered by the user
 * @param strUserEntry command string entered by the user
 */
void ProcessAddAttributeCommand(string strUserEntry) {
    // the what variable is actually an array that will be populated by the regex_match function
    //    when matched groups are found
    cmatch what;
    // what[0] contains the entire matched string
    // what[1] contains the first matched group
    // what[2] contains the second matched group
    // what[3] etc.

    // regular expression to pick out the name of the element to which the new attribute is to be added,
    //    the name of the new attribute, and the value of that attribute
    regex reAddAttributeCmd("^\\s*add\\s*attribute\\s(\\w+)\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);

    // note that the following variant of the regex_match command requires a C string, not an STL string
    if (regex_match(strUserEntry.c_str(), what, reAddAttributeCmd)) {
        
        AddToDOM(strUserEntry);
    } else {
        cout << "  Invalid 'add attribute' command." << endl;
        cout << "    'add attribute' must be followed by three more parameters:" << endl;
        cout << "      (1) the name of the element to which the new attribute to be added," << endl;
        cout << "      (2) the name of the new attribute to be added, and " << endl;
        cout << "      (3) the value of the new attribute to be added." << endl;
    }
}

/**
 * Handle a print command entered by the user
 * @param strUserEntry command string entered by the user
 */
void ProcessPrintCommand(string strUserEntry) {
    
    if(aRoot == true){
    cout << endl;
    XercesDOMParser *parser = new XercesDOMParser;
    //parser->parse(doc);
    // get a serializer, an instance of DOMLSSerializer
    XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
    DOMLSSerializer *theSerializer = ((DOMImplementationLS*) impl)->createLSSerializer();
    DOMLSOutput *theOutputDesc = ((DOMImplementationLS*) impl)->createLSOutput();

    theSerializer->setNewLine(X("\n"));

    if (theSerializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
        theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }



    XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
    theOutputDesc->setByteStream(myFormTarget);
    // set user specified output encoding
    theOutputDesc->setEncoding(gOutputEncoding);

    //writes the serializer 
    theSerializer->write(doc, theOutputDesc);

    //releases both the serializer and outputdesc to prevent memory leaks/improve performance 
    theOutputDesc->release();
    theSerializer->release();
}
    else{
        cout << "please add a root!" << endl;
    }
}
/**
 * 
 * @param strUserEntry this is the string with what the user has entered to be translated with regex and added to the Dom
 */
void AddToDOM(string strUserEntry) {
    //initalize the XML platform utils
    XMLPlatformUtils::Initialize();
    cmatch what;
    regex reAddRootCmd("^\\s*add\\s*root\\s(\\w+)$", regex::icase);//add root
    regex reAddElementCmd("^\\s*add\\s*element\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);//add element
    regex reAddAttributeCmd("^\\s*add\\s*attribute\\s(\\w+)\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);//add attribute
    string myStringName;//name of string

    //regex create element command
    regex reAddTextElementCmd("^\\s*add\\s*element\\s*text\\s(\\w+)\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);
    
 //regex for attribute text
    regex reAddTextAttributeCmd("^\\s*add\\s*attribute\\s*text\\s(\\w+)\\s(\\w+)\\s(\\w+)\\s(\\w+)(.*)$", regex::icase);
    
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(X("Core"));


    if (impl != NULL) {
        //to add text into an already existing element node
        if (regex_match(strUserEntry.c_str(), what, reAddTextAttributeCmd)) {
            cout << endl;
            cout << "add attribute text";
            cout << "  You have specified that you want to add a text: " << what[4] << " to attribute named '" << what[2]
                   << "with a value of: " << what[3] << "' to parent element '" << what[1] << "'."<< endl;
            
            //go into the document, find the current element (what[2]) we want to add text to
            DOMNodeList* list = doc->getElementsByTagName(str_to_XMLCh(what[2].str()));
            //now execute setTextContent onto the node we want, adding the desired content (what[3]) 
            //into the desired line of XML
            list->item(0)->setTextContent(str_to_XMLCh(what[4].str()));
        }
        //to add text into an already existing element node
        
        
        if (regex_match(strUserEntry.c_str(), what, reAddTextElementCmd)) {
            cout << endl;
            cout << "add element text";
            cout << "  You have specified that you want to add a text: " << what[3] << " to element named '" << what[2]
                    << "' to parent element '" << what[1] << "'." << endl;
            
            
            
            //go into the document, find the current element (what[2]) we want to add text to
            DOMNodeList* list = doc->getElementsByTagName(str_to_XMLCh(what[2].str()));
            //now execute setTextContent onto the node we want, adding the desired content (what[3]) 
            //into the desired line of XML
            list->item(0)->setTextContent(str_to_XMLCh(what[3].str()));
        }
        
        
        
        //this if statement/regex combination is for adding a root
        if (regex_match(strUserEntry.c_str(), what, reAddRootCmd)) {
            cout << "Root added: " << what[1] << endl;
            myStringName = what[1];
            doc = impl->createDocument(
                    0, // root element namespace URI.
                    str_to_XMLCh(what[1].str()), // root element name ****NEED TO FIX THIS*****
                    0); // document type object (DTD).

            rootElem = doc->getDocumentElement();


        }
        //this if statement/regex combination is for adding an element, with no name.
        if (regex_match(strUserEntry.c_str(), what, reAddElementCmd)) {
            cout << "Element added: " << what[2] << endl;

            //what[2] is where the name of the element lives
            //what[1] is the name of it's parent
            myStringName = what[2];

            //Using a list is the only way I could do this whole process to work without
            //causing a seg fault
            DOMNodeList* list = doc->getElementsByTagName(str_to_XMLCh(what[1].str()));

            //create a new Element with uaer submitted namee
            DOMElement* newElem = doc->createElement(str_to_XMLCh(what[2].str()));
            //if the list is empty, there is no root function. this is a problem 
            //user must create a root element first. 
            
            
            if (list->getLength() == 0) {
                   cout << "The DomNodeList is empty. Please add a root." << endl;
            } else {
                //adds user's new element to their desired parent (item(0))
                list->item(0)->appendChild(newElem);
            }
            

        }
        //this if statement/regex combination is for adding an attribute with a value
        if (regex_match(strUserEntry.c_str(), what, reAddAttributeCmd)) {

            //what[1] is parent
            //what[2] is new attributes name
            //what[3] is the value of the new attribute
            //etc
            cout << "  You have specified that you want to add a new attribute named '" << what[2]
                << "' with a value of '" << what[3] << "' to element '" << what[1] << "'." << endl;
            cout << what[2] <<": " << what[3]<< " added." << endl;

            //Using a list is the only way I could do this whole process to work without
            //causing a seg fault
            DOMNodeList* list = doc->getElementsByTagName(str_to_XMLCh(what[1].str()));

            //create a new Attribute with uaer submitted namee
            DOMElement* newAttr = doc->createElement(str_to_XMLCh(what[2].str()));

            //adds the value to the attribute
            newAttr->setAttribute(str_to_XMLCh(what[2].str()), str_to_XMLCh(what[3].str()));

            //if the list is empty, there is no root function. this is a problem 
            //user must create a root element first. 
            if (list->getLength() == 0) {

                cout << "The DomNodeList is empty. Please add a root." << endl;
            } else {
                //adds user's new attribute to their desired parent (item(0))
                list->item(0)->appendChild(newAttr);
            }
        }
    }

}
//xml string
/**
 *@param str - string to xml
 */
XMLCh* str_to_XMLCh(string str) {
    return XMLString::transcode(str.c_str());
}

/**
 * 
 * @param strUserEntry - this is the users 
 */
void removeNode(string strUserEntry) {

    cmatch what;
    regex reRemoveCmd("^\\s*remove\\s*root\\s(\\w+)$", regex::icase);
//remove is still under construction and does not work
    if (regex_match(strUserEntry.c_str(), what, reRemoveCmd)) {
        cout << "Remove: " << what[1]<< endl;
        DOMNodeList* list = doc->getElementsByTagName(str_to_XMLCh(what[1].str()));
    }


}