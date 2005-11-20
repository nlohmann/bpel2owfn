/*!
 * \file main.h
 *
 * \brief Main file header
 *
 * \author  
 *          - responsible: Christian Gierds <gierds@informatik.hu-berlin.de>
 *          - last changes of: \$Author: nlohmann $
 *          
 * \date
 *          - created: 2005/11/09
 *          - last changed: \$Date: 2005/11/20 13:28:58 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.10 $
 *          - 2005-11-09 (gierds) Initial version.
 *          - 2005-11-15 (gierds) Added Exception class.
 *          - 2005-11-16 (gierds) Use of error() and cleanup() as defined in helpers.cc
 *
 */

#ifndef MAIN_H
#define MAIN_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>


#include "bpel-kc-k.h"          // generated by kimwitu++
#include "bpel-kc-rk.h"         // generated by kimwitu++
#include "bpel-kc-unpk.h"       // generated by kimwitu++
#include "bpel-unparse-xml.h"   // pretty-printers
#include "bpel-unparse-petri.h" // pretty-printers
#include "petrinet.h"           // Petri Net support
#include "debug.h"		// debugging help
#include "exception.h"		// exception handling

/* defined by Bison */
extern int yyparse();
extern int yydebug;
extern kc::tProcess TheProcess;


/* defined by flex */
extern int yy_flex_debug; ///< if set to 1 the lexer trace is printed
extern FILE *yyin;        ///< pointer to the input file


/// used to indicate trace_level
extern int debug_level;

#endif
