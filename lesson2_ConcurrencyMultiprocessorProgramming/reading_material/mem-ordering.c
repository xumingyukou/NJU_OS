#include "thread.h"

int x = 0, y = 0;

atomic_int flag;  // 用flag的最后两个bit作为锁
#define FLAG atomic_load(&flag)
#define FLAG_XOR(val) atomic_fetch_xor(&flag, val)
#define WAIT_FOR(cond) while (!(cond)) ;

 __attribute__((noinline))
void write_x_read_y() {
  int y_val;
  asm volatile(
    "movl $1, %0;" // x = 1  cache miss，先进行读y，处理器也是编译器，在执行时也可以进行调换执行顺序
    "mfence;"
    "movl %2, %1;" // y_val = y
    : "=m"(x), "=r"(y_val) : "m"(y)
  );
  printf("%d ", y_val);
}

 __attribute__((noinline))
void write_y_read_x() {
  int x_val;
  asm volatile(
    "movl $1, %0;" // y = 1  cache miss
    "mfence;"
    "movl %2, %1;" // x_val = x
    : "=m"(y), "=r"(x_val) : "m"(x)
  );
  printf("%d ", x_val);
}

void T1(int id) {
  while (1) {
    WAIT_FOR((FLAG & 1));  // 等待锁，将flag的最后一位置为0
    write_x_read_y();
    FLAG_XOR(1);  // 释放锁，将flag的最后一位置为1
  }
}

void T2() {
  while (1) {
    WAIT_FOR((FLAG & 2));  // 等待锁，将flag的倒数第二位置为0
    write_y_read_x();
    FLAG_XOR(2);  // 等待锁，将flag的倒数第二位置为1
  }
}

void Tsync() {
  while (1) {
    x = y = 0;
    __sync_synchronize(); // full barrier
    usleep(1);            // + delay
    assert(FLAG == 0);
    FLAG_XOR(3);  // 释放锁，即将flag的最后两bit置为1
    // T1 and T2 clear 0/1-bit, respectively
    WAIT_FOR(FLAG == 0);
    printf("\n"); fflush(stdout);
  }
}

int main() {
  create(T1);
  create(T2);
  create(Tsync);
}
