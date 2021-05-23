#pragma once

#include <memory>
#include <vector>

#include "node.h"

using uint = unsigned int;

struct TRunResult {
    std::shared_ptr<TNode> FinishNode;

    uint NodesInOpen;
    uint NodesInClosed;

    TRunResult(std::shared_ptr<TNode> f, uint open_size, uint closed_size) :
        FinishNode(f),
        NodesInOpen(open_size),
        NodesInClosed(closed_size) 
    {}
};

std::vector<TNode> RestorePath(std::shared_ptr<TNode> fin) {
    std::vector<TNode> path;
    if (!fin)
        return path;
    path.push_back(*fin);
    while (fin->Parent){
        fin = fin->Parent;
        path.push_back(*fin);
    }
    return path;
}
