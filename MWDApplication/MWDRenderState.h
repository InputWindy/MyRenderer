#pragma once
#include "headers.h"
class MWDRenderState
{
public:
	static inline MWDRenderState* defaultRenderState = NULL;
	static MWDRenderState* GetDefault() {
		if (!defaultRenderState) {
			defaultRenderState = new MWDRenderState();
		}
		return defaultRenderState;
	}
};

