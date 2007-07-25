/*****************************************************************************\
  GNU BPEL2oWFN -- Translating BPEL Processes into Petri Net Models

  Copyright (C) 2006, 2007  Niels Lohmann,
                            Christian Gierds, and
                            Martin Znamirowski
  Copyright (C) 2005        Niels Lohmann and
			    Christian Gierds

  GNU BPEL2oWFN is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 3 of the License, or (at your option) any
  later version.

  GNU BPEL2oWFN is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  GNU BPEL2oWFN (see file COPYING); if not, see http://www.gnu.org/licenses
  or write to the Free Software Foundation,Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

%{
/*!
 * \file    frontend-parser-chor.cc
 *
 * \brief   BPEL4Chor topology parser
 *
 * \author  Niels Lohmann <nlohmann@informatik.hu-berlin.de>,
 *          last changes of: \$Author: znamirow $
 *
 * \since   2007/04/29
 *
 * \date    \$Date: 2007/07/25 09:28:45 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universit�t zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 * \note    This file was created using GNU Bison reading file
 *          frontend-parser-chor.yy.
 *          See http://www.gnu.org/software/bison/bison.html for details
 *
 * \version \$Revision: 1.16 $
 *
 * \ingroup frontend
 */
%}


/******************************************************************************
 * Terminal symbols (tokens).
 *****************************************************************************/

%token APOSTROPHE EQUAL GREATER GREATEROREQUAL K_AND K_ASSIGN K_BRANCHES K_CASE
%token K_CATCH K_CATCHALL K_COMPENSATE K_COMPENSATESCOPE K_COMPENSATIONHANDLER
%token K_COMPLETIONCONDITION K_CONDITION K_COPY K_CORRELATION K_CORRELATIONS
%token K_CORRELATIONSET K_CORRELATIONSETS K_ELSE K_ELSEIF K_EMPTY
%token K_EVENTHANDLERS K_EXIT K_EXTENSION K_EXTENSIONACTIVITY
%token K_EXTENSIONASSIGNOPERATION K_EXTENSIONS K_FAULTHANDLERS
%token K_FINALCOUNTERVALUE K_FLOW K_FOR K_FOREACH K_FROM K_FROMPART K_FROMPARTS
%token K_GETLINKSTATUS K_IF K_IMPORT K_INVOKE K_JOINCONDITION K_LINK K_LINKS
%token K_LITERAL K_MESSAGEEXCHANGE K_MESSAGEEXCHANGES K_ONALARM K_ONEVENT
%token K_ONMESSAGE K_OPAQUEACTIVITY K_OPAQUEFROM K_OR K_OTHERWISE K_PARTNER
%token K_PARTNERLINK K_PARTNERLINKS K_PARTNERS K_PICK K_PROCESS K_QUERY K_RECEIVE
%token K_REPEATEVERY K_REPEATUNTIL K_REPLY K_RETHROW K_SCOPE K_SEQUENCE
%token K_SOURCE K_SOURCES K_STARTCOUNTERVALUE K_SWITCH K_TARGET K_TARGETS
%token K_TERMINATE K_TERMINATIONHANDLER K_THROW K_TO K_TOPART K_TOPARTS
%token K_TRANSITIONCONDITION K_UNTIL K_VALIDATE K_VARIABLE K_VARIABLES K_WAIT
%token K_WHILE LBRACKET LESS LESSOREQUAL NOTEQUAL RBRACKET X_CLOSE X_EQUALS
%token X_NEXT X_OPEN X_SLASH
%token K_TOPOLOGY K_PARTICIPANTTYPES K_PARTICIPANTTYPE K_PARTICIPANTS K_PARTICIPANT K_PARTICIPANTSET K_MESSAGELINKS K_MESSAGELINK
%token K_TYPES K_PORTTYPE K_FAULT K_OPERATION K_DEFINITIONS K_OUTPUT K_INPUT K_MESSAGE K_PART K_BINDING K_SERVICE K_PORT
%token K_PARTNERLINKTYPE K_ROLE K_SCHEMA K_PROPERTY K_PROPERTYALIAS
%token P_NET P_PLACE P_GRAPHICS P_NAME P_DESCRIPTION P_TRANSITION P_PAGE
%token P_POSITION P_TEXT P_INITIALMARKING P_DIMENSION P_PNML
%token P_ARC P_INSCRIPTION P_OFFSET P_REFERENCEPLACE P_TYPE P_TRANSFORMATION
%token P_TOOLSPECIFIC
%token <yt_casestring> NUMBER
%token <yt_casestring> X_NAME
%token <yt_casestring> VARIABLENAME
%token <yt_casestring> X_STRING
%token <yt_casestring> X_TEXT

