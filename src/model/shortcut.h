#ifndef __SR_MODEL_SHORTCUT_H__
#define __SR_MODEL_SHORTCUT_H__

#include "model/appendix.h"
#include "util/arguments_product.h"

#define __STATE_U(name)                 us_map_t name;
#define __STATE_B(name1, name2)         bs_map_t name1##name2;
#define __STATE_T(name1, name2, name3)  ts_map_t name1##name2##name3;

// (name1, name2) * (name3, name4)
#define __STATE_B_MATRIX_2_2(name1, name2, name3, name4) \
  __BATCH_FUNCTION_2_MATRIX_2_2(__STATE_B, name1, name2, name3, name4);


// (name1, name2) * (name3, name4, name5)
#define __STATE_B_MATRIX_2_3(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_2_MATRIX_2_3(__STATE_B, name1, name2, name3, name4, name5)


// (name1, name2, name3) * (name4, name5, name6)
#define __STATE_B_MATRIX_3_3(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_2_MATRIX_3_3(__STATE_B, name1, name2, name3, name4, name5, name6)


// (name1, name2) * (name3, name4) * name5
#define __STATE_T_MATRIX_2_2_1(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_3_MATRIX_2_2_1(__STATE_T, name1, name2, name3, name4, name5);


// (name1, name2) * (name3, name4) * (name5, name6)
#define __STATE_T_MATRIX_2_2_2(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_3_MATRIX_2_2_2(__STATE_T, name1, name2, name3, name4, name5, name6);


#define __STATE_T_CIRCLE_4(name1, name2, name3, name4) \
  __BATCH_FUNCTION_3_CIRCLE_4(__STATE_T, name1, name2, name3, name4); 


#define __STATE_U_EXT(name)         __APPENDIX_U(__STATE_U, name)
#define __STATE_B_EXT(name1, name2) __APPENDIX_B(__STATE_B, name1, name2)



#endif  //  end for __SR_MODEL_SHORTCUT_H__
