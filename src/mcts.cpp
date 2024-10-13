#include "mcts.h"
#include "movegen.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <random>

#include <iostream>

namespace breakthrough {

namespace {

static std::random_device rd;
static std::mt19937 gen(rd());
thread_local MoveGen movegen;
thread_local std::unordered_map<uint64_t, Node>* stats;

inline double ucb1(const Node& node, double C) {
    if (node.visits == 0) {
        return std::numeric_limits<double>::max();
    }
    double avrg = node.reward / node.visits;
    double expl = C * std::sqrt(std::log(stats->at(node.parent).visits) / node.visits);
    return avrg + expl;

}

inline bool is_leaf(const Node& node) {
    return node.children.empty();
}

Node& select_ucb(const Node& node, double C = 1.4142135623730951) {
    auto it = std::max_element(
        node.children.begin(), node.children.end(),
        [C](const auto& a, const auto& b) {
            return ucb1(stats->at(a), C) < ucb1(stats->at(b), C);
        });
    return stats->at(*it);
}

Node& expand(Node& node) {
    assert(node.children.empty() && "Trying to expand already expanded node");
    Node* parent = &node;
    auto parent_board = node.state;
    auto parent_hash = parent_board.hash();
    const auto& valid_moves = movegen.valid_moves(node.state);

    // Reserve enough space so that `parent` pointer doesn't get invalidated
    auto size_before_rehashing = stats->max_load_factor() * stats->bucket_count();
    if (stats->size() + valid_moves.size() > size_before_rehashing) {
        stats->reserve(stats->size() + valid_moves.size());
        parent = &stats->at(parent_hash);
    }
    for (const auto& move : valid_moves) {
        auto board = parent_board;
        board.play(move);
        auto child_hash = board.hash();
        parent->children.push_back(child_hash);
        auto [it, inserted] = stats->try_emplace(child_hash);
        if (inserted) {
            it->second.state = board;
        }
        it->second.move = move;
        it->second.parent = parent_hash;
    }

    return *parent;
}

double rollout(const Node& node) {
    Board board = node.state;
    int initial_ply = board.ply();
    while (not board.is_terminal()) {
        const std::vector<Move>& valid_moves = movegen.valid_moves(board);
        std::uniform_int_distribution<> dis(0, valid_moves.size() - 1);
        Move move = valid_moves[dis(gen)];
        board.play(move);
    }
    int rollout_length = board.ply() - initial_ply;
    double discount = std::pow(0.99, rollout_length);
    bool is_win = !(rollout_length & 1);
    double reward = (2.0 * (double)is_win - 1) * discount;
    return reward;
}

void backpropagate(Node& node, double reward) {
    for (Node* pnode = &node; pnode->parent != 0; pnode = &stats->at(pnode->parent)) {
        pnode->reward += reward;
        ++pnode->visits;
        reward *= -1.0;
    }
}

void step(Node& root) {
    Node* node = &root;
    bool is_mine = true;
    while(not is_leaf(*node)) {
        node = &select_ucb(*node);
        is_mine = !is_mine;
    }
    node = &expand(*node);

    if (node->state.is_terminal()) {
        double reward = 2.0 * (double)(is_mine) - 1;
        backpropagate(*node, reward);
    }

    int n_rollouts = 5;
    double total_reward = 0.0;
    for (const auto& child_hash : node->children) {
        auto& child_node = stats->at(child_hash);
        for (auto i = 0; i < n_rollouts; ++i) {
            double reward = rollout(child_node);
            total_reward -= reward;
        }
    }

    double reward = total_reward / (n_rollouts * node->children.size());
    backpropagate(*node, reward);
}

} // namespace

void MCTS::ponder(const Board& board, int ms) {
    stats = &m_stats;

    Node& root = m_stats[board.hash()];
    root.state = board;
    root.parent = 0;
    root.move = {-1, -1};

    auto start_time = std::chrono::steady_clock::now();
    while (true) {
        auto current_time =
            std::chrono::steady_clock::now();
        auto elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                current_time - start_time);
        if (elapsed_time.count() >= ms) {
            break;
        }
        step(root);
    }
}

Move MCTS::choose_best(const Board& board) {
    auto& root = m_stats[board.hash()];
    assert(not root.children.empty() && "cannot choose best on leaf node");

    auto child_hash = *std::max_element(root.children.begin(), root.children.end(),
                     [&](const auto& a, const auto& b) {
                         return m_stats[a].visits < m_stats[b].visits;
                     });
    const auto& child = m_stats[child_hash];
    return child.move;
}

void MCTS::reset() {
    m_stats.clear();
}


} // namespace breakthrough
