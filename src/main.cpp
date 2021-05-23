#include <cassert>
#include <iostream>
#include <filesystem>

#include "algo/astar.h"

#include "common/map.h"
#include "common/metrics.h"

namespace fs = std::__fs::filesystem;

int main(int argc, char* argv[]){
// for (const auto& entry : fs::directory_iterator(argv[2]))
//         std::cout << entry.path() << std::endl;
    auto path = "../dataset/my-life-for-the-horde/maps/battleground.map";
    auto result = AStar(221, 102, 221, 101, TMap(path), EuclidianDistance, TrivialHeuristic);
    for (auto node : RestorePath(result.FinishNode))
        std::cout << node.x << ' ' << node.y << std::endl;
    std::cout << std::endl << result.NodesInOpen << ' ' << result.NodesInClosed << std::endl;
    return 0;
}
