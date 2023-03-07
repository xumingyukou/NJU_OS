#include "thread.h"

#define N 100000000

long sum = 0;

void Tsum() {
  for (int i = 0; i < N; i++) {
    // asm volatile("lock add $1, %0": "+m"(sum));
    // asm volatile("add $1, %0": "+m"(sum));
    sum++;
  }
}

int main() {
  create(Tsum);
  create(Tsum);
  join();
  printf("sum = %ld\n", sum);
}
