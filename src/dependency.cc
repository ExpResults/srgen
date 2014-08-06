#include "dependency.h"

namespace ZGen {

namespace ShiftReduce {

DependencyTree::DependencyTree() : ref(0) {
  clear();
}


void DependencyTree::set_ref(const unordered_dependency_t * _ref) {
  ref = _ref;

  int N = _ref->forms.size();
  clear(N);

  for (int i = 0; i < N; ++ i) {
    int v = _ref->heads[i];
    if (v >= 0) {
      add_edge(v, i);
    }
  }

  int root = -1;
  for (int i = 0; i < N; ++ i) {
    if (indgr[i] == 0) {
      root = i;
      break;
    }
  }

  go(root);
}


void DependencyTree::clear(int N) {
  // Set all parent to un init.
  memset(parent, -1, sizeof(parent));

  // Set all the in degree to zero.
  memset(indgr, 0, sizeof(indgr));

  for (int i = 0; i < N; ++ i) {
    // Clear chidren cache.
    children[i].clear();

    // Clear descendant cache.
    descendant[i].clear();

    // Clear siblings cache.
    sibling[i].clear();
  }
}


void DependencyTree::add_edge(int u, int v) {
  // Link v to u
  parent[v] = u;

  // Increase the in-degree
  ++ indgr[v];

  //
  children[u].push_back(v);
}


void DependencyTree::go(int now) {
  for (int i = 0; i < children[now].size(); ++ i) {
    int child = children[now][i];
    go(child);
  }

  edgeset_t & tree = descendant[now];

  for (int i = 0; i < children[now].size(); ++ i) {
    int child = children[now][i];
    edgeset_t & subtree = descendant[child];

    // First push the certain children
    tree.push_back(child);

    // Then extend the descendant.
    tree.reserve(tree.size() + std::distance(subtree.begin(), subtree.end()));
    tree.insert(tree.end(), subtree.begin(), subtree.end());
  }

  // Main the siblings.
  for (int i = 0; i < children[now].size(); ++ i) {
    int child = children[now][i];
    edgeset_t & sib = sibling[child];

    for (int j = 0; j < children[now].size(); ++ j) {
      int child2 = children[now][j];
      if (child == child2) {
        continue;
      }

      edgeset_t & subtree = descendant[child2];
      // Push back the sibling node.
      sib.push_back(child2);
      //
      sib.reserve(sib.size() + std::distance(subtree.begin(), subtree.end()));
      sib.insert(sib.end(), subtree.begin(), subtree.end());
    }
  }
}

std::ostream & operator << (std::ostream & os, const DependencyTree & tree) {
  int N = tree.ref->forms.size();

  for (int i = 0; i < N; ++ i) {
    os << "id : " << i << std::endl;

    const DependencyTree::edgeset_t & chi = tree.children[i];
    const DependencyTree::edgeset_t & des = tree.descendant[i];
    const DependencyTree::edgeset_t & sib = tree.sibling[i];

#define SHOW(fullname, name) os << " - " << #fullname << ": ["; \
    for (int j = 0; j < (name).size(); ++ j) { \
      os << (name)[j]; if (j < (name).size() - 1) { os << ", "; } \
    } \
    os << "]" << std::endl;

    SHOW(children, chi);
    SHOW(descendant, des);
    SHOW(siblings, sib);

#undef SHOW
  }

  return os;
}

}

}
