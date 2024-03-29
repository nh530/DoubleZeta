#include "threadpool/Job.h"
#include <gtest/gtest.h>
// TODO: There is an issue in some of the move assignment tests. it is actually calling move constructor instead of assignment. NEED FIX.

template <int_or_float T> T simp_add(const T *a, const T *b) { return *a + *b; };
NumericVariant simp_add(const NumericVariant *a, const NumericVariant *b) {
  if (std::holds_alternative<int>(*a)) {
    return std::get<int>(*a) + std::get<int>(*b);
  } else
    return std::get<float>(*a) + std::get<float>(*b);
}
template <int_or_float T> T simp_mul(const T *a) { return *a * 2; }
NumericVariant simp_mul(const NumericVariant *a) {
  if (std::holds_alternative<int>(*a)) {
    return std::get<int>(*a) * 2;
  } else
    return std::get<float>(*a) * 2;
}

template <Numeric T> T simp_func() { return 100; }
template <Numeric T> Job<T> *move_func(Job<T> &&x) {
  Job<T> *out = new Job(std::move(x));
  return out;
}
template <Numeric T> JobNoParam<T> *move_func(JobNoParam<T> &&x) {
  JobNoParam<T> *out = new JobNoParam(std::move(x));
  return out;
}
template <Numeric T> JobArr<T> *move_func(JobArr<T> &&x) {
  JobArr<T> *out = new JobArr(std::move(x));
  return out;
}

template <int_or_float T> T simp_add_ref(const T &a, const T &b) { return a + b; }
template <int_or_float T> T simp_mul_ref(const T &a) { return a * 2; }
template <int_or_float T> T simp_add_c(const T a, const T b) { return a + b; }
template <int_or_float T> T simp_mul_c(const T a) { return a * 2; }

TEST(Job, initialization) {
  // Note that this does not test if the function pointer is correctly initialized.
  int x = 10;
  int y = 20;
  Job<int> test{&simp_add, &x, &y};
  EXPECT_EQ(test.GetArg1(), 10);
  EXPECT_EQ(test.GetArg2(), 20);
  EXPECT_EQ(test._debug_func(), 1);
  float x_f = 10.0;
  float y_f = 20.0;
  Job<float> test2{&simp_add, &x_f, &y_f};
  EXPECT_EQ(test2.GetArg1(), 10);
  EXPECT_EQ(test2.GetArg2(), 20);
  EXPECT_EQ(test2._debug_func(), 1);
  Job<int> test3{&simp_mul, &x};
  EXPECT_EQ(test3.GetArg1(), 10);
  EXPECT_EQ(test3.GetArg2(), 0);
  EXPECT_EQ(test3._debug_func(), 2);
  Job<float> test4{&simp_mul, &x_f};
  EXPECT_EQ(test4.GetArg1(), 10.0f);
  EXPECT_EQ(test4.GetArg2(), 0);
  EXPECT_EQ(test4._debug_func(), 2);
  Job<int> test5{&simp_add, &x, &y};
  EXPECT_EQ(test5.GetArg1(), 10);
  EXPECT_EQ(test5.GetArg2(), 20);
  EXPECT_EQ(test5._debug_func(), 1);
  Job<float> test6{&simp_add, &x_f, &y_f};
  EXPECT_EQ(test6.GetArg1(), 10.0f);
  EXPECT_EQ(test6.GetArg2(), 20.0f);
  EXPECT_EQ(test6._debug_func(), 1);
  Job<int> test7{&simp_func};
  EXPECT_EQ(test7.GetArg1(), 0);
  EXPECT_EQ(test7.GetArg2(), 0);
  EXPECT_EQ(test7._debug_func(), 3);
  Job<float> test8{&simp_func};
  EXPECT_EQ(test8.GetArg1(), 0.0f);
  EXPECT_EQ(test8.GetArg2(), 0.0f);
  EXPECT_EQ(test8._debug_func(), 3);
  Job<int> test9{};
  EXPECT_EQ(test9.GetArg1(), 0);
  EXPECT_EQ(test9.GetArg2(), 0);
  EXPECT_EQ(test9._debug_func(), 0);
  Job<float> test10{};
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  EXPECT_EQ(test10.GetArg2(), 0.0f);
  EXPECT_EQ(test10._debug_func(), 0);
  Job<NumericVariant> test11(&simp_add, &x, &y);
  EXPECT_EQ(std::get<int>(test11.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test11.GetArg2()), 20);
  EXPECT_EQ(test11._debug_func(), 1);
  Job<NumericVariant> test12{&simp_mul, &x};
  EXPECT_EQ(std::get<int>(test12.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test12.GetArg2()), 0);
  EXPECT_EQ(test12._debug_func(), 2);
  Job<NumericVariant> test13{&simp_func};
  EXPECT_EQ(std::get<int>(test13.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test13.GetArg2()), 0);
  EXPECT_EQ(test13._debug_func(), 3);
  Job<NumericVariant> test14(&simp_add, &x_f, &y_f);
  EXPECT_EQ(std::get<float>(test14.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test14.GetArg2()), 20.0f);
  EXPECT_EQ(test14._debug_func(), 1);
  Job<NumericVariant> test15{&simp_mul, &x_f};
  EXPECT_EQ(std::get<float>(test15.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test15.GetArg2()), 0.0f);
  EXPECT_EQ(test15._debug_func(), 2);
  Job<NumericVariant> test16{&simp_func};
  EXPECT_EQ(std::get<int>(test16.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test16.GetArg2()), 0);
  EXPECT_EQ(test16._debug_func(), 3);
  NumericVariant x_n = NumericVariant(10); // int variant.
  NumericVariant y_n = NumericVariant(20);

  Job<NumericVariant> test17(&simp_add, &x_n, &y_n);
  EXPECT_EQ(std::get<int>(test17.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test17.GetArg2()), 20);
  EXPECT_EQ(test17._debug_func(), 1);
  Job<NumericVariant> test18{&simp_mul, &x_n};
  EXPECT_EQ(std::get<int>(test18.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test18.GetArg2()), 0);
  EXPECT_EQ(test18._debug_func(), 2);
  Job<NumericVariant> test19{&simp_func};
  EXPECT_EQ(std::get<int>(test19.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test19.GetArg2()), 0);
  EXPECT_EQ(test19._debug_func(), 3);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  NumericVariant y_f_n = NumericVariant(20.2f);

  Job<NumericVariant> test20(&simp_add, &x_f_n, &y_f_n);
  EXPECT_EQ(std::get<float>(test20.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test20.GetArg2()), 20.2f);
  EXPECT_EQ(test20._debug_func(), 1);
  Job<NumericVariant> test21{&simp_mul, &x_f_n};
  EXPECT_EQ(std::get<float>(test21.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test21.GetArg2()), 0.0f);
  EXPECT_EQ(test21._debug_func(), 2);
  Job<NumericVariant> test22{&simp_func};
  EXPECT_EQ(std::get<int>(test22.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test22.GetArg2()), 0);
  EXPECT_EQ(test22._debug_func(), 3);
  Job<NumericVariant> test23{};
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test23.GetArg2()), 0);
  EXPECT_EQ(test23._debug_func(), 0);
  Job<NumericVariant> test24{};
  EXPECT_EQ(std::get<int>(test24.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test24.GetArg2()), 0);
  EXPECT_EQ(test24._debug_func(), 0);
}

TEST(Job, move_assignment) {
  // Note that this does not test if the function pointer is correctly moved over.
  int x = 10;
  int y = 20;
  Job<int> test = std::move(Job<int>(&simp_add, &x, &y));
  EXPECT_EQ(test.GetArg1(), 10);
  EXPECT_EQ(test.GetArg2(), 20);
  EXPECT_EQ(test._debug_func(), 1);
  float x_f = 10.0;
  float y_f = 20.0;
  Job<float> test2 = std::move(Job<float>(&simp_add, &x_f, &y_f));
  EXPECT_EQ(test2.GetArg1(), 10);
  EXPECT_EQ(test2.GetArg2(), 20);
  EXPECT_EQ(test2._debug_func(), 1);
  Job<int> test3 = std::move(Job<int>(&simp_mul, &x));
  EXPECT_EQ(test3.GetArg1(), 10);
  EXPECT_EQ(test3.GetArg2(), 0);
  EXPECT_EQ(test3._debug_func(), 2);
  Job<float> test4 = std::move(Job<float>(&simp_mul, &x_f));
  EXPECT_EQ(test4.GetArg1(), 10.0f);
  EXPECT_EQ(test4.GetArg2(), 0);
  EXPECT_EQ(test4._debug_func(), 2);
  Job<int> test5 = std::move(Job<int>(&simp_add, &x, &y));
  EXPECT_EQ(test5.GetArg1(), 10);
  EXPECT_EQ(test5.GetArg2(), 20);
  EXPECT_EQ(test5._debug_func(), 1);
  Job<float> test6 = std::move(Job<float>(&simp_add, &x_f, &y_f));
  EXPECT_EQ(test6.GetArg1(), 10.0f);
  EXPECT_EQ(test6.GetArg2(), 20.0f);
  EXPECT_EQ(test6._debug_func(), 1);
  Job<int> test7 = std::move(Job<int>(&simp_func));
  EXPECT_EQ(test7.GetArg1(), 0);
  EXPECT_EQ(test7.GetArg2(), 0);
  EXPECT_EQ(test7._debug_func(), 3);
  Job<float> test8 = std::move(Job<float>(&simp_func));
  EXPECT_EQ(test8.GetArg1(), 0.0f);
  EXPECT_EQ(test8.GetArg2(), 0.0f);
  EXPECT_EQ(test8._debug_func(), 3);
  Job<int> test9 = std::move(Job<int>());
  EXPECT_EQ(test9.GetArg1(), 0);
  EXPECT_EQ(test9.GetArg2(), 0);
  EXPECT_EQ(test9._debug_func(), 0);
  Job<float> test10 = std::move(Job<float>());
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  EXPECT_EQ(test10.GetArg2(), 0.0f);
  EXPECT_EQ(test10._debug_func(), 0);
  Job<NumericVariant> test11 = std::move(Job<NumericVariant>(&simp_add, &x, &y));
  EXPECT_EQ(std::get<int>(test11.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test11.GetArg2()), 20);
  EXPECT_EQ(test11._debug_func(), 1);
  Job<NumericVariant> test12 = std::move(Job<NumericVariant>{&simp_mul, &x});
  EXPECT_EQ(std::get<int>(test12.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test12.GetArg2()), 0);
  EXPECT_EQ(test12._debug_func(), 2);
  Job<NumericVariant> test13 = std::move(Job<NumericVariant>{&simp_func});
  EXPECT_EQ(std::get<int>(test13.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test13.GetArg2()), 0);
  EXPECT_EQ(test13._debug_func(), 3);
  Job<NumericVariant> test14 = std::move(Job<NumericVariant>(&simp_add, &x_f, &y_f));
  EXPECT_EQ(std::get<float>(test14.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test14.GetArg2()), 20.0f);
  EXPECT_EQ(test14._debug_func(), 1);
  Job<NumericVariant> test15 = std::move(Job<NumericVariant>{&simp_mul, &x_f});
  EXPECT_EQ(std::get<float>(test15.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test15.GetArg2()), 0.0f);
  EXPECT_EQ(test15._debug_func(), 2);
  Job<NumericVariant> test16 = std::move(Job<NumericVariant>{&simp_func});
  EXPECT_EQ(std::get<int>(test16.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test16.GetArg2()), 0);
  EXPECT_EQ(test16._debug_func(), 3);
  NumericVariant x_n = NumericVariant(10); // int variant.
  NumericVariant y_n = NumericVariant(20);

  Job<NumericVariant> test17 = std::move(Job<NumericVariant>(&simp_add, &x_n, &y_n));
  EXPECT_EQ(std::get<int>(test17.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test17.GetArg2()), 20);
  EXPECT_EQ(test17._debug_func(), 1);
  Job<NumericVariant> test18 = std::move(Job<NumericVariant>{&simp_mul, &x_n});
  EXPECT_EQ(std::get<int>(test18.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test18.GetArg2()), 0);
  EXPECT_EQ(test18._debug_func(), 2);
  Job<NumericVariant> test19 = std::move(Job<NumericVariant>{&simp_func});
  EXPECT_EQ(std::get<int>(test19.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test19.GetArg2()), 0);
  EXPECT_EQ(test19._debug_func(), 3);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  NumericVariant y_f_n = NumericVariant(20.2f);

  Job<NumericVariant> test20 = std::move(Job<NumericVariant>(&simp_add, &x_f_n, &y_f_n));
  EXPECT_EQ(std::get<float>(test20.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test20.GetArg2()), 20.2f);
  EXPECT_EQ(test20._debug_func(), 1);
  Job<NumericVariant> test21 = std::move(Job<NumericVariant>{&simp_mul, &x_f_n});
  EXPECT_EQ(std::get<float>(test21.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test21.GetArg2()), 0.0f);
  EXPECT_EQ(test21._debug_func(), 2);
  Job<NumericVariant> test22 = std::move(Job<NumericVariant>{&simp_func});
  EXPECT_EQ(std::get<int>(test22.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test22.GetArg2()), 0);
  EXPECT_EQ(test22._debug_func(), 3);
  Job<NumericVariant> test23 = std::move(Job<NumericVariant>{});
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test23.GetArg2()), 0);
  EXPECT_EQ(test23._debug_func(), 0);
}

