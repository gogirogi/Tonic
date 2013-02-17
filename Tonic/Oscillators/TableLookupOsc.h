//
//  TableLookupOsc.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
 This class is heavily borrowed from the PD implementation of a table-lookup oscillator.
 Necessary attribution/license TBD.
*/

#ifndef __Tonic__TableLookupOsc__
#define __Tonic__TableLookupOsc__

#include <iostream>
#include "Generator.h"
#include "FixedValue.h"

// Causes 32nd bit in double to have fractional value 1 (decimal point on 32-bit word boundary)
// Allowing some efficient shortcuts for table lookup using power-of-two tables
#define BIT32DECPT 1572864.  

namespace Tonic {
  
  namespace Tonic_ {
    
    // For fast computation of int/fract using some bit-twiddlery
    // inspired by the pd implementation
    union ShiftedDouble {
      double d;
      uint32_t i[2];
    };
    
    const unsigned long TABLE_SIZE = 2048;
    
    class TableLookupOsc_ : public Generator_{
      
      
      //------------------------------------
      //
      // Many of the original STK methods are not applicable in our use case
      // (direct phase/freq manipulation) and have been removed for optimization.
      // In the future, phase inputs may be added, but will be optimized and made
      // Tonic-friendly for universal routing.
      //
      //-----------------------------------
      
    protected:

      //! Subclasses MUST override to provide pointer to table
      /*
        Necessary to maintain efficient memory usage by only
        using one set of static wavetable data per subclss, regardless
        of number of instances.
       
        Subclasses should return static member variable.
      */
      virtual TonicFrames & tableReference() = 0;
      
      //! Subclasses implement this to fill the wavetable
      /*
          Subclasses should ALWAYS call this method from their constructor!
      */
      virtual void fillTable() = 0;
      
      double phase_;
      
      Generator frequencyGenerator;
      TonicFrames modFrames;
      
    public:
      
      TableLookupOsc_();
      ~TableLookupOsc_();

      
      void tick( TonicFrames& frames);
      
      //! Clear output and reset time pointer to zero.
      void reset( void );
      
      //! Set frequency generator input
      void setFrequency( Generator & genArg){
        // need to lock mutex so we don't replace this in the middle of a tick
        lockMutex();
        frequencyGenerator = genArg;
        unlockMutex();
      };

    };

  }

}

#endif /* defined(__Tonic__TableLookupOsc__) */

