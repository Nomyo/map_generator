#pragma once

#include <ostream>

enum ColorCode
{
    FG_DEFAULT = 39,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_LIGHT_GRAY = 37,
    FG_DARK_GRAY = 90,
    FG_LIGHT_RED = 91,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_YELLOW = 93,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_CYAN = 96,
    FG_LIGHT_WHITE = 97
};

class ColorModifier
{

public:
    ColorModifier(ColorCode ncode)
	: color_code(ncode) { }

    friend std::ostream&
    operator<<(std::ostream& os, const ColorModifier& modifier)
	{
	    return os << "\033[" << modifier.color_code << "m";
	}

private:
    ColorCode color_code;
};