TEST(Job, move_constr) {
  // Note that this does not test if the function pointer is correctly moved over.
  int x = 10;
  int y = 20;
  Job<int> test{&simp_add, &x, &y};
  Job<int> *out = move_func(std::move(test));
  EXPECT_EQ((*out).GetArg1(), 10);
  EXPECT_EQ((*out).GetArg2(), 20);
  EXPECT_EQ((*out)._debug_func(), 1);
  float x_f = 10.0;
  float y_f = 20.0;
  Job<float> test2{&simp_add, &x_f, &y_f};
  Job<float> *out2 = move_func(std::move(test2));
  EXPECT_EQ((*out2).GetArg1(), 10.0f);
  EXPECT_EQ((*out2).GetArg2(), 20.0f);
  EXPECT_EQ((*out2)._debug_func(), 1);
  Job<int> test3{&simp_mul, &x};
  Job<int> *out3 = move_func(std::move(test3));
  EXPECT_EQ((*out3).GetArg1(), 10);
  EXPECT_EQ((*out3).GetArg2(), 0);
  EXPECT_EQ((*out3)._debug_func(), 2);
  Job<float> test4{&simp_mul, &x_f};
  Job<float> *out4 = move_func(std::move(test4));
  EXPECT_EQ((*out4).GetArg1(), 10.0f);
  EXPECT_EQ((*out4).GetArg2(), 0);
  EXPECT_EQ((*out4)._debug_func(), 2);
  Job<int> test5{&simp_add, &x, &y};
  Job<int> *out5 = move_func(std::move(test5));
  EXPECT_EQ((*out5).GetArg1(), 10);
  EXPECT_EQ((*out5).GetArg2(), 20);
  EXPECT_EQ((*out5)._debug_func(), 1);
  Job<float> test6{&simp_add, &x_f, &y_f};
  Job<float> *out6 = move_func(std::move(test6));
  EXPECT_EQ((*out6).GetArg1(), 10.0f);
  EXPECT_EQ((*out6).GetArg2(), 20.0f);
  EXPECT_EQ((*out6)._debug_func(), 1);
  Job<int> test7{&simp_func};
  Job<int> *out7 = move_func(std::move(test7));
  EXPECT_EQ((*out7).GetArg1(), 0);
  EXPECT_EQ((*out7).GetArg2(), 0);
  EXPECT_EQ((*out7)._debug_func(), 3);
  Job<float> test8{&simp_func};
  Job<float> *out8 = move_func(std::move(test8));
  EXPECT_EQ((*out8).GetArg1(), 0.0f);
  EXPECT_EQ((*out8).GetArg2(), 0.0f);
  EXPECT_EQ((*out8)._debug_func(), 3);
  Job<int> test9{};
  Job<int> *out9 = move_func(std::move(test9));
  EXPECT_EQ((*out9).GetArg1(), 0);
  EXPECT_EQ((*out9).GetArg2(), 0);
  EXPECT_EQ((*out9)._debug_func(), 0);
  Job<float> test10{};
  Job<float> *out10 = move_func(std::move(test10));
  EXPECT_EQ((*out10).GetArg1(), 0.0f);
  EXPECT_EQ((*out10).GetArg2(), 0.0f);
  EXPECT_EQ((*out10)._debug_func(), 0);
  Job<NumericVariant> test11(&simp_add, &x, &y);
  Job<NumericVariant> *out11 = move_func(std::move(test11));
  EXPECT_EQ(std::get<int>((*out11).GetArg1()), 10);
  EXPECT_EQ(std::get<int>((*out11).GetArg2()), 20);
  EXPECT_EQ((*out11)._debug_func(), 1);
  Job<NumericVariant> test12{&simp_mul, &x};
  Job<NumericVariant> *out12 = move_func(std::move(test12));
  EXPECT_EQ(std::get<int>((*out12).GetArg1()), 10);
  EXPECT_EQ(std::get<int>((*out12).GetArg2()), 0);
  EXPECT_EQ((*out12)._debug_func(), 2);
  Job<NumericVariant> test13{&simp_func};
  Job<NumericVariant> *out13 = move_func(std::move(test13));
  EXPECT_EQ(std::get<int>((*out13).GetArg1()), 0);
  EXPECT_EQ(std::get<int>((*out13).GetArg2()), 0);
  EXPECT_EQ((*out13)._debug_func(), 3);
  Job<NumericVariant> test14(&simp_add, &x_f, &y_f);
  Job<NumericVariant> *out14 = move_func(std::move(test14));
  EXPECT_EQ(std::get<float>((*out14).GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>((*out14).GetArg2()), 20.0f);
  EXPECT_EQ((*out14)._debug_func(), 1);
  Job<NumericVariant> test15{&simp_mul, &x_f};
  Job<NumericVariant> *out15 = move_func(std::move(test15));
  EXPECT_EQ(std::get<float>((*out15).GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>((*out15).GetArg2()), 0.0f);
  EXPECT_EQ((*out15)._debug_func(), 2);
  Job<NumericVariant> test16{&simp_func};
  Job<NumericVariant> *out16 = move_func(std::move(test16));
  EXPECT_EQ(std::get<int>((*out16).GetArg1()), 0);
  EXPECT_EQ(std::get<int>((*out16).GetArg2()), 0);
  EXPECT_EQ((*out16)._debug_func(), 3);
  NumericVariant x_n = NumericVariant(10); // int variant.
  NumericVariant y_n = NumericVariant(20);

  Job<NumericVariant> test17(&simp_add, &x_n, &y_n);
  Job<NumericVariant> *out17 = move_func(std::move(test17));
  EXPECT_EQ(std::get<int>((*out17).GetArg1()), 10);
  EXPECT_EQ(std::get<int>((*out17).GetArg2()), 20);
  EXPECT_EQ((*out17)._debug_func(), 1);
  Job<NumericVariant> test18{&simp_mul, &x_n};
  Job<NumericVariant> *out18 = move_func(std::move(test18));
  EXPECT_EQ(std::get<int>((*out18).GetArg1()), 10);
  EXPECT_EQ(std::get<int>((*out18).GetArg2()), 0);
  EXPECT_EQ((*out18)._debug_func(), 2);
  Job<NumericVariant> test19{&simp_func};
  Job<NumericVariant> *out19 = move_func(std::move(test19));
  EXPECT_EQ(std::get<int>((*out19).GetArg1()), 0);
  EXPECT_EQ(std::get<int>((*out19).GetArg2()), 0);
  EXPECT_EQ((*out19)._debug_func(), 3);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  NumericVariant y_f_n = NumericVariant(20.2f);

  Job<NumericVariant> test20(&simp_add, &x_f_n, &y_f_n);
  Job<NumericVariant> *out20 = move_func(std::move(test20));
  EXPECT_EQ(std::get<float>((*out20).GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>((*out20).GetArg2()), 20.2f);
  EXPECT_EQ((*out20)._debug_func(), 1);
  Job<NumericVariant> test21{&simp_mul, &x_f_n};
  Job<NumericVariant> *out21 = move_func(std::move(test21));
  EXPECT_EQ(std::get<float>((*out21).GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>((*out21).GetArg2()), 0.0f);
  EXPECT_EQ((*out21)._debug_func(), 2);
  Job<NumericVariant> test22{&simp_func};
  Job<NumericVariant> *out22 = move_func(std::move(test22));
  EXPECT_EQ(std::get<int>((*out22).GetArg1()), 0);
  EXPECT_EQ(std::get<int>((*out22).GetArg2()), 0);
  EXPECT_EQ((*out22)._debug_func(), 3);
  Job<NumericVariant> test23 = std::move(Job<NumericVariant>{});
  Job<NumericVariant> *out23 = move_func(std::move(test22));
  EXPECT_EQ(std::get<int>((*out23).GetArg1()), 0);
  EXPECT_EQ(std::get<int>((*out23).GetArg2()), 0);
  EXPECT_EQ((*out23)._debug_func(), 0);
}

TEST(Job, run) {
  // This is just a sanity check to see that the Run method works.
  int x = 10;
  int y = 20;
  Job<int> test{&simp_add, &x, &y};
  test.Run();

  float x_f = 10.0;
  float y_f = 20.0;
  Job<float> test2{&simp_add, &x_f, &y_f};
  test2.Run();
  Job<int> test3{&simp_mul, &x};
  test3.Run();
  Job<float> test4{&simp_mul, &x_f};
  test4.Run();
  Job<int> test7{&simp_func};
  test7.Run();
  Job<float> test8{&simp_func};
  test8.Run();
  // Null should silently Run and do nothing.
  Job<int> test9{};
  test9.Run();
  Job<float> test10{};
  test10.Run();

  Job<NumericVariant> test11{&simp_add, &x, &y};
  test11.Run();
  Job<NumericVariant> test12{&simp_add, &x_f, &y_f};
  test12.Run();
  Job<NumericVariant> test13{&simp_mul, &x};
  test13.Run();
  Job<NumericVariant> test14{&simp_mul, &x_f};
  test14.Run();
  Job<NumericVariant> test15{&simp_func};
  test15.Run();
  Job<NumericVariant> test16{&simp_func};
  test16.Run();
}

TEST(Job, get_future) {
  // This is just a sanity check to see that the GetFuture method works.
  int x = 10;
  int y = 20;
  Job<int> test{&simp_add, &x, &y};
  std::future<int> out = test.GetFuture();

  float x_f = 10.0;
  float y_f = 20.0;
  Job<float> test2{&simp_add, &x_f, &y_f};
  std::future<float> out2 = test2.GetFuture();

  Job<int> test3{&simp_mul, &x};
  std::future<int> out3 = test3.GetFuture();
  Job<float> test4{&simp_mul, &x_f};
  std::future<float> out4 = test4.GetFuture();
  Job<int> test7{&simp_func};
  std::future<int> out7 = test7.GetFuture();
  Job<float> test8{&simp_func};
  std::future<float> out8 = test8.GetFuture();
  Job<int> test9{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  Job<float> test10{};
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);

  Job<NumericVariant> test11{&simp_add, &x, &y};
  std::future<NumericVariant> out11 = test11.GetFuture();
  Job<NumericVariant> test12{&simp_add, &x_f, &y_f};
  std::future<NumericVariant> out12 = test12.GetFuture();
  Job<NumericVariant> test13{&simp_mul, &x};
  std::future<NumericVariant> out13 = test13.GetFuture();
  Job<NumericVariant> test14{&simp_mul, &x_f};
  std::future<NumericVariant> out14 = test14.GetFuture();
  Job<NumericVariant> test15{&simp_func};
  std::future<NumericVariant> out15 = test15.GetFuture();
  Job<NumericVariant> test16{&simp_func};
  std::future<NumericVariant> out16 = test16.GetFuture();
  Job<NumericVariant> test17{};
  EXPECT_THROW(test17.GetFuture(), std::runtime_error);
  Job<NumericVariant> test18{};
  EXPECT_THROW(test18.GetFuture(), std::runtime_error);
}

TEST(Job, run_and_get_future) {
  int x = 10;
  int y = 20;
  Job<int> test{&simp_add, &x, &y};
  auto res = test.GetFuture();
  test.Run();
  EXPECT_EQ(res.get(), 30);

  float x_f = 10.0;
  float y_f = 20.0;
  Job<float> test2{&simp_add, &x_f, &y_f};
  auto res2 = test2.GetFuture();
  test2.Run();
  EXPECT_EQ(res2.get(), 30.0f);

  Job<int> test3{&simp_mul, &x};
  auto res3 = test3.GetFuture();
  test3.Run();
  EXPECT_EQ(res3.get(), 20);

  Job<float> test4{&simp_mul, &x_f};
  auto res4 = test4.GetFuture();
  test4.Run();
  EXPECT_EQ(res4.get(), 20.0f);

  Job<int> test7{&simp_func};
  auto res7 = test7.GetFuture();
  test7.Run();
  EXPECT_EQ(res7.get(), 100);

  Job<float> test8{&simp_func};
  auto res8 = test8.GetFuture();
  test8.Run();
  EXPECT_EQ(res8.get(), 100.0f);

  Job<NumericVariant> test11{&simp_add, &x, &y};
  std::future<NumericVariant> out11 = test11.GetFuture();
  test11.Run();
  EXPECT_EQ(std::get<int>(out11.get()), 30);

  Job<NumericVariant> test9{&simp_add, &x_f, &y_f};
  std::future<NumericVariant> res9 = test9.GetFuture();
  test9.Run();
  EXPECT_EQ(std::get<float>(res9.get()), 30.0f);

  Job<NumericVariant> test10{&simp_mul, &x};
  auto res10 = test10.GetFuture();
  test10.Run();
  EXPECT_EQ(std::get<int>(res10.get()), 20);

  Job<NumericVariant> test12{&simp_mul, &x_f};
  auto res12 = test12.GetFuture();
  test12.Run();
  EXPECT_EQ(std::get<float>(res12.get()), 20.0f);

  Job<NumericVariant> test13{&simp_func};
  auto res13 = test13.GetFuture();
  test13.Run();
  EXPECT_EQ(std::get<int>(res13.get()), 100);
}

template <int_or_float T> T simp_add_n(const T *arr, const int *len) {
  T out = 0;
  for (int i = 0; i < *len; i++) {
    out += arr[i];
  }
  return out;
}

template <int_or_float T> T *simp_add_arr(const T *arr, const int *len) {
  T *out = new T[*len];
  ;
  for (int i = 0; i < *len; i++) {
    out[i] = arr[i] + 2;
  }
  return out;
}

template <int_or_float T> T *simp_add_arr(const T *arr, const int len) {
  T *out = new T[len];
  ;
  for (int i = 0; i < len; i++) {
    out[i] = arr[i] + 2;
  }
  return out;
}

template <int_or_float T> T ele_add_n(const T *arr, const T *arr2, const int *len1, const int *len2) {
  T out = 0;
  for (int i = 0; i < *len1; i++) {
    out += (arr[i] + arr2[i]);
  }
  return out;
}

template <int_or_float T> T *ele_add_arr(const T *arr, const T *arr2, const int *len1, const int *len2) {
  T *out = new T[*len1];
  for (int i = 0; i < *len1; i++) {
    out[i] = (arr[i] + arr2[i]);
  }
  return out;
}
template <int_or_float T> T *ele_add_arr(const T *arr, const T *arr2, const int len1, const int len2) {
  T *out = new T[len1];
  for (int i = 0; i < len1; i++) {
    out[i] = (arr[i] + arr2[i]);
  }
  return out;
}

template <int_or_float T> T scalar_add_n(const T *arr, const int *len, const T *c) {
  T out = 0;
  for (int i = 0; i < *len; i++) {
    out += (arr[i] * *c);
  }
  return out;
}

template <int_or_float T> T *scalar_add_arr(const T *arr, const int *len, const T *c) {
  T *out = new T[*len];
  for (int i = 0; i < *len; i++) {
    out[i] = (arr[i] * *c);
  }
  return out;
}

TEST(JobArr, initialization) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7.5, 8, 9.125};
  int d = 10;

  JobArr<int> test{&simp_add_n, x, d};
  auto *res1 = test.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res1[i], x[i]);
  }
  EXPECT_EQ(test.GetArg2(), nullptr);
  EXPECT_EQ(test.GetLen1(), 10);
  EXPECT_EQ(test.GetLen2(), -1);
  EXPECT_EQ(test._debug_func(), 2);
  JobArr<float> test2{&simp_add_n, y, d};
  auto *res2 = test2.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res2[i], y[i]);
  }
  EXPECT_EQ(test2.GetArg2(), nullptr);
  EXPECT_EQ(test2.GetLen1(), 10);
  EXPECT_EQ(test2.GetLen2(), -1);
  EXPECT_EQ(test2._debug_func(), 2);
  JobArr<int> test3{&ele_add_n, x, x, d, d};
  auto *res3 = test3.GetArg1();
  auto *res4 = test3.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res3[i], x[i]);
    EXPECT_EQ(res4[i], x[i]);
  }
  EXPECT_EQ(test3.GetLen1(), 10);
  EXPECT_EQ(test3.GetLen2(), 10);
  EXPECT_EQ(test3._debug_func(), 1);
  JobArr<float> test4{&ele_add_n, y, y, d, d};
  auto *res5 = test4.GetArg1();
  auto *res6 = test4.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res5[i], y[i]);
    EXPECT_EQ(res6[i], y[i]);
  }
  EXPECT_EQ(test4.GetLen1(), 10);
  EXPECT_EQ(test4.GetLen2(), 10);
  EXPECT_EQ(test4._debug_func(), 1);
  JobArr<int> test5{};
  EXPECT_EQ(test5.GetArg1(), nullptr);
  EXPECT_EQ(test5.GetArg2(), nullptr);
  EXPECT_EQ(test5._debug_func(), 0);
  JobArr<float> test6{};
  EXPECT_EQ(test6.GetArg1(), nullptr);
  EXPECT_EQ(test6.GetArg2(), nullptr);
  EXPECT_EQ(test6._debug_func(), 0);
  int cons = 2;
  float cons_f = 2.125;
  JobArr<int> test7{&scalar_add_n, x, d, cons};
  auto *res7 = test7.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res7[i], x[i]);
  }
  EXPECT_EQ(test7.GetArg2(), nullptr);
  EXPECT_EQ(test7.GetLen1(), 10);
  EXPECT_EQ(test7.GetLen2(), -1);
  EXPECT_EQ(test7._debug_func(), 4);
  JobArr<float> test8{&scalar_add_n, y, d, cons_f};
  auto *res8 = test8.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res8[i], y[i]);
  }
  EXPECT_EQ(test8.GetArg2(), nullptr);
  EXPECT_EQ(test8.GetLen1(), 10);
  EXPECT_EQ(test8.GetLen2(), -1);
  EXPECT_EQ(test8._debug_func(), 5);
  JobArr<int> test9{&scalar_add_arr, x, d, cons};
  auto *res9 = test9.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res9[i], x[i]);
  }
  EXPECT_EQ(test9.GetArg2(), nullptr);
  EXPECT_EQ(test9.GetLen1(), 10);
  EXPECT_EQ(test9.GetLen2(), -1);
  EXPECT_EQ(test9._debug_func(), 9);
  JobArr<float> test10{&scalar_add_arr, y, d, cons_f};
  auto *res10 = test10.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res10[i], y[i]);
  }
  EXPECT_EQ(test10.GetArg2(), nullptr);
  EXPECT_EQ(test10.GetLen1(), 10);
  EXPECT_EQ(test10.GetLen2(), -1);
  EXPECT_EQ(test10._debug_func(), 10);
  JobArr<int> test11{&ele_add_arr, x, x, d, d};
  auto *res11 = test11.GetArg1();
  auto *res12 = test11.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
    EXPECT_EQ(res12[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  EXPECT_EQ(test11.GetLen2(), 10);
  EXPECT_EQ(test11._debug_func(), 6);
  JobArr<float> test12{&ele_add_arr, y, y, d, d};
  auto *res13 = test12.GetArg1();
  auto *res14 = test12.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
    EXPECT_EQ(res14[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);
  EXPECT_EQ(test12.GetLen2(), 10);
  EXPECT_EQ(test12._debug_func(), 6);
  JobArr<int> test13{&simp_add_arr, x, d};
  auto *res15 = test13.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res15[i], x[i]);
  }
  EXPECT_EQ(test13.GetArg2(), nullptr);
  EXPECT_EQ(test13.GetLen1(), 10);
  EXPECT_EQ(test13.GetLen2(), -1);
  EXPECT_EQ(test13._debug_func(), 7);
  JobArr<float> test14{&simp_add_arr, y, d};
  auto *res16 = test14.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res16[i], y[i]);
  }
  EXPECT_EQ(test14.GetArg2(), nullptr);
  EXPECT_EQ(test14.GetLen1(), 10);
  EXPECT_EQ(test14.GetLen2(), -1);
  EXPECT_EQ(test14._debug_func(), 7);
}

