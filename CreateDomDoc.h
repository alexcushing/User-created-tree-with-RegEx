/* 
 * File:   CreateDomDoc.h
 * Author: Alex Cushing
 * Email: acushing@cs.uml.edu
 * Created: 12/6/15
 * Edited 12/9
 */
#ifndef CREATEDOMDOC_H
#define	CREATEDOMDOC_H

/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: CreateDOMDocument.cpp 676796 2008-07-15 05:04:13Z dbertoni $
 */

/*
 * This sample illustrates how you can create a DOM tree in memory.
 * It then prints the count of elements in the tree.
 */


// ---------------------------------------------------------------------------
//  Includes from DOMPrint-listing.pdf
// ---------------------------------------------------------------------------
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMTreeWalker.hpp>

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif

XERCES_CPP_NAMESPACE_USE

/**
*  Simple class used to convert from char* data into XMLCh data. Taken from the example
* found at:
* https://xerces.apache.org/xerces-c/createdoc-3.html
*/
class XStr {
public:
        /**
        * @name CONSTRUCTOR
        * @param toTransCode - string to be converted
        * @return - string converted to XMLCh data
        */
  XStr(const char* const toTranscode);
  
        /**
        * @name DESTRUCTOR
        * @purpose properly release fUnicodeForm
        */
  ~XStr();

        /**
        * @purpose GETTER function to retrieve XMLCh
        * @return - fUnicodeForm
        */
  const XMLCh* unicodeForm() const;

private:
  /**
   * @name fUnicodeForm
   * @purpose Unicode format for the XML string 
   */
  XMLCh* fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()

/**
 * This is a class that create a tree of elements and output it in different formats.
 * Note: This class has no private members and creates objects from other classes.
 */
class CreateDOMDocument {
public:

private:

};

#endif	/* CREATEDOMDOCUMENT_H */

