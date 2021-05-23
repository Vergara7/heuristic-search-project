#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "algo/astar.h"

#include "common/map.h"
#include "common/scen.h"
#include "common/metrics.h"

namespace fs = std::__fs::filesystem;

auto CreateAlgo(const std::string& algo_name){
    if (algo_name == "astar")
        return [](int xs, int ys, int xf, int yf, TMap map){
            return AStar(xs, ys, xf, yf, map, EuclidianDistance, DiagonalDistance);
        };
    assert(false);
}


int main(int argc, char* argv[]){
    auto algoName = argv[1];
    auto datasetPath = argv[2];
    auto artifactsPath = fs::path(argv[3]);

    auto algo = CreateAlgo(algoName);

    fs::create_directory(artifactsPath);
    fs::create_directory(artifactsPath / algoName);
    for (const auto& datasetsDir : fs::directory_iterator(datasetPath)){
        assert(datasetsDir.is_directory());
        fs::create_directory(artifactsPath / algoName / datasetsDir.path().filename().filename());
        for (const auto& scensFile : fs::directory_iterator(datasetsDir.path() / "scens")){
            std::vector<TScen> scens = LoadScens(scensFile.path());
            std::cout << "Running scens on map " << scens[0].MapName << 
                " from dataset " << std::string(datasetsDir.path().filename()) << std::endl;

            TMap map(datasetsDir.path() / "maps" / scens[0].MapName);
            auto save_path = artifactsPath / algoName /
                 datasetsDir.path().filename().filename() / scensFile.path().filename();
           
            std::ofstream out(save_path);
            for (auto scen : scens){
                if (scen.Level >= 2)
                    continue;
                auto result = algo(scen.xst, scen.yst, scen.xfin, scen.yfin, map);
                auto path = RestorePath(result.FinishNode);
                if (path.size() == 0)
                    out << "No path???" << std::endl;
                for (auto node : path)
                    out << node.x << ' ' << node.y << ' ';
                out << std::endl;
                out << result.NodesInOpen << ' ' << result.NodesInClosed << std::endl;
            }
        }
    }
    return 0;
}
