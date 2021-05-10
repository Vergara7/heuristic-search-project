#pragma once

#include <queue>
#include <memory>

#include "map.h"
#include "open.h"
#include "node.h"
#include "run_result.h"

using Func = dbl(const TNode*, const TNode*);

TRunResult AStar(uint xst, uint yst, uint xfin, uint yfin,
        const TMap& map, Func computeCost, Func heuristic)
{
    TOpen open;
    open.Push(std::make_shared<TNode>(xst, yst));

    while (!open.IsEmpty()) {

    }
    
    return TRunResult();     
}

