/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file clp.h
* @author Simon
* @brief Control Loading Parameters
*/
#pragma once
#if defined(WIN32) || defined(_WIN64)
#include <cstdint>
#endif // X86

#define MAX_OPER_SIZE (256 * 1024) // 256 KB

#define MAX_TABLE_COL 64
#define MAX_TABLE_ROW 64
#define MAX_TABLE_SIZE (MAX_TABLE_COL * MAX_TABLE_ROW)

enum eOperType
{
	OPER_TYPE_VALUE = 0,
	OPER_TYPE_PROPERTY,
	OPER_TYPE_SUM,
	OPER_TYPE_PRODUCT,
	OPER_TYPE_DIVIDE,
	OPER_TYPE_LIMIT,
	OPER_TYPE_TABLE,
	OPER_TYPE_SWITCH,
};

#pragma pack(push, 1)
struct pointerSubOper {
	uint32_t addr;
};

struct pointerSwitchSubOper {
	float cond;
	uint8_t id;
	uint32_t addr;
};

struct operBase {
	float value;
	uint8_t type;
	uint8_t subOperNum;
};

struct operProperty {
	operBase base;
	uint8_t id;
	uint8_t nFlag;
};

struct operLimit {
	operBase base;
	float max;
	float min;
	uint32_t subAddr;
};

struct operTable {
	operBase base;
	uint8_t colId;
	uint8_t rowId;
	uint8_t colSize;
	uint8_t rowSize;
};
#pragma pack(pop)