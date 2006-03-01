/*****************************************************************************\
 * Copyright 2005, 2006 Niels Lohmann, Christian Gierds, Dennis Reinert      *
 *                                                                           *
 * This file is part of BPEL2oWFN.                                           *
 *                                                                           *
 * BPEL2oWFN is free software; you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * BPEL2oWFN is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
 * more details.                                                             *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with BPEL2oWFN; if not, write to the Free Software Foundation, Inc., 51   *
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.                      *
\****************************************************************************/

/*!
 * \file bpel-attributes.h
 *
 */


#ifndef BPEL_ATTRIBUTES_H
#define BPEL_ATTRIBUTES_H


// generated by Kimwitu++
#include "bpel-kc-k.h" // phylum definitions
#include "bpel-kc-yystype.h" // data types for tokens and non-terminals

#include <map>
#include <stack>

using namespace std;

/// BPEL attribute names
const string A__CONDITION = "condition";
const string A__ENDPOINT_REFERENCE = "endpointReference";
const string A__EXPRESSION = "expression";
const string A__EXPRESSION_LANGUAGE = "expressionLanguage";
const string A__FAULT_NAME = "faultName";
const string A__FOR = "for";
const string A__INPUT_VARIABLE = "inputVariable";
const string A__LINK_NAME = "linkName";
const string A__NAME = "name";
const string A__OPERATION = "operation";
//const string A__OUTPUT_VARIABLE = "outputVariable";
const string A__PART = "part";
const string A__PARTNER_LINK = "partnerLink";
const string A__PARTNER_LINK_TYPE = "partnerLinkType";
const string A__PORT_TYPE = "portType";
const string A__PROPERTY = "property";
const string A__PROPERTIES = "properties";
const string A__QUERY = "query";
const string A__QUERY_LANGUAGE = "queryLanguage";
const string A__SET = "set";
const string A__TARGET_NAMESPACE = "targetNamespace";
const string A__TRANSITION_CONDITION = "transitionCondition";
const string A__UNTIL = "until";
const string A__VARIABLE = "variable";
const string A__XMLNS = "xmlns";

/// yes or no

const string A__OPAQUE = "opaque";

const string A__ABSTRACT_PROCESS = "abstractProcess";
const string A__CREATE_INSTANCE = "createInstance";
const string A__ENABLE_INSTANCE_COMPENSATION = "enableInstanceCompensation";
const string A__INITIATE = "initiate";
const string A__SUPPRESS_JOIN_FAILURE = "suppressJoinFailure";
const string A__VARIABLE_ACCESS_SERIALIZABLE = "variableAccessSerializable";

/// types of BPEL attributes
const string T__NCNAME = "NCName";
const string T__QNAME = "QName";
const string T__ANYURI = "anyURI";
const string T__DURATION_EXPR = "bpws:tDuration-expr";
const string T__DEADLINE_EXPR = "bpws:tDeadline-expr";
const string T__BOOLEAN_EXPR = "bpws:tBoolean-expr";

/// default values of attributes
const string DV__QUERY_LANGUAGE = "http://www.w3.org/TR/1999/REC-xpath-19991116";
const string DV__EXPRESSION_LANGUAGE = "http://www.w3.org/TR/1999/REC-xpath-19991116";
const string DV__SUPPRESS_JOIN_FAILURE = "no";
const string DV__ENABLE_INSTANCE_COMPENSATION = "no";
const string DV__ABSTRACT_PROCESS = "no";
const string DV__XMLNS = "http://schemas.xmlsoap.org/ws/2003/03/business-process/";
const string DV__INITIATE = "no";
const string DV__CREATE_INSTANCE = "no";
const string DV__VARIABLE_ACCESS_SERIALIZABLE = "no";
const string DV__TRANSITION_CONDITION = "true";


class SJFStackElement
{
  private:
	
	///
	kc::integer elementId;
	
	///
	kc::casestring sjfValue;
  
  public:
	/// constructor
	SJFStackElement(kc::integer elementId, kc::casestring value) {setElementId(elementId); setSJFValue(value);}
	
	///
	void setElementId(kc::integer elementId);
	void setSJFValue(kc::casestring value);
	
	///
	kc::integer getElementId();
	kc::casestring getSJFValue();

};

class attributeManager
{
  private:
    
    /// an array to store attributes of XML-elements
    /// <bpel-element-id, <attribute name, attribute value>>
    std::map<unsigned int, std::map<std::string, std::string> > scannerResult;
	
	///
	std::stack<SJFStackElement*> SJFStack;
	
	/// print formatted attribute manager error message
	void printErrorMsg(std::string errorMsg);

	/// domain check
	void checkAttributeValueYesNo(std::string attributeName, std::string attributeValue);	
	
	/// returns valid or unvalid depending on attribute value
	bool isValidAttributeValue(std::string attributeName, std::string attributeValue);
	
	/// returns valid or unvalid depending on attribute already exists or not
	bool isDuplicate(std::string attributeName);
  
  public:
    /// constructor
    attributeManager();

    /// consecutive enumeration of XML-elements
    unsigned int nodeId;
	
	///
	bool isAttributeValueEmpty(kc::integer elementId, std::string attributeName);
	
	///
	void pushSJFStack(kc::integer elementId, kc::casestring attributeValue);
	
	///
	void popSJFStack();
	
	///
	SJFStackElement topSJFStack();
	
	///
	bool emptySJFStack();
	
	/// AM traces
	void traceAM(std::string traceMsg);

    /// returns the value of an attribute
    kc::casestring read(kc::integer elementId, std::string attributeName);
    
    /// returns the value of an attribute
    kc::casestring read(kc::integer elementId, std::string attributeName, kc::casestring defaultValue);    

    /// increases the id
    kc::integer nextId();

    /// defines an attribute
    void define(kc::casestring attributeName, kc::casestring attributeValue);

    /// checked the attributes of BPEL-elements
    void check(kc::integer elementId, unsigned int elementType);
    
    /// checked the attributes and the value of BPEL-elements
    void check(kc::integer elementId, kc::casestring elementValue, unsigned int elementType);
};


/*
    list of default values of attributes:
    -------------------------------------
    tProcess.queryLanguage = http://www.w3.org/TR/1999/REC-xpath-19991116
            .expressionLanguage = http://www.w3.org/TR/1999/REC-xpath-19991116
            .suppressJoinFailure = no
            .enableInstanceCompensation = no
            .abstractProcess = no

    tActivity.suppressJoinFailure = no

    tCorrelation.initiate = no

    tReceive.createInstance = no

    tPick.createInstance = no

    tScope.variableAccessSerializable = no
*/

/*
    list of data types of attributes:
    ---------------------------------
    tBoolean-expr
      tActivity.joinCondition
      tSource.transitionCondition
      tCase.condition
      tWhile.condition

    tDuration-expr
      tOnAlarm.for
      tWait.for

    tDeadline-expr
      tOnAlarm.until
      tWait.until

    tBoolean (yes|no)
      tProcess.suppressJoinFailure, tProcess.enableInstanceCompensation, tProcess.abstractProcess
      tActivity.suppressJoinFailure
      tCorrelation.initiate
      tReceive.createInstance
      tFrom.opaque
      tTo.opaque
      tPick.createInstance
      tScope.variableAccessSerializable

    tRoles (myRole|partnerRole)
      tFrom.endpointReference
      tTo.endpointReference
*/

#endif
