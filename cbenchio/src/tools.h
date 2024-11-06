#include <array>

auto toInt(std::array<size_t,3> shortVec )
{
    std::array<int,3> shortVecInt { (int)(shortVec[0]),(int)(shortVec[1]),(int)(shortVec[2]) };

    return shortVecInt;
}