/*****************************************************************************\
 * Copyright 2006 Niels Lohmann                                              *
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
 * \file    petrinet-output.cc
 *
 * \brief   Petri Net API: file output
 * 
 * \author  Niels Lohmann <nlohmann@informatik.hu-berlin.de>,
 *          last changes of: \$Author: nielslohmann $
 *
 * \since   created: 2006-03-16
 *
 * \date    \$Date: 2006/12/30 00:53:04 $
 *
 * \note    This file is part of the tool GNU BPEL2oWFN and was created during
 *          the project Tools4BPEL at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 * \version \$Revision: 1.70 $
 *
 * \ingroup petrinet
 */





/******************************************************************************
 * Headers
 *****************************************************************************/

#include <iomanip>

#include "petrinet.h"		// to define member functions
#include "helpers.h"		// helper functions
#include "options.h"

#ifdef USING_BPEL2OWFN
#include "bpel2owfn.h"
#include "ast-details.h"
#endif

#ifndef USING_BPEL2OWFN
#include "bpel2owfn_wrapper.h"
#endif





/******************************************************************************
 * Functions to print information of the net and its nodes
 *****************************************************************************/

/*!
 * \return the name of the node type
 */
string Node::nodeTypeName() const
{
  return "";
}





/*!
 * \return the name of the node type
 */
string Place::nodeTypeName() const
{
  return "place";
}





/*!
 * \return the name of the node type
 */
string Transition::nodeTypeName() const
{
  return "transition";
}





/*!
 * dummy-implementation of virtual function
 */
string Node::nodeShortName() const
{
  return "";
}





/*!
 * \return the (nice) name of the node for DOT output
 */
string Node::nodeName() const
{
  string result = history[0];

  if ( type == INTERNAL || nodeType == TRANSITION )
  {
    result = result.substr(result.find_last_of(".")+1,result.length());
    result = prefix + result;
  }

  return result;
}


/*!
 * \return the name of the node for DOT output
 */
string Node::nodeFullName() const
{
  string result = history[0];

  if ( type == INTERNAL || nodeType == TRANSITION )
    result = prefix + result;

  return result;
}






/*!
 * \return the short name of the place, e.g. for LoLA output
 */
string Place::nodeShortName() const
{
  if (type == INTERNAL)
    return ("p" + toString(id));
  else
    return history[0];
}





/*!
 * \return the short name of the transition, e.g. for LoLA output
 */
string Transition::nodeShortName() const
{
  return ("t" + toString(id));
}





/*!
 * \return string containing information about the net
 */
string PetriNet::information() const
{
  string result = "|P|=" + toString(P.size() + P_in.size() + P_out.size());
  result += ", |P_in|= " + toString(P_in.size());
  result += ", |P_out|= " + toString(P_out.size());
  result += ", |T|=" + toString(T.size());
  result += ", |F|=" + toString(F.size());
  return result;
}





/*!
 * Prints information about the generated Petri net. In particular, for each
 * place and transition all roles of the history are printed to understand
 * the net and maybe LoLA's witness pathes later.
 *
 * \todo put this to the nodes
 */
