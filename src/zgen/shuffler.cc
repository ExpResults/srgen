#include <iostream>
#include <vector>
#include "io/io.h"
#include "types/instance.h"
#include "util/utils.h"

int main() {
  namespace SR = ZGen::ShiftReduce;

  std::vector< SR::dependency_t > data;
  SR::read_from_dep(std::cin, data);

  for (int i = 0; i < data.size(); ++ i) {
    SR::write_dep_instance(std::cout, data[i]);
  }
  return 0;
}