TEST(JobArr, move_assign) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8.5, 9.125};
  int d = 10;

  JobArr<int> test = std::move(JobArr<int>{&simp_add_n, x, d});
  auto *res1 = test.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res1[i], x[i]);
  }
  EXPECT_EQ(test.GetArg2(), nullptr);
  EXPECT_EQ(test.GetLen1(), 10);
  EXPECT_EQ(test.GetLen2(), -1);
  EXPECT_EQ(test._debug_func(), 2);
  JobArr<float> test2 = std::move(JobArr<float>{&simp_add_n, y, d});
  auto res2 = test2.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res2[i], y[i]);
  }
  EXPECT_EQ(test2.GetArg2(), nullptr);
  EXPECT_EQ(test2.GetLen1(), 10);
  EXPECT_EQ(test2.GetLen2(), -1);
  EXPECT_EQ(test2._debug_func(), 2);
  JobArr<int> test3 = std::move(JobArr<int>{&ele_add_n, x, x, d, d});
  auto *res3 = test3.GetArg1();
  auto *res4 = test3.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res3[i], x[i]);
    EXPECT_EQ(res4[i], x[i]);
  }
  EXPECT_EQ(test3.GetLen1(), 10);
  EXPECT_EQ(test3.GetLen2(), 10);
  EXPECT_EQ(test3._debug_func(), 1);
  JobArr<float> test4 = std::move(JobArr<float>{&ele_add_n, y, y, d, d});
  auto *res5 = test4.GetArg1();
  auto *res6 = test4.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res5[i], y[i]);
    EXPECT_EQ(res6[i], y[i]);
  }
  EXPECT_EQ(test4.GetLen1(), 10);
  EXPECT_EQ(test4.GetLen2(), 10);
  EXPECT_EQ(test4._debug_func(), 1);
  JobArr<int> test5 = std::move(JobArr<int>{});
  EXPECT_EQ(test5.GetArg1(), nullptr);
  EXPECT_EQ(test5.GetArg2(), nullptr);
  EXPECT_EQ(test5._debug_func(), 0);
  JobArr<float> test6 = std::move(JobArr<float>{});
  EXPECT_EQ(test6.GetArg1(), nullptr);
  EXPECT_EQ(test6.GetArg2(), nullptr);
  EXPECT_EQ(test6._debug_func(), 0);
  int cons = 2;
  float cons_f = 2.125;
  JobArr<int> test7 = std::move(JobArr<int>{&scalar_add_n, x, d, cons});
  auto *res7 = test7.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res7[i], x[i]);
  }
  EXPECT_EQ(test7.GetArg2(), nullptr);
  EXPECT_EQ(test7.GetLen1(), 10);
  EXPECT_EQ(test7.GetLen2(), -1);
  EXPECT_EQ(test7._debug_func(), 4);
  JobArr<float> test8 = std::move(JobArr<float>{&scalar_add_n, y, d, cons_f});
  auto *res8 = test8.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res8[i], y[i]);
  }
  EXPECT_EQ(test8.GetArg2(), nullptr);
  EXPECT_EQ(test8.GetLen1(), 10);
  EXPECT_EQ(test8.GetLen2(), -1);
  EXPECT_EQ(test8._debug_func(), 5);
  JobArr<int> test9 = JobArr<int>{&scalar_add_arr, x, d, cons};
  auto *res9 = test9.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res9[i], x[i]);
  }
  EXPECT_EQ(test9.GetArg2(), nullptr);
  EXPECT_EQ(test9.GetLen1(), 10);
  EXPECT_EQ(test9.GetLen2(), -1);
  EXPECT_EQ(test9._debug_func(), 9);
  JobArr<float> test10 = JobArr<float>{&scalar_add_arr, y, d, cons_f};
  auto *res10 = test10.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res10[i], y[i]);
  }
  EXPECT_EQ(test10.GetArg2(), nullptr);
  EXPECT_EQ(test10.GetLen1(), 10);
  EXPECT_EQ(test10.GetLen2(), -1);
  EXPECT_EQ(test10._debug_func(), 10);
  JobArr<int> test11 = JobArr<int>{&ele_add_arr, x, x, d, d};
  auto *res11 = test11.GetArg1();
  auto *res12 = test11.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
    EXPECT_EQ(res12[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  EXPECT_EQ(test11.GetLen2(), 10);
  EXPECT_EQ(test11._debug_func(), 6);
  JobArr<float> test12 = JobArr<float>{&ele_add_arr, y, y, d, d};
  auto *res13 = test12.GetArg1();
  auto *res14 = test12.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
    EXPECT_EQ(res14[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);
  EXPECT_EQ(test12.GetLen2(), 10);
  EXPECT_EQ(test12._debug_func(), 6);
  JobArr<int> test13 = JobArr<int>{&simp_add_arr, x, d};
  auto *res15 = test13.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res15[i], x[i]);
  }
  EXPECT_EQ(test13.GetArg2(), nullptr);
  EXPECT_EQ(test13.GetLen1(), 10);
  EXPECT_EQ(test13.GetLen2(), -1);
  EXPECT_EQ(test13._debug_func(), 7);
  JobArr<float> test14 = JobArr<float>{&simp_add_arr, y, d};
  auto *res16 = test14.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res16[i], y[i]);
  }
  EXPECT_EQ(test14.GetArg2(), nullptr);
  EXPECT_EQ(test14.GetLen1(), 10);
  EXPECT_EQ(test14.GetLen2(), -1);
  EXPECT_EQ(test14._debug_func(), 7);
}

