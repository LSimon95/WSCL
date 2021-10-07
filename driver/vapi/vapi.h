/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file vapi.h
* @author Simon
* @brief Viusal API
*/
#pragma once

#include "../driver.h"

struct sClProperty {
	float tas; /* Ture Airspeed (kts)*/
	float cas; /* Calibrate Airspeed (kts)*/
	float pAlt; /* Pressure Altitude (ft)*/
	float onGround; /* Aircraft on Groud (bool)*/
	float stall; /* Stall Signal (bool)*/
	float sw1; /* User switch(num)*/
};

class vapi : public driver
{
public:
	vapi();
	~vapi();

	float* getClProperty(std::string name);

private:
	sClProperty clProperty;
	std::map<std::string, float*>* pPropertyMap;
};

