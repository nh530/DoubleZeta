#include "config/settings.h"
#include "double_word/bsc_core.h"
#include "double_word/dw.h"
#include <iomanip>
#include <iostream>


int main() {
  dbl_wrd test(10);

  print(test);
  print(dbl_wrd(912.123123213));
  print(dbl_wrd(1290.009, 12312.1233));
  print(dbl_wrd(123, 123));
	std::cout << std::setprecision(18) << 100000000000000000000000.0 << '\n';
  return 0;
}
