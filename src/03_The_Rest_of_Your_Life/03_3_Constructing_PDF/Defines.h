#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <Public/Ingredients/defines.h>

namespace Define
{
    const std::string str_Chapter = "03_The_Rest_of_Your_Life";
	const std::string str_Subchapter = "03_3_Constructing_PDF";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;
	inline float pdf_t(float x) {
		return 0.5f * x;
	}
}

#endif // !_DEFINES_H_
