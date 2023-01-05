#include "puzzle_solver.hpp"

#include <functional>
#include <unordered_set>

#include "puzzle.hpp"
#include "state.hpp"
#include "frontier_queue.hpp"

// specialize the std::hash function for Puzzles
namespace std {
template <> struct hash<Puzzle> {
  typedef Puzzle argument_type;
  typedef uint64_t result_type;

  result_type operator()(const argument_type &x) const { return x.hash(); }
};
}

PuzzleSolver::PuzzleSolver(const Puzzle &initialPuzzle,
                           const Puzzle &goalPuzzle)
    : initial(initialPuzzle), goal(goalPuzzle) {}

std::pair<bool, std::size_t> PuzzleSolver::search() {

  std::size_t solution_cost;

  frontier_queue<Puzzle> frontier;
  //TODO: instantiate explored as an unordered_set

  frontier.push(initial, 0, initial.heuristic(goal));

  bool found = false;
  while (true) {
    if (frontier.empty()) {
      found = false;
      break;
    }

    auto node = frontier.pop();
    Puzzle value = node.getValue();
    //TODO: add state to explored 

    std::size_t cost = node.getPathCost();

    if (value.heuristic(goal) == 0) {
      found = true;
      solution_cost = node.getPathCost();
      break;
    }

    for (int a = Puzzle::LEFT; a <= Puzzle::DOWN; ++a) {
      bool ok;
      Puzzle result;
      std::tie(ok, result) = value.apply(static_cast<Puzzle::Action>(a));

      if (ok) {
        //TODO: define inexplored as whether the result is in explored
        bool inexplored = false;
        bool infrontier = frontier.contains(result);
        if (!(inexplored || infrontier)) {
          frontier.push(result, cost + 1, result.heuristic(goal));
        } else if (infrontier) {
          frontier.replaceif(result, cost + 1);
        }
      }
    }
  }

  return std::make_pair(found, solution_cost);
}
