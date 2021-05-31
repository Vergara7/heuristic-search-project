#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "map.h"
#include "node.h"

auto BuildKNeighborsShifts(int maxk){
    std::vector<std::vector<std::pair<int, int>>> shifts(maxk);
    shifts[0] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (int k = 1; k < maxk; k++){
        int sz = shifts[k - 1].size();
        for (int i = 0; i < sz; i++){
            shifts[k].push_back(shifts[k - 1][i]);
            int dx = shifts[k - 1][i].first + shifts[k - 1][(i + 1) % sz].first;
            int dy = shifts[k - 1][i].second + shifts[k - 1][(i + 1) % sz].second;
            shifts[k].push_back({dx, dy});
        }
    }
    return shifts;
}

const auto KNeighborsShifts = BuildKNeighborsShifts(12);

std::vector<std::shared_ptr<TNode>> GetKNeighbors(const TNode& node, const TMap& map, int k) {
    std::vector<std::shared_ptr<TNode>> neighbors;
    for (auto[dx, dy] : KNeighborsShifts[k - 1]){
        if (!map.NodeIsClear(node.x + dx, node.y + dy))
            continue;
        if (map.PathIsClear(node.x, node.y, node.x + dx, node.y + dy))
            neighbors.push_back(std::make_shared<TNode>(node.x + dx, node.y + dy));
    }
    return neighbors;
}
