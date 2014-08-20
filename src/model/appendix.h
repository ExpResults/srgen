#ifndef __SR_MODEL_EXT_H__
#define __SR_MODEL_EXT_H__

#define __APPENDIX_U(fun, name) \
  fun(name##w);     fun(name##p); \
  fun(name##ldw);   fun(name##ldp);   fun(name##ldl);   \
  fun(name##l2dw);  fun(name##l2dp);  fun(name##l2dl);  \
  fun(name##lddw);  fun(name##lddp);  fun(name##lddl);  \
  fun(name##rdw);   fun(name##rdp);   fun(name##rdl);   \
  fun(name##r2dw);  fun(name##r2dp);  fun(name##r2dl);  \
  fun(name##rddw);  fun(name##rddp);  fun(name##rddl);  

#define __APPENDIX_B(fun, name1, name2) \
  fun(name1##w, name2##w); \
  fun(name1##p, name2##p); \
  fun(name1##l, name2##l);


#endif  //  end for __SR_MODEL_EXT_H__
