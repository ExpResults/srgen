#ifndef __SR_DEPENDENCY_H__
#define __SR_DEPENDENCY_H__

#include "settings.h"

namespace ZGen {

namespace ShiftReduce {

class DependencyTree {
public:
  DependencyTree();

  void set_ref(const unordered_dependency_t * _ref);

  //
  friend std::ostream & operator << (std::ostream & ofs, const DependencyTree & tree);

private:
  /**
   * Clear all the cached information.
   */
  void clear(int N = kMaxNumberOfWords);


  /**
   * The deap first process.
   *
   *  @param[in]  now   The currently visited ode.
   */
  void go(int now);


  /**
   * Add edge from `u` to `v`
   *
   *  @param[in]  u   The out link node.
   *  @param[in]  v   The in link node.
   */
  void add_edge(int u, int v);


  typedef std::vector<int> edgeset_t;

  const unordered_dependency_t * ref;

  int parent[kMaxNumberOfWords];

  // Record the in degree of the graph.
  int indgr[kMaxNumberOfWords];

  // Record the children for each node.
  edgeset_t children[kMaxNumberOfWords];

  // Record the descendant for each node.
  edgeset_t descendant[kMaxNumberOfWords];

  // Record the siblings for each node.
  edgeset_t sibling[kMaxNumberOfWords];
};

}

}

#endif  //  end for __SR_DEPENDENCY_H__
