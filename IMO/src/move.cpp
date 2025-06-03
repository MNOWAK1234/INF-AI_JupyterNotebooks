#include <vector>

#include "move.h"

Move::Move() : type(VERTEX), delta(0.0) {};

Move::Move(moveType t, double d) : type(t), delta(d) {};

bool Move::operator<(const Move &other) const
{
    return delta > other.delta;
}

VertexMove::VertexMove(const VertexNeighbourhood &v1, const VertexNeighbourhood &v2, double d) : Move(VERTEX, d)
{
    neighbourhood1 = v1;
    neighbourhood2 = v2;
}

EdgeMove::EdgeMove(const Cut &c1, const Cut &c2, double d) : Move(EDGE, d)
{
    cut1 = c1;
    cut2 = c2;
}

bool EdgeMove::operator==(const EdgeMove &other) const
{
    // Case 1: same order
    if (cut1 == other.cut1 && cut2 == other.cut2)
    {
        return ((cut1.isSameDirectionAs(other.cut1) && cut2.isSameDirectionAs(other.cut2)) || (cut1.isReversedOf(other.cut1) && cut2.isReversedOf(other.cut2)));
    }

    // Case 2: reversed order
    if (cut1 == other.cut2 && cut2 == other.cut1)
    {
        return ((cut1.isSameDirectionAs(other.cut2) && cut2.isSameDirectionAs(other.cut1)) || (cut1.isReversedOf(other.cut2) && cut2.isReversedOf(other.cut1)));
    }

    return false;
}

bool EdgeMove::operator!=(const EdgeMove &other) const
{
    return !(*this == other);
}

