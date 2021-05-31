#include "anya.h"

#include <vector>
#include <cstdlib>
#include <algorithm>

#include <iostream>
using std::cout, std::endl;

namespace NAnya {

dbl TAnya::Run(int xst, int yst, int xfin, int yfin, std::vector<std::pair<int, int>>& path) {
    path.clear();

    TPosition st{xst, yst}, fin{xfin, yfin};

    if (st == fin) {
        path.push_back({xst, yst});
        return 0;
    } 

    InitializeSearch(st, fin);

    bool pathFound = false;
    dbl cost = INF;
    uint goal;


    while (!OpenList.empty()) {
        uint cur = Top().StateID;
        Pop();


        if (States[cur].I.RowID == yfin &&
                States[cur].I.Left - EPS < xfin && States[cur].I.Right + EPS > xfin){
            pathFound = true;
            cost = States[cur].g + EuclidianDistance(States[cur].r, fin);
            goal = cur;
            break;
        }

        for (uint succ : Successors(States[cur].r, States[cur].I, fin)) {
            if (States[succ].State != EState::STATE_IN_CLOSED) {
                dbl g = States[cur].g + EuclidianDistance(States[cur].r, States[succ].r);
                if (g + EPS < States[succ].g) {
                    States[succ].g = g;
                    States[succ].Parent = cur;
                    AddToOpen(succ);
                }
            }
        }
    }

    if (pathFound) {
        path.push_back({fin.x, fin.y});
        uint cur = goal;

        while (States[cur].r != st) {
            uint prev = States[cur].Parent;
            if (States[cur].r != States[prev].r)
                path.push_back({States[cur].r.x, States[cur].r.y});
            cur = prev;
        }

        path.push_back({st.x, st.y});
        std::reverse(path.begin(), path.end());
    }

    return cost;
}

TAnya::TAnya(const TMap& map) {
    Height = map.GetHeight() + 2;
    Width = map.GetWidth() + 2;

    MapGrid.resize(Width, std::vector<bool>(Height));
    for (uint i = 0; i < Width - 2; i++)
        for (uint j = 0; j < Height - 2; j++)
            MapGrid[i + 1][j + 1] = map.CellIsClear(j, i);

    Corners.resize(Width - 1);
    for (auto& row : Corners)
        row.resize(Height - 1);

    for (int j = 0; j < Height - 1; j++){
        bool topBlocked = true, bottomBlocked = true;
        int clearance = 0, topClearance = 0, bottomClearance = 0;

        for (int i = 0; i < Width - 1; i++){
            bool nextTopBlocked = !MapGrid[i + 1][j];
            bool nextBottomBlocked = !MapGrid[i + 1][j + 1];

            if (topBlocked && bottomBlocked)
                clearance = 0;

            Corners[i][j].IntervalClearance[0] = clearance;
            Corners[i][j].IntervalBlocked[0][0] = topBlocked;
            Corners[i][j].IntervalBlocked[1][0] = bottomBlocked;

            if (topBlocked)
                topClearance = 0;

            if (bottomBlocked)
                bottomClearance = 0;

            Corners[i][j].Clearance[0][0] = topClearance;
            Corners[i][j].Clearance[1][0] = bottomClearance;

            if (topBlocked != nextTopBlocked || bottomBlocked != nextBottomBlocked) {
                clearance = 0;
                topBlocked = nextTopBlocked;
                bottomBlocked = nextBottomBlocked;
            }
            
            clearance++;
            topClearance++;
            bottomClearance++;
        }
    }

    for (int j = 0; j < Height - 1; j++){
        bool topBlocked = true, bottomBlocked = true;
        int clearance = 0, topClearance = 0, bottomClearance = 0;

        for (int i = Width - 2; i >= 0; i--){
            bool nextTopBlocked = !MapGrid[i][j];
            bool nextBottomBlocked = !MapGrid[i][j + 1];

            if (topBlocked && bottomBlocked)
                clearance = 0;

            Corners[i][j].IntervalClearance[1] = clearance;
            Corners[i][j].IntervalBlocked[0][1] = topBlocked;
            Corners[i][j].IntervalBlocked[1][1] = bottomBlocked;

            if (topBlocked)
                topClearance = 0;

            if (bottomBlocked)
                bottomClearance = 0;

            Corners[i][j].Clearance[0][1] = topClearance;
            Corners[i][j].Clearance[1][1] = bottomClearance;

            if (topBlocked != nextTopBlocked || bottomBlocked != nextBottomBlocked) {
                clearance = 0;
                topBlocked = nextTopBlocked;
                bottomBlocked = nextBottomBlocked;
                Corners[i][j].IsTransitionPoint = true;
            } else
                Corners[i][j].IsTransitionPoint = false;

            int nw = MapGrid[i][j], ne = MapGrid[i][j + 1];
            int sw = MapGrid[i + 1][j], se = MapGrid[i + 1][j + 1];
            int summa = nw + ne + sw + se;
            Corners[i][j].IsConvexCorner = (summa == 2 && (nw + se) % 2 == 0) || summa == 3;

            clearance++;
            topClearance++;
            bottomClearance++;
        }
    }

}

TAnyaStateKey TAnya::GenerateStateKey(TPosition r, const TInterval& I) const {
    ull hash = (((I.GetHash() << 12) + r.x) << 12) + r.y;
    return TAnyaStateKey{hash, I.Left, I.Right};
}

uint TAnya::GenerateState(TPosition r, const TInterval& I, TPosition fin) {
    TAnyaStateKey key = GenerateStateKey(r, I);
    uint id = AnyaHashMap.Get(key);
    
    if (id != ANYA_HASHMAP_NO_KEY)
        return id;

    id = States.size();
    States.push_back(TState{r, I});
    AnyaHashMap.Add(key, id);

    States.back().h = Heuristic(r, I, fin);
    States.back().g = INF;
    States.back().State = EState::STATE_UNTRACKED;

    return id;
}

std::vector<uint> TAnya::Intervals(int row, dbl left, dbl right, TPosition r, TPosition fin) {
    std::vector<TInterval> intervals;

    int xLeft = LeftColumn(left);
    int xRight = RightColumn(right);
    
    for (int x = xLeft; x < xRight;){
        int delta = Corners[x][row].IntervalClearance[1];

        TInterval I(row, x, x + delta);
        I.Blocked[0] = Corners[x][row].IntervalBlocked[0][1];
        I.Blocked[1] = Corners[x][row].IntervalBlocked[1][1];
        intervals.push_back(I);

        x += delta;
        if (delta == 0)
            break;
    }

    if (!intervals.empty()) {
        intervals[0].Left = left;
        intervals.back().Right = right;
        intervals.back().IRight = xRight;
    }

    std::vector<uint> ids;
    for (auto& I : intervals)
        ids.push_back(GenerateState(r, I, fin));
    return ids;
}

TInterval TAnya::GetLeftInterval(int x, int y) const {
    TInterval I(y, x - Corners[x][y].IntervalClearance[0], x);
    I.Blocked[0] = Corners[x][y].IntervalBlocked[0][0];
    I.Blocked[1] = Corners[x][y].IntervalBlocked[1][0];

    return I;
}

TInterval TAnya::GetRightInterval(int x, int y) const {
    TInterval I(y, x, x + Corners[x][y].IntervalClearance[1]);
    I.Blocked[0] = Corners[x][y].IntervalBlocked[0][1];
    I.Blocked[1] = Corners[x][y].IntervalBlocked[1][1];

    return I;
}

void TAnya::LeftIntervals(dbl fx, int y, int dir,
        TPosition r, TPosition fin, std::vector<uint>& successors, dbl bound) {
    int x = LeftColumn(fx);
    dbl leftBound = std::max(static_cast<dbl>(x) - Corners[x][y].Clearance[dir][0], bound);
    if (leftBound < fx){
        for (auto& successor : Intervals(y, leftBound, fx, r, fin))
            successors.push_back(successor);
    }
}

void TAnya::RightIntervals(dbl fx, int y, int dir,
        TPosition r, TPosition fin, std::vector<uint>& successors, dbl bound) {
    int x = RightColumn(fx);
    dbl rightBound = std::min(static_cast<dbl>(x) + Corners[x][y].Clearance[dir][1], bound);
    if (rightBound > fx){
        for (auto& successor : Intervals(y, fx, rightBound, r, fin))
            successors.push_back(successor);
    }
}

std::vector<uint> TAnya::Successors(TPosition r, TInterval I, TPosition fin) {
    if (I.Right - I.Left < EPS)
        return {};

    return r.y == I.RowID ? ThisRowSuccessors(r, I, fin) : OtherRowSuccessors(r, I, fin);
}

std::vector<uint> TAnya::ThisRowSuccessors(TPosition r, const TInterval& I, TPosition fin) {
    std::vector<uint> successors;
    if (r.x >= I.IRight) {
        if (Corners[I.ILeft][I.RowID].IntervalClearance[0] > 0) {
            successors.push_back(GenerateState(r, GetLeftInterval(I.ILeft, I.RowID), fin));
            if (I.Blocked[0])
                LeftIntervals(I.ILeft, I.RowID - 1, 1, TPosition(I.ILeft, I.RowID), fin, successors);
            if (I.Blocked[1])
                LeftIntervals(I.ILeft, I.RowID + 1, 0, TPosition(I.ILeft, I.RowID), fin, successors);
        }
    }
    if (r.x <= I.ILeft) {
        if (Corners[I.IRight][I.RowID].IntervalClearance[1] > 0) {
            successors.push_back(GenerateState(r, GetRightInterval(I.IRight, I.RowID), fin));
            if (I.Blocked[0])
                RightIntervals(I.IRight, I.RowID - 1, 1, TPosition(I.IRight, I.RowID), fin, successors);
            if (I.Blocked[1])
                RightIntervals(I.IRight, I.RowID + 1, 0, TPosition(I.IRight, I.RowID), fin, successors);
        }
    }
    return successors;
}

std::vector<uint> TAnya::OtherRowSuccessors(TPosition r, const TInterval& I, TPosition fin) {
    int dir = 0, newRow = I.RowID - 1;
    if (I.RowID > r.y) {
        dir = 1;
        newRow = I.RowID + 1;
    }
    dbl leftIntersection = GetIntersectingX(r.x, r.y, I.Left, I.RowID, newRow);
    dbl rightIntersection = GetIntersectingX(r.x, r.y, I.Right, I.RowID, newRow);


    int leftBound = I.ILeft - Corners[I.ILeft][I.RowID].Clearance[dir][0];
    int rightBound = I.IRight + Corners[I.IRight][I.RowID].Clearance[dir][1];

    dbl left = leftIntersection < leftBound ? leftBound : leftIntersection;
    dbl right = rightIntersection > rightBound ? rightBound : rightIntersection;
   
    std::vector<uint> successors;
    if (!I.Blocked[dir]) {
        successors = Intervals(newRow, left, right, r, fin);
    }

    if (abs(I.Left - I.ILeft) < EPS && Corners[I.ILeft][I.RowID].IsConvexCorner) {
        if (Corners[I.ILeft][I.RowID].IntervalBlocked[1 - dir][0])
            successors.push_back(GenerateState(TPosition(
                I.ILeft, I.RowID), GetLeftInterval(I.ILeft, I.RowID), fin));
        if (Corners[I.ILeft][I.RowID].IntervalBlocked[dir][1] &&
                Corners[I.ILeft][I.RowID].IntervalBlocked[1 - dir][0])
             LeftIntervals(I.ILeft, newRow, 1 - dir, TPosition(I.ILeft, I.RowID), fin, successors);
        else {
            if (Corners[I.ILeft][I.RowID].IntervalBlocked[dir][0] && I.ILeft > r.x)
                RightIntervals(
                    I.ILeft, newRow, 1 - dir, TPosition(I.ILeft, I.RowID), fin, successors, left);
            if (Corners[I.ILeft][I.RowID].IntervalBlocked[1 - dir][0] && left == leftIntersection)
                LeftIntervals(left, newRow, 1 - dir, TPosition(I.ILeft, I.RowID), fin, successors);
        }
    }
    
    if (abs(I.Right - I.IRight) < EPS && Corners[I.IRight][I.RowID].IsConvexCorner) {
        if (Corners[I.IRight][I.RowID].IntervalBlocked[1 - dir][1])
            successors.push_back(GenerateState(TPosition(
                I.IRight, I.RowID), GetRightInterval(I.IRight, I.RowID), fin));
        if (Corners[I.IRight][I.RowID].IntervalBlocked[dir][0] &&
                Corners[I.IRight][I.RowID].IntervalBlocked[1 - dir][1])
             RightIntervals(I.IRight, newRow, 1 - dir, TPosition(I.IRight, I.RowID), fin, successors);
        else {
            if (Corners[I.IRight][I.RowID].IntervalBlocked[dir][1] && I.IRight < r.x)
                LeftIntervals(
                    I.IRight, newRow, 1 - dir, TPosition(I.IRight, I.RowID), fin, successors, right);
            if (Corners[I.IRight][I.RowID].IntervalBlocked[1 - dir][1] && right == rightIntersection)
                RightIntervals(right, newRow, 1 - dir, TPosition(I.IRight, I.RowID), fin, successors);
        }
    }

    return successors;
}

void TAnya::InitializeSearch(TPosition st, TPosition fin) {
    Reset();

    std::vector<uint> initialStates;

    for (int dir = 0; dir < 2; dir++){
        int leftBound = st.x - Corners[st.x][st.y].Clearance[dir][0];
        int rightBound = st.x + Corners[st.x][st.y].Clearance[dir][1];

        if (leftBound != rightBound) {
            int delta = dir * 2 - 1;
            for (auto state : Intervals(st.y + delta, leftBound, rightBound, st, fin))
                initialStates.push_back(state);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (Corners[st.x][st.y].IntervalClearance[i] > 0) {
            TInterval I = i == 0 ? GetLeftInterval(st.x, st.y) : GetRightInterval(st.x, st.y);
            initialStates.push_back(GenerateState(st, I, fin));
        }
    }

    for (uint stateID : initialStates) {
        States[stateID].g = 0;
        AddToOpen(stateID);
    }
}

dbl TAnya::EuclidianDistance(TPosition lhs, TPosition rhs) {
    dbl dx = abs(lhs.x - rhs.x), dy = abs(lhs.y - rhs.y);
    return sqrt(dx * dx + dy * dy);
}

dbl TAnya::Heuristic(TPosition r, const TInterval& I, TPosition fin) {
    return 0.;
}

void TAnya::Reset() {
    States.clear();
    AnyaHashMap.Clear();
    OpenList.clear();
}

void TAnya::AddToOpen(uint id) {
    uint index = OpenList.size();
    if (States[id].State == EState::STATE_IN_OPEN)
        index = States[id].Index;
    else {
        States[id].Index = index;
        States[id].State = EState::STATE_IN_OPEN;
        OpenList.push_back(THeapElementh{id, 0, 0});
    }

    OpenList[index].g = States[id].g;
    OpenList[index].F = States[id].g + States[id].h;
    PushUp(index);
}

void TAnya::Pop() {
    States[OpenList[0].StateID].State = EState::STATE_IN_CLOSED;
    HeapSwap(0, static_cast<uint>(OpenList.size()) - 1);
    OpenList.pop_back();
    if (!OpenList.empty())
        PushDown(0);
}

void TAnya::PushUp(uint index) {
    while (index > 0 && OpenList[index].F < OpenList[(index - 1) >> 1].F) {
        uint parent = (index - 1) >> 1;
        HeapSwap(index, parent);
        index = parent;
    }
}

void TAnya::PushDown(uint index) {
    while (index * 2 + 1 < OpenList.size()) {
        uint ch1 = index * 2 + 1, ch2 = index * 2 + 2;
        uint ch = ch2 >= OpenList.size() || OpenList[ch1].F < OpenList[ch2].F ? ch1 : ch2;
       
        if (OpenList[index].F < OpenList[ch].F)
            break;

        HeapSwap(index, ch);
        index = ch;
    }
}

void TAnya::HeapSwap(uint lhs, uint rhs) {
    std::swap(States[OpenList[lhs].StateID].Index, States[OpenList[rhs].StateID].Index);
    std::swap(OpenList[lhs], OpenList[rhs]);
}

} // namespace NAnya
