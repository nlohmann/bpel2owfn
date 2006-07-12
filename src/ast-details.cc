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
 * \file ast-details.cc
 *
 * \author  
 *          - responsible: Niels Lohmann <nlohmann@informatik.hu-berlin.de>
 *          - last changes of: \$Author: nlohmann $
 *          
 * \date
 *          - created: 2005/07/02
 *          - last changed: \$Date: 2006/07/12 10:55:18 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.11 $
 */





/******************************************************************************
 * Headers
 *****************************************************************************/

#include "ast-details.h"
#include <assert.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;





/******************************************************************************
 * External variables
 *****************************************************************************/

extern map<unsigned int, map<string, string> > temporaryAttributeMap;





/******************************************************************************
 * Member functions
 *****************************************************************************/

/*!
 * Constructor
 *
 * \param mynode a node of the abstract syntax tree
 * \param mytype value of the type of the node using the token values defined
 *               by flex and bison
 */
ASTE::ASTE(kc::impl_abstract_phylum *mynode, int mytype)
{
  assert(mynode != NULL);

  // get the identifier from the abstract syntax tree
  switch (mytype)
  {
    case(K_ASSIGN):		id = ((kc::impl_tAssign*)mynode)->id; break;
    case(K_CASE):		id = ((kc::impl_tCase*)mynode)->id; break;
    case(K_CATCH):		id = ((kc::impl_tCatch*)mynode)->id; break;
    case(K_CATCHALL):		id = ((kc::impl_tCatchAll*)mynode)->id; break;
    case(K_COMPENSATE):		id = ((kc::impl_tCompensate*)mynode)->id; break;
    case(K_COMPENSATIONHANDLER):id = ((kc::impl_tCompensationHandler*)mynode)->id; break;
    case(K_CORRELATION):	id = ((kc::impl_tCorrelation*)mynode)->id; break;
    case(K_CORRELATIONSET):	id = ((kc::impl_tCorrelationSet*)mynode)->id; break;
    case(K_EMPTY):		id = ((kc::impl_tEmpty*)mynode)->id; break;
    case(K_EVENTHANDLERS):	id = ((kc::impl_tEventHandlers*)mynode)->id; break;
    case(K_FAULTHANDLERS):	id = ((kc::impl_tFaultHandlers*)mynode)->id; break;
    case(K_FROM):		id = ((kc::impl_tFrom*)mynode)->id; break;
    case(K_FLOW):		id = ((kc::impl_tFlow*)mynode)->id; break;
    case(K_INVOKE):		id = ((kc::impl_tInvoke*)mynode)->id; break;
    case(K_LINK):		id = ((kc::impl_tLink*)mynode)->id; break;
    case(K_ONALARM):		id = ((kc::impl_tOnAlarm*)mynode)->id; break;
    case(K_ONMESSAGE):		id = ((kc::impl_tOnMessage*)mynode)->id; break;
    case(K_OTHERWISE):		id = ((kc::impl_tOtherwise*)mynode)->id; break;
    case(K_PICK):		id = ((kc::impl_tPick*)mynode)->id; break;
    case(K_PARTNERLINK):	id = ((kc::impl_tPartnerLink*)mynode)->id; break;
    case(K_PROCESS):		id = ((kc::impl_tProcess*)mynode)->id; break;
    case(K_RECEIVE):		id = ((kc::impl_tReceive*)mynode)->id; break;
    case(K_REPLY):		id = ((kc::impl_tReply*)mynode)->id; break;
    case(K_SCOPE):		id = ((kc::impl_tScope*)mynode)->id; break;
    case(K_SOURCE):		id = ((kc::impl_tSource*)mynode)->id; break;
    case(K_SEQUENCE):		id = ((kc::impl_tSequence*)mynode)->id; break;
    case(K_SWITCH):		id = ((kc::impl_tSwitch*)mynode)->id; break;
    case(K_TERMINATE):		id = ((kc::impl_tTerminate*)mynode)->id; break;
    case(K_TARGET):		id = ((kc::impl_tTarget*)mynode)->id; break;
    case(K_THROW):		id = ((kc::impl_tThrow*)mynode)->id; break;
    case(K_TO):			id = ((kc::impl_tTo*)mynode)->id; break;
    case(K_VARIABLE):		id = ((kc::impl_tVariable*)mynode)->id; break;
    case(K_WAIT):		id = ((kc::impl_tWait*)mynode)->id; break;
    case(K_WHILE):		id = ((kc::impl_tWhile*)mynode)->id; break;
    default:			id = 0;
  }

  // make sure the node in the ast was really found
  assert(id != 0);

  // initialize values
  node = mynode;
  type = mytype;
  attributes = temporaryAttributeMap[id];
  suppressJF = false; // required initialization!
  controlFlow = POSITIVECF;
}
