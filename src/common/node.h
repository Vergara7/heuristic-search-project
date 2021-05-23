#pragma once

#include <memory>
#include <limits.h>
#include <cstdlib>

using dbl = long double;
using ll = long long;

const dbl EPS = 1e-6;

struct TNode{
    int x, y;
    dbl g, h, F;
    std::shared_ptr<TNode> Parent;

    TNode(int x, int y, dbl g=-1, dbl h=0, std::shared_ptr<TNode> parent=nullptr);
    ll Hash() const {
        return static_cast<ll>(x) * INT_MAX + y; 
    }
};

bool operator<(const TNode& lhs, const TNode& rhs) {
    return lhs.F + EPS < rhs.F;
}

struct TNodeGreaterComp{
    bool operator()(const std::shared_ptr<TNode>& lhs, const std::shared_ptr<TNode>& rhs) {
        return *rhs < *lhs;
    }
};

TNode::TNode(int x, int y, dbl g, dbl h, std::shared_ptr<TNode> parent) :
    x(x),
    y(y),
    g(g == -1 ? std::numeric_limits<dbl>::max() : g),
    h(h),
    F(g + h),
    Parent(std::move(parent)) {}
