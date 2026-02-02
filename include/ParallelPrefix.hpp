/**
 * @file ParallelPrefix.hpp
 * @brief Template-based Parallel Prefix Sum (Ladner-Fischer) library.
 */

#ifndef PARALLEL_PREFIX_HPP
#define PARALLEL_PREFIX_HPP

#include <vector>
#include <future>

/**
 * @class Heaper
 * @brief Tree navigation logic for array-mapped binary trees.
 */
class Heaper {
protected:
    int n;          //leaf node amount: N
    int m;          //paddingsize: power of 2 >= N
    int tree_size;  //node amount: 2N-1

public:
    Heaper(int N) : n(N) {
        // Padding to the next power of 2
        // calculate m: the smallest power of 2 >= n
        // ex: n = 5 -> m = 8; n = 8 -> m = 8
        m = 1;
        while (m < n) m <<= 1; 
        tree_size = 2 * m - 1;
    }

    // use inline for efficiency 
    inline int left(int i) const { 
        return 2 * i + 1; 
    }
    inline int right(int i) const {
        return 2 * i + 2; 
    }

    inline bool isLeaf(int i) const {
        return i >= (m - 1); 
    }

    // get the index of leaf node in original data
    inline int leafIdx(int i) const {
         return i - (m - 1); 
    }
};

/**
 * @class ParallelSum
 * @brief Generic Parallel Prefix Sum implementation.
 * @tparam T Numeric type (int, float, double, etc.)
 */
template <typename T>
class ParallelSum : public Heaper {
private:
    const std::vector<T>* data;     // store the pointer of original data
    std::vector<T> tree;            // store the sum tree   

    // First part: Up-sweep (parrallel first 4 levels)
    T calcSum(int i, int level) {
        if (isLeaf(i)) {
            int idx = leafIdx(i);
            // padding zero for extra leaf nodes
            return tree[i] = (idx < n) ? (*data)[idx] : static_cast<T>(0);
        }

        if (level < 4) {
            auto h = std::async(std::launch::async, &ParallelSum::calcSum, this, left(i), level + 1);

            T r = calcSum(right(i), level + 1);
            return tree[i] = h.get() + r;
        }
        return tree[i] = calcSum(left(i), level + 1) + calcSum(right(i), level + 1);
    }

    // Second part: Down-sweep (parallel first 4 levels)
    void calcPrefix(int i, int level, T incomingSum, std::vector<T>* prefix) {
        if (isLeaf(i)) {
            int idx = leafIdx(i);
            // only write result within original range
            if (idx < n) (*prefix)[idx] = incomingSum + (*data)[idx];
            return;
        }
        if (level < 4) {
            auto h = std::async(std::launch::async, &ParallelSum::calcPrefix, this, left(i), level + 1, incomingSum, prefix);
            calcPrefix(right(i), level + 1, incomingSum + tree[left(i)], prefix);
            h.get();
        } else {
            calcPrefix(left(i), level + 1, incomingSum, prefix);
            calcPrefix(right(i), level + 1, incomingSum + tree[left(i)], prefix);
        }
    }

public:
    // Constructor
    ParallelSum(const std::vector<T>* d) : Heaper(d->size()), data(d) {
        tree.resize(tree_size);
        calcSum(0, 0);      // build the sum heap from top to bottom
    }

    // Compute the prefix sum and store it in the provided vector
    void compute(std::vector<T>* prefix) {
        calcPrefix(0, 0, static_cast<T>(0), prefix);
    }
};

#endif