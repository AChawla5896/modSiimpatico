#ifndef DDMD_ACTION_CPP
#define DDMD_ACTION_CPP

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include "Action.h"

namespace DdMd
{

   using namespace Util;

   // Constructor.
   Action::Action(Simulation& simulation)
    : simulationPtr_(&simulation),
      interval_(1),
      hasSetupPostExchange_(false),
      hasSetupPostNeighbor_(false),
      hasSetupPostForce_(false),
      hasPreIntegrate_(false),
      hasPostIntegrate_(false),
      hasPreTransform_(false),
      hasPreExchange_(false),
      hasPostExchange_(false),
      hasPostNeighbor_(false),
      hasPreUpdate_(false),
      hasPostUpdate_(false),
      hasPreForce_(false),
      hasPostForce_(false),
      hasEndOfStep_(false),
      hasPackExchange_(false),
      hasUnpackExchange_(false),
      hasPackUpdate_(false),
      hasUnpackUpdate_(false),
      hasPackreverseUpdate_(false),
      hasUnpackreverseUpdate_(false)
   {}

   // Destructor.
   virtual ~Action()
   {};

}
#endif