void PetriNet::output_info(ostream *output) const
{
  assert(output != NULL);

  // the places
  (*output) << "PLACES:\nID\tTYPE\t\tROLES\n";

  // the internal places
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    (*output) << (*p)->nodeShortName() << "\tinternal";

    for (vector<string>::iterator role = (*p)->history.begin(); role != (*p)->history.end(); role++)
      if (role == (*p)->history.begin())
	(*output) << "\t" + (*p)->prefix + *role + "\n";
      else
	(*output) << "\t\t\t" + (*p)->prefix + *role + "\n";
  }

  // the input places
  for (set<Place *>::iterator p = P_in.begin(); p != P_in.end(); p++)
  {
    (*output) << (*p)->nodeShortName() << "\tinput   ";

    for (vector<string>::iterator role = (*p)->history.begin(); role != (*p)->history.end(); role++)
      if (role == (*p)->history.begin())
	(*output) << "\t" + (*p)->prefix + *role + "\n";
      else
	(*output) << "\t\t\t" + (*p)->prefix + *role + "\n";
  }

  // the output places
  for (set<Place *>::iterator p = P_out.begin(); p != P_out.end(); p++)
  {
    (*output) << (*p)->nodeShortName() << "\toutput  ";

    for (vector<string>::iterator role = (*p)->history.begin(); role != (*p)->history.end(); role++)
      if (role == (*p)->history.begin())
	(*output) << "\t" + (*p)->prefix + *role + "\n";
      else
	(*output) << "\t\t\t" + (*p)->prefix + *role + "\n";
  }

  // the transitions
  (*output) << "\nTRANSITIONS:\n";
  (*output) << "ID\tROLES\n";

  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
  {
    (*output) << (*t)->nodeShortName() + "\t";

    for (vector<string>::iterator role = (*t)->history.begin(); role != (*t)->history.end(); role++)
      if (role == (*t)->history.begin())
	(*output) << (*t)->prefix + *role + "\n";
      else
	(*output) << "\t" + (*t)->prefix + *role + "\n";
  }
}





/******************************************************************************
 * DOT output of the net
 *****************************************************************************/


/*!
 * DOT-output of the arc.
*/
string Arc::output_dot() const
{
  string result = " ";
  if (source->nodeType == PLACE)
    result += "p" + toString(source->id) + " -> t" + toString(target->id);
  else
    result += "t" + toString(source->id) + " -> p" + toString(target->id);

  if (weight != 1)
    result += " [label=\"" + toString(weight) + "\"]";

  result += ";\n";

  return result;
}





/*!
 * DOT-output of the transition. Transitions are colored corresponding to their
 * initial role.
*/
string Transition::output_dot() const
{
  string result;
#ifdef USING_BPEL2OWFN
  result += " t" + toString(id) + "\t[label=\"" + nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
  result += " t" + toString(id) + "\t[label=\"" + nodeName();
#endif
 
  if (parameters[P_COMMUNICATIONONLY])
  {
    result += "\\n";
    result += nodeName();
  }

  result += "\"";

  if (type == IN)
    result += " style=filled fillcolor=orange";
  if (type == OUT)
    result += " style=filled fillcolor=yellow";
  if (type == INOUT)
    result += " style=filled fillcolor=gold";
  
  result += "];\n";
  return result;
}





/*!
 * DOT-output of the place. Places are colored corresponding to their initial
 * role. Places get bold outlines if they are marked.
*/
string Place::output_dot() const
{
  string result;
  
#ifdef USING_BPEL2OWFN
  result += " p" + toString(id) + "\t[label=\"" + nodeShortName() + "\"";
#endif
#ifndef USING_BPEL2OWFN
  result += " p" + toString(id) + "\t[label=\"" + nodeName() + "\"";
#endif

  if (tokens > 0)
    result += " style=\"filled,setlinewidth(3)\"";
  else
    result += " style=\"filled\"";

  switch (type)
  {
    case (IN):  result += " fillcolor=orange shape=ellipse"; break;
    case (OUT): result += " fillcolor=yellow shape=ellipse"; break;
    default:    break;
  }

  if (historyContains("1.internal.initial") || isFinal)
    result += " fillcolor=gray";
  else if (firstMemberIs("!link."))
    result += " fillcolor=red";
  else if (firstMemberIs("link."))
    result += " fillcolor=green";
  else if (firstMemberIs("variable."))
    result += " fillcolor=cyan";
  else if (historyContains("1.internal.clock"))
    result += " fillcolor=seagreen";
  else if (type == INTERNAL)
    result += " fillcolor=white";
    
  result += "];\n";

  return result;
}





/*!
 * Creates a DOT output (see http://www.graphviz.org) of the Petri net. It uses
 * the digraph-statement and adds labels to transitions, places and arcs if
 * neccessary. It also distinguishes the three arc types of #arc_type.
 */
