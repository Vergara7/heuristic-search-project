#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "algo/anya.h"

#include "common/map.h"
#include "common/scen.h"

namespace fs = std::__fs::filesystem;

int main(int argc, char* argv[]){
    auto datasetPath = argv[1];
    auto artifactsPath = fs::path(argv[2]);

    fs::create_directory(artifactsPath);
    artifactsPath /= "anya";
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
            NAnya::TAnya anyaAlgo(map);
            for (auto scen : scens){
                if (scen.Level >= 7)
                    continue;
                std::vector<std::pair<int, int>> path;
                anyaAlgo.Run(scen.xst, scen.yst, scen.xfin, scen.yfin, path);
                if (path.size() == 0) {
                    out << "No path???" << std::endl;
                    cntFailed++;
                }
                for (auto node : path)
                    out << node.first << ' ' << node.second << ' ';
                out << std::endl;
            }
            if (cntFailed)
                std::cout << "    Could not find path for " << cntFailed << " scens" << endl;
        }
    }
    return 0;
}
