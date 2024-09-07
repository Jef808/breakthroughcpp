#ifndef ZOBRIST_H_
#define ZOBRIST_H_

#include <random>
#include <unordered_map>

namespace breakthrough {

class Zobrist {
public:
    /**
     * Seed the random number generator.
     */
    explicit Zobrist(unsigned int seed = 42) : m_rng(seed) {}

    /**
     * Generate a random value at the given key.
     */
    void generate(int key) { m_table[key] = m_rng(); }

    /**
     * Retrieve the value associated to the given key.
     */
    uint64_t operator[](int key) { return m_table[key]; }
private:
    std::mt19937_64 m_rng;
    std::unordered_map<int, uint64_t> m_table;
};

}  // namespace breakthrough

#endif // ZOBRIST_H_
