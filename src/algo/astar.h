#pragma once

#include <memory>
#include <iostream>

#include "common/map.h"
#include "common/node.h"
#include "common/run_result.h"
#include "common/successors_util.h"

#include "util/open.h"
#include "util/closed.h"

template<class SuccessorsFunc, class CostFunc, class HeuristicFunc>
TRunResult AStar(uint xst, uint yst, uint xfin, uint yfin, const TMap& map,
        SuccessorsFunc getNeighbors, CostFunc computeCost, HeuristicFunc heuristic, bool thetaAlgo)
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
        if (v->x == xfin && v->y == yfin) {
            return TRunResult(v, open.Size(), closed.Size());
        }
        auto neighbors = getNeighbors(*v, map);
        for (auto u : getNeighbors(*v, map)){
            if (!closed.Contains(u)) {
                std::shared_ptr<TNode> prev = v;
                if (thetaAlgo && v->Parent && map.PathIsClear(v->Parent->x, v->Parent->y, u->x, u->y))
                    prev = v->Parent;
                u->g = prev->g + computeCost(*prev, *u);
                u->h = heuristic(*u, *f);
                u->F = u->g + u->h;
                u->Parent = std::move(prev);
                open.Push(std::move(u));     
            }    
        }
    }
   
    return TRunResult(nullptr, open.Size(), closed.Size());     
}