TEST(JobArr, move_constructor) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8.5, 9.125};
  int d = 10;

  JobArr<int> test{&simp_add_n, x, d};
  JobArr<int> *out = move_func(std::move(test));
  auto res = (*out).GetArg1();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(res[i], x[i]);
  }
  EXPECT_EQ((*out).GetArg2(), nullptr);
  EXPECT_EQ((*out)._debug_func(), 2);
  JobArr<float> test2{&simp_add_n, y, d};
  JobArr<float> *out2 = move_func(std::move(test2));
  auto res2 = (*out2).GetArg1();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(res2[i], y[i]);
  }
  EXPECT_EQ((*out2).GetArg2(), nullptr);
  EXPECT_EQ((*out2)._debug_func(), 2);
  JobArr<int> test3{&ele_add_n, x, x, d, d};
  JobArr<int> *out3 = move_func(std::move(test3));
  auto res3 = (*out3).GetArg1();
  EXPECT_EQ((*out3)._debug_func(), 1);
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(res3[i], x[i]);
  }
  auto res4 = (*out3).GetArg2();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(res4[i], x[i]);
  }
  JobArr<float> test4{&ele_add_n, y, y, d, d};
  JobArr<float> *out4 = move_func(std::move(test4));
  auto res5 = (*out4).GetArg1();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(res5[i], y[i]);
  }
  auto res6 = (*out4).GetArg2();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(res6[i], y[i]);
  }
  EXPECT_EQ((*out4)._debug_func(), 1);
  JobArr<int> test5{};
  JobArr<int> *out5 = move_func(std::move(test5));
  EXPECT_EQ((*out5).GetArg1(), nullptr);
  EXPECT_EQ((*out5).GetArg2(), nullptr);
  EXPECT_EQ((*out5)._debug_func(), 0);
  JobArr<float> test6{};
  JobArr<float> *out6 = move_func(std::move(test6));
  EXPECT_EQ((*out6).GetArg1(), nullptr);
  EXPECT_EQ((*out6).GetArg2(), nullptr);
  EXPECT_EQ((*out6)._debug_func(), 0);

  int cons = 2;
  float cons_f = 2.125;
  JobArr<int> test7{&scalar_add_n, x, d, cons};
  JobArr<int> *out7 = move_func(std::move(test7));
  auto *res7 = (*out7).GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res7[i], x[i]);
  }
  EXPECT_EQ((*out7).GetArg2(), nullptr);
  EXPECT_EQ((*out7).GetLen1(), 10);
  EXPECT_EQ((*out7).GetLen2(), -1);
  EXPECT_EQ((*out7)._debug_func(), 4);
  JobArr<float> test8{&scalar_add_n, y, d, cons_f};
  JobArr<float> *out8 = move_func(std::move(test8));
  auto *res8 = (*out8).GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res8[i], y[i]);
  }
  EXPECT_EQ((*out8).GetArg2(), nullptr);
  EXPECT_EQ((*out8).GetLen1(), 10);
  EXPECT_EQ((*out8).GetLen2(), -1);
  EXPECT_EQ((*out8)._debug_func(), 5);
  JobArr<int> test9{&scalar_add_arr, x, d, cons};
  JobArr<int> *out9 = move_func(std::move(test9));
  auto *res9 = (*out9).GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res9[i], x[i]);
  }
  EXPECT_EQ((*out9).GetArg2(), nullptr);
  EXPECT_EQ((*out9).GetLen1(), 10);
  EXPECT_EQ((*out9).GetLen2(), -1);
  EXPECT_EQ((*out9)._debug_func(), 9);
  JobArr<float> test10{&scalar_add_arr, y, d, cons_f};
  JobArr<float> *out10 = move_func(std::move(test10));
  auto *res10 = (*out10).GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res10[i], y[i]);
  }
  EXPECT_EQ((*out10).GetArg2(), nullptr);
  EXPECT_EQ((*out10).GetLen1(), 10);
  EXPECT_EQ((*out10).GetLen2(), -1);
  EXPECT_EQ((*out10)._debug_func(), 10);
  JobArr<int> test11 = JobArr<int>{&ele_add_arr, x, x, d, d};
  JobArr<int> *out11 = move_func(std::move(test11));
  auto *res11 = (*out11).GetArg1();
  auto *res12 = (*out11).GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
    EXPECT_EQ(res12[i], x[i]);
  }
  EXPECT_EQ((*out11).GetLen1(), 10);
  EXPECT_EQ((*out11).GetLen2(), 10);
  EXPECT_EQ((*out11)._debug_func(), 6);
  JobArr<float> test12 = JobArr<float>{&ele_add_arr, y, y, d, d};
  JobArr<float> *out12 = move_func(std::move(test12));
  auto *res13 = (*out12).GetArg1();
  auto *res14 = (*out12).GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
    EXPECT_EQ(res14[i], y[i]);
  }
  EXPECT_EQ((*out12).GetLen1(), 10);
  EXPECT_EQ((*out12).GetLen2(), 10);
  EXPECT_EQ((*out12)._debug_func(), 6);
  JobArr<int> test13 = JobArr<int>{&simp_add_arr, x, d};
  JobArr<int> *out13 = move_func(std::move(test13));
  auto *res15 = (*out13).GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res15[i], x[i]);
  }
  EXPECT_EQ((*out13).GetArg2(), nullptr);
  EXPECT_EQ((*out13).GetLen1(), 10);
  EXPECT_EQ((*out13).GetLen2(), -1);
  EXPECT_EQ((*out13)._debug_func(), 7);
  JobArr<float> test14 = JobArr<float>{&simp_add_arr, y, d};
  JobArr<float> *out14 = move_func(std::move(test14));
  auto *res16 = (*out14).GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res16[i], y[i]);
  }
  EXPECT_EQ((*out14).GetArg2(), nullptr);
  EXPECT_EQ((*out14).GetLen1(), 10);
  EXPECT_EQ((*out14).GetLen2(), -1);
  EXPECT_EQ((*out14)._debug_func(), 7);
}

TEST(JobArr, run) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  JobArr<int> test{&simp_add_n, x, d};
  test.Run();

  JobArr<float> test2{&simp_add_n, y, d};
  test2.Run();
  JobArr<int> test3{&ele_add_n, x, x, d, d};
  test3.Run();
  JobArr<float> test4{&ele_add_n, y, y, d, d};
  test4.Run();
  // Null should silently Run and do nothing.
  JobArr<int> test9{};
  test9.Run();
  JobArr<float> test10{};
  test10.Run();

  int cons = 2;
  float cons_f = 2.125;
  JobArr<int> test7{&scalar_add_n, x, d, cons};
  test7.Run();
  JobArr<float> test8{&scalar_add_n, y, d, cons_f};
  test8.Run();

  JobArr<int> test11{&scalar_add_arr, x, d, cons};
  test11.Run();
  JobArr<float> test12{&scalar_add_arr, y, d, cons_f};
  test12.Run();
  JobArr<int> test13 = JobArr<int>{&ele_add_arr, x, x, d, d};
  test13.Run();
  JobArr<float> test14 = JobArr<float>{&ele_add_arr, y, y, d, d};
  test14.Run();
  JobArr<int> test15 = JobArr<int>{&simp_add_arr, x, d};
  test15.Run();
  JobArr<float> test16 = JobArr<float>{&simp_add_arr, y, d};
  test16.Run();
}

TEST(JobArr, get_future) {
  // This is just a sanity check to see that the GetFuture method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9.125};
  int d = 10;
  JobArr<int> test{&simp_add_n, x, d};
  std::future<int> out = test.GetFuture();
  JobArr<float> test2{&simp_add_n, y, d};
  std::future<float> out2 = test2.GetFuture();
  JobArr<int> test3{&ele_add_n, x, x, d, d};
  std::future<int> out3 = test3.GetFuture();
  JobArr<float> test4{&ele_add_n, y, y, d, d};
  std::future<float> out4 = test4.GetFuture();
  JobArr<int> test9{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  JobArr<float> test10{};
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);
  int cons = 2;
  float cons_f = 2.125;
  JobArr<float> test8{&scalar_add_n, y, d, cons_f};
  std::future<float> out8 = test8.GetFuture();
  JobArr<int> test7{&scalar_add_n, x, d, cons};
  std::future<int> out7 = test7.GetFuture();

  JobArr<int> test11{&scalar_add_arr, x, d, cons};
  std::future<int *> out11 = test11.GetFuturePtr();
  JobArr<float> test12{&scalar_add_arr, y, d, cons_f};
  std::future<float *> out12 = test12.GetFuturePtr();
  JobArr<int> test13 = JobArr<int>{&ele_add_arr, x, x, d, d};
  std::future<int *> out13 = test13.GetFuturePtr();
  JobArr<float> test14 = JobArr<float>{&ele_add_arr, y, y, d, d};
  std::future<float *> out14 = test14.GetFuturePtr();
  JobArr<int> test15 = JobArr<int>{&simp_add_arr, x, d};
  std::future<int *> out15 = test15.GetFuturePtr();
  JobArr<float> test16 = JobArr<float>{&simp_add_arr, y, d};
  std::future<float *> out16 = test16.GetFuturePtr();
}

TEST(JobArr, run_and_get_future) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9.125};
  int d = 10;
  JobArr<int> test{&simp_add_n, x, d};
  std::future<int> out = test.GetFuture();
  test.Run();
  EXPECT_EQ(out.get(), 45);
  JobArr<float> test2{&simp_add_n, y, d};
  std::future<float> out2 = test2.GetFuture();
  test2.Run();
  EXPECT_EQ(out2.get(), 45.125f);
  JobArr<int> test3{&ele_add_n, x, x, d, d};
  std::future<int> out3 = test3.GetFuture();
  test3.Run();
  EXPECT_EQ(out3.get(), 90);
  JobArr<float> test4{&ele_add_n, y, y, d, d};
  std::future<float> out4 = test4.GetFuture();
  test4.Run();
  EXPECT_EQ(out4.get(), 90.250f);

  int cons = 2;
  float cons_f = 2.125;
  JobArr<float> test8{&scalar_add_n, y, d, cons_f};
  std::future<float> out8 = test8.GetFuture();
  test8.Run();
  EXPECT_EQ(out8.get(), 95.890625f);
  JobArr<int> test7{&scalar_add_n, x, d, cons};
  std::future<int> out7 = test7.GetFuture();
  test7.Run();
  EXPECT_EQ(out7.get(), 90);

  JobArr<int> test11{&scalar_add_arr, x, d, cons};
  std::future<int *> out11 = test11.GetFuturePtr();
  test11.Run();
  auto temp = out11.get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp[i], x[i] * 2);
  }
  JobArr<float> test12{&scalar_add_arr, y, d, cons_f};
  std::future<float *> out12 = test12.GetFuturePtr();
  test12.Run();
  auto temp2 = out12.get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp2[i], y[i] * 2.125f);
  }
  JobArr<int> test13 = JobArr<int>{&ele_add_arr, x, x, d, d};
  std::future<int *> out13 = test13.GetFuturePtr();
  test13.Run();
  auto temp3 = out13.get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp3[i], x[i] * 2);
  }
  JobArr<float> test14 = JobArr<float>{&ele_add_arr, y, y, d, d};
  std::future<float *> out14 = test14.GetFuturePtr();
  test14.Run();
  auto temp4 = out14.get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp4[i], y[i] * 2.0f);
  }
  JobArr<int> test15 = JobArr<int>{&simp_add_arr, x, d};
  std::future<int *> out15 = test15.GetFuturePtr();
  test15.Run();
  auto temp5 = out15.get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp5[i], x[i] + 2);
  }
  JobArr<float> test16 = JobArr<float>{&simp_add_arr, y, d};
  std::future<float *> out16 = test16.GetFuturePtr();
  test16.Run();
  auto temp6 = out16.get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp6[i], y[i] + 2);
  }
}

