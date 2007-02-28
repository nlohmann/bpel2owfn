/*****************************************************************************\
 * Copyright 2005, 2006 Niels Lohmann, Christian Gierds                      *
 *                                                                           *
 * This file is part of GNU BPEL2oWFN.                                       *
 *                                                                           *
 * GNU BPEL2oWFN is free software; you can redistribute it and/or modify it  *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * GNU BPEL2oWFN is distributed in the hope that it will be useful, but      *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General  *
 * Public License for more details.                                          *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with GNU BPEL2oWFN; see file COPYING. if not, write to the Free Software  *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA. *
\*****************************************************************************/

/*!
 * \file    bpel2owfn.cc
 *
 * \brief   BPEL2oWFN's main
 *
 * \author  Niels Lohmann <nlohmann@informatik.hu-berlin.de>,
 *          Christian Gierds <gierds@informatik.hu-berlin.de>,
 *          last changes of: \$Author: gierds $
 *
 * \since   2005/10/18
 *
 * \date    \$Date: 2007/02/28 09:56:23 $
 *
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.144 $
 */





/******************************************************************************
 * Headers
 *****************************************************************************/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "bpel2owfn.h"          // generated configuration file

#include "petrinet.h"           // Petri Net support
#include "cfg.h"		// Control Flow Graph
#include "debug.h"		// debugging help
#include "options.h"
#include "ast-config.h"

using std::cerr;
using std::endl;





/******************************************************************************
 * External functions
 *****************************************************************************/

extern int frontend_parse();			// from Bison
extern int frontend_debug;			// from Bison
extern int frontend__flex_debug;		// from flex
extern FILE *frontend_in;			// from flex
extern kc::tProcess AST;





/******************************************************************************
 * Global variables
 *****************************************************************************/

/// The Petri Net
PetriNet PN = PetriNet();

/// string holding the invocation of BPEL2oWFN
string invocation;

/// string holding the called program name of BPEL2oWFN
string program_name;





/*!
 * \brief entry point of BPEL2oWFN
 *
 * Controls the behaviour of input and output.
 *
 * \param argc	number of command line arguments
 * \param argv	array with command line arguments
 *
 * \returns 0 if everything went well
 * \returns 1 if an error occurred
 *
 * \todo This function is definitly too long. It should be partitioned!
 */
