#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "algo/astar.h"

#include "common/map.h"
#include "common/scen.h"
#include "common/metrics.h"
#include "common/successors_util.h"

namespace fs = std::__fs::filesystem;

using AlgoFunc = std::function<TRunResult(uint, uint, uint, uint, const TMap&)>;

AlgoFunc CreateAlgo(const std::string& algo_name, const std::string heuristicName){
    std::function<dbl(const TNode&, const TNode&)> heuristic = TrivialHeuristic;
    if (heuristicName == "euclidian")
        heuristic = EuclidianDistance;
    if (heuristicName == "manhattan")
        heuristic = ManhattanDistance;
    if (heuristicName == "chebyshev")
        heuristic = ChebyshevDistance;
    if (heuristicName == "diagonal")
        heuristic = DiagonalDistance;
    if (algo_name == "astar"){
        auto getNeighbors = [=](const TNode& node, const TMap& map){
            return GetKNeighbors(node, map, 2);
        };
        return [=](int xs, int ys, int xf, int yf, TMap map){
            return AStar(xs, ys, xf, yf, map, getNeighbors, EuclidianDistance, heuristic, false);
        };
    }
    if (algo_name.substr(0, 2) == "2k"){
        int k = std::stoi(algo_name.substr(3));
        auto getNeighbors = [=](const TNode& node, const TMap& map){
            return GetKNeighbors(node, map, k);
        };
        return [=](int xs, int ys, int xf, int yf, TMap map){
            return AStar(xs, ys, xf, yf, map, getNeighbors, EuclidianDistance, heuristic, false);
        };
    }
    if (algo_name == "theta"){
        auto getNeighbors = [=](const TNode& node, const TMap& map){
            return GetKNeighbors(node, map, 2);
        };
        return [=](int xs, int ys, int xf, int yf, TMap map){
            return AStar(xs, ys, xf, yf, map, getNeighbors, EuclidianDistance, heuristic, false);
        };
    }
    assert(false);
}

int main(int argc, char* argv[]){
    auto algoName = std::string(argv[1]);
    auto heuristicName = std::string(argv[2]);
    auto datasetPath = argv[3];
    auto artifactsPath = fs::path(argv[4]);

    auto algo = CreateAlgo(algoName, heuristicName);

    fs::create_directory(artifactsPath);
    artifactsPath /= algoName + '.' + heuristicName;
    fs::create_directory(artifactsPath);
    for (const auto& datasetsDir : fs::directory_iterator(datasetPath)){
        assert(datasetsDir.is_directory());
        std::string datasetName = datasetsDir.path().filename().filename();
        fs::create_directory(artifactsPath / datasetName);
        for (const auto& scensFile : fs::directory_iterator(datasetsDir.path() / "scens")){
            std::vector<TScen> scens = LoadScens(scensFile.path());
            std::cout << "Running scens on map " << scens[0].MapName << 
                " from dataset " << std::string(datasetsDir.path().filename()) << std::endl;

            TMap map(datasetsDir.path() / "maps" / scens[0].MapName);
            auto save_path = artifactsPath /
                datasetsDir.path().filename().filename() / scensFile.path().filename();
           
            std::ofstream out(save_path);
            int cntFailed = 0;
            for (auto scen : scens){
                if (scen.Level >= 7)
                    continue;
                auto result = algo(scen.xst, scen.yst, scen.xfin, scen.yfin, map);
                auto path = RestorePath(result.FinishNode);
                if (path.size() == 0) {
                    out << "No path???" << std::endl;
                    cntFailed++;
                }
                for (auto node : path)
                    out << node.x << ' ' << node.y << ' ';
                out << std::endl;
                out << "nodes expanded: " << result.NodesInOpen + result.NodesInClosed << std::endl;
            }
            if (cntFailed)
                std::cout << "    Could not find path for " << cntFailed << " scens" << endl;
        }
    }
    return 0;
}
