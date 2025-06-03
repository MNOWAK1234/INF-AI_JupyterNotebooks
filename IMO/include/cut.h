#ifndef CUT_H
#define CUT_H

struct Cut
{
    int prev;
    int next;

    Cut();

    Cut(int p, int n);

    bool operator==(const Cut &other) const;

    bool operator!=(const Cut &other) const;

    bool isReversedOf(const Cut &other) const;

    bool isSameDirectionAs(const Cut &other) const;
};

#endif