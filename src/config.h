/**
  @brief
   开发者模式条件编译开关，数值为对应参与者的学号。
   打开后，会在最外层命名空间添加一层对应参与者的命名空间，防止冲突；
   开发完成后关闭条件编译，即可合并到TinyTL命名空间。
  @example
  @code
   #if _IN_YUJIANLIN_DEV_MOD_ == 2018202296L
   namespace YujianLinDev
   {
   #endif
   namespace TinyTL {}
   #if _IN_YUJIANLIN_DEV_MOD_ == 2018202296L
   }
   #endif
  @endcode
 */
#ifndef _TTL226_CONFIG_H_
#define _TTL226_CONFIG_H_

#define _IN_YUJIANLIN_DEV_MOD_ 2018202296L

/* Add New Contents Here */
#endif // _TTL226_CONFIG_H_