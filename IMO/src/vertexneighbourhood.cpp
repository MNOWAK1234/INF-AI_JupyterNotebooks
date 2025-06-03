#include "vertexneighbourhood.h"

VertexNeighbourhood::VertexNeighbourhood(int p, int m, int n) : prev(p), mid(m), next(n) {};

bool VertexNeighbourhood::operator==(const VertexNeighbourhood &other) const
{
    return mid == other.mid &&
            ((prev == other.prev && next == other.next) ||
            (prev == other.next && next == other.prev));
}

bool VertexNeighbourhood::operator!=(const VertexNeighbourhood &other) const
{
    return !(*this == other);
}