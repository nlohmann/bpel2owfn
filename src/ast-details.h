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
 * \file    ast-details.h
 *
 * \brief   annotations of the AST
 *
 * \author  responsible: Niels Lohmann <nlohmann@informatik.hu-berlin.de>,
 *          last changes of: \$Author: nlohmann $
 *          
 * \since   2005/07/02
 *
 * \date    \$Date: 2006/10/26 10:42:03 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.26 $
 */





#ifndef AST_DETAILS_H
#define AST_DETAILS_H





/******************************************************************************
 * Headers
 *****************************************************************************/

#include <map>
#include <string>
#include <set>

using namespace std;





/******************************************************************************
 * Data structures
 *****************************************************************************/

/*!
 * \brief enumeration of possible types of control flow
 */
typedef enum
{
  POSITIVECF = 0,	///< positive control flow
  FHCF = 1,		///< control flow in fault handler
  CHCF = 2,		///< control flow in compensation handler
  THCF = 3		///< control flow in termination handler
} controlFlowType;





/******************************************************************************
 * Class definitions
 *****************************************************************************/

/*!
 * \brief AST extensions
 */
class ASTE
{
  private:
    unsigned int id;	///< the id of the AST phylum
    unsigned int type;	///< the type of the node, identified by the keywords as parsed by Flex

  public:
    map<string, string> attributes;	///< the parsed attributes
    set<int> enclosedSourceLinks;	///< the identifiers of all (recursively) enclosed source links
    set<int> enclosedScopes;		///< the identifiers of all (direct!) enclosed scopes

    unsigned int parentScopeId;		///< the identifier of the parent scope
    unsigned int parentActivityId;	///< the identifier of the parent activity
    
    controlFlowType controlFlow;	///< the kind of control flow the activity is embedded in

    string channelName;		///< name of a channel
    string variableName;	///< name of a variable
    string inputVariableName;	///< name of an input variable
    string outputVariableName;	///< name of an output variable
    string linkName;		///< name of a link

    bool hasEH;			///< true if process or scope has an event handler
    bool hasCatchAll;		///< true if fault handler has a catchAll branch
    bool inProcess;		///< true if compensation handler is embedded to a process
    bool inWhile;		///< true if activity is embedded in a while, repeatUntil activity or in onEvent
    bool suppressJF;		///< true if join failures are suppressed
    bool exitOnStandardFault;	///< true if attribute is set to "yes" or left out and set at a parent scope

    ASTE(int myid, int mytype);

    map<string, string> getAttributes();
    string createChannel(bool synchronousCommunication = false);

    string checkVariable();
    string checkInputVariable();
    string checkOutputVariable();

    bool calculateExitOnStandardFault();
    bool calculateSuppressJoinFailure();

    void defineVariable();
    string activityTypeName();
};





#endif
