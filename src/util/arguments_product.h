#ifndef __SR_UTIL_ARGUMENTS_PRODUCT_H__
#define __SR_UTIL_ARGUMENTS_PRODUCT_H__

//
#define __BATCH_FUNCTION_2_MATRIX_2_1(fun, name1, name2, name3) \
  fun(name1, name3); fun(name2, name3);


//
#define __BATCH_FUNCTION_2_MATRIX_2_2(fun, name1, name2, name3, name4) \
  fun(name1, name3); fun(name1, name4); \
  fun(name2, name3); fun(name2, name4);


//
#define __BATCH_FUNCTION_2_MATRIX_2_3(fun, name1, name2, name3, name4, name5) \
  fun(name1, name3); fun(name1, name4); fun(name1, name5); \
  fun(name2, name3); fun(name2, name4); fun(name2, name5);


//
#define __BATCH_FUNCTION_2_MATRIX_3_3(fun, name1, name2, name3, name4, name5, name6) \
  fun(name1, name4); fun(name1, name5); fun(name1, name6); \
  fun(name2, name4); fun(name2, name5); fun(name2, name6); \
  fun(name3, name4); fun(name3, name5); fun(name3, name6);


//
#define __BATCH_FUNCTION_3_MATRIX_2_2_1(fun, name1, name2, name3, name4, name5) \
  fun(name1, name3, name5); fun(name1, name4, name5); \
  fun(name2, name3, name5); fun(name2, name4, name5);


//
#define __BATCH_FUNCTION_3_CIRCLE_4(fun, name1, name2, name3, name4) \
  fun(name1, name2, name3); fun(name1, name2, name4); \
  fun(name1, name3, name4); fun(name2, name3, name4);



#endif  //  end for __SR_UTIL_ARGUMENTS_PRODUCT_H__
