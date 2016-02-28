/* 
 * File:   CreateDomDoc.cpp
 * Author: Alex Cushing
 * Email: acushing@cs.uml.edu
 * Created: 12/6/15
 * Edited 12/9
 */

#include "CreateDomDoc.h"

using namespace std;
/*
 * Global variables for easy access 
 */
static char* gXPathExpression = 0;
static XMLCh* gOutputEncoding = 0;
static bool gUseFilter = false;
static char* goutputfile = 0;
static bool gFormatPrettyPrint = true;
static bool gSplitCdataSections = true;
static bool gDiscardDefaultContent = true;
static bool gWriteBOM = false;

/*
 * Xstr constructors
 */
XStr::XStr(const char* const toTranscode) {
    // Call the private transcoding method
    fUnicodeForm = XMLString::transcode(toTranscode);
}
/**
 * 
 * @return  returns xstr
 * see main.cpp for documentation of full function
 */
XStr::~XStr() {
    XMLString::release(&fUnicodeForm);
}

/**
 * @purpose GETTER function to retrieve XMLCh
 * @return - fUnicodeForm
 */
const XMLCh* XStr::unicodeForm() const {
    return fUnicodeForm;
}