TEST(JobNoParam, initialization) {
  // Note that this does not test if the function pointer is correctly initialized.
  JobNoParam<int> test1{&simp_func};
  JobNoParam<float> test8{&simp_func};
  JobNoParam<int> test9{};
  JobNoParam<float> test10{};
  JobNoParam<NumericVariant> test13{&simp_func};
  JobNoParam<NumericVariant> test23{};
}

TEST(JobNoParam, move_assignment) {
  // Note that this does not test if the function pointer is correctly moved over.
  JobNoParam<int> test7 = std::move(JobNoParam<int>(&simp_func));
  JobNoParam<float> test8 = std::move(JobNoParam<float>(&simp_func));
  JobNoParam<int> test9 = std::move(JobNoParam<int>());
  JobNoParam<float> test10 = std::move(JobNoParam<float>());
  JobNoParam<NumericVariant> test13 = std::move(JobNoParam<NumericVariant>{&simp_func});
  JobNoParam<NumericVariant> test23 = std::move(JobNoParam<NumericVariant>{});
}

TEST(JobNoParam, move_constr) {
  // Note that this does not test if the function pointer is correctly moved over.
  JobNoParam<int> test7{&simp_func};
  JobNoParam<int> *out7 = move_func(std::move(test7));
  JobNoParam<int> test9{};
  JobNoParam<int> *out9 = move_func(std::move(test9));
  JobNoParam<NumericVariant> test13{&simp_func};
  JobNoParam<NumericVariant> *out13 = move_func(std::move(test13));
  JobNoParam<NumericVariant> test23 = std::move(JobNoParam<NumericVariant>{});
  JobNoParam<NumericVariant> *out23 = move_func(std::move(test23));
}

TEST(JobNoParam, run) {
  // This is just a sanity check to see that the Run method works.
  JobNoParam<int> test7{&simp_func};
  test7.Run();
  JobNoParam<float> test8{&simp_func};
  test8.Run();
  // Null should silently Run and do nothing.
  JobNoParam<int> test9{};
  test9.Run();
  JobNoParam<float> test10{};
  test10.Run();
  JobNoParam<NumericVariant> test15{&simp_func};
  test15.Run();
  JobNoParam<NumericVariant> test16{&simp_func};
  test16.Run();
}

TEST(JobNoParam, get_future) {
  // This is just a sanity check to see that the GetFuture method works.
  JobNoParam<int> test7{&simp_func};
  std::future<int> out7 = test7.GetFuture();
  JobNoParam<float> test8{&simp_func};
  std::future<float> out8 = test8.GetFuture();
  JobNoParam<int> test9{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  JobNoParam<float> test10{};
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);

  JobNoParam<NumericVariant> test15{&simp_func};
  std::future<NumericVariant> out15 = test15.GetFuture();
  JobNoParam<NumericVariant> test16{&simp_func};
  std::future<NumericVariant> out16 = test16.GetFuture();
  JobNoParam<NumericVariant> test17{};
  EXPECT_THROW(test17.GetFuture(), std::runtime_error);
  JobNoParam<NumericVariant> test18{};
  EXPECT_THROW(test18.GetFuture(), std::runtime_error);

  JobNoParam<int> test1{&simp_func};
  std::future<int> out1;
  test1.GetFuture(out1);
  JobNoParam<float> test2{&simp_func};
  std::future<float> out2;
  test2.GetFuture(out2);
  JobNoParam<int> test3{};
  std::future<int> out3;
  EXPECT_THROW(test3.GetFuture(out3), std::runtime_error);
  JobNoParam<float> test4{};
  std::future<float> out4;
  EXPECT_THROW(test4.GetFuture(out4), std::runtime_error);

  JobNoParam<NumericVariant> test5{&simp_func};
  std::future<NumericVariant> out5;
  test5.GetFuture(out5);
  JobNoParam<NumericVariant> test6{&simp_func};
  std::future<NumericVariant> out6 = test6.GetFuture();
  JobNoParam<NumericVariant> test11{};
  std::future<NumericVariant> out11;
  EXPECT_THROW(test11.GetFuture(out11), std::runtime_error);
  JobNoParam<NumericVariant> test12{};
  std::future<NumericVariant> out12;
  EXPECT_THROW(test12.GetFuture(out12), std::runtime_error);
}

TEST(JobNoParam, run_and_get_future) {
  JobNoParam<int> test7{&simp_func};
  auto res7 = test7.GetFuture();
  test7.Run();
  EXPECT_EQ(res7.get(), 100);

  JobNoParam<float> test8{&simp_func};
  auto res8 = test8.GetFuture();
  test8.Run();
  EXPECT_EQ(res8.get(), 100.0f);

  JobNoParam<NumericVariant> test13{&simp_func};
  auto res13 = test13.GetFuture();
  test13.Run();
  EXPECT_EQ(std::get<int>(res13.get()), 100);

  JobNoParam<int> test1{&simp_func};
  std::future<int> res1;
  test1.GetFuture(res1);
  test1.Run();
  EXPECT_EQ(res1.get(), 100);

  JobNoParam<float> test2{&simp_func};
  std::future<float> res2;
  test2.GetFuture(res2);
  test2.Run();
  EXPECT_EQ(res2.get(), 100.0f);

  JobNoParam<NumericVariant> test3{&simp_func};
  std::future<NumericVariant> res3;
  test3.GetFuture(res3);
  test3.Run();
  EXPECT_EQ(std::get<int>(res3.get()), 100);
}

TEST(JobOneParam, initialization) {
  // Note that this does not test if the function pointer is correctly initialized.
  int x = 10;
  float x_f = 10.0;
  JobOneParam<int> test3{&simp_mul, x};
  EXPECT_EQ(test3.GetArg1(), 10);
  JobOneParam<float> test4{&simp_mul, x_f};
  EXPECT_EQ(test4.GetArg1(), 10.0f);
  NumericVariant x_n = NumericVariant(10); // int variant.

  JobOneParam<NumericVariant> test18{&simp_mul, x_n};
  EXPECT_EQ(std::get<int>(test18.GetArg1()), 10);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  JobOneParam<NumericVariant> test21{&simp_mul, x_f_n};
  EXPECT_EQ(std::get<float>(test21.GetArg1()), 10.1f);

  JobOneParam<int> test5{&simp_mul_ref, x};
  EXPECT_EQ(test5.GetArg1(), 10);
  JobOneParam<float> test6{&simp_mul_ref, x_f};
  EXPECT_EQ(test6.GetArg1(), 10.0f);

  JobOneParam<int> test9{};
  EXPECT_EQ(test9.GetArg1(), 0.0);
  JobOneParam<float> test10{};
  EXPECT_EQ(test10.GetArg1(), 0.0);

  JobOneParam<int> test7{&simp_mul_c, x};
  EXPECT_EQ(test7.GetArg1(), 10);
  JobOneParam<float> test8{&simp_mul_c, x_f};
  EXPECT_EQ(test8.GetArg1(), 10.0f);
}

TEST(JobOneParam, move_assignment) {
  // Note that this does not test if the function pointer is correctly moved over.
  int x = 10;
  float x_f = 10.0;
  auto temp = JobOneParam<int>(&simp_mul, x);
  JobOneParam<int> test3;
  test3 = std::move(temp);
  EXPECT_EQ(test3.GetArg1(), 10);
  auto temp2 = JobOneParam<float>(&simp_mul, x_f);
  JobOneParam<float> test4;
  test4 = std::move(temp2);
  EXPECT_EQ(test4.GetArg1(), 10.0f);
  auto temp3 = JobOneParam<int>();
  JobOneParam<int> test9;
  test9 = std::move(temp3);
  EXPECT_EQ(test9.GetArg1(), 0);
  auto temp4 = JobOneParam<float>();
  JobOneParam<float> test10;
  test10 = std::move(temp4);
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  auto temp5 = JobOneParam<NumericVariant>{&simp_mul, x};
  JobOneParam<NumericVariant> test12;
  test12 = std::move(temp5);
  EXPECT_EQ(std::get<int>(test12.GetArg1()), 10);
  auto temp6 = JobOneParam<NumericVariant>{&simp_mul, x_f};
  JobOneParam<NumericVariant> test15;
  test15 = std::move(temp6);
  EXPECT_EQ(std::get<float>(test15.GetArg1()), 10.0f);
  NumericVariant x_n = NumericVariant(10); // int variant.

  auto temp7 = JobOneParam<NumericVariant>{&simp_mul, x_n};
  JobOneParam<NumericVariant> test18;
  test18 = std::move(temp7);
  EXPECT_EQ(std::get<int>(test18.GetArg1()), 10);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  auto temp8 = JobOneParam<NumericVariant>{&simp_mul, x_f_n};
  JobOneParam<NumericVariant> test21;
  test21 = std::move(temp8);
  EXPECT_EQ(std::get<float>(test21.GetArg1()), 10.1f);
  auto temp9 = JobOneParam<NumericVariant>{};
  JobOneParam<NumericVariant> test23;
  test23 = std::move(temp9);
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);

  auto temp10 = JobOneParam<int>(&simp_mul_ref, x);
  JobOneParam<int> test19;
  test19 = std::move(temp10);
  EXPECT_EQ(test19.GetArg1(), 10);
  auto temp11 = JobOneParam<float>(&simp_mul_ref, x_f);
  JobOneParam<float> test20;
  test20 = std::move(temp11);
  EXPECT_EQ(test20.GetArg1(), 10.0f);

  auto temp24 = JobOneParam<int>(&simp_mul_c, x);
  JobOneParam<int> test24;
  test24 = std::move(temp24);
  EXPECT_EQ(test24.GetArg1(), 10);
  auto temp25 = JobOneParam<float>(&simp_mul_c, x_f);
  JobOneParam<float> test25;
  test25 = std::move(temp25);
  EXPECT_EQ(test25.GetArg1(), 10.0f);
}

TEST(JobOneParam, move_constr) {
  // Note that this does not test if the function pointer is correctly moved over.
  int x = 10;
  float x_f = 10.0;
  auto temp = JobOneParam<int>(&simp_mul, x);
  JobOneParam<int> test3 = std::move(temp);
  EXPECT_EQ(test3.GetArg1(), 10);
  auto temp2 = JobOneParam<float>(&simp_mul, x_f);
  JobOneParam<float> test4 = std::move(temp2);
  EXPECT_EQ(test4.GetArg1(), 10.0f);
  auto temp3 = JobOneParam<int>();
  JobOneParam<int> test9 = std::move(temp3);
  EXPECT_EQ(test9.GetArg1(), 0);
  auto temp4 = JobOneParam<float>();
  JobOneParam<float> test10 = std::move(temp4);
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  auto temp5 = JobOneParam<NumericVariant>{&simp_mul, x};
  JobOneParam<NumericVariant> test12 = std::move(temp5);
  EXPECT_EQ(std::get<int>(test12.GetArg1()), 10);
  auto temp6 = JobOneParam<NumericVariant>{&simp_mul, x_f};
  JobOneParam<NumericVariant> test15 = std::move(temp6);
  EXPECT_EQ(std::get<float>(test15.GetArg1()), 10.0f);
  NumericVariant x_n = NumericVariant(10); // int variant.

  auto temp7 = JobOneParam<NumericVariant>{&simp_mul, x_n};
  JobOneParam<NumericVariant> test18 = std::move(temp7);
  EXPECT_EQ(std::get<int>(test18.GetArg1()), 10);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  auto temp8 = JobOneParam<NumericVariant>{&simp_mul, x_f_n};
  JobOneParam<NumericVariant> test21 = std::move(temp8);
  EXPECT_EQ(std::get<float>(test21.GetArg1()), 10.1f);
  auto temp9 = JobOneParam<NumericVariant>{};
  JobOneParam<NumericVariant> test23 = std::move(temp9);
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);

  auto temp10 = JobOneParam<int>(&simp_mul_ref, x);
  JobOneParam<int> test19 = std::move(temp10);
  EXPECT_EQ(test19.GetArg1(), 10);
  auto temp11 = JobOneParam<float>(&simp_mul_ref, x_f);
  JobOneParam<float> test20 = std::move(temp11);
  EXPECT_EQ(test20.GetArg1(), 10.0f);

  auto temp12 = JobOneParam<int>(&simp_mul_c, x);
  JobOneParam<int> test24 = std::move(temp12);
  EXPECT_EQ(test24.GetArg1(), 10);
  auto temp13 = JobOneParam<float>(&simp_mul_c, x_f);
  JobOneParam<float> test25 = std::move(temp13);
  EXPECT_EQ(test25.GetArg1(), 10.0f);
}

