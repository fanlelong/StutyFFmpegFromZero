###  Cmake入门

```cmake


cmake_minimum_required(VERSION 3.18.1) # 最低支持版本并不是最终版本最终版本在app模块下的build.gradle下指定
#         externalNativeBuild {
#             cmake {
#                 abiFilters 'armeabi-v7a'
#                version '3.18.2'
#            }
#        }


include_directories(${CMAKE_SOURCE_DIR}/include) # todo 第一步 导入头文件

project("ffmpegdemo") # 当前工程名，旧版本没有，这里也可以删掉

file(GLOB allCPP *.c *.h *.cpp)  #批量导入源文件

add_library(  #添加一个库
        ffmpegdemo # 生成库的名字
        SHARED
        ${allCPP} # 将源文件打包到库里面
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}") # 相当于设置win平台环境变量

# 查找ndk的一个动态库
# 如何判断哪些包可以写， 为什么log可以查到，可以从下面中找到
#C:\Users\admin\AppData\Local\Android\Sdk\ndk\21.1.6352462\build\cmake\system_libs.cmake
## 里面代码是： set(NDK_SYSTEM_LIBS "libEGL.so;libGLESv1_CM.so;libGLESv2.so;libGLESv3.so;libOpenMAXAL.so;libOpenSLES.so;libaaudio.so;libamidi.so;libandroid.so;libbinder_ndk.so;libc.so;libcamera2ndk.so;libdl.so;libjnigraphics.so;liblog.so;libm.so;libmediandk.so;libnativewindow.so;libneuralnetworks.so;libstdc++.so;libsync.so;libvulkan.so;libz.so")
# log.so路径： C:\Users\admin\AppData\Local\Android\Sdk\ndk\21.1.6352462\toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\lib\i686-linux-android\16

# 为什么是16，找到是项目要求的最低版本
find_library( # 好处是定义一个变量
        log-lib
        log)



# 将链接库打包到总库里面
target_link_libraries(
        ffmpegdemo # 总库存
        avformat avcodec avfilter avutil swresample swscale ## 这里有顺序问题，一定要按这个顺序
        ${log-lib} #链接的库
        )

#忽略顺序 加下面码
#-Wl, --start-group
#avformat avcodec avfilter avutil swresample swscale
#-Wl,--end-group
```

#### 打印日志

``` cmake
message("111sdfsldkfsldkfjlskdflsdlfds")
# 在cmake所有的变量都是string类型 可以用set()和unset()来声明或移除一个变量
set(var 666)
message("var = ${var}")

unset(var)
message("un_var = ${var}")

set(list_var 1 2 3 4 5)
set(list_var2 "1;2;3;4;5")
message("list_var=${list_var}")
message("list_var2=${list_var2}")

# true 1 YES ON Y  非0的值     false（0 OFF  NO FALSE N IGNORE NOTFOUND）
set(`if_tap`  OFF)
set(elseif_tap ON)

if(${if_tap})
    message("if")
elseif(${elseif_tap})
    message("elseif")
else(${if_tap})
    message("else")
endif(${if_tap}) # 可以不加
```



#### 循环

```cmake

set(a "")
while (NOT a STREQUAL "xxx") # 这句话的意思是 a等不等于xxx      (a STREQUAL "xxx" 判断a是否等于xxx) 加个NOT 就是不等于
    set(a "${a}x") # 如果a不等于xxx则在a后面拼接一个x
    message(">>>>>> a = ${a}")
endwhile ()
```

```cmake
break() # 可以跳出循环
continue() # 可以跳过这一次循环
```

```cmake

foreach (item 1 2 3)  # 结果打印的是 0 1 2 3
    message("item = ${item}")
endforeach (item) # 结束循环

foreach (item RANGE 6) # 结果打印的是 0 1 2 3 4 5 6
    message("item2 = ${item}")
endforeach ()

foreach (item RANGE 1 6 2) # 结果得到的是 1 3 5  跳级的效果
    message("item3 = ${item}")
endforeach ()

set(list_val 1 2 3) # 结果打印的是 1 2 3
foreach (item IN LISTS list_val)
    message("item4 = ${item}")
endforeach ()

set(list_val2 1;2;3) # 结果打印的是 1 2 3
foreach (item IN LISTS list_val2)
    message("item5 = ${item}")
endforeach ()
```







####  函数

```cmake
#[[
ARGC： 表示传入参数的个数
ARGV0: 表示第一个参数； ARGV1 第二个参数；ARGV2：第三个参数；
ARGV： 表示所有参数
]]
function(method1 n1 n2 n3)
    message("call num method method")
    message("n1=${n1}") # n1=1
    message("n2=${n2}") # n2=5
    message("n3=${n3}") # n3=7
    message("ARGC=${ARGC}") # ARGC=3
    message("arg1=${ARGV0} arg2=${ARGV1} arg3=${ARGV2}") # arg1=1 arg2=5 arg3=7
    message("all args=${ARGV}") # all args=1;5;7
endfunction(method1)
method1(1 5 7)
```

 

#### 动态库和静态库相互转换

```cmake
set(IS_STATIC OFF) # 定义一个变量

if (${IS_STATIC})
    add_library(getndk STATIC IMPORTED)
    set_target_properties(getndk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libgetndk.a)
    message("IS_STATIC = static")
else(${IS_STATIC})
#    add_library(getndk SHARED IMPORTED)
#    set_target_properties(getndk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jinLibs/${CMAKE_ANDROID_ARCH_ABI}/libgetndk.so)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")
    message("IS_STATIC = share")
endif(${IS_STATIC})
```



#### rmtp使用的方式

* 在cpp目录下创建一个lIbget文件,命名随意

* 创建CmakeLists.txt文件

  ```cmake
  cmake_minimum_required(VERSION 3.10.2)
  
  file(GLOB SOURCE (*.c))
  
  add_library(
          get
          STATIC
          ${SOURCE}
  )
  ```

* 创建getutil.h和三个.c文件 get.c get2.c get3.c

  getutil.h

  ```c
  //
  // Created by admin on 2022/7/11.
  //
  
  #ifndef JIAOCHABIANYI_GETUTIL_H
  #define JIAOCHABIANYI_GETUTIL_H
  
  #include <stdio.h>
  const char * get_action();
  const char * get2_action();
  const char * get3_action();
  
  #endif //JIAOCHABIANYI_GETUTIL_H
  
  ```

  get.c

  ```c
  #include "getutil.h"
  
  const char *get_action() {
      return "get3_action";
  }
  ```
  
  get3.c
  
  ```c
  #include "getutil.h"
  
  const char *get2_action() {
      return "get3_action";
  }
  ```
  
  get3.c
  
  ```c
  
  #include "getutil.h"
  
  const char *get3_action() {
      return "get3_action";
  }
  ```
  
* 在项目配置的CMake文件下添加下面代码

  ```cmake
  # 导入子目录的
  add_subdirectory(${CMAKE_SOURCE_DIR}/libget)
  
  # 把get链接到主库
  target_link_libraries(
          native-lib
          ${log-lib}
          get
          getndk)
  ```

  

