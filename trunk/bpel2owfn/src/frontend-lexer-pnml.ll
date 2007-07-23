/*****************************************************************************\
 * Copyright 2006, 2007 Peter Massuthe, Dennis Reinert, Daniela Weinberg     *
 *                      Niels Lohmann, Christian Gierds                      *
 *                                                                           *
 * This file is part of PNML2oWFN.                                           *
 *                                                                           *
 * PNML2oWFN is free software; you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * PNML2oWFN is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
 * more details.                                                             *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with PNML2oWFN; if not, write to the Free Software Foundation, Inc., 51   *
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.                      *
\****************************************************************************/

%{
/*!
 * \file lexic_pnml.cc
 *
 * \brief EPNML 1.1 lexic (implementation)
 *
 * This file defines and implements the lexic of EPNML 1.1.
 *
 * The lexer has some additional "start conditions":
 *  - ATTRIBUTE to avoid lexical errors while reading an attribute
 *  - COMMENT   to merge all XML-comments
 *  - XMLHEADER
 *  - TEXT
 * 
 * \author  
 *          - responsible: Dennis Reinert <reinert@informatik.hu-berlin.de>
 *          - last changes of: \$Author: znamirow $
 *          
 * \date
 *          - created 2006/09/10
 *          - last changed: \$Date: 2007/07/23 12:43:09 $
 * 
 * \note    This file is part of the tool PNML2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 * \note    This file was created using Flex reading file lexic_pnml.ll.
 *          See http://www.gnu.org/software/flex for details.
 *
 * \version \$Revision: 1.1 $
 *
 * \todo
 */
%}

/* flex options */
%option noyywrap
%option nodefault
%option yylineno
%option debug
%option always-interactive
%option outfile="lex.yy.c"
%option prefix="frontend_pnml_"

%{
#include <cstring>	//for C strings
#include <string> 	//for C++ strings

// generated by Bison
#include "ast-config.h"		// all you need from Kimwitu++
#include "frontend-parser-pnml.h" // list of all tokens used
#include "debug.h"
#include "helpers.h"

#define YY_NO_UNPUT

using namespace std;

extern int frontend_pnml_error(const char *msg);

static void setlval();

/// Store the current start condition of the lexer to return safely after
/// comments.
int pnmlCurrentView;

// two additional views for attributes and comments
#define ATTRIBUTE 1 ///< start condition to allow lexing attributes
#define COMMENT 2   ///< start condition to allow lexing comments
#define XMLHEADER 3 ///< start condition to allow lexing xml headers
#define TEXT 4 		///< start condition to allow lexing text

%}

namestart		[A-Za-z\200-\377_]
namechar		[A-Za-z\200-\377_0-9.\-:]
name			{namestart}{namechar}*
text            [\040-\176]*
/* text            [0-9:;=\"\101-\377 \t\r\n]* */
esc				"&#"[0-9]+";"|"&#x"[0-9a-fA-F]+";"
quote			\"
string			{quote}([^"]|{esc})*{quote}
comment			([^-]|"-"[^-])*
xmlheader		([^?]|"-"[^?])*

 /* start conditions of the lexer */
%s ATTRIBUTE
%s COMMENT
%s XMLHEADER
%s TEXT

%%

 /* comments */
"!--"						{ pnmlCurrentView = YY_START; BEGIN(COMMENT); }
<COMMENT>{comment}			{ /* skip */ }
<COMMENT>"-->"[ \t\r\n]*"<"	{ BEGIN(pnmlCurrentView); }

"?xml"							{ pnmlCurrentView = YY_START; BEGIN(XMLHEADER); }
<XMLHEADER>{xmlheader}			{ /* skip */ }
<XMLHEADER>"?>"[ \t\r\n]*"<"	{ BEGIN(pnmlCurrentView); }

 /* attributes */
<ATTRIBUTE>{name}	{ setlval();
					  return X_NAME;
					}
<ATTRIBUTE>{string}	{ setlval();
					  return X_STRING;
					}
<ATTRIBUTE>"="		{ return X_EQUALS;}

 /* text */
<TEXT>">"{text}"<"	{ BEGIN(INITIAL);
					  setlval();
					  return X_TEXT;
					}

 /* XML-elements */
"<"					{ 
					return X_OPEN; 
					}
"/"					{ return X_SLASH; }
">"					{ BEGIN(INITIAL); return X_CLOSE; }
">"[ \t\r\n]*"<"	{ BEGIN(INITIAL); return X_NEXT; }


 /* names of PNML-elements */
"pnml"				{ BEGIN(ATTRIBUTE); return P_PNML; }
"net"				{ BEGIN(ATTRIBUTE); return P_NET; }
"place"				{ BEGIN(ATTRIBUTE); return P_PLACE; }
"graphics"			{ BEGIN(ATTRIBUTE); return P_GRAPHICS; }
"name"				{ BEGIN(ATTRIBUTE); return P_NAME; }
"description"		{ BEGIN(ATTRIBUTE); return P_DESCRIPTION; }
"transition"		{ BEGIN(ATTRIBUTE); return P_TRANSITION; }
"position"			{ BEGIN(ATTRIBUTE); return P_POSITION; }
"text"				{ BEGIN(TEXT); return P_TEXT; }
"initialMarking"	{ BEGIN(ATTRIBUTE); return P_INITIALMARKING; }
"dimension"			{ BEGIN(ATTRIBUTE); return P_DIMENSION; }
"arc"				{ BEGIN(ATTRIBUTE); return P_ARC; }
"inscription"		{ BEGIN(ATTRIBUTE); return P_INSCRIPTION; }
"offset"			{ BEGIN(ATTRIBUTE); return P_OFFSET; }
"referencePlace"	{ BEGIN(ATTRIBUTE); return P_REFERENCEPLACE; }
"transformation"	{ BEGIN(ATTRIBUTE); return P_TRANSFORMATION; }
"page"				{ BEGIN(ATTRIBUTE); return P_PAGE;}
"type"				{ BEGIN(ATTRIBUTE); return P_TYPE;}
"toolspecific"		{ BEGIN(ATTRIBUTE); return P_TOOLSPECIFIC; }

 /* white space */
[ \t\r\n]*			{ /* skip white space */ }
\xEF\xBB\xBF	    { /* skip UTF-8 header */ }

"="					{ return X_EQUALS;}
{string} 			{
					  setlval();;
					  return X_STRING;
					}

{name}				{
					  setlval();;
					  return X_NAME;
					}

 /* end of input file */
<<EOF>>				{ return EOF; }


 /* anything else */
.					{ frontend_pnml_error("lexical error"); }

%%
// USER CODE

// pass token string as attribute to bison
static void setlval() 
{
  frontend_pnml_lval.yt_casestring = kc::mkcasestring(frontend_pnml_text);
}

