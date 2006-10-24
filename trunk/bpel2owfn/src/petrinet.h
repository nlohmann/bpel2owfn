/*****************************************************************************\
 * Copyright 2005, 2006 Niels Lohmann, Christian Gierds, Dennis Reinert      *
 *                                                                           *
 * This file is part of BPEL2oWFN.                                           *
 *                                                                           *
 * BPEL2oWFN is free software; you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or(at your     *
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
\*****************************************************************************/

/*!
 * \file petrinet.h
 *
 * \brief Functions for Petri nets (interface)
 *
 * This file contains the data structures and classes to store and organize
 * Petri nets generated from the abstract syntax tree. It contains the
 * following classes:
 *
 * - Node
 *   - Transition
 *   - Place
 * - Arc
 * - PetriNet
 *
 *
 * \author
 *          - responsible: Niels Lohmann <nlohmann@informatik.hu-berlin.de>
 *          - last changes of: \$Author: nlohmann $
 *
 * \date
 *          - created: 2005/10/18
 *          - last changed: \$Date: 2006/10/24 13:47:32 $
 *
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 * \version \$Revision: 1.97 $
 */





#ifndef PETRINET_H
#define PETRINET_H





/******************************************************************************
 * Headers
 *****************************************************************************/

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>

#include "ast-config.h"		// all you need from Kimwitu++

using namespace std;





/******************************************************************************
 * Data structures
 *****************************************************************************/

/// Enumeration of the possible types of a place or transition.
typedef enum
{
  INTERNAL,			///< non-communicating (standard)
  IN,				///< input of an open workflow net (oWFN)
  OUT,				///< output of an open workflow net (oWFN)
  INOUT				///< input and output of an open workflow net (oWFN)
} communication_type;





/// Enumeration of the possible types of an arc.
typedef enum
{
  STANDARD,			///< standard arc
  READ				///< read arc (will be unfolded to a loop)
} arc_type;





/// Enumeration of the possible types of a node.
typedef enum
{
  PLACE,			///< a place
  TRANSITION			///< a transition
} node_type;





/******************************************************************************
 * Classes
 *****************************************************************************/

/*!
 * \class Node
 *
 * \brief Nodes of the Petri net
 *
 * Class to represent nodes (i.e. places and transitions) of Petri nets. Each
 * node has an id and a history (i.e. the list of roles the node had during
 * the processing of a BPEL-file).
 *
*/

class Node
{
  public:
    /// the id of the node
    unsigned int id;

    /// the name of the type
    virtual string nodeTypeName();

    /// the short name of the node
    virtual string nodeShortName();

    /// the type of the node
    node_type nodeType;

    /// the set of roles (i.e. the history) of the node
    vector<string> history;

    /// true if first role contains role
    bool firstMemberAs(string role);

    /// true if first role begins with role
    bool firstMemberIs(string role);
    
    /// true if history contains role
    bool historyContains(string role);

    /// type of node as defined in #communication_type
    communication_type type;

    /// the name of the node
    string nodeName();

    /// an additional prefix for the name in order to distinguish nodes of different nets
    string prefix;

    virtual ~Node();
};





/*****************************************************************************/


/*!
 * \class Transition
 *
 * \brief Transitions of the Petri net
 *
 * Class to represent transitions of Petri nets. Each transition inherits the
 * variables #id and #history.
 *
*/

class Transition: public Node
{
  public:
    /// constructor which creates a transition and adds a first role to the history
    Transition(unsigned int id, string role);

    /// DOT-output of the transition (used by PetriNet::dotOut())
    string dotOut();

    /// the name of the type
    string nodeTypeName();
    
    /// the short name of the transition
    string nodeShortName();
};





/*****************************************************************************/


/*!
 * \class  Place
 *
 * \brief Places of the Petri net
 *
 * Class to represent places of Petri nets. In addition to the inherited
 * variables #id and #history, each place has a type defined in the
 * enumeation #communication_type and an initial marking. As the generated net
 * and its inital marking is 1-safe, it is sufficent to represent the initial
 * marking as a Boolean value.
 *
*/

class Place: public Node
{
  public:
    /// constructor which creates a place and adds a first role to the history
    Place(unsigned int id, string role, communication_type type);

    /// DOT-output of the place (used by PetriNet::dotOut())
    string dotOut();

    /// the name of the type
    string nodeTypeName();

    /// the short name of the place
    string nodeShortName();

    /// Mark the current place.
    void mark();

    /// the initial marking of the place
    bool marked;

    /// this (communication) place is embedded in a while activity
    bool inWhile;
};





/*****************************************************************************/


/*!
 * \class  Arc
 *
 * \brief Arcs of the Petri net
 *
 * Class to represent arcs of Petri nets. An arc written as a tupel (n1,n2)
 * has n1 as #source and n2 as #target.
 *
*/

class Arc
{
  public:
    /// source node of the arc
    Node *source;
  