void PetriNet::output_dot(ostream *output) const
{
  assert(output != NULL);

  (*output) << "digraph N {" << endl;
  (*output) << " graph [fontname=\"Helvetica\", label=\"";

  if (parameters[P_REDUCE])
    (*output) << "structural simplified ";

  (*output) << "Petri net generated from " << filename << "\"];" << endl;
  (*output) <<
    " node [fontname=\"Helvetica\" fontsize=10 fixedsize];" << endl;
  (*output) << " edge [fontname=\"Helvetica\" fontsize=10];" <<
    endl << endl;

  // list the places
  (*output) << endl << " node [shape=circle];" << endl;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
    (*output) << (*p)->output_dot();
  for (set<Place *>::iterator p = P_in.begin(); p != P_in.end(); p++)
    (*output) << (*p)->output_dot();
  for (set<Place *>::iterator p = P_out.begin(); p != P_out.end(); p++)
    (*output) << (*p)->output_dot();

  // list the transitions
  (*output) << endl << " node [shape=box regular=true];" << endl;
  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
    (*output) << (*t)->output_dot();

  // list the arcs
  for (set<Arc *>::iterator f = F.begin(); f != F.end(); f++)
    (*output) << (*f)->output_dot();

  (*output) << "}" << endl;
}





/******************************************************************************
 * Petri net file formats
 *****************************************************************************/

/*!
 * Outputs the net in PNML (Petri Net Markup Language).
 *
 * \param output  output stream
 *
 * \pre output != NULL
 *
 * \todo Somehow add the input and output places to the PNML output.
 */
void PetriNet::output_pnml(ostream *output) const
{
  assert(output != NULL);

  (*output) << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl << endl;
  (*output) << "<!--" << endl;
  (*output) << "  Petri net created by " << PACKAGE_STRING << " reading file " << filename << "." << endl;
  (*output) << "  See http://www.gnu.org/software/bpel2owfn for more details." << endl;
  (*output) << "-->" << endl << endl;

  (*output) << "<pnml>" << endl;
  (*output) << "  <net id=\"bpel-net\" type=\"owfn\">" << endl << endl;


  // places
  (*output) << "<!-- input places -->" << endl;
  for (set<Place *>::iterator p = P_in.begin(); p != P_in.end(); p++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "    <place id=\"" << (*p)->nodeShortName() << "\">" << endl;
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "    <place id=\"" << (*p)->nodeName() << "\">" << endl;
#endif
    (*output) << "      <name>" << endl;
    (*output) << "        <text>" << (*p)->history[0] << "</text>" << endl;
    (*output) << "      </name>" << endl;
    (*output) << "      <!-- <type>" << endl;
    (*output) << "        <text>input</text>" << endl;
    (*output) << "      </type> -->" << endl;
    (*output) << "    </place>" << endl << endl;
  }

  (*output) << "<!-- output places -->" << endl;
  for (set<Place *>::iterator p = P_out.begin(); p != P_out.end(); p++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "    <place id=\"" << (*p)->nodeShortName() << "\">" << endl;
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "    <place id=\"" << (*p)->nodeName() << "\">" << endl;
#endif
    (*output) << "      <name>" << endl;
    (*output) << "        <text>" << (*p)->history[0] << "</text>" << endl;
    (*output) << "      </name>" << endl;
    (*output) << "      <!-- <type>" << endl;
    (*output) << "        <text>output</text>" << endl;
    (*output) << "      </type> -->" << endl;
    (*output) << "    </place>" << endl << endl;
  }

  (*output) << "<!-- internal places -->" << endl;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "    <place id=\"" << (*p)->nodeShortName() << "\">" << endl;
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "    <place id=\"" << (*p)->nodeName() << "\">" << endl;
#endif
    (*output) << "      <name>" << endl;
    (*output) << "        <text>" << (*p)->history[0] << "</text>" << endl;
    (*output) << "      </name>" << endl;
    if ((*p)->tokens > 0)
    {
      (*output) << "      <initialMarking>" << endl;
      (*output) << "        <text>" << (*p)->tokens << "</text>" << endl;
      (*output) << "      </initialMarking>" << endl;
    }
    (*output) << "    </place>" << endl << endl;
  }


  // transitions
  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "    <transition id=\"" << (*t)->nodeShortName() << "\">" << endl;
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "    <transition id=\"" << (*t)->nodeName() << "\">" << endl;
#endif
    (*output) << "      <name>" << endl;
    (*output) << "        <text>" << (*t)->history[0] << "</text>" << endl;
    (*output) << "      </name>" << endl;
    (*output) << "    </transition>" << endl << endl;
  }
  (*output) << endl;

  // arcs
  int arcNumber = 1;
  for (set<Arc *>::iterator f = F.begin(); f != F.end(); f++, arcNumber++)
  {
    (*output) << "    <arc id=\"a" << arcNumber << "\" ";
#ifdef USING_BPEL2OWFN
    (*output) << "source=\"" << (*f)->source->nodeShortName() << "\" ";
    (*output) << "target=\"" << (*f)->target->nodeShortName() << "\">" << endl;
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "source=\"" << (*f)->source->nodeName() << "\" ";
    (*output) << "target=\"" << (*f)->target->nodeName() << "\">" << endl;
#endif
    (*output) << "      <inscription>\n        <text>" << (*f)->weight << "</text>\n      </inscription>" << endl;
    (*output) << "    </arc>" << endl;
  }
  (*output) << endl;

  (*output) << "    <toolspecific tool=\"owfn\" version=\"1.0\">" << endl;
  (*output) << "      <finalmarking xmlns=\"http://www.informatik.hu-berlin.de/top/tools4bpel\">" << endl;
  (*output) << "        <text>";

  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
    if ((*p)->isFinal)
      (*output) << (*p)->nodeShortName() << " ";

  (*output) << "</text>" << endl;
  (*output) << "      </finalmarking>" << endl;
  (*output) << "    </toolspecific>" << endl;
  (*output) << "  </net>" << endl;
  (*output) << "</pnml>" << endl;
}





