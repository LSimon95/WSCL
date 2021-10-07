/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file hc.cpp
* @author Simon
* @brief Hardware control
*/
#include "hc.h"

#include "tinyxml2.h"

#include "vapi/vapi.h"
#include "clp.h"

hc::hc(const char* id, class vapi* vapi) : driver("hc") {
	pVapi = vapi;
	pProperties = nullptr;
	pPropertyNames = nullptr;

	InitializeCriticalSection(&csSim);
}

void hc::sim() {

}

void hc::setCLConfig(tinyxml2::XMLDocument* pConfig) {
	EnterCriticalSection(&csSim);

	auto* controlLoad = pConfig->FirstChildElement();

	checkInfomation(controlLoad->FirstChildElement("information"));
	setProperty(controlLoad->FirstChildElement("clProperty"));
	transCLConfig(controlLoad->FirstChildElement("dynamic"));
	LeaveCriticalSection(&csSim);

}

int hc::checkInfomation(tinyxml2::XMLElement* information) {
	if (!information) return -1;

	return NULL;
}

int hc::setProperty(tinyxml2::XMLElement* clProperty) {
	if (!clProperty) return -1;

	auto* clp = clProperty->FirstChildElement();
	if (pProperties) delete pProperties;
	pProperties = new std::vector<float*>;
	if (pPropertyNames) delete pPropertyNames;
	pPropertyNames = new std::vector<std::string>;

	while (clp)
	{
		float* pProperty = pVapi->getClProperty(clp->Name());
		if (!pProperty) return -1;

		pPropertyNames->push_back(clp->Name());
		pProperties->push_back(pProperty);

		clp = clp->NextSiblingElement();
	}

	return NULL;
}

int hc::transCLConfig(tinyxml2::XMLElement* dynamic)
{
	if (!dynamic) return -1;

	void* axis[MAX_AXIS];
	int axisIndex = 0;

	auto* eleAxis = dynamic->FirstChildElement();

	while (eleAxis && !strcmp(eleAxis->Name(), "axis"))
	{
		char* pOperStart = (char*)malloc(MAX_OPER_SIZE);
		ZeroMemory(pOperStart, sizeof(MAX_OPER_SIZE));

		char* pOperNode = pOperStart;
		transCLNode(pOperStart, &pOperNode, eleAxis->FirstChildElement());

		axisIndex++;
		eleAxis = eleAxis->NextSiblingElement();

		free(pOperStart);
	}
}

eOperType hc::getOperType(const char* type) {
	if (!strcmp(type, "value")) return OPER_TYPE_VALUE;
	else if (!strcmp(type, "property")) return OPER_TYPE_PROPERTY;
	else if (!strcmp(type, "sum")) return OPER_TYPE_SUM;
	else if (!strcmp(type, "product")) return OPER_TYPE_PRODUCT;
	else if (!strcmp(type, "divide")) return OPER_TYPE_DIVIDE;
	else if (!strcmp(type, "limit")) return OPER_TYPE_LIMIT;
	else if (!strcmp(type, "table")) return OPER_TYPE_TABLE;
	else if (!strcmp(type, "switch")) return OPER_TYPE_SWITCH;
	else return OPER_TYPE_VALUE;
}

uint8_t hc::getPropertyId(const char* name) {
	auto itPropertyName = std::find(pPropertyNames->begin(), pPropertyNames->end(), std::string(name));
	if (itPropertyName == pPropertyNames->end()) {
		setError("Can not find property: %s", name);
		return -1;
	}
	return itPropertyName - pPropertyNames->begin();
}

int hc::parseTable(const char* table, operTable* pOperTable) {
	char* ps = (char*)table, *pe = (char*)table;

	// Seek to first number
	while (*pe < '-' && *pe > '9') pe++;
	ps = pe;

	float cols[MAX_TABLE_COL];
	float rows[MAX_TABLE_ROW];
	float data[MAX_TABLE_SIZE];

	uint8_t col = 0;

	while (true) {
		pe++;
		if (*pe < '-' && *pe > '9') {
			// Read data
			if (pOperTable->rowSize == 0) {
				cols[pOperTable->colSize] = atof(ps);
				pOperTable->colSize++;
			}
			else if (pOperTable->rowSize > 0 && col == 0) {
				rows[pOperTable->rowSize - 1] = atof(ps);
			}
			else {
				data[(pOperTable->rowSize - 1) * pOperTable->colSize + col] = atof(ps);
				col++;
			}

			while (*pe < '-' && *pe > '9') {
				if (*pe == '\n') {
					col = 0;
					pOperTable->rowSize++;
				}
				else if (*pe == '\0')
					break;
				pe++;
			}
			ps = pe;
		}
	}

	int dataSize = pOperTable->colSize * pOperTable->rowSize * 4;
	memcpy_s(((char*)pOperTable) + sizeof(operTable), dataSize, data, dataSize);
	return dataSize;
}

int hc::transCLNode(char* pOperStart, char** pOperNode, tinyxml2::XMLElement* node) {
	if (!pOperStart || !node || !pOperNode) return -1;
	if (((char*)pOperNode - (char*)pOperStart) >= MAX_OPER_SIZE) return -2;

	operBase* pOper = (operBase*)pOperNode;
	pOper->type = getOperType(node->Name());

	auto pSubSeek = node->FirstChildElement();
	std::vector<char*> subOpers;
	pointerSubOper* pSubOper = nullptr;

	switch (pOper->type) {
	case OPER_TYPE_VALUE:
		pOper->subOperNum = 0;
		pOper->value = atof(node->GetText());
		*pOperNode += sizeof(operBase);
		break;
	case OPER_TYPE_PROPERTY:
		((operProperty*)pOper)->base.subOperNum = 0;
		((operProperty*)pOper)->id = getPropertyId(node->GetText());
		((operProperty*)pOper)->nFlag = node->GetText()[0] == '-';
		*pOperNode += sizeof(operProperty);
		break;
	case OPER_TYPE_SUM:
	case OPER_TYPE_PRODUCT:
	case OPER_TYPE_DIVIDE:
	case OPER_TYPE_SWITCH:
		while (pSubSeek) {
			subOpers.push_back(*pOperNode);
			transCLNode(pOperStart, pOperNode, pSubSeek);
			pSubSeek = pSubSeek->NextSiblingElement();
		}
		pOper->subOperNum = subOpers.size();
		for (int i = 0; i < pOper->subOperNum; i++) {
			pSubOper = (pointerSubOper*)(*pOperNode + sizeof(operBase) + i * sizeof(pointerSubOper));
			pSubOper->addr = subOpers[i] - pOperStart;
		}
		*pOperNode += sizeof(operBase) + pOper->subOperNum * sizeof(pointerSubOper);
		if (pOper->type != OPER_TYPE_SWITCH) break;

		break;
	case OPER_TYPE_LIMIT:
		((operLimit*)pOper)->base.subOperNum = 1;
		((operLimit*)pOper)->max = atof(node->Attribute("max"));
		((operLimit*)pOper)->min = atof(node->Attribute("min"));
		((operLimit*)pOper)->subAddr = *pOperNode - pOperStart;
		transCLNode(pOperStart, pOperNode, pSubSeek);
		*pOperNode += sizeof(operLimit);
		break;
	case OPER_TYPE_TABLE:
		((operTable*)pOper)->base.subOperNum = 0;
		((operTable*)pOper)->colId = getPropertyId(node->Attribute("col"));
		((operTable*)pOper)->rowId = getPropertyId(node->Attribute("row"));
		*pOperNode += sizeof(operTable) + parseTable(node->GetText(), (operTable*)pOper);
		break;

	default:
		break;
	}



}