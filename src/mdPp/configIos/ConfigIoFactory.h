#ifndef MDPP_CONFIG_IO_FACTORY_H
#define MDPP_CONFIG_IO_FACTORY_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <util/param/Factory.h>  
#include <mdPp/configIos/ConfigIo.h>

#include <string>

namespace MdPp
{

   using namespace Util;
   class Processor;

   /**
   * Default Factory for subclasses of ConfigIo.
   */
   class ConfigIoFactory : public Factory<ConfigIo> 
   {

   public:

      /**
      * Constructor
      *
      * \param processor parent Processor post-processor object
      */
      ConfigIoFactory(Processor& processor);

      /**
      * Create an instance of a specified subclass of ConfigIo.
      *
      * If the subclassName is recognized, this method returns a
      * pointer to new object. If the name is not recognized, it
      * returns a null pointer.
      *
      * The new object is created using a constructor that takes
      * the parent DdMd::Simulation as a parameter. The calling
      * function must invoke DdMd::ConfigIo::initialize() before
      * using the new ConfigIo to read or write configuration
      * files.
      *
      * \param  subclassName  name of a subclass of ConfigIo 
      * \return ConfigIo*     pointer to new instance of subclassName
      */
      ConfigIo* factory(const std::string &subclassName) const;

   private:

      /// Pointer to a parent Processor.
      Processor* processorPtr_;

   };

}
#endif
