#include "types/tree.h"

namespace ZGen {

namespace ShiftReduce {

DependencyTree::DependencyTree() : ref(0) {
}

DependencyTree::~DependencyTree() {
}

DependencyTree& DependencyTree::operator = (const DependencyTree& other) {
  std::cerr << "Copy operation is not allowed." << std::endl;
  return (*this);
}


void DependencyTree::dependency_to_tree(const dependency_t* _ref) {
  ref = _ref;
  int N = _ref->size();

  reset(N);

  for (int i = 0; i < N; ++ i) {
    int v = _ref->heads[i];
    if (v >= 0) {
      add_edge(v, i);
    }
  }
}

int DependencyTree::set_ref(const dependency_t* _ref) {
  dependency_to_tree(_ref);

  int root = -1;
  int N = _ref->size();
  for (int i = 0; i < N; ++ i) {
    if (indgr[i] == 0) {
      root = i;
      break;
    }
  }

  go(root);

  return root;
}


const DependencyTree::edgeset_t& DependencyTree::descendants(int i) {
  return descendants_[i];
}


const DependencyTree::edgeset_t& DependencyTree::siblings(int i) {
  return siblings_[i];
}

const DependencyTree::edgeset_t& DependencyTree::children(int i) {
  return children_[i];
}

int DependencyTree::head(int i) {
  return parent[i];
}

bool DependencyTree::arc(int from, int to) {
  return parent[to] == from;
}

void DependencyTree::reset(int N) {
  // Set all parent to un init.
  memset(parent, -1, sizeof(parent));

  // Set all the in degree to zero.
  memset(indgr, 0, sizeof(indgr));

  for (int i = 0; i < N; ++ i) {
    // Clear chidren cache.
    children_[i].clear();

    // Clear descendant cache.
    descendants_[i].clear();

    // Clear siblings cache.
    siblings_[i].clear();
  }
}


void DependencyTree::add_edge(int u, int v) {
  // Link v to u
  parent[v] = u;

  // Increase the in-degree
  ++ indgr[v];

  //
  children_[u].push_back(v);
}


void DependencyTree::go(int now) {
  for (int i = 0; i < children_[now].size(); ++ i) {
    int child = children_[now][i];
    go(child);
  }

  edgeset_t& tree = descendants_[now];

  for (int i = 0; i < children_[now].size(); ++ i) {
    int child = children_[now][i];
    const edgeset_t& subtree = descendants_[child];

    // First push the certain children
    tree.push_back(child);

    // Then extend the descendant.
    tree.reserve(tree.size() + std::distance(subtree.begin(), subtree.end()));
    tree.insert(tree.end(), subtree.begin(), subtree.end());
  }

  // Main the siblings.
  for (int i = 0; i < children_[now].size(); ++ i) {
    int child = children_[now][i];
    edgeset_t& sib = siblings_[child];

    for (int j = 0; j < children_[now].size(); ++ j) {
      int child2 = children_[now][j];
      if (child == child2) {
        continue;
      }

      const edgeset_t& subtree = descendants_[child2];
      // Push back the sibling node.
      sib.push_back(child2);
      //
      sib.reserve(sib.size() + std::distance(subtree.begin(), subtree.end()));
      sib.insert(sib.end(), subtree.begin(), subtree.end());
    }
  }
}

std::ostream& operator << (std::ostream & os, const DependencyTree & tree) {
  int N = tree.ref->forms.size();

  for (int i = 0; i < N; ++ i) {
    os << "id : " << i << std::endl;

    const DependencyTree::edgeset_t & chi = tree.children_[i];
    const DependencyTree::edgeset_t & des = tree.descendants_[i];
    const DependencyTree::edgeset_t & sib = tree.siblings_[i];

#define SHOW(fullname, name) os << " - " << #fullname << ": ["; \
    for (int j = 0; j < (name).size(); ++ j) { \
      os << (name)[j]; if (j < (name).size() - 1) { os << ", "; } \
    } \
    os << "]" << std::endl;

    SHOW(children,   chi);
    SHOW(descendants,des);
    SHOW(siblings, sib);

#undef SHOW
  }

  return os;
}

void DependencyTreeWithGuidance::reset(int N) {
  DependencyTree::reset(N);

  memset(lvl0_deprels, 0, sizeof(lvl0_deprels));
  memset(lvl1_deprels, 0, sizeof(lvl1_deprels));
  memset(lvl2_deprels, 0, sizeof(lvl2_deprels));
}

int DependencyTreeWithGuidance::set_ref(const dependency_t* ref) {
  int root = DependencyTree::set_ref(ref);
  color(root, 0, 0, 0);
  return root;
}


void DependencyTreeWithGuidance::color(int now, int _0, int _1, int _2) {
  //std::cout << now << " " << _0 << " " << _1 << " " << _2 << std::endl;
  lvl0_deprels[now]= _0;
  lvl1_deprels[now]= _1;
  lvl2_deprels[now]= _2;
  for (int i = 0; i < children_[now].size(); ++ i) {
    int child = children_[now][i];
    int new_0 = (_0 ?             _0: ref->deprels[child]);
    int new_1 = (_1 && _0 ?       _1: (_0 ? ref->deprels[child]: 0));
    int new_2 = (_2 && _1 && _0 ? _2: (_1 ? ref->deprels[child]: 0));
    color(child, new_0, new_1, new_2);
  }
}

int DependencyTreeWithGuidance::lvl0(int i) const {
  return lvl0_deprels[i];
}

int DependencyTreeWithGuidance::lvl1(int i) const {
  return lvl1_deprels[i];
}

int DependencyTreeWithGuidance::lvl2(int i) const {
  return lvl2_deprels[i];
}

int DependencyForest::set_ref(const dependency_t* ref) {
  dependency_to_tree(ref);

  int N = ref->size();

  for (int i = 0; i < N; ++ i) {
    if (indgr[i] == 0) {
      go(i);
      rootize(i, i);
    }
  }
  return -1;
}

void DependencyForest::rootize(int now, int root_flag) {
  root_[now] = root_flag;
  const edgeset_t& cc = children_[now];
  for (int i = 0; i < cc.size(); ++ i) {
    int c = cc[i];
    rootize(c, root_flag);
  }
}


int DependencyForest::root(int i) {
  return root_[i];
}

bool DependencyForest::is_root(int i) {
  return (root_[i] == i);
}

} //  end for namespace ShiftReduce

} //  end for namespace ZGen
