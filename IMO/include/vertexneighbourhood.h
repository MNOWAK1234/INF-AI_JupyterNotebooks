#ifndef VERTEXNEIGHBOURHOOD_H
#define VERTEXNEIGHBOURHOOD_H

struct VertexNeighbourhood
{
    int prev;
    int mid;
    int next;

    VertexNeighbourhood(int p, int m, int n);

    bool operator==(const VertexNeighbourhood &other) const;

    bool operator!=(const VertexNeighbourhood &other) const;
};

#endif