#include "Rainbow.hpp"



QColor Rainbow::getColor(int i, int a, int l)
{
    return QColor::fromHsl( abs(i) * 31 % 360, 200, l, a);
}
