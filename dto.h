// dto.h

#ifndef _DTO_h
#define _DTO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct dto
{


 public:
	void init();
};


#endif

