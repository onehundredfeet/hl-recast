#ifndef __NAV_VERTS_H_
#define __NAV_VERTS_H_
#pragma once
#include <unordered_map>
#include <unordered_set>

struct iVert {
    iVert() {
        x = 0;
        y = 0;
        z = 0;
    }
    iVert(unsigned short int x, unsigned short int y, unsigned short int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    unsigned short int x, y, z;
    bool operator==(const iVert &other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};
// The specialized hash function for `unordered_map` keys
struct ivert_hash_fn {
    std::size_t operator()(const iVert &node) const {
        std::size_t h1 = std::hash<unsigned short int>()(node.x);
        std::size_t h2 = std::hash<unsigned short int>()(node.y);
        std::size_t h3 = std::hash<unsigned short int>()(node.z);

        return h1 << 32 | h2 << 16 | h3;
    }
};

#endif