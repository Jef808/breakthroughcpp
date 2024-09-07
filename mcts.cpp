#include "mcts.h"
#include "movegen.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <random>

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
    return (double)node.wins / node.visits + C * std::sqrt(std::log(stats->at(node.parent).visits));
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

bool rollout(const Node& node) {
    Board board = node.state;
    bool black_to_play = board.ply() & 1;
    while (not board.is_terminal()) {
        const std::vector<Move>& valid_moves = movegen.valid_moves(board);
        std::uniform_int_distribution<> dis(0, valid_moves.size() - 1);
        Move move = valid_moves[dis(gen)];
        board.play(move);
    }
    bool black_wins = not (board.ply() & 1);
    return black_to_play && black_wins;
}

Node& expand(Node& node) {
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

void backpropagate(Node& node, bool win) {
    for (Node* pnode = &node; pnode != nullptr; pnode = &stats->at(pnode->parent)) {
        pnode->wins += win;
        ++pnode->visits;
        win = !win;
    }
}

void step(Node& root) {
    Node* node = &root;
    while(not is_leaf(*node)) {
        node = &select_ucb(*node);
    }
    node = &expand(*node);
    for (const auto& child_fen : node->children) {
        auto& child_node = stats->at(child_fen);
        bool win = rollout(child_node);
        backpropagate(child_node, win);
    }
}

} // namespace

void MCTS::ponder_mcts(const Board& board, int ms) {
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
    const auto& root = m_stats[board.hash()];
    const auto& child = select_ucb(root, 0);
    return child.move;
}


} // namespace breakthrough
