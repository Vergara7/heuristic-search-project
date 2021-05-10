#pragma once

#include <memory>

#include "node.h"

using uint = unsigned int;

struct TRunResult {
    std::shared_ptr<TNode> FinishNode;

    uint NodesInOpen;
    uint NodesInClosed;
};
