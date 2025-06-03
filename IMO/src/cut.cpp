#include "cut.h"

Cut::Cut() : prev(-1), next(-1) {};

Cut::Cut(int p, int n) : prev(p), next(n) {};

bool Cut::operator==(const Cut &other) const
{
    return (prev == other.prev && next == other.next) ||
            (prev == other.next && next == other.prev);
}

bool Cut::operator!=(const Cut &other) const
{
    return !(*this == other);
}

bool Cut::isReversedOf(const Cut &other) const
{
    return (prev == other.next && next == other.prev);
}

bool Cut::isSameDirectionAs(const Cut &other) const
{
    return (prev == other.prev && next == other.next);
}