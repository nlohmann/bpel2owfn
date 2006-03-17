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
\*****************************************************************************/

/**
 * \file cfg.h
 *
 * \brief Functions for the Control Flow Graph (interface)
 *
 * This file provides the necessary classes for building a Control Flow Graph
 * 
 * \author  
 *          - responsible: Christian Gierds <gierds@informatik.hu-berlin.de>
 *          - last changes of: \$Author: gierds $
 *          
 * \date
 *          - created: 2006-01-19
 *          - last changed: \$Date: 2006/03/17 14:43:55 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.8 $
 */

#ifndef CFG_H
#define CFG_H

#include <string>
#include <iostream>
#include <list>
#include <map>
#include "exception.h"
#include "helpers.h"
#include "bpel-kc-k.h"		// generated by kimwitu++
#include "bpel-kc-rk.h"		// generated by kimwitu++
#include "bpel-kc-unpk.h"	// generated by kimwitu++

extern std::string filename;

/// enumeration for the possible block types, that can occur in the CFG
typedef enum
{
  CFGProcess = 1,
  CFGEmpty,
  CFGInvoke,
  CFGReceive,
  CFGReply,
  CFGAssign,
  CFGCopy,
  CFGWait,
  CFGThrow,
  CFGCompensate,
  CFGTerminate,
  CFGFlow,
  CFGSwitch,
  CFGCase,
  CFGOtherwise,
  CFGWhile,
  CFGSequence,
  CFGPick,
  CFGScope,
  CFGTarget,
  CFGSource
} CFGBlockType;

// forward declarations
class CFGBlock;

/// map for assigning a target to its appropriate source object
extern map<std::string, CFGBlock *> sources;
/// map for assigning a source to its appropriate target object
extern map<std::string, CFGBlock *> targets;

/**
 * This class is a generic template for all blocks within
 * a CFG.
 * 
 */
class CFGBlock {
  public:
    /// list of pointers to the previous blocks	  
    list<CFGBlock *> prevBlocks;
    /// list of pointers to the next blocks 
    list<CFGBlock *> nextBlocks;

    /// pointer to the first block of an activity
    CFGBlock * firstBlock;
    /// pointer to the last block of an activity
    CFGBlock * lastBlock;

    
    /// type of the block
    CFGBlockType type;
    /// ID of the block
    kc::integer id;
    /// label of the block
    std::string label;

    /// constructor
    CFGBlock();
    /// constructor
    CFGBlock(CFGBlockType, kc::integer, std::string);
    /// generic constructor
    CFGBlock(std::string id);
    /// generic constructor
    CFGBlock(kc::integer id);
    /// generic destructor
    virtual ~CFGBlock();
    
    /// returns the concrete type
    CFGBlockType getType(); 
    /// dot_output
    virtual void print_dot();
    /// the name of the dot node
    virtual std::string dot_name();

    /// additional name of the channel
    std::string channel_name;

    /// checks which blocks need DPE
    bool needsDPE(int hasStartingBlock, list<kc::integer>& lastTarget);
    
  private:
    /// flag, if block was dotted
    bool dotted;
    /// flag, if block was processed by an algorithm (has to be resetted afterwards)
    bool processed;

    /// \todo: move to symbol table
    bool dpe;
    list<CFGBlock * > dpeList;
	  
};

void cfgDot(CFGBlock *);

void connectBlocks(CFGBlock *, CFGBlock *);

#endif

