#pragma once

#include <fstream>
#include <vector>

using uint = unsigned int;

class TMap{
public:
    TMap(std::string file_path);
    bool IsTraversable(uint i, uint j) const;
    bool Inside(int x, int y) const;

private:
    uint Height, Width;
    std::vector<std::vector<bool>> MapGrid;
};
   

TMap::TMap(std::string file_path) {
    std::ifstream in(file_path);
    std::string s;
    getline(in, s);
    assert(s == "type octile");
    in >> s;
    assert(s == "height");
    in >> Height;
    in >> s;
    assert(s == "width");
    in >> Width;
    in >> s;
    assert(s == "map");

    MapGrid.resize(Height);
    for (int i = 0; i < Height; i++)
        MapGrid[i].resize(Width);
    for (int i = 0; i < Height; i++){
        in >> s;
        assert(s.size() == Width);
        for (int j = 0; j < Width; j++){
            if (s[j] != '.' && s[j] != 'G')
                MapGrid[i][j] = true;
        }
    }
}

bool TMap::IsTraversable(uint i, uint j) const {
    return !MapGrid[i][j];
}

bool TMap::Inside(int x, int y) const {
    return x >= 0 && x <= Height && y >= 0 && y <= Width;
}