int main( int argc, char *argv[])
{
  program_name = string(argv[0]);

  // generate the invocation string
  for (int i = 0; i < argc; i++)
  {
    invocation += string(argv[i]);
    if (i != (argc-1))
      invocation += " ";
  }

  /*
   * Reading command line arguments and triggering appropriate behaviour.
   * In case of false parameters call command line help function and exit.
   */
  parse_command_line(argc, argv);

  PetriNet PN2 = PetriNet();

  list< string >::iterator file = inputfiles.begin();

  do
  {
    if (inputfiles.size() >= 1)
    {
      filename = *file;
      if (!(frontend_in = fopen(filename.c_str(), "r"))) {
		cerr << "Could not open file for reading: " << filename.c_str() << endl;
		exit(1);
	    }
    }

    trace(TRACE_INFORMATION, "Parsing " + filename + " ...\n");

    // invoke Bison parser
    int error = frontend_parse();

    if (!error)
    {
      trace(TRACE_INFORMATION, "Parsing complete.\n");

      if ( filename != "<STDIN>" && frontend_in != NULL)
      {
	trace(TRACE_INFORMATION," + Closing input file: " + filename + "\n");
	fclose(frontend_in);
      }


      // apply first set of rewrite rules
      trace(TRACE_INFORMATION, "Rewriting...\n");
      AST = AST->rewrite(kc::invoke);
      AST = AST->rewrite(kc::implicit);
      trace(TRACE_INFORMATION, "Rewriting complete...\n");

      // postprocess and annotate the AST
      trace(TRACE_INFORMATION, "Postprocessing...\n");
      AST->unparse(kc::printer, kc::postprocessing);
      trace(TRACE_INFORMATION, "Postprocessing complete...\n");

      // apply second set of rewrite rules
      trace(TRACE_INFORMATION, "Rewriting 2...\n");
      AST = AST->rewrite(kc::newNames);
      trace(TRACE_INFORMATION, "Rewriting 2 complete...\n");


      // print the AST?
      if (modus == M_AST)
      {
	trace(TRACE_INFORMATION, "-> Printing AST ...\n");
	if (formats[F_DOT])
	{
	  string dot_filename = output_filename + "." + suffixes[F_DOT];
	  FILE *dotfile = fopen(dot_filename.c_str(), "w+");
	  AST->fprintdot(dotfile, "", "", "", true, true, true);
	  fclose(dotfile);
#ifdef HAVE_DOT
  	  string systemcall = "dot -q -Tpng -o" + output_filename + ".png " + output_filename + "." + suffixes[F_DOT];
  	  trace(TRACE_INFORMATION, "Invoking dot with the following options:\n");
  	  trace(TRACE_INFORMATION, systemcall + "\n\n");
  	  system(systemcall.c_str());
#endif
	}
	else
	  AST->print();
      }

      if (modus == M_PRETTY)
      {
        if (formats[F_XML])
        {
          if (output_filename != "")
          {
            output = openOutput(output_filename + "." + suffixes[F_XML]);
          }
          trace(TRACE_INFORMATION, "-> Printing \"pretty\" XML ...\n");
          AST->unparse(kc::printer, kc::xml);
          if (output_filename != "")
          {
            closeOutput(output);
	    output = NULL;
          }
        }
      }

        // generate and process the control flow graph?
      if (modus == M_CFG)
	processCFG();


      // generate a Petri net?
      if (modus == M_PETRINET || modus == M_CONSISTENCY)
      {
	trace(TRACE_INFORMATION, "-> Unparsing AST to Petri net ...\n");

	// choose Petri net patterns
	if (parameters[P_COMMUNICATIONONLY] == true)
	  AST->unparse(kc::pseudoPrinter, kc::petrinetsmall);
	else if (parameters[P_WSBPEL] == true)
	  AST->unparse(kc::pseudoPrinter, kc::petrinetnew);
	else
	  AST->unparse(kc::pseudoPrinter, kc::petrinet);

        // calculate maximum occurences
        PN.calculate_max_occurrences();

        if (modus == M_CONSISTENCY)
	{
	  unsigned int pos = file->rfind(".bpel", file->length());
	  unsigned int pos2 = file->rfind("/", file->length());
	  string prefix = "";
	  if (pos == (file->length() - 5))
	  {
	    prefix = file->substr(pos2 + 1, pos - pos2 - 1) + "_";
	  }

	  // apply structural reduction rules?
	  if (parameters[P_REDUCE])
	  {
	    trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
	    PN.reduce();
	  }

	  PN.addPrefix(prefix);
	  PN2.compose(PN);
	  PN = PetriNet();

	  AST = NULL;
	}
      }
    }
    else /* parse error */
    {
      cleanup();
      return error;
    }


    file++;
  } while (modus == M_CONSISTENCY && file != inputfiles.end());



  /*
   * parsing complete
   */

  if (modus == M_CONSISTENCY)
    PN = PN2;

  if (modus == M_PETRINET || modus == M_CONSISTENCY)
  {
    // apply structural reduction rules?
    if ( parameters[P_REDUCE] )
    {
      trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
      PN.reduce();
    }


    // now the net will not change any more, thus the nodes are re-enumerated
    // and the maximal occurrences of the nodes are calculated.
    PN.reenumerate();
//    PN.calculate_max_occurrences();
    cerr << PN.information() << endl;


    // create oWFN output ?
    if (formats[F_OWFN])
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_OWFN]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for oWFN ...\n");
      PN.set_format(FORMAT_OWFN);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }


    // create LoLA output ?
    if ( formats[F_LOLA] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_LOLA]);
      }
      if (modus == M_CONSISTENCY)
      {
	PN.makeChannelsInternal();
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for LoLA ...\n");
      PN.set_format(FORMAT_LOLA);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }

      if (modus == M_CONSISTENCY || modus == M_PETRINET)
      {
	if (output_filename != "")
	{
	  output = openOutput(output_filename + ".task");
	}
	string comment = "{ AG EF (";
	string formula = "FORMULA\n  ALLPATH ALWAYS EXPATH EVENTUALLY (";
	string andStr = "";

        set< Place * > finalPlaces = PN.getFinalPlaces();
	for (set< Place * >::iterator place = finalPlaces.begin(); place != finalPlaces.end(); place++)
	{
	  comment += andStr + (*place)->nodeFullName();
	  formula += andStr + (*place)->nodeShortName() + " > 0";
       	  andStr = " AND ";
	}
	comment += ") }";
	formula += ")";
	(*output) << comment << endl << endl;
	(*output) << formula << endl << endl;
	if (output_filename != "")
	{
	  closeOutput(output);
	  output = NULL;
	}
      }
    }


    // create PNML output ?
    if ( formats[F_PNML] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_PNML]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for PNML ...\n");
      PN.set_format(FORMAT_PNML);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }


    // create PEP output ?
    if ( formats[F_PEP] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_PEP]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for PEP ...\n");
      PN.set_format(FORMAT_PEP);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }


    // create INA output ?
    if ( formats[F_INA] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_INA]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for INA ...\n");
      PN.set_format(FORMAT_INA);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }

    // create SPIN output ?
    if ( formats[F_SPIN] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_SPIN]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for SPIN ...\n");
      PN.set_format(FORMAT_SPIN);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }

    // create APNN output ?
    if ( formats[F_APNN] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_APNN]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for APNN ...\n");
      PN.set_format(FORMAT_APNN);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }


    // create dot output ?
    if ( formats[F_DOT] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_DOT]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net for dot ...\n");
      PN.set_format(FORMAT_DOT);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;

#ifdef HAVE_DOT
	string systemcall = "dot -q -Tpng -o" + output_filename + ".png " + output_filename + "." + suffixes[F_DOT];
	trace(TRACE_INFORMATION, "Invoking dot with the following options:\n");
	trace(TRACE_INFORMATION, systemcall + "\n\n");
	system(systemcall.c_str());
#endif
      }
    }


    // create info file ?
    if ( formats[F_INFO] )
    {
      if (output_filename != "")
      {
	output = openOutput(output_filename + "." + suffixes[F_INFO]);
      }
      trace(TRACE_INFORMATION, "-> Printing Petri net information ...\n");
      PN.set_format(FORMAT_INFO);
      (*output) << PN;
      if (output_filename != "")
      {
	closeOutput(output);
	output = NULL;
      }
    }
  }


  // everything went fine
  return 0;
}





/*!
 * \defgroup frontend Front End
 */
