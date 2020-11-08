
#include <iostream>

int main() {
  int accum = 0;
  for (int i = 1; i <= 100; ++i) {
    accum += i;
  }
  std::cout << "Sum from 1 to 100 is " << accum << '\n';
  return 0;
}