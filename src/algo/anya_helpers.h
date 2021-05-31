#pragma once

#include <cmath>
#include <unordered_map>

namespace NAnya {

typedef long long ll;
typedef long double dbl;
typedef unsigned int uint;
typedef unsigned long long ull;

constexpr dbl EPS = 1e-8;

struct TPosition{
    int x, y;
    TPosition(int x = 0, int y = 0) : x(x), y(0) {}
};

inline bool operator == (const TPosition& lhs, const TPosition& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator != (const TPosition& lhs, const TPosition& rhs) {
    return !(lhs == rhs);
}

struct TCorner {
    int IntervalClearance[2];
    bool IntervalBlocked[2][2];
    int Clearance[2][2];
    bool IsTransitionPoint;
    bool IsConvexCorner;
};

struct TInterval {
    uint RowID;
    dbl Left, Right;
    uint ILeft, IRight;

    bool Blocked[2];

    TInterval(uint rowID, dbl left, dbl right) : RowID(rowID), Left(left), Right(right),
        ILeft(floor(left + EPS)), IRight(ceil(right - EPS)) {}

    ll GetHash() const {
        return ((((ll)RowID << 12) + ILeft) << 12) + IRight;
    }
};

enum EState {
    STATE_UNTRACKED = 0,
    STATE_IN_OPEN = 1,
    STATE_IN_CLOSED = 2,
};

struct TState {
    TPosition r;
    TInterval I;
    dbl g;
    dbl h;
    EState State;
    uint Index;
    uint Parent;
};

struct THeapElementh {
    uint StateID;
    dbl g, F;
};

inline bool operator < (const THeapElementh& lhs, const THeapElementh& rhs){
    if (abs(lhs.F - rhs.F) > EPS)
        return lhs.F < rhs.F;
    return lhs.g < rhs.g;
}

struct TAnyaStateKey {
    TAnyaStateKey(ull hash, dbl left, dbl right) : Hash(hash), Left(left), Right(right) {}

    ull Hash;
    dbl Left, Right;
};

inline bool operator == (const TAnyaStateKey& lhs, const TAnyaStateKey& rhs) {
    if (lhs.Hash != rhs.Hash)
        return false;
    return abs(lhs.Left - rhs.Left) < EPS && abs(lhs.Right - rhs.Right) < EPS;
}

struct TAnyaHasher {
    size_t operator ()(const TAnyaStateKey& state) const { return static_cast<size_t>(state.Hash); }
};

const uint ANYA_HASHMAP_NO_KEY = 777777777;

class TAnyaHashMap{
public:
    void Add(const TAnyaStateKey& key, uint value) {
        HashMap[key] = value;
    }

    uint Get(const TAnyaStateKey& key) {
        auto f = HashMap.find(key);
        if (f == HashMap.end())
            return ANYA_HASHMAP_NO_KEY;
        return f->second;
    }

    void Clear() {
        HashMap.clear();
    }

private:
    std::unordered_map<TAnyaStateKey, uint, TAnyaHasher> HashMap;
};

} // namespace NAnya