TEST(JobOneParam, run) {
  // This is just a sanity check to see that the Run method works.
  int x = 10;
  float x_f = 10.0;
  JobOneParam<int> test3{&simp_mul, x};
  test3.Run();
  JobOneParam<float> test4{&simp_mul, x_f};
  test4.Run();
  // Null should silently Run and do nothing.
  JobOneParam<int> test9{};
  test9.Run();
  JobOneParam<float> test10{};
  test10.Run();

  JobOneParam<NumericVariant> test13{&simp_mul, x};
  test13.Run();
  JobOneParam<NumericVariant> test14{&simp_mul, x_f};
  test14.Run();

  JobOneParam<int> test19 = std::move(JobOneParam<int>(&simp_mul_ref, x));
  test19.Run();
  JobOneParam<float> test20 = std::move(JobOneParam<float>(&simp_mul_ref, x_f));
  test20.Run();

  JobOneParam<int> test21 = std::move(JobOneParam<int>(&simp_mul_c, x));
  test21.Run();
  JobOneParam<float> test22 = std::move(JobOneParam<float>(&simp_mul_c, x_f));
  test22.Run();
}

TEST(JobOneParam, get_future) {
  // This is just a sanity check to see that the GetFuture method works.
  int x = 10;
  float x_f = 10.0;
  JobOneParam<int> test3{&simp_mul, x};
  std::future<int> out3 = test3.GetFuture();
  JobOneParam<int> test3_1{&simp_mul, x};
  std::future<int> out3_1;
  test3_1.GetFuture(out3_1);
  JobOneParam<float> test4{&simp_mul, x_f};
  std::future<float> out4 = test4.GetFuture();
  JobOneParam<float> test4_1{&simp_mul, x_f};
  std::future<float> out4_1;
  test4_1.GetFuture(out4_1);
  JobOneParam<int> test9{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  JobOneParam<int> test9_1{};
  std::future<int> out9_1;
  EXPECT_THROW(test9_1.GetFuture(out9_1), std::runtime_error);
  JobOneParam<float> test10{};
  std::future<float> out10_1;
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);
  JobOneParam<float> test10_1{};
  EXPECT_THROW(test10_1.GetFuture(out10_1), std::runtime_error);

  JobOneParam<NumericVariant> test13{&simp_mul, x};
  std::future<NumericVariant> out13 = test13.GetFuture();
  JobOneParam<NumericVariant> test14{&simp_mul, x_f};
  std::future<NumericVariant> out14 = test14.GetFuture();
  JobOneParam<NumericVariant> test17{};
  EXPECT_THROW(test17.GetFuture(), std::runtime_error);
  JobOneParam<NumericVariant> test18{};
  EXPECT_THROW(test18.GetFuture(), std::runtime_error);

  JobOneParam<NumericVariant> test7{&simp_mul, x};
  std::future<NumericVariant> res7;
  test7.GetFuture(res7);

  JobOneParam<NumericVariant> test8{&simp_mul, x_f};
  std::future<NumericVariant> res8;
  test8.GetFuture(res8);

  JobOneParam<int> test19 = std::move(JobOneParam<int>(&simp_mul_ref, x));
  std::future<int> out19 = test19.GetFuture();
  JobOneParam<float> test20 = std::move(JobOneParam<float>(&simp_mul_ref, x_f));
  std::future<float> out20 = test20.GetFuture();

  JobOneParam<int> test21 = std::move(JobOneParam<int>(&simp_mul_c, x));
  std::future<int> out21 = test21.GetFuture();
  JobOneParam<float> test22 = std::move(JobOneParam<float>(&simp_mul_c, x_f));
  std::future<float> out22 = test22.GetFuture();
}

TEST(JobOneParam, run_and_get_future) {
  int x = 10;
  float x_f = 10.0;
  JobOneParam<int> test3{&simp_mul, x};
  auto res3 = test3.GetFuture();
  test3.Run();
  EXPECT_EQ(res3.get(), 20);

  JobOneParam<float> test4{&simp_mul, x_f};
  auto res4 = test4.GetFuture();
  test4.Run();
  EXPECT_EQ(res4.get(), 20.0f);

  JobOneParam<NumericVariant> test10{&simp_mul, x};
  auto res10 = test10.GetFuture();
  test10.Run();
  EXPECT_EQ(std::get<int>(res10.get()), 20);

  JobOneParam<NumericVariant> test12{&simp_mul, x_f};
  auto res12 = test12.GetFuture();
  test12.Run();
  EXPECT_EQ(std::get<float>(res12.get()), 20.0f);

  JobOneParam<int> test19 = std::move(JobOneParam<int>(&simp_mul_ref, x));
  std::future<int> out19 = test19.GetFuture();
  test19.Run();
  EXPECT_EQ(out19.get(), 20);
  JobOneParam<float> test20 = std::move(JobOneParam<float>(&simp_mul_ref, x_f));
  std::future<float> out20 = test20.GetFuture();
  test20.Run();
  EXPECT_EQ(out20.get(), 20.0f);

  JobOneParam<int> test21 = std::move(JobOneParam<int>(&simp_mul_c, x));
  std::future<int> out21 = test21.GetFuture();
  test21.Run();
  EXPECT_EQ(out21.get(), 20);
  JobOneParam<float> test22 = std::move(JobOneParam<float>(&simp_mul_c, x_f));
  std::future<float> out22 = test22.GetFuture();
  test22.Run();
  EXPECT_EQ(out22.get(), 20.0f);

  JobOneParam<int> test5{&simp_mul, x};
  std::future<int> res5;
  test5.GetFuture(res5);
  test5.Run();
  EXPECT_EQ(res5.get(), 20);

  JobOneParam<float> test6{&simp_mul, x_f};
  std::future<float> res6;
  test6.GetFuture(res6);
  test6.Run();
  EXPECT_EQ(res6.get(), 20.0f);

  JobOneParam<NumericVariant> test7{&simp_mul, x};
  std::future<NumericVariant> res7;
  test7.GetFuture(res7);
  test7.Run();
  EXPECT_EQ(std::get<int>(res7.get()), 20);

  JobOneParam<NumericVariant> test8{&simp_mul, x_f};
  std::future<NumericVariant> res8;
  test8.GetFuture(res8);
  test8.Run();
  EXPECT_EQ(std::get<float>(res8.get()), 20.0f);

  // Tests move constructor.
  JobOneParam<int> test9 = std::move(JobOneParam<int>(&simp_mul_ref, x));
  std::future<int> out9;
  test9.GetFuture(out9);
  test9.Run();
  EXPECT_EQ(out9.get(), 20);
  JobOneParam<float> test15 = std::move(JobOneParam<float>(&simp_mul_ref, x_f));
  std::future<float> out15;
  test15.GetFuture(out15);
  test15.Run();
  EXPECT_EQ(out15.get(), 20.0f);

  JobOneParam<int> test16 = std::move(JobOneParam<int>(&simp_mul_c, x));
  std::future<int> out16;
  test16.GetFuture(out16);
  test16.Run();
  EXPECT_EQ(out16.get(), 20);
  JobOneParam<float> test17 = std::move(JobOneParam<float>(&simp_mul_c, x_f));
  std::future<float> out17;
  test17.GetFuture(out17);
  test17.Run();
  EXPECT_EQ(out17.get(), 20.0f);
}

TEST(JobTwoParam, initialization) {
  // Note that this does not test if the function pointer is correctly initialized.
  int x = 10;
  int y = 20;
  JobTwoParam<int> test{&simp_add, x, y};
  EXPECT_EQ(test.GetArg1(), 10);
  EXPECT_EQ(test.GetArg2(), 20);
  float x_f = 10.0;
  float y_f = 20.0;
  JobTwoParam<float> test2{&simp_add, x_f, y_f};
  EXPECT_EQ(test2.GetArg1(), 10.0f);
  EXPECT_EQ(test2.GetArg2(), 20.0f);
  JobTwoParam<int> test5{&simp_add, x, y};
  EXPECT_EQ(test5.GetArg1(), 10);
  EXPECT_EQ(test5.GetArg2(), 20);
  JobTwoParam<float> test6{&simp_add, x_f, y_f};
  EXPECT_EQ(test6.GetArg1(), 10.0f);
  EXPECT_EQ(test6.GetArg2(), 20.0f);
  JobTwoParam<int> test9{};
  EXPECT_EQ(test9.GetArg1(), 0);
  EXPECT_EQ(test9.GetArg2(), 0);
  JobTwoParam<float> test10{};
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  EXPECT_EQ(test10.GetArg2(), 0.0f);
  JobTwoParam<NumericVariant> test14(&simp_add, x_f, y_f);
  EXPECT_EQ(std::get<float>(test14.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test14.GetArg2()), 20.0f);
  NumericVariant x_n = NumericVariant(10); // int variant.
  NumericVariant y_n = NumericVariant(20);

  JobTwoParam<NumericVariant> test17(&simp_add, x_n, y_n);
  EXPECT_EQ(std::get<int>(test17.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test17.GetArg2()), 20);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  NumericVariant y_f_n = NumericVariant(20.2f);

  JobTwoParam<NumericVariant> test20(&simp_add, x_f_n, y_f_n);
  EXPECT_EQ(std::get<float>(test20.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test20.GetArg2()), 20.2f);
  JobTwoParam<NumericVariant> test23{};
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test23.GetArg2()), 0);
  JobTwoParam<NumericVariant> test24{};
  EXPECT_EQ(std::get<int>(test24.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test24.GetArg2()), 0);

  JobTwoParam<float> test25{&simp_add_ref, x_f, y_f};
  EXPECT_EQ((test25.GetArg1()), 10.0f);
  EXPECT_EQ((test25.GetArg2()), 20.0f);
  JobTwoParam<int> test26{&simp_add_ref, x, y};
  EXPECT_EQ(test5.GetArg1(), 10);
  EXPECT_EQ(test5.GetArg2(), 20);

  JobTwoParam<float> test27{&simp_add_c, x_f, y_f};
  EXPECT_EQ((test27.GetArg1()), 10.0f);
  EXPECT_EQ((test27.GetArg2()), 20.0f);
  JobTwoParam<int> test28{&simp_add_c, x, y};
  EXPECT_EQ(test28.GetArg1(), 10);
  EXPECT_EQ(test28.GetArg2(), 20);
}

