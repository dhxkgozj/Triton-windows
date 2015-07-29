#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "IRBuilder.h"
#include <pin.H>


// Return an IR object corresponding to the given instruction.
IRBuilder *createIRBuilder(INS ins);

#endif // DIRECTOR_H