    /// target node of the arc
    Node *target;
  
    /// Constructor to create an arc of certain type.
    Arc(Node *source, Node *target);
  
    /// DOT-output of the arc (used by PetriNet::dotOut())
    string dotOut();
};





/*****************************************************************************/


/*!
 * \class  PetriNet
 *
 * \brief A Petri net
 *
 * Class to represent Petri nets. The net is consists of places of class
 * #Place, transitions of class #Transition and arcs of class #Arc. The sets
 * are saved in three lists #P, #T and #F.
 *
 */


class PetriNet
{
  public:
    /// Adds a place with a given role and type.
    Place* newPlace(string role, communication_type type = INTERNAL);
  
    /// Adds a transition with a given role.
    Transition*newTransition(string role);
  
    /// Adds an arc given source and target node, and arc type.
    Arc *newArc(Node *source, Node *target, arc_type type = STANDARD);

    /// Information about the net including histories of all nodes.
    void printInformation();


    /// DOT (Graphviz) output.
    void dotOut();

    /// PNML (Petri Net Markup Language) output.
    void pnmlOut();

    /// low-level PEP output.
    void pepOut();

    /// APNN (Abstract Petri Net Notation) output.
    void apnnOut();

    /// LoLA-output.
    void lolaOut();

    /// oWFN-output.
    void owfnOut();


    /// Merges places given two places.
    void mergePlaces(Place *p1, Place *p2);

    /// Merges places given two roles.
    void mergePlaces(string role1, string role2);

    /// Merges places given two identifiers and roles.
    void mergePlaces(int id1, string role1, int id2, string role2);
    
    /// Merges places given two activities with roles.
    void mergePlaces(kc::impl_activity *act1, string role1,
		     kc::impl_activity *act2, string role2);
    
    /// Merges transitions given two transitions.
    void mergeTransitions(Transition *t1, Transition *t2);


    /// Finds place given a role.
    Place* findPlace(string role);

    /// Finds place given an activity with a role.
    Place* findPlace(kc::impl_activity *activity, string role);

    /// Finds place given an id with a role.
    Place* findPlace(int id, string role);

    /// Finds a place given the ids of two transitions (one in the pre and one in the postset).
    Place *findPlace(unsigned int id1, unsigned int id2);


    /// Finds transition given a role.
    Transition* findTransition(string role);

    /// Calculates the preset of a node.
    set<Node*> preset(Node *n);

    /// Calculates the postset of a node.
    set<Node*> postset(Node *n);

    /// Simplifies the Petri net.
    void simplify();

    /// Removes all variable places.
    void removeVariables();

    /// Adds a prefix to the name of all nodes of the net
    void addPrefix(string prefix);

    /// Connects a second oWFN
    void connectNet(PetriNet *net);

    /// moves channel places to the list of internal places
    void makeChannelsInternal();

    /// re-enumerates the nodes
    void reenumerate();

    /// Statistical output.
    string information();
    
    /// Constructor to create an empty Petri net.
    PetriNet();


  private:
    /// Adds a place without an initial role.
    Place* newPlace();

    /// Removes a place from the net.
    void removePlace(Place *p);

    /// Adds a transition without an initial role.
    Transition* newTransition();

    /// Removes a transition from the net.
    void removeTransition(Transition *t);

    /// Removes an arc from the net.
    void removeArc(Arc *f);

    /// Removes all ingoing and outgoing arcs of a node.
    void detachNode(Node *n);


    /// remove unused status places
    void removeUnusedStatusPlaces();

    /// remove transitions with empty pre or postset
    void removeSuspiciousTransitions();

    /// remove dead nodes of the Petri net
    void removeDeadNodes();

    /// elimination of identical places (RB1)
    void elminiationOfIdenticalPlaces();
    
    /// elimination of identical transitions (RB2)
    void elminiationOfIdenticalTransitions();

    /// fusion of series places (RA1)
    void fusionOfSeriesPlaces();

    /// fusion of series transitions (RA2)
    void fusionOfSeriesTransitions();

    /// remove transitive nodes and arcs
    void transitiveReduction();

    
    /// Removes interface places (for non-oWFN formats)
    void removeInterface();
  
    /// Returns an id for new nodes.
    unsigned int getId();

    /// Returns current id.
    unsigned int id();


    /// the list of places of the Petri net
    set<Place *> P;

    /// the list of input places of the oWFN
    set<Place *> P_in;

    /// the list of output places of the oWFN
    set<Place *> P_out;

    /// the list of transitions of the Petri net
    set<Transition *> T;

    /// the list of arcs of the Petri net
    set<Arc *> F;


    /// the id that will be assigned to the next node
    unsigned int nextId;

    /// true if function #PetriNet::removeInterface() was called
    bool hasNoInterface;

    /// Mapping of roles to nodes of the Petri net.
    map<string, Node *> roleMap;
};





#endif
