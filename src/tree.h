#ifndef __SR_DEPENDENCY_H__
#define __SR_DEPENDENCY_H__

#include "instance.h"
#include "settings.h"

namespace ZGen {

namespace ShiftReduce {

class DependencyTree {
public:
  typedef std::vector<int> edgeset_t;

  DependencyTree();

  ~DependencyTree();

  void set_ref(const unordered_dependency_t * _ref);

  //
  friend std::ostream & operator << (std::ostream & ofs, const DependencyTree & tree);

  const edgeset_t& siblings(int i);

  const edgeset_t& descendants(int i);

  int head(int i);

  bool arc(int from, int to);

private:
  DependencyTree(DependencyTree & other);

  DependencyTree& operator = (const DependencyTree & other);

  /**
   * Clear all the cached information.
   */
  void reset(int N = kMaxNumberOfWords);


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


  const dependency_t * ref;

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