// OR and AND are left-associative.
%left K_OR
%left K_AND

// The start symbol of the grammar.
%start tTopology



/******************************************************************************
 * Bison options.
 *****************************************************************************/

// Bison generates a list of all used tokens in file "frontend-parser.h" (for
// Flex).
%token_table

%yacc

// We know what we are doing. Abort if any shift/reduce or reduce/reduce errors
// arise.
%expect 0

// All "yy"-prefixes are replaced with "frontend_chor_".
%name-prefix="frontend_chor_"





%{
#include <cassert>
#include <map>

#include "ast-config.h"
#include "helpers.h"
#include "globals.h"
#include "extension-chor.h"


using std::cerr;
using std::endl;



/******************************************************************************
 * External variables
 *****************************************************************************/

extern int frontend_lex();	// from flex: the lexer funtion
extern int frontend_lineno;

// use the functions of the BPEL parser
#define frontend_chor_lex() frontend_lex()
#define frontend_chor_error(a) frontend_error(a)
#define frontend_chor_in frontend_in
%}



%%



tTopology:
  X_OPEN K_TOPOLOGY arbitraryAttributes X_NEXT tParticipantTypes tParticipants tMessageLinks X_SLASH K_TOPOLOGY X_CLOSE
    { globals::BPEL4ChorInfo.print_information(); }
;


/******************************************************************************
 * PARTICIPANT TYPES
 *****************************************************************************/

tParticipantTypes:
  K_PARTICIPANTTYPES X_NEXT tParticipantType_list X_NEXT X_SLASH K_PARTICIPANTTYPES X_NEXT
;

tParticipantType_list:
  tParticipantType
| tParticipantType_list X_NEXT tParticipantType
;

tParticipantType:
  K_PARTICIPANTTYPE arbitraryAttributes X_SLASH
    { globals::BPEL4ChorInfo.add_participantType(globals::tempAttributes); }
;


/******************************************************************************
 * PARTICIPANTS
 *****************************************************************************/

tParticipants:
  K_PARTICIPANTS X_NEXT tParticipant_list X_NEXT X_SLASH K_PARTICIPANTS X_NEXT
;

tParticipant_list:
  tParticipant
| tParticipantSet
| tParticipant_list X_NEXT tParticipant
| tParticipant_list X_NEXT tParticipantSet
;

tParticipant:
  K_PARTICIPANT arbitraryAttributes X_NEXT X_SLASH K_PARTICIPANT
    { globals::BPEL4ChorInfo.add_participant(globals::tempAttributes); }
| K_PARTICIPANT arbitraryAttributes X_SLASH
    { globals::BPEL4ChorInfo.add_participant(globals::tempAttributes); }
;

tParticipantSet:
  K_PARTICIPANTSET arbitraryAttributes
    { globals::BPEL4ChorInfo.start_participantSet(globals::tempAttributes); }
  X_NEXT tParticipant_list X_NEXT X_SLASH K_PARTICIPANTSET
    { globals::BPEL4ChorInfo.end_participantSet(); }
| K_PARTICIPANTSET arbitraryAttributes X_SLASH /* added later */
;


/******************************************************************************
 * MESSAGELINKS
 *****************************************************************************/

tMessageLinks:
  K_MESSAGELINKS X_NEXT tMessageLink_list X_NEXT X_SLASH K_MESSAGELINKS X_NEXT
;

tMessageLink_list:
  tMessageLink
| tMessageLink_list X_NEXT tMessageLink
;

tMessageLink:
  K_MESSAGELINK { globals::tempAttributes.clear(); } arbitraryAttributes X_SLASH
    { globals::BPEL4ChorInfo.add_messageLink(globals::tempAttributes);
      globals::tempAttributes.clear(); }
;


/******************************************************************************
 * ATTRIBUTES
 *****************************************************************************/

arbitraryAttributes:
  /* empty */
| X_NAME X_EQUALS X_STRING arbitraryAttributes
    { globals::tempAttributes[strip_namespace($1->name)] = strip_namespace($3->name); }
;