TEST(JobTwoParam, move_assignment) {
  // Note that this does not test if the function pointer is correctly moved over.
  int x = 10;
  int y = 20;
  auto temp1 = JobTwoParam<int>(&simp_add, x, y);
  JobTwoParam<int> test;
  test = std::move(temp1);
  EXPECT_EQ(test.GetArg1(), 10);
  EXPECT_EQ(test.GetArg2(), 20);
  float x_f = 10.0;
  float y_f = 20.0;
  auto temp2 = JobTwoParam<float>(&simp_add, x_f, y_f);
  JobTwoParam<float> test2;
  test2 = std::move(temp2);
  EXPECT_EQ(test2.GetArg1(), 10);
  EXPECT_EQ(test2.GetArg2(), 20);
  auto temp3 = JobTwoParam<int>(&simp_add, x, y);
  JobTwoParam<int> test5;
  test5 = std::move(temp3);
  EXPECT_EQ(test5.GetArg1(), 10);
  EXPECT_EQ(test5.GetArg2(), 20);
  auto temp4 = JobTwoParam<float>(&simp_add, x_f, y_f);
  JobTwoParam<float> test6;
  test6 = std::move(temp4);
  EXPECT_EQ(test6.GetArg1(), 10.0f);
  EXPECT_EQ(test6.GetArg2(), 20.0f);
  auto temp5 = JobTwoParam<int>();
  JobTwoParam<int> test9;
  test9 = std::move(temp5);
  EXPECT_EQ(test9.GetArg1(), 0);
  EXPECT_EQ(test9.GetArg2(), 0);
  auto temp6 = JobTwoParam<float>();
  JobTwoParam<float> test10;
  test10 = std::move(temp6);
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  EXPECT_EQ(test10.GetArg2(), 0.0f);
  auto temp7 = JobTwoParam<NumericVariant>(&simp_add, x, y);
  JobTwoParam<NumericVariant> test11;
  test11 = std::move(temp7);
  EXPECT_EQ(std::get<int>(test11.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test11.GetArg2()), 20);
  auto temp8 = JobTwoParam<NumericVariant>(&simp_add, x_f, y_f);
  JobTwoParam<NumericVariant> test14;
  test14 = std::move(temp8);
  EXPECT_EQ(std::get<float>(test14.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test14.GetArg2()), 20.0f);
  NumericVariant x_n = NumericVariant(10); // int variant.
  NumericVariant y_n = NumericVariant(20);

  auto temp9 = JobTwoParam<NumericVariant>(&simp_add, x_n, y_n);
  JobTwoParam<NumericVariant> test17;
  test17 = std::move(temp9);
  EXPECT_EQ(std::get<int>(test17.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test17.GetArg2()), 20);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  NumericVariant y_f_n = NumericVariant(20.2f);

  auto temp10 = JobTwoParam<NumericVariant>(&simp_add, x_f_n, y_f_n);
  JobTwoParam<NumericVariant> test20;
  test20 = std::move(temp10);
  EXPECT_EQ(std::get<float>(test20.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test20.GetArg2()), 20.2f);
  auto temp11 = Job<NumericVariant>{};
  Job<NumericVariant> test23;
  test23 = std::move(temp11);
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test23.GetArg2()), 0);
  EXPECT_EQ(test23._debug_func(), 0);

  auto temp24 = JobTwoParam<int>(&simp_add_ref, x, y);
  JobTwoParam<int> test24;
  test24 = std::move(temp24);
  EXPECT_EQ(test24.GetArg1(), 10);
  EXPECT_EQ(test24.GetArg2(), 20);
  auto temp25 = JobTwoParam<float>(&simp_add_ref, x_f, y_f);
  JobTwoParam<float> test25;
  test25 = std::move(temp25);
  EXPECT_EQ(test25.GetArg1(), 10.0f);
  EXPECT_EQ(test25.GetArg2(), 20.0f);

  auto temp26 = JobTwoParam<int>(&simp_add_c, x, y);
  JobTwoParam<int> test26;
  test26 = std::move(temp26);
  EXPECT_EQ(test26.GetArg1(), 10);
  EXPECT_EQ(test26.GetArg2(), 20);
  auto temp27 = JobTwoParam<float>(&simp_add_c, x_f, y_f);
  JobTwoParam<float> test27;
  test27 = std::move(temp27);
  EXPECT_EQ(test27.GetArg1(), 10.0f);
  EXPECT_EQ(test27.GetArg2(), 20.0f);
}

TEST(JobTwoParam, move_constr) {
  // Note that this does not test if the function pointer is correctly moved over.
  int x = 10;
  int y = 20;
  auto temp1 = JobTwoParam<int>(&simp_add, x, y);
  JobTwoParam<int> test = std::move(temp1);
  EXPECT_EQ(test.GetArg1(), 10);
  EXPECT_EQ(test.GetArg2(), 20);
  float x_f = 10.0;
  float y_f = 20.0;
  auto temp2 = JobTwoParam<float>(&simp_add, x_f, y_f);
  JobTwoParam<float> test2 = std::move(temp2);
  EXPECT_EQ(test2.GetArg1(), 10);
  EXPECT_EQ(test2.GetArg2(), 20);
  auto temp3 = JobTwoParam<int>(&simp_add, x, y);
  JobTwoParam<int> test5 = std::move(temp3);
  EXPECT_EQ(test5.GetArg1(), 10);
  EXPECT_EQ(test5.GetArg2(), 20);
  auto temp4 = JobTwoParam<float>(&simp_add, x_f, y_f);
  JobTwoParam<float> test6 = std::move(temp4);
  EXPECT_EQ(test6.GetArg1(), 10.0f);
  EXPECT_EQ(test6.GetArg2(), 20.0f);
  auto temp5 = JobTwoParam<int>();
  JobTwoParam<int> test9 = std::move(temp5);
  EXPECT_EQ(test9.GetArg1(), 0);
  EXPECT_EQ(test9.GetArg2(), 0);
  auto temp6 = JobTwoParam<float>();
  JobTwoParam<float> test10 = std::move(temp6);
  EXPECT_EQ(test10.GetArg1(), 0.0f);
  EXPECT_EQ(test10.GetArg2(), 0.0f);
  auto temp7 = JobTwoParam<NumericVariant>(&simp_add, x, y);
  JobTwoParam<NumericVariant> test11 = std::move(temp7);
  EXPECT_EQ(std::get<int>(test11.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test11.GetArg2()), 20);
  auto temp8 = JobTwoParam<NumericVariant>(&simp_add, x_f, y_f);
  JobTwoParam<NumericVariant> test14 = std::move(temp8);
  EXPECT_EQ(std::get<float>(test14.GetArg1()), 10.0f);
  EXPECT_EQ(std::get<float>(test14.GetArg2()), 20.0f);
  NumericVariant x_n = NumericVariant(10); // int variant.
  NumericVariant y_n = NumericVariant(20);

  auto temp9 = JobTwoParam<NumericVariant>(&simp_add, x_n, y_n);
  JobTwoParam<NumericVariant> test17 = std::move(temp9);
  EXPECT_EQ(std::get<int>(test17.GetArg1()), 10);
  EXPECT_EQ(std::get<int>(test17.GetArg2()), 20);
  NumericVariant x_f_n = NumericVariant(10.1f); // float variant.
  NumericVariant y_f_n = NumericVariant(20.2f);

  auto temp10 = JobTwoParam<NumericVariant>(&simp_add, x_f_n, y_f_n);
  JobTwoParam<NumericVariant> test20 = std::move(temp10);
  EXPECT_EQ(std::get<float>(test20.GetArg1()), 10.1f);
  EXPECT_EQ(std::get<float>(test20.GetArg2()), 20.2f);
  auto temp11 = Job<NumericVariant>{};
  Job<NumericVariant> test23 = std::move(temp11);
  EXPECT_EQ(std::get<int>(test23.GetArg1()), 0);
  EXPECT_EQ(std::get<int>(test23.GetArg2()), 0);
  EXPECT_EQ(test23._debug_func(), 0);

  auto temp24 = JobTwoParam<int>(&simp_add_ref, x, y);
  JobTwoParam<int> test24 = std::move(temp24);
  EXPECT_EQ(test24.GetArg1(), 10);
  EXPECT_EQ(test24.GetArg2(), 20);
  auto temp25 = JobTwoParam<float>(&simp_add_ref, x_f, y_f);
  JobTwoParam<float> test25 = std::move(temp25);
  EXPECT_EQ(test25.GetArg1(), 10.0f);
  EXPECT_EQ(test25.GetArg2(), 20.0f);

  auto temp26 = JobTwoParam<int>(&simp_add_c, x, y);
  JobTwoParam<int> test26 = std::move(temp26);
  EXPECT_EQ(test26.GetArg1(), 10);
  EXPECT_EQ(test26.GetArg2(), 20);
  auto temp27 = JobTwoParam<float>(&simp_add_c, x_f, y_f);
  JobTwoParam<float> test27 = std::move(temp27);
  EXPECT_EQ(test27.GetArg1(), 10.0f);
  EXPECT_EQ(test27.GetArg2(), 20.0f);
}

TEST(JobTwoParam, run) {
  // This is just a sanity check to see that the Run method works.
  int x = 10;
  int y = 20;
  JobTwoParam<int> test{&simp_add, x, y};
  test.Run();

  float x_f = 10.0;
  float y_f = 20.0;
  JobTwoParam<float> test2{&simp_add, x_f, y_f};
  test2.Run();
  // Null should silently Run and do nothing.
  JobTwoParam<int> test9{};
  test9.Run();
  JobTwoParam<float> test10{};
  test10.Run();

  JobTwoParam<NumericVariant> test11{&simp_add, x, y};
  test11.Run();
  JobTwoParam<NumericVariant> test12{&simp_add, x_f, y_f};
  test12.Run();

  JobTwoParam<int> test3{&simp_add_ref, x, y};
  test3.Run();
  JobTwoParam<int> test4{&simp_add_c, x, y};
  test4.Run();

  JobTwoParam<float> test5{&simp_add_ref, x_f, y_f};
  test5.Run();
  JobTwoParam<float> test6{&simp_add_c, x_f, y_f};
  test6.Run();
}

TEST(JobTwoParam, get_future) {
  // This is just a sanity check to see that the GetFuture method works.
  int x = 10;
  int y = 20;
  JobTwoParam<int> test{&simp_add, x, y};
  std::future<int> out = test.GetFuture();

  float x_f = 10.0;
  float y_f = 20.0;
  JobTwoParam<float> test2{&simp_add, x_f, y_f};
  std::future<float> out2 = test2.GetFuture();

  JobTwoParam<int> test9{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  JobTwoParam<float> test10{};
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);

  JobTwoParam<NumericVariant> test11{&simp_add, x, y};
  std::future<NumericVariant> out11 = test11.GetFuture();
  JobTwoParam<NumericVariant> test12{&simp_add, x_f, y_f};
  std::future<NumericVariant> out12 = test12.GetFuture();
  JobTwoParam<NumericVariant> test17{};
  EXPECT_THROW(test17.GetFuture(), std::runtime_error);
  JobTwoParam<NumericVariant> test18{};
  EXPECT_THROW(test18.GetFuture(), std::runtime_error);

  JobTwoParam<int> test3{&simp_add_ref, x, y};
  std::future<int> out3 = test3.GetFuture();
  JobTwoParam<float> test4{&simp_add_ref, x_f, y_f};
  std::future<float> out4 = test4.GetFuture();

  JobTwoParam<int> test5{&simp_add_c, x, y};
  std::future<int> out5 = test5.GetFuture();
  JobTwoParam<float> test6{&simp_add_c, x_f, y_f};
  std::future<float> out6 = test6.GetFuture();

  JobTwoParam<int> test7{&simp_add, x, y};
  std::future<int> out7;
  test7.GetFuture(out7);
  JobTwoParam<float> test8{&simp_add, x_f, y_f};
  std::future<float> out8;
  test8.GetFuture(out8);

  JobTwoParam<int> test13{};
  std::future<int> out13;
  EXPECT_THROW(test13.GetFuture(out13), std::runtime_error);
  JobTwoParam<float> test14{};
  std::future<float> out14_1;
  EXPECT_THROW(test14.GetFuture(out14_1), std::runtime_error);
}