/*!
 * Outputs the net in low-level PEP notation.
 *
 * \param output  output stream
 *
 * \pre output != NULL
 */
void PetriNet::output_pep(ostream *output) const
{
  assert(output != NULL);

  // header
  (*output) << "PEP" << endl << "PTNet" << endl << "FORMAT_N" << endl;

  // places (only internal)
  (*output) << "PL" << endl;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    (*output) << (*p)->id << "\"" << (*p)->nodeShortName() << "\"80@40";
    if ((*p)->tokens > 0)
      (*output) << "M" << (*p)->tokens;
    (*output) << "k1" << endl;
  }

  // transitions
  (*output) << "TR" << endl;
  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
    (*output) << (*t)->id << "\"" << (*t)->nodeShortName() << "\"80@40" << endl;

  // arcs from transitions to places (no output places)
  (*output) << "TP" << endl;
  for (set<Arc *>::iterator f = F.begin(); f != F.end(); f++)
    if (((*f)->source->nodeType) == TRANSITION)
      if ( P_out.find(static_cast<Place*>((*f)->target)) == P_out.end())
	(*output) << (*f)->source->id << "<" << (*f)->target->id << "w" << (*f)->weight << endl;

  // arcs from places to transitions
  (*output) << "PT" << endl;
  for (set<Arc *>::iterator f = F.begin(); f != F.end(); f++)
    if (((*f)->source->nodeType) == PLACE)
      if ( P_in.find(static_cast<Place*>((*f)->source)) == P_in.end())
	(*output) << (*f)->source->id << ">" << (*f)->target->id << "w" << (*f)->weight << endl;
}





/*!
 * Outputs the net in INA format.
 *
 * The complete syntax can be found at
 * http://www2.informatik.hu-berlin.de/lehrstuehle/automaten/ina/node14.html
 *
 * \param output  output stream
 *
 * \pre output != NULL
 *
 * \todo check whether arc weights are in the correct order
 */
