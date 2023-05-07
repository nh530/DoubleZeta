#include "threadpool/ZetaSession.h"
#include "algba_cmd/Algebra.h"
#include <any>
#include <unistd.h>


void some_task(int value, int *to_return) { *to_return = value + 10; }

int main() {
  ZetaSession newZeta{10};
  sleep(0);
  // NumericVariant *constt = new NumericVariant{float{20}};
  int constt = 10;
  int consta = 20;
  Job<NumericVariant> task2{&subtract, &constt, &consta};
  std::future<NumericVariant> test = task2.GetFuture();
  newZeta.SubmitTask(std::move(task2));
  auto anoda = newZeta.SubmitTask(&multiply, &constt, &consta);
  auto result = newZeta.SubmitTask(&divide, &consta, &constt);
  auto result2 = newZeta.SubmitTask(&add, &consta, &constt);
  auto result3 = newZeta.SubmitTask(&add, &consta, &constt);
  auto result4 = newZeta.SubmitTask(&sin, &consta);
  float x = 20;
  auto result5 = newZeta.SubmitTask(&tan, &x);
  result3.wait();
  std::cout << std::get<int>(test.get()) << '\n';
  std::cout << std::get<int>(anoda.get()) << '\n';
  std::cout << std::get<int>(result.get()) << '\n';
  std::cout << std::get<int>(result2.get()) << '\n';
  std::cout << std::get<int>(result3.get()) << '\n';
  std::cout << std::get<int>(result4.get()) << '\n';
  std::cout << std::get<float>(result5.get()) << '\n';
  while (true) {
    if (newZeta.Busy()) {
      std::cout << "Still Busy!" << '\n';
      continue;
    } else {
      break;
    }
  }
  newZeta.ShutdownPool();
  return 0;
}