TEST(JobTwoParam, run_and_get_future) {
  int x = 10;
  int y = 20;
  JobTwoParam<int> test{&simp_add, x, y};
  auto res = test.GetFuture();
  test.Run();
  EXPECT_EQ(res.get(), 30);

  float x_f = 10.0;
  float y_f = 20.0;
  JobTwoParam<float> test2{&simp_add, x_f, y_f};
  auto res2 = test2.GetFuture();
  test2.Run();
  EXPECT_EQ(res2.get(), 30.0f);

  JobTwoParam<NumericVariant> test11{&simp_add, x, y};
  std::future<NumericVariant> out11 = test11.GetFuture();
  test11.Run();
  EXPECT_EQ(std::get<int>(out11.get()), 30);

  JobTwoParam<NumericVariant> test9{&simp_add, x_f, y_f};
  std::future<NumericVariant> res9 = test9.GetFuture();
  test9.Run();
  EXPECT_EQ(std::get<float>(res9.get()), 30.0f);

  JobTwoParam<int> test3{&simp_add_ref, x, y};
  std::future<int> out3 = test3.GetFuture();
  test3.Run();
  EXPECT_EQ(out3.get(), 30);
  JobTwoParam<float> test4{&simp_add_ref, x_f, y_f};
  std::future<float> out4 = test4.GetFuture();
  test4.Run();
  EXPECT_EQ(out4.get(), 30.0f);

  JobTwoParam<int> test5{&simp_add_c, x, y};
  std::future<int> out5 = test5.GetFuture();
  test5.Run();
  EXPECT_EQ(out5.get(), 30);
  JobTwoParam<float> test6{&simp_add_c, x_f, y_f};
  std::future<float> out6 = test6.GetFuture();
  test6.Run();
  EXPECT_EQ(out6.get(), 30.0f);

  JobTwoParam<int> test7{&simp_add, x, y};
  std::future<int> res7;
  test7.GetFuture(res7);
  test7.Run();
  EXPECT_EQ(res7.get(), 30);

  JobTwoParam<float> test8{&simp_add, x_f, y_f};
  std::future<float> res8;
  test8.GetFuture(res8);
  test8.Run();
  EXPECT_EQ(res8.get(), 30.0f);

  JobTwoParam<NumericVariant> test10{&simp_add, x, y};
  std::future<NumericVariant> out10;
  test10.GetFuture(out10);
  test10.Run();
  EXPECT_EQ(std::get<int>(out10.get()), 30);

  JobTwoParam<NumericVariant> test12{&simp_add, x_f, y_f};
  std::future<NumericVariant> res12;
  test12.GetFuture(res12);
  test12.Run();
  EXPECT_EQ(std::get<float>(res12.get()), 30.0f);

  JobTwoParam<int> test13{&simp_add_ref, x, y};
  std::future<int> out13;
  test13.GetFuture(out13);
  test13.Run();
  EXPECT_EQ(out13.get(), 30);

  JobTwoParam<float> test14{&simp_add_ref, x_f, y_f};
  std::future<float> out14;
  test14.GetFuture(out14);
  test14.Run();
  EXPECT_EQ(out14.get(), 30.0f);

  JobTwoParam<int> test15{&simp_add_c, x, y};
  std::future<int> out15;
  test15.GetFuture(out15);
  test15.Run();
  EXPECT_EQ(out15.get(), 30);
  JobTwoParam<float> test16{&simp_add_c, x_f, y_f};
  std::future<float> out16;
  test16.GetFuture(out16);
  test16.Run();
  EXPECT_EQ(out16.get(), 30.0f);
}

TEST(JobTwoArray, initailization) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7.5, 8, 9.125};
  int d = 10;

  int cons = 2;
  float cons_f = 2.125;
  JobTwoArray<int> test11{&ele_add_arr, x, x, d, d};
  auto *res11 = test11.GetArg1();
  auto *res12 = test11.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
    EXPECT_EQ(res12[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  EXPECT_EQ(test11.GetLen2(), 10);
  JobTwoArray<float> test12{&ele_add_arr, y, y, d, d};
  auto *res13 = test12.GetArg1();
  auto *res14 = test12.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
    EXPECT_EQ(res14[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);
  EXPECT_EQ(test12.GetLen2(), 10);

	int x_1[]{0, 1, 2, 3, 4};
	float y_1[]{0,1,2,3,4,5,6};
	int a = 5;
	JobTwoArray<int> test1(&ele_add_arr, x, x_1, d, a);	
	EXPECT_EQ(test1.GetLen1(), 10);
	EXPECT_EQ(test1.GetLen2(), 5);
	JobTwoArray<float> test2(&ele_add_arr, y, y_1, d, a);	
	EXPECT_EQ(test2.GetLen1(), 10);
	EXPECT_EQ(test2.GetLen2(), 5);


  JobTwoArray<int> test9{};
  EXPECT_EQ(test9.GetLen1(), -1);
  EXPECT_EQ(test9.GetLen2(), -1);
  JobTwoArray<float> test10{};
  EXPECT_EQ(test10.GetLen1(), -1);
  EXPECT_EQ(test10.GetLen2(), -1);
}

TEST(JobTwoArray, move_assign) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8.5, 9.125};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobTwoArray<int> test11;
  auto temp = JobTwoArray<int>{&ele_add_arr, x, x, d, d};
  test11 = std::move(temp);
  auto *res11 = test11.GetArg1();
  auto *res12 = test11.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
    EXPECT_EQ(res12[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  EXPECT_EQ(test11.GetLen2(), 10);
  JobTwoArray<float> test12;
  auto temp2 = JobTwoArray<float>{&ele_add_arr, y, y, d, d};
  test12 = std::move(temp2);
  auto *res13 = test12.GetArg1();
  auto *res14 = test12.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
    EXPECT_EQ(res14[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);
  EXPECT_EQ(test12.GetLen2(), 10);

  JobTwoArray<int> test9;
  auto temp3 = JobTwoArray<int>{};
  test9 = std::move(temp3);
  EXPECT_EQ(test9.GetLen1(), -1);
  EXPECT_EQ(test9.GetLen2(), -1);
  JobTwoArray<float> test10;
  auto temp4 = JobTwoArray<float>{};
  test10 = std::move(temp4);
  EXPECT_EQ(test10.GetLen1(), -1);
  EXPECT_EQ(test10.GetLen2(), -1);
}

TEST(JobTwoArray, move_constr) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8.5, 9.125};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobTwoArray<int> test11 = std::move(JobTwoArray<int>{&ele_add_arr, x, x, d, d});
  auto *res11 = test11.GetArg1();
  auto *res12 = test11.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
    EXPECT_EQ(res12[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  EXPECT_EQ(test11.GetLen2(), 10);
  JobTwoArray<float> test12 = std::move(JobTwoArray<float>{&ele_add_arr, y, y, d, d});
  auto *res13 = test12.GetArg1();
  auto *res14 = test12.GetArg2();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
    EXPECT_EQ(res14[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);
  EXPECT_EQ(test12.GetLen2(), 10);

  JobTwoArray<int> test9 = JobTwoArray<int>{};
  EXPECT_EQ(test9.GetLen1(), -1);
  EXPECT_EQ(test9.GetLen2(), -1);
  JobTwoArray<float> test10 = JobTwoArray<float>{};
  EXPECT_EQ(test10.GetLen1(), -1);
  EXPECT_EQ(test10.GetLen2(), -1);
}

TEST(JobTwoArray, run) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobTwoArray<int> test13 = JobTwoArray<int>{&ele_add_arr, x, x, d, d};
  test13.Run();
  JobTwoArray<float> test14 = JobTwoArray<float>{&ele_add_arr, y, y, d, d};
  test14.Run();
  // Run does nothing. exists gracefully.
  JobTwoArray<int> test9 = JobTwoArray<int>{};
  test9.Run();
  JobTwoArray<float> test10 = JobTwoArray<float>{};
  test10.Run();
}

TEST(JobTwoArray, get_future) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobTwoArray<int> test13 = JobTwoArray<int>{&ele_add_arr, x, x, d, d};
  test13.GetFuture();
  JobTwoArray<int> test1 = JobTwoArray<int>{&ele_add_arr, x, x, d, d};
  std::future<int *> out1;
  test1.GetFuture(out1);
  JobTwoArray<float> test2 = JobTwoArray<float>{&ele_add_arr, y, y, d, d};
  test2.GetFuture();
  JobTwoArray<float> test3 = JobTwoArray<float>{&ele_add_arr, y, y, d, d};
  std::future<float *> out3;
  test3.GetFuture(out3);
  // Run does nothing. exists gracefully.
  JobTwoArray<int> test9 = JobTwoArray<int>{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  JobTwoArray<float> test10 = JobTwoArray<float>{};
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);
}

TEST(JobTwoArray, run_and_get_future) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobTwoArray<int> test1 = JobTwoArray<int>{&ele_add_arr, x, x, d, d};
  test1.Run();
  auto temp3 = test1.GetFuture().get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp3[i], x[i] * 2);
  }

  JobTwoArray<float> test2 = JobTwoArray<float>{&ele_add_arr, y, y, d, d};
  test2.Run();
  auto temp4 = test2.GetFuture().get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp4[i], y[i] * 2.0f);
  }
}

TEST(JobOneArray, initialization) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7.5, 8, 9.125};
  int d = 10;

  JobOneArray<int> test5{};
  EXPECT_EQ(test5.GetArg1(), nullptr);
  JobOneArray<float> test6{};
  EXPECT_EQ(test6.GetArg1(), nullptr);
  int cons = 2;
  float cons_f = 2.125;
  JobOneArray<int> test13{&simp_add_arr, x, d};
  auto *res15 = test13.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res15[i], x[i]);
  }
  EXPECT_EQ(test13.GetLen1(), 10);
  JobOneArray<float> test14{&simp_add_arr, y, d};
  auto *res16 = test14.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res16[i], y[i]);
  }
  EXPECT_EQ(test14.GetLen1(), 10);
}

TEST(JobOneArray, move_assign) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8.5, 9.125};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobOneArray<int> test11;
  auto temp = JobOneArray<int>{&simp_add_arr, x, d};
  test11 = std::move(temp);
  auto *res11 = test11.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  JobOneArray<float> test12;
  auto temp2 = JobOneArray<float>{&simp_add_arr, y, d};
  test12 = std::move(temp2);
  auto *res13 = test12.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);

  JobOneArray<int> test9;
  auto temp3 = JobOneArray<int>{};
  test9 = std::move(temp3);
  EXPECT_EQ(test9.GetLen1(), -1);
  JobOneArray<float> test10;
  auto temp4 = JobOneArray<float>{};
  test10 = std::move(temp4);
  EXPECT_EQ(test10.GetLen1(), -1);
}

TEST(JobOneArray, move_constr) {
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8.5, 9.125};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobOneArray<int> test11 = std::move(JobOneArray<int>{&simp_add_arr, x, d});
  auto *res11 = test11.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res11[i], x[i]);
  }
  EXPECT_EQ(test11.GetLen1(), 10);
  JobOneArray<float> test12 = std::move(JobOneArray<float>{&simp_add_arr, y, d});
  auto *res13 = test12.GetArg1();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(res13[i], y[i]);
  }
  EXPECT_EQ(test12.GetLen1(), 10);

  JobOneArray<int> test9 = std::move(JobOneArray<int>{});
  EXPECT_EQ(test9.GetLen1(), -1);
  JobOneArray<float> test10 = std::move(JobOneArray<float>{});
  EXPECT_EQ(test10.GetLen1(), -1);
}

TEST(JobOneArray, run) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobOneArray<int> test11 = std::move(JobOneArray<int>{&simp_add_arr, x, d});
  test11.Run();
  JobOneArray<float> test12 = std::move(JobOneArray<float>{&simp_add_arr, y, d});
  test12.Run();
  // Run does nothing. exists gracefully.
  JobOneArray<int> test9 = JobOneArray<int>{};
  test9.Run();
  JobOneArray<float> test10 = JobOneArray<float>{};
  test10.Run();
}


TEST(JobOneArray, get_future) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobOneArray<int> test11 = std::move(JobOneArray<int>{&simp_add_arr, x, d});
  test11.GetFuture();
  JobOneArray<int> test1 = std::move(JobOneArray<int>{&simp_add_arr, x, d});
  std::future<int *> out1;
  test1.GetFuture(out1);
  JobOneArray<float> test2 = std::move(JobOneArray<float>{&simp_add_arr, y, d});
  test2.GetFuture();
  JobOneArray<float> test3 = std::move(JobOneArray<float>{&simp_add_arr, y, d});
  std::future<float *> out3;
  test3.GetFuture(out3);
  // Run does nothing. exists gracefully.
  JobOneArray<int> test9 = JobOneArray<int>{};
  EXPECT_THROW(test9.GetFuture(), std::runtime_error);
  JobOneArray<float> test10 = JobOneArray<float>{};
  EXPECT_THROW(test10.GetFuture(), std::runtime_error);
}

TEST(JobOneArray, run_and_get_future) {
  // This is just a sanity check to see that the Run method works.
  int x[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  float y[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int d = 10;
  int cons = 2;
  float cons_f = 2.125;
  JobOneArray<int> test1 = std::move(JobOneArray<int>{&simp_add_arr, x, d});
  test1.Run();
  auto temp3 = test1.GetFuture().get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp3[i], x[i] + 2);
  }

  JobOneArray<float> test2 = std::move(JobOneArray<float>{&simp_add_arr, y, d});
  test2.Run();
  auto temp4 = test2.GetFuture().get();
  for (int i = 0; i < d; i++) {
    EXPECT_EQ(temp4[i], y[i] + 2.0f);
  }
}