void PetriNet::output_ina(ostream *output) const
{
  assert(output != NULL);

  // net header
  (*output) << "P   M   PRE,POST  NETZ 1:" << filename.substr(0, filename.find_first_of(".")) << endl;

  // places (only internal)
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    (*output) << setw(3) << (*p)->id << " " << (*p)->tokens << "     ";

    set<Node*> pre = preset(*p);
    set<Node*> post = postset(*p);

    for (set<Node*>::iterator t = pre.begin(); t != pre.end(); t++)
    {
      if (t != pre.begin())
	(*output) << " ";
      
      (*output) << (*t)->id << ":" << arc_weight(*t, *p);
    }

    (*output) << ", ";

    for (set<Node*>::iterator t = post.begin(); t != post.end(); t++)
    {
      if (t != post.begin())
	(*output) << " ";
      
      (*output) << (*t)->id << ":" << arc_weight(*p, *t);
    }

    (*output) << endl;
  }

  (*output) << "@" << endl;


  // place names
  (*output) << "place nr.             name capacity time" << endl;

  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    (*output) << right << setw(8) << (*p)->id << ": ";
    (*output) << left << setw(16) << (*p)->nodeShortName();
    (*output) << "       oo    0" << endl;
  }

  (*output) << "@" << endl;

  
  // transition names
  (*output) << "trans nr.             name priority time" << endl;

  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
  {
    (*output) << right << setw(8) << (*t)->id << ": ";
    (*output) << left << setw(16) << (*t)->nodeShortName();
    (*output) << "        0    0" << endl;
  }

  (*output) << "@" << endl;

  // info (ignored after last '@')
  (*output) << "generated by: " << PACKAGE_STRING << endl;
  (*output) << "input file:   `" << filename << "'" << endl;
  (*output) << "net size:     " << information() << endl;  
}





/*!
 * Outputs the net in APNN (Abstract Petri Net Notation).
 *
 * \param output  output stream
 *
 * \pre output != NULL
*/
void PetriNet::output_apnn(ostream *output) const
{
  assert(output != NULL);

  (*output) << "\\beginnet{" << filename << "}" << endl << endl;

  // places(only internal)
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    (*output) << "  \\place{" << (*p)->nodeShortName() << "}{";
    if ((*p)->tokens > 0)
      (*output) << "\\init{" << (*p)->tokens << "}";
    (*output) << "}" << endl;
  }
  (*output) << endl;

  // transitions
  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
  {
    (*output) << "  \\transition{" << (*t)->nodeShortName() << "}{}" << endl;
  }
  (*output) << endl;

  // arcs
  int arcNumber = 1;
  for (set<Arc *>::iterator f = F.begin(); f != F.end(); f++, arcNumber++)
  {
    // ignore input places
    if ((*f)->source->nodeType == PLACE)
      if ( P_in.find(static_cast<Place*>((*f)->source)) != P_in.end())
	continue;

    // ignore output places
    if ((*f)->target->nodeType == PLACE)
      if ( P_out.find(static_cast<Place*>((*f)->target)) != P_out.end())
	continue;

    (*output) << "  \\arc{a" << arcNumber << "}{ ";
    (*output) << "\\from{" << (*f)->source->nodeShortName() << " } ";
    (*output) << "\\to{" << (*f)->target->nodeShortName() << "} \\weight{" << (*f)->weight << "} }" << endl;
  }
  (*output) << endl;

  (*output) << "\\endnet" << endl;
}





/*!
 * Outputs the net in LoLA-format.
 *
 * \param output  output stream
 *
 * \pre output != NULL
 */
