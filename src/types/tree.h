#ifndef __SR_DEPENDENCY_H__
#define __SR_DEPENDENCY_H__

#include "types/instance.h"
#include "types/settings.h"

namespace ZGen {

namespace ShiftReduce {

class DependencyTree {
public:
  typedef std::vector<int> edgeset_t;

  DependencyTree();

  ~DependencyTree();

  /**
   * Set the dependency tree reference onto the tree.
   *
   *  @param[in]  _ref    The reference of the dependency tree.
   *  @return     int     The root index.
   */
  virtual int set_ref(const dependency_t* _ref);

  //
  friend std::ostream & operator << (std::ostream & ofs, const DependencyTree & tree);

  /**/
  const edgeset_t& siblings(int i);

  /**/
  const edgeset_t& descendants(int i);

  /**/
  const edgeset_t& children(int i);

  /**/
  int head(int i);

  /**/
  bool arc(int from, int to);

protected:
  /**
   * Clear all the cached information.
   */
  virtual void reset(int N = kMaxNumberOfWords);

  //
  void dependency_to_tree(const dependency_t* parse);

  /**
   * The deap first process.
   *
   *  @param[in]  now   The currently visited ode.
   */
  void go(int now);

  //
  const dependency_t * ref;

  // Record the children for each node.
  edgeset_t children_[kMaxNumberOfWords];

  int parent[kMaxNumberOfWords];

  // Record the in degree of the graph.
  int indgr[kMaxNumberOfWords];

  // Record the descendant for each node.
  edgeset_t descendants_[kMaxNumberOfWords];

  // Record the siblings for each node.
  edgeset_t siblings_[kMaxNumberOfWords];

private:
  DependencyTree(DependencyTree & other);

  // ban the reference assign
  DependencyTree& operator = (const DependencyTree & other);

  /**
   * Add edge from `u` to `v`
   *
   *  @param[in]  u   The out link node.
   *  @param[in]  v   The in link node.
   */
  void add_edge(int u, int v);
};


class DependencyTreeWithGuidance: public DependencyTree {
public:
  int set_ref(const dependency_t* ref);

  /**/
  int lvl0(int i) const;

  /**/
  int lvl1(int i) const;

  /**/
  int lvl2(int i) const;

protected:
  void reset(int N = kMaxNumberOfWords);

private:
  void color(int now, int _0, int _1, int _2);

  int lvl0_deprels[kMaxNumberOfWords];
  int lvl1_deprels[kMaxNumberOfWords];
  int lvl2_deprels[kMaxNumberOfWords];
};

//
class DependencyForest : public DependencyTree {
public:
  /**/
  int set_ref(const dependency_t* ref);

  int root(int i);

  bool is_root(int i);
private:
  void rootize(int now, int root);

  int root_[kMaxNumberOfWords];
};


}

}

#endif  //  end for __SR_DEPENDENCY_H__
