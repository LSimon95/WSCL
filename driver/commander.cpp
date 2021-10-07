/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file commander.cpp
* @author Simon
* @brief Commander control
*/
#include "commander.h"
#include "hc.h"
#include "vapi/vapi.h"

commander::commander() : driver("commander") {
	pHc = nullptr;
	pVapi = nullptr;
}

commander::~commander() {

	if (pHc) delete pHc;
	if (pVapi) delete pVapi;
}

int commander::connectHardware() {
	if (pHc) delete pHc;
	pHc = nullptr;
	pHc = new hc("0000-0000-0000-0000", pVapi);

	tinyxml2::XMLDocument config;
	config.LoadFile("./tmpl.xml");
	pHc->setCLConfig(&config);

	return NULL;
}

int commander::connectVisual() {
	pVapi = new vapi();

	return NULL;
}

void commander::sim() {
	if (!pHc || !pVapi) return;

	pHc->sim();
}