#pragma once

#include <cmath>

#include "anya_helpers.h"

#include "common/map.h"

namespace NAnya {

constexpr dbl INF = 1e18;

class TAnya {
public:
    uint NodesExpanded = 0;
    TAnya(const TMap& map);
    dbl Run(int xst, int yst, int xfin, int yfin, std::vector<std::pair<int, int>>& path);

private:
    uint Height, Width;
    std::vector<TPosition> Path;
    std::vector<std::vector<bool>> MapGrid;
    std::vector<std::vector<TCorner>> Corners;
    std::vector<TState> States;
    std::vector<THeapElementh> OpenList;
    TAnyaHashMap AnyaHashMap;

private:
    void ComputeClearances();
    TAnyaStateKey GenerateStateKey(TPosition r, const TInterval& I) const;
    uint GenerateState(TPosition r, const TInterval& I, TPosition fin);

    TInterval GetLeftInterval(int x, int y) const;
    TInterval GetRightInterval(int x, int y) const;

    std::vector<uint> Intervals(int row, dbl left, dbl right, TPosition r, TPosition fin);

    void LeftIntervals(dbl fx, int y, int dir,
        TPosition r, TPosition fin, std::vector<uint>& successors, dbl bound = 0);
    void RightIntervals(dbl fx, int y, int dir,
        TPosition r, TPosition fin, std::vector<uint>& successors, dbl bound = INF);

    dbl GetIntersectingX(dbl x1, dbl y1, dbl x2, dbl y2, dbl y) const {
        return abs(y2 - y1) < EPS ? y1 : ((x2 - x1) / (y2 - y1)) * (y - y1) + x1;
    }

    std::vector<uint> Successors(TPosition r, TInterval I, TPosition fin);
    std::vector<uint> ThisRowSuccessors(TPosition r, const TInterval& I, TPosition fin);
    std::vector<uint> OtherRowSuccessors(TPosition r, const TInterval& I, TPosition fin);

    void InitializeSearch(TPosition st, TPosition fin);

    int LeftColumn(dbl x) { return floor(x + EPS); }
    int RightColumn(dbl x) { return ceil(x - EPS); }

    dbl EuclidianDistance(TPosition lhs, TPosition rhs);
    dbl Heuristic(TPosition r, const TInterval& I, TPosition fin);
    
    void Reset();

    void AddToOpen(uint stateID);
    THeapElementh Top() const { return OpenList.front(); }
    void Pop();
    void PushUp(uint index);
    void PushDown(uint index);
    void HeapSwap(uint lhs, uint rhs);
};

} // namespace NAnya
