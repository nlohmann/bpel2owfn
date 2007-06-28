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

/*!
 * \file    extension-chor.h
 *
 * \brief   BPEL4Chor extension
 *
 * \author  Niels Lohmann <nlohmann@informatik.hu-berlin.de>,
 *          last changes of: \$Author: nielslohmann $
 *
 * \since   2007/04/30
 *
 * \date    \$Date: 2007/06/28 07:38:16 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universit�t zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 * \version \$Revision: 1.17 $
 */





#ifndef EXTENSION_CHOR_H
#define EXTENSION_CHOR_H


#include <map>
#include <set>
#include <string>
#include <utility>

using std::map;
using std::string;
using std::pair;
using std::set;





class BPEL4Chor_participantSet
{
  friend class BPEL4Chor;
  friend class BPEL4Chor_messageLink;

  public:
    /// maximal number of iterations (attribute "count")
    unsigned int count;
  
  private:
    /// name of the participantSet
    string name;

    /// name of the forEach
    string forEach_name;

    /// name of the participantType
    string participantType_name;

    /// a set of iterator participant names
    set<string> iterator_participant_names;

    /// a set of unique participant names
    set<string> unique_participant_names;

    /// constructor
    BPEL4Chor_participantSet(map<string, string> &attribute_map);
};





/*!
 * \class BPEL4Chor_messageLink
 *
 * A BPEL4Chor messageLink.
 */
class BPEL4Chor_messageLink
{
  friend class BPEL4Chor;

  public:
    /// attribute "name"
    string name;
  
    /// a pointer to a forEach if an iterator participant is involved
    BPEL4Chor_participantSet *participantSet;

    // variables to organize multi-instances
    bool sender_is_unique;
    bool sender_is_iterator;
    bool receiver_is_unique;
    bool receiver_is_iterator;  

  private:
    /// the identifier of the messageLink (either attribute "id" or "name")
    string id;

    /// attribute "sender" or "senders"
    string sender;

    /// attribute "sendActivity" or "sendActivties"
    string sendActivity;

    /// attribute "receiver" or "receivers"
    string receiver;

    /// attribute "receiveActivity" or "receiveActivities"
    string receiveActivity;

    /// attribute "messageName"
    string messageName;

    /// constructor
    BPEL4Chor_messageLink(map<string, string> &attribute_map);
};





/*!
 * \class   BPEL4Chor
 *
 * This class organizes informations read from a BPEL4Chor topology file.
 */
class BPEL4Chor
{
  friend class BPEL4Chor_participantSet;
  friend class BPEL4Chor_messageLink;

  private:
    /// the currently parsed participantSet
    BPEL4Chor_participantSet *current_participantSet;

    /// the messageLinks, indexed by their name
    map<string, BPEL4Chor_messageLink*> messageLinks;

    /// the participantTypes with XMLNS, indexed by their name
    map<string, string> participantTypes;

    /// the participantSets, indexed by their name
    map<string, BPEL4Chor_participantSet*> participantSets;

  public:
    /// add a <participantType>
    void add_participantType(map<string, string> &attribute_map);

    /// add a <participant>
    void add_participant(map<string, string> &attribute_map);

    /// start processing a <participantSet>
    void start_participantSet(map<string, string> &attribute_map);

    /// end processing a <participantSet>
    void end_participantSet();

    /// add a <messageLink>
    void add_messageLink(map<string, string> &attribute_map);

    /// find a channel name given an activity id or name
    BPEL4Chor_messageLink *get_messageLink(unsigned int ASTE_id) const;
    
    /// find the number of instances to be created, given a process name
    int instances(string process_name) const;

    /// find a forEach name given an activity id or name
    unsigned int forEach_count(unsigned int ASTE_id) const;

    /// prints information about the BPEL4Chor topology
    void print_information() const;

    /// constructor
    BPEL4Chor();
};


#endif
