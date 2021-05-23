#pragma once

#include <memory>
#include <iostream>

#include "common/map.h"
#include "common/node.h"
#include "common/run_result.h"
#include "common/successors_util.h"

#include "util/open.h"
#include "util/closed.h"

template<class CostFunc, class HeuristicFunc>
TRunResult AStar(uint xst, uint yst, uint xfin, uint yfin,
        const TMap& map, CostFunc computeCost, HeuristicFunc heuristic)
{
    TOpen open;
    TClosed closed;
    auto f = std::make_shared<TNode>(xfin, yfin);
    open.Push(std::make_shared<TNode>(xst, yst, 0));

    while (!open.IsEmpty()) {
        auto v = open.Pop();
        if (closed.Contains(v))
            continue;
        closed.Push(v);
        if (v->x == xfin && v->y == yfin)
            return TRunResult(v, open.Size(), closed.Size());
        for (auto u : GetNeighbors(*v, map)){
            if (!open.Contains(u) && !closed.Contains(u))
                open.Push(std::make_shared<TNode>(u->x, u->y,
                    v->g + computeCost(*v, *u), heuristic(*u, *f), v)); 
        }
    }
   
    return TRunResult(nullptr, open.Size(), closed.Size());     
}

