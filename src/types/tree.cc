#include "types/tree.h"

namespace ZGen {

namespace ShiftReduce {

DependencyTree::DependencyTree() : ref(0) {
  //reset();
}

DependencyTree::~DependencyTree() {
  std::cerr << "DEPENDENCY-TREE: Should not be dellocate" << std::endl;
}

DependencyTree::DependencyTree(DependencyTree& o) {
  std::cerr << "Copy assignment is not allowed." << std::endl;
}

DependencyTree& DependencyTree::operator = (const DependencyTree& other) {
  std::cerr << "Copy operation is not allowed." << std::endl;
}

int DependencyTree::set_ref(const dependency_t* _ref) {
  ref = _ref;
  int N = _ref->forms.size();

  reset(N);

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

  return root;
}


const DependencyTree::edgeset_t& DependencyTree::descendants(int i) {
  return descendant[i];
}


const DependencyTree::edgeset_t& DependencyTree::siblings(int i) {
  return sibling[i];
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
    const edgeset_t & subtree = descendant[child];

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

      const edgeset_t & subtree = descendant[child2];
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

void DependencyTreeWithGuidance::reset(int N) {
  DependencyTree::reset(N);

  memset(lvl0_deprels, 0, sizeof(lvl0_deprels));
  memset(lvl1_deprels, 0, sizeof(lvl1_deprels));
  memset(lvl2_deprels, 0, sizeof(lvl2_deprels));
}

int DependencyTreeWithGuidance::set_ref(const dependency_t* ref) {
  int root = DependencyTree::set_ref(ref);
  color(root, 0, 0, 0);
}


void DependencyTreeWithGuidance::color(int now, int _0, int _1, int _2) {
  for (int i = 0; i < children[now].size(); ++ i) {
    int child = children[now][i];
    int new_0 = (_0 ?             _0: ref->deprels[child]);
    int new_1 = (_1 && _0 ?       _1: ref->deprels[child]);
    int new_2 = (_2 && _1 && _0 ? _2: ref->deprels[child]);
    color(child, new_0, new_1, new_2);
  }
}

}

}