void PetriNet::output_lola(ostream *output) const
{
  assert(output != NULL);

  (*output) << "{ Petri net created by " << PACKAGE_STRING << " reading " << filename << " }" << endl << endl;

  
  // places (only internal)
  (*output) << "PLACE" << endl;
  unsigned int count = 1;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); count++, p++)
  {
    (*output) << "  " << (*p)->nodeShortName();
    
    if (count < P.size())
      (*output) << "," << endl;
  }
  (*output) << endl << ";" << endl << endl << endl;


  // initial marking
  (*output) << "MARKING" << endl;
  count = 1;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    if ((*p)->tokens > 0)
    {
      if (count++ != 1)
	(*output) << "," << endl;
      
      (*output) << "  " << (*p)->nodeShortName() << ":\t" << (*p)->tokens;
    }
  }
  (*output) << endl << ";" << endl << endl << endl;


  // transitions
  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
  {
    (*output) << "TRANSITION " << (*t)->nodeShortName();

    (*output) << endl;
    set<Node *> consume = preset(*t);
    set<Node *> produce = postset(*t);
    
    (*output) << "CONSUME" << endl;
    count = 1;
    for (set<Node *>::iterator pre = consume.begin(); pre != consume.end(); count++, pre++)
    {
      // ignore input places
      if ( (*pre)->nodeType == PLACE )
	if ( P_in.find(static_cast<Place*>(*pre)) != P_in.end())
	  continue;

      (*output) << "  " << (*pre)->nodeShortName() << ":\t" << arc_weight(*pre, *t);

      if (count < consume.size())
	(*output) << "," << endl;
    }
    (*output) << ";" << endl;
    
    (*output) << "PRODUCE" << endl;
    count = 1;
    for (set<Node *>::iterator post = produce.begin(); post != produce.end(); count++, post++)
    {
      // ignore output places
      if ( (*post)->nodeType == PLACE )
	if ( P_out.find(static_cast<Place*>(*post)) != P_out.end())
	  continue;

      (*output) << "  " << (*post)->nodeShortName() << ":\t" << arc_weight(*t, *post);

      if (count < produce.size())
	(*output) << "," << endl;
    }
    
    (*output) << ";" << endl << endl;
  }  
  (*output) << "{ END OF FILE }" << endl;
}





/******************************************************************************
 * oWFN file format
 *****************************************************************************/

/*!
 * Outputs the net in oWFN-format.
 *
 * \param output  output stream
 *
 * \pre output != NULL
 * \pre PetriNet::calculate_max_occurrences() called
 */
void PetriNet::output_owfn(ostream *output) const
{
  assert(output != NULL);

  extern map<unsigned int, ASTE*> ASTEmap;
  extern string invocation;

  (*output) << "{" << endl;
  (*output) << "  generated by: " << PACKAGE_STRING << endl;
#ifdef USING_BPEL2OWFN
  (*output) << "  input file:   `" << filename << "' (process `" << ASTEmap[1]->attributes["name"] << "')" << endl;
#endif
#ifndef USING_BPEL2OWFN
  (*output) << "  input file:   `" << filename << "'" << endl;
#endif
  (*output) << "  invocation:   `" << invocation << "'" << endl;
  (*output) << "  net size:     " << information() << endl;
  (*output) << "}" << endl << endl;

  // places
  (*output) << "PLACE" << endl;

  // internal places
  (*output) << "  INTERNAL" << endl;
  (*output) << "    ";

  unsigned int count = 1;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); count++, p++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << (*p)->nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
    (*output) << (*p)->nodeName();
