#ifndef __SR_EXTRACE_SHORTCUT_H__
#define __SR_EXTRACE_SHORTCUT_H__

#include "model/appendix.h"
#include "util/arguments_product.h"

// Get Unigram Score (GUS)
#define __GUS(name) do { \
  if (ctx.name) { \
    scores[act] += get_score<us_map_t, us_t>(weight.name, \
        us_t(ctx.name, act), !learn, 0); \
  } \
} while (0);

#define __GBS(name1, name2) do { \
  if (ctx.name1 && ctx.name2) { \
    scores[act] += get_score<bs_map_t, bs_t>(weight.name1##name2, \
        bs_t(ctx.name1, ctx.name2, act), !learn, 0); \
  } \
} while (0);

#define __GTS(name1, name2, name3) do { \
  if (ctx.name1 && ctx.name2 && ctx.name3) { \
    scores[act] += get_score<ts_map_t, ts_t>(weight.name1##name2##name3, \
        ts_t(ctx.name1, ctx.name2, ctx.name3, act), !learn, 0); \
  } \
} while (0);

#define __UUS(name) do { \
  if (ctx.name) { \
    update_score<us_map_t, us_t>(weight.name, \
        us_t(ctx.name, act), now, scale); \
  } \
} while (0);

#define __UBS(name1, name2) do { \
  if (ctx.name1 && ctx.name2) { \
    update_score<bs_map_t, bs_t>(weight.name1##name2, \
        bs_t(ctx.name1, ctx.name2, act), now, scale); \
  } \
} while (0);

#define __UTS(name1, name2, name3) do { \
  if (ctx.name1 && ctx.name2 && ctx.name3) { \
    update_score<ts_map_t, ts_t>(weight.name1##name2##name3, \
        ts_t(ctx.name1, ctx.name2, ctx.name3, act), now, scale); \
  } \
} while (0);

#define __NC_GUS(name) do { \
  if (name) { \
    scores[act] += get_score<us_map_t, us_t>(weight.name, \
        us_t(name, act), true, 0); \
  } \
} while (0);

#define __NC_UUS(name) do { \
  if (name) { \
    update_score<us_map_t, us_t>(weight.name, \
        us_t(name, act), now, scale); \
  } \
} while (0);

#define __GUS_EXT(name) __APPENDIX_U(__GUS, name)

#define __GBS_EXT(name1, name2) __APPENDIX_B(__GBS, name1, name2)

#define __GBS_MATRIX_2_2(name1, name2, name3, name4) \
  __BATCH_FUNCTION_2_MATRIX_2_2(__GBS, name1, name2, name3, name4)

#define __GBS_MATRIX_2_3(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_2_MATRIX_2_3(__GBS, name1, name2, name3, name4, name5)

#define __GBS_MATRIX_3_3(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_2_MATRIX_3_3(__GBS, name1, name2, name3, name4, name5, name6)

#define __GTS_MATRIX_2_2_1(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_3_MATRIX_2_2_1(__GTS, name1, name2, name3, name4, name5)

#define __GTS_CIRCLE_4(name1, name2, name3, name4) \
  __BATCH_FUNCTION_3_CIRCLE_4(__GTS, name1, name2, name3, name4)

#define __UUS_EXT(name) __APPENDIX_U(__UUS, name)

#define __UBS_EXT(name1, name2) __APPENDIX_B(__UBS, name1, name2)

#define __UBS_MATRIX_2_2(name1, name2, name3, name4) \
  __BATCH_FUNCTION_2_MATRIX_2_2(__UBS, name1, name2, name3, name4)

#define __UBS_MATRIX_2_3(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_2_MATRIX_2_3(__UBS, name1, name2, name3, name4, name5)

#define __UBS_MATRIX_3_3(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_2_MATRIX_3_3(__UBS, name1, name2, name3, name4, name5, name6)

#define __UTS_MATRIX_2_2_1(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_3_MATRIX_2_2_1(__UTS, name1, name2, name3, name4, name5)

#define __UTS_CIRCLE_4(name1, name2, name3, name4) \
  __BATCH_FUNCTION_3_CIRCLE_4(__UTS, name1, name2, name3, name4)


#endif  //  end for __SR_EXTRACE_SHORTCUT_H__
