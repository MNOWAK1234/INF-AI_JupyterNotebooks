#ifndef MOVE_H
#define MOVE_H

#include "vertexneighbourhood.h"
#include "cut.h"

enum moveType
{
    VERTEX = 0,
    EDGE = 1
};

struct Move
{
    moveType type;
    double delta;

    // Optional fields depending on type
    VertexNeighbourhood neighbourhood1{0, 0, 0}; 
    VertexNeighbourhood neighbourhood2{0, 0, 0};
    Cut cut1{0, 0};
    Cut cut2{0, 0};

    Move();
    Move(moveType t, double d);

    // Sorting by delta (min-heap priority queue)
    bool operator<(const Move &other) const;
};

struct VertexMove : public Move
{
    VertexMove(const VertexNeighbourhood &v1, const VertexNeighbourhood &v2, double d);
};

struct EdgeMove : public Move
{
    EdgeMove(const Cut &c1, const Cut &c2, double d);

    bool operator==(const EdgeMove &other) const;

    bool operator!=(const EdgeMove &other) const;
};

#endif