/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file vapi.cpp
* @author Simon
* @brief Viusal API
*/
#include <Windows.h>
#include "vapi.h"

#define PROPERTY_NAME(clProperty) (#clProperty)
#define REGIST_PROPERTY(clProperty) pPropertyMap->insert(std::pair<std::string, float *>(std::string(PROPERTY_NAME(clProperty)), &clProperty))

vapi::vapi() : driver("vapi") {
	ZeroMemory(&clProperty, sizeof(sClProperty));
	pPropertyMap = new std::map<std::string, float*>;

	REGIST_PROPERTY(clProperty.tas);
	REGIST_PROPERTY(clProperty.cas);
	REGIST_PROPERTY(clProperty.pAlt);
	REGIST_PROPERTY(clProperty.onGround);
	REGIST_PROPERTY(clProperty.stall);
	REGIST_PROPERTY(clProperty.sw1);
}

vapi::~vapi() {
	delete pPropertyMap;
}

float* vapi::getClProperty(std::string name) {
	auto iter = pPropertyMap->find(name);

	if (iter != pPropertyMap->end()) return iter->second;
	else return nullptr;
}