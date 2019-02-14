#pragma once
#include<cstdint>
#include "SmartsuitDef.h"
class SmartsuitLib
{
public:
	SmartsuitLib();
	~SmartsuitLib();
	SuitDataDef* ParseFrame(uint8_t *data, int bytes_read);
	HubInfo* ParseInfo(uint8_t *data);
	void ParseBody(Body* body, uint8_t *data);
};

