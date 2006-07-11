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
 * \file main.cc
 *
 * \brief The control component for BPEL2oWFN
 *
 * This file controls the behaviour of BPEL2oWFN and is the interface to the 
 * environment.
 * 
 * \author  
 *          - responsible: Christian Gierds <gierds@informatik.hu-berlin.de>
 *          - last changes of: \$Author: nlohmann $
 *          
 * \date
 *          - created: 2005/10/18
 *          - last changed: \$Date: 2006/07/11 21:17:26 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.92 $
 *
 */


#include "main.h"
#include "options.h"
#include "ast-printers.h"
#include <assert.h>

/// The Petri Net
PetriNet *TheNet = new PetriNet();






void petrinet_unparse()
{
  trace(TRACE_INFORMATION, "-> Unparsing AST to Petri net ...\n");
  
  if (parameters[P_COMMUNICATIONONLY] == false)
    TheProcess->unparse(kc::pseudoPrinter, kc::petrinet);
  else
    TheProcess->unparse(kc::pseudoPrinter, kc::petrinetsmall);
}





/**
 * Entry point for BPEL2oWFN.
 * Controls the behaviour of input and output.
 *
 * \param argc	number of command line arguments
 * \param argv	array with command line arguments
 *
 * \returns Error code (0 if everything went well)
 */
int main( int argc, char *argv[])
{
//  try
//  {
    /***
     * Reading command line arguments and triggering appropriate behaviour.
     * In case of false parameters call command line help function and exit.
     */
    parse_command_line(argc, argv);
  
    PetriNet *TheNet2 = new PetriNet();
    assert(TheNet2 != NULL);

    list< std::string >::iterator file = inputfiles.begin();

    do {
      if (inputfiles.size() >= 1)
      {
	filename = *file;
	if (!(yyin = fopen(filename.c_str(), "r")))
	{
		/* TODO generate message: file not found */
//	  throw Exception(FILE_NOT_FOUND, "File '" + filename + "' not found.\n");
	}
      }
      trace(TRACE_INFORMATION, "Parsing " + filename + " ...\n");
  
      // invoke Bison parser
      int error = yyparse();

      if (!error)
      {
	trace(TRACE_INFORMATION, "Parsing complete.\n");

	if ( filename != "<STDIN>" && yyin != NULL)
	{
	  trace(TRACE_INFORMATION," + Closing input file: " + filename + "\n");
	  fclose(yyin);
	}
 
	trace(TRACE_INFORMATION, "Postprocessing...\n");
	TheProcess->unparse(kc::printer, kc::postprocessing);
	trace(TRACE_INFORMATION, "Postprocessing complete...\n");

	if (modus == M_CFG)
	  processCFG();



	if (modus == M_PETRINET || modus == M_CONSISTENCY)
	{
	  petrinet_unparse();
    
	  if (modus == M_CONSISTENCY)
	  {
            unsigned int pos = file->rfind(".bpel", file->length());
	    unsigned int pos2 = file->rfind("/", file->length());
	    std::string prefix = "";
	    if (pos == (file->length() - 5))
	    {
	      prefix = file->substr(pos2 + 1, pos - pos2 - 1) + "_";
	    }
	    if ( parameters[P_SIMPLIFY] )
	    {
	      trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
	      TheNet->simplify();
	    }

	    TheNet->addPrefix(prefix);

	    TheNet2->connectNet(TheNet);

	    TheNet = new PetriNet();
	    assert(TheNet != NULL);

	    TheProcess = NULL;
	  }
	}

      }
      else
      {
	cleanup();  
	return error;
      }

      file++;
    } while (modus == M_CONSISTENCY && file != inputfiles.end());



    if (modus == M_CONSISTENCY)
    {
      TheNet = TheNet2;
    }



    if (modus == M_AST)
    {
      trace(TRACE_INFORMATION, "-> Printing AST ...\n");
      TheProcess->print();
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
        TheProcess->unparse(kc::printer, kc::xml);
        if (output_filename != "")
        {
          closeOutput(output);
          output = NULL;
        }
      }
    }



    if (modus == M_PETRINET || modus == M_CONSISTENCY)
    {

      // remove variables?
      if ( parameters[P_NOVARIABLES] )
      {
 	trace(TRACE_INFORMATION, "-> Remove variable places from Petri Net ...\n");
        TheNet->removeVariables();
      }    
      // simplify net ?
      if ( parameters[P_SIMPLIFY] )
      {
 	trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
        TheNet->simplify();
      }

      // now the net will not change any more (hopefully...)
      TheNet->reenumerate(); 

      // create oWFN output ?
      if ( formats[F_OWFN] )
      {
        if (output_filename != "")
	{
          output = openOutput(output_filename + "." + suffixes[F_OWFN]);
    	}
 	trace(TRACE_INFORMATION, "-> Printing Petri net for oWFN ...\n");
        TheNet->owfnOut();
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
	  TheNet->makeChannelsInternal();
	}

	trace(TRACE_INFORMATION, "-> Printing Petri net for LoLA ...\n");
        TheNet->lolaOut();
	if (output_filename != "")
	{
	  closeOutput(output);
	  output = NULL;
	}

	if (modus == M_CONSISTENCY)
	{
	  if (output_filename != "")
	  {
	    output = openOutput(output_filename + ".task");
	  }
	  std::string comment = "{ AG EF (";
	  std::string formula = "FORMULA\n  ALLPATH ALWAYS EXPATH EVENTUALLY (";
	  std::string andStr = "";
	  for (list< std::string >::iterator file = inputfiles.begin(); file != inputfiles.end(); file++)
	  {
            unsigned int pos = file->rfind(".bpel", file->length());
	    unsigned int pos2 = file->rfind("/", file->length());
	    std::string prefix = "";
	    if (pos == (file->length() - 5))
	    {
	      prefix = file->substr(pos2 + 1, pos - pos2 - 1) + "_";
	    }
	    comment += andStr + prefix + "1.internal.final";
	    formula += andStr + TheNet->findPlace(prefix + "1.internal.final")->nodeShortName() + " > 0";
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
        TheNet->pnmlOut();
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
        TheNet->pepOut();
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
        TheNet->apnnOut();
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
        TheNet->dotOut();
	if (output_filename != "")
	{
	  closeOutput(output);
	  output = NULL;
	  std::string systemcall = "dot -q -Tpng -o" + output_filename + ".png " + output_filename + "." + suffixes[F_DOT];
	  trace(TRACE_INFORMATION, "Invoking dot with the following options:\n");
	  trace(TRACE_INFORMATION, systemcall + "\n\n");
	  system(systemcall.c_str());
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
        TheNet->printInformation();
	if (output_filename != "")
	{
	  closeOutput(output);
	  output = NULL;
	}
      }

      // a hack for Turku: show that our tool exists
      trace(TheNet->information() + "\n");
    }
//  }
//  catch (Exception& e)
//  {
//    // output info file ?
//    if ( formats[F_INFO] )
//    {
//      output = log_output;
//      // just print to std::out
//      TheNet->printInformation();
//      output = NULL;
//    }
//    error(e);
//  }

  return 0;  
}