#endif
    if (count < P.size())
      (*output) << ", ";
  }
  (*output) << ";" << endl << endl;


  // input places
  (*output) << "  INPUT" << endl;
  count = 1;
  for (set<Place *>::iterator p = P_in.begin(); p != P_in.end(); count++, p++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "    " << (*p)->nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "    " << (*p)->nodeName();
#endif
   
    if ((*p)->max_occurrences != UINT_MAX && (*p)->max_occurrences != 0)
      (*output) << " {$ MAX_OCCURRENCES = " << (*p)->max_occurrences << " $}";

    if (count < P_in.size())
      (*output) << "," << endl;
  }
  (*output) << ";" << endl << endl;


  // output places
  (*output) << "  OUTPUT" << endl;
  count = 1;
  for (set<Place *>::iterator p = P_out.begin(); p != P_out.end(); count++, p++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "    " << (*p)->nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "    " << (*p)->nodeName();
#endif
    
    if ((*p)->max_occurrences != UINT_MAX && (*p)->max_occurrences != 0)
      (*output) << " {$ MAX_OCCURRENCES = " << (*p)->max_occurrences << " $}";

    if (count < P_out.size())
      (*output) << "," << endl;
  }
  (*output) << ";" << endl << endl << endl;
  

  // initial marking
  (*output) << "INITIALMARKING" << endl;
  count = 1;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    if ((*p)->tokens > 0)
    {
      if (count++ != 1)
	(*output) << "," << endl;

#ifdef USING_BPEL2OWFN
      (*output) << " " << (*p)->nodeShortName() << ":\t" << (*p)->tokens;
#endif
#ifndef USING_BPEL2OWFN
      (*output) << " " << (*p)->nodeName() << ":\t" << (*p)->tokens;
#endif

      if ((*p)->historyContains("1.internal.initial"))
	(*output) << " {initial place}";
    }
  }
  (*output) << ";" << endl << endl;  


  // final marking
  (*output) << "FINALMARKING" << endl;
  count = 1;
  for (set<Place *>::iterator p = P.begin(); p != P.end(); p++)
  {
    if ( (*p)->isFinal )
    {
      if (count++ != 1)
	(*output) << ",";

#ifdef USING_BPEL2OWFN
      (*output) << "  " << (*p)->nodeShortName();
#else
// #ifndef USING_BPEL2OWFN
      (*output) << "  " << (*p)->nodeName();
#endif

      if ( (*p)->isFinal )
  	(*output) << " {final place}";
    }
  }
  (*output) << ";" << endl << endl << endl;


  // transitions
  for (set<Transition *>::iterator t = T.begin(); t != T.end(); t++)
  {
#ifdef USING_BPEL2OWFN
    (*output) << "TRANSITION " << (*t)->nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
    (*output) << "TRANSITION " << (*t)->nodeName();
#endif
    switch( (*t)->type )
    {
      case (INTERNAL):	(*output) << endl; break;
      case(IN):		(*output) << " { input }" << endl; break;
      case(OUT):	(*output) << " { output }" << endl; break;
      case(INOUT):	(*output) << " { input/output }" << endl; break;
    }

    set<Node *> consume = preset(*t);
    set<Node *> produce = postset(*t);
    
    (*output) << "  CONSUME ";
    count = 1;
    for (set<Node *>::iterator pre = consume.begin(); pre != consume.end(); count++, pre++)
    {
#ifdef USING_BPEL2OWFN
      (*output) << (*pre)->nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
      (*output) << (*pre)->nodeName();
#endif
      if (arc_weight(*pre, *t) != 1)
	(*output) << ":" << arc_weight(*pre, *t);

      if (count < consume.size())
	(*output) << ", ";
    }
    (*output) << ";" << endl;
    
    (*output) << "  PRODUCE ";

    count = 1;
    for (set<Node *>::iterator post = produce.begin(); post != produce.end(); count++, post++)
    {
#ifdef USING_BPEL2OWFN
      (*output) << (*post)->nodeShortName();
#endif
#ifndef USING_BPEL2OWFN
      (*output) << (*post)->nodeName();
#endif
      if (arc_weight(*t, *post) != 1)
	(*output) << ":" << arc_weight(*t, *post);
      
      if (count < produce.size())
	(*output) << ", ";
    }
    
    (*output) << ";" << endl << endl;
  }  
  (*output) << endl << "{ END OF FILE `" << output_filename << ".owfn' }" << endl;
}





/*!
 * Stream the PetriNet object to a given output stream, using the file format
 * set before uing PetriNet::set_format().
 *
 * \param os  an output stream to which the net is streamed
 * \param obj a PetriNet object
 */
ostream& operator<< (ostream& os, const PetriNet &obj)
{
  switch (obj.format)
  {
    case(FORMAT_APNN):	obj.output_apnn(&os); break;
    case(FORMAT_DOT):	obj.output_dot(&os); break;
    case(FORMAT_INA):	obj.output_ina(&os); break;
    case(FORMAT_INFO):	obj.output_info(&os); break;
    case(FORMAT_LOLA):	obj.output_lola(&os); break;
    case(FORMAT_OWFN):	obj.output_owfn(&os); break;
    case(FORMAT_PEP):	obj.output_pep(&os); break;
    case(FORMAT_PNML):	obj.output_pnml(&os); break;
    default:		break;
  }

  return os;
}




/*!
 * Set the output format to be used when the <<-operator is called the next
 * time.
 *
 * \param my_format the output format from the enumeration output_format.
 */
void PetriNet::set_format(output_format my_format)
{
  format = my_format;
}
