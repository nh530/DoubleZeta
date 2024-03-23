#include "threadpool/ZetaSession.h"
#include "threadpool/Algebra.h"
#include <any>
#include <iostream>
#include <unistd.h>

void some_task(int value, int *to_return) { *to_return = value + 10; }

template <Numeric Type> Type sub(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = *a - *b;
  // Py_END_ALLOW_THREADS
  return out;
}

int main() {
  ZetaSession newZeta{10};
  sleep(0);
  // NumericVariant *constt = new NumericVariant{float{20}};
  int constt = 10;
  int consta = 20;
  Job<NumericVariant> task2{&sub, &constt, &consta};
  std::future<NumericVariant> test = task2.GetFuture();
  newZeta.SubmitTask(task2);
  test.wait();
  std::cout << std::get<int>(test.get()) << '\n';
  Status<int> anoda = newZeta.SubmitTask(&multiply, constt, consta);
  std::future<int> temp = (anoda).GetFuture();
  temp.wait();
  std::cout << temp.get() << '\n';
  auto result = newZeta.SubmitTask(&divide, consta, constt);
  auto result2 = newZeta.SubmitTask(&add, consta, constt);
  auto result3 = newZeta.SubmitTask(&add, consta, constt);
  float x = 20;
  auto result4 = newZeta.SubmitTask(&sin, x);
  auto result5 = newZeta.SubmitTask(&tan, x);
  std::future<int> temp2 = (result3).GetFuture();
  temp2.wait();
  while (true) {
    if (newZeta.Busy()) {
      std::cout << "Still Busy!" << '\n';
      continue;
    } else {
      break;
    }
  }
  std::cout << (result).GetFuture().get() << '\n';
  std::cout << (result2).GetFuture().get() << '\n';
  std::cout << temp2.get() << '\n';
  std::cout << (result4).GetFuture().get() << '\n';
  std::cout << (result5).GetFuture().get() << '\n';

  int testarr[6] = {0, 1, 2, 3, 4, 5};
  int sz = 6;
  JobArr<int>(&sum_n, testarr, sz);
  JobArr<int> temp3{};
  temp3 = JobArr<int>(&sum_n, testarr, sz);
  JobArr<int> temp4(std::move(temp3));

  auto res = newZeta.SubmitTask(&add_const, testarr, sz);
  (res).GetFuturePtr().wait();

  std::cout << "FINISHED" << '\n';

  newZeta.ShutdownPool();
  return 0;
}
