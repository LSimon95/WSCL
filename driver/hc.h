/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file hc.h
* @author Simon
* @brief Hardware control
*/
#pragma once
#include "driver.h"

#define MAX_AXIS 3

class hc : public driver
{
	friend class commander;
public:
	hc(const char* id, class vapi* vapi);
	~hc() {};

	void sim();
protected:
	void setCLConfig(tinyxml2::XMLDocument *pConfig);

private:
	std::vector<std::string>* pPropertyNames;
	std::vector<float*>* pProperties;
	vapi* pVapi;

	int checkInfomation(tinyxml2::XMLElement *information);
	int setProperty(tinyxml2::XMLElement* clProperty);
	int transCLConfig(tinyxml2::XMLElement* dynamic);

	eOperType getOperType(const char* type);
	uint8_t getPropertyId(const char* name);
	int parseTable(const char* table, operTable* pOperTable);
	int transCLNode(char* pOperStart, char** pOperNode, tinyxml2::XMLElement* node);


	CRITICAL_SECTION csSim;
};

