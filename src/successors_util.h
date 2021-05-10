#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "map.h"
#include "node.h"

std::vector<std::shared_ptr<TNode>> GetNeighbors(const TNode& node, const TMap& map) {
    std::vector<std::shared_ptr<TNode>> neighbors;
    for (int dx = -1; dx <= 1; dx++){
        for (int dy = -1; dy <= 1; dy++){
            if (!map.Inside(node.x + dx, node.y + dy))
                continue;
            if (dx * dy == 0) {
                if (dx + dy != 0)
                    neighbors.push_back(std::make_shared<TNode>(node.x + dx, node.y + dy));
            } else if (map.IsTraversable(std::min(node.x, node.x + dx), std::min(node.y, node.y + dy)))
                neighbors.push_back(std::make_shared<TNode>(node.x + dx, node.y + dy));
        }
    }
    return neighbors;
}
