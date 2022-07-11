cmake_minimum_required(VERSION 3.10.2)

file(GLOB allCPP *.c *.h *.cpp)
#file(GLOB allCPP ${CMAKE_SOURCE_DIR}/cpp/*.cpp ${CMAKE_SOURCE_DIR}/cpp/*.c ${CMAKE_SOURCE_DIR}/cpp/) # 如果CMake文件路径有变动，这里就得写路径出来

project("jiaochabianyi")

add_library( # Sets the name of the library.
        native-lib
        SHARED
        ${allCPP})

find_library(
        log-lib
        log)

# todo rtmp的时候 就全盘源码构建方式 定义libget文件夹

set(IS_STATIC OFF)


if (${IS_STATIC})
    #导入静态库
    add_library(getndk STATIC IMPORTED)
    set_target_properties(getndk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libgetndk.a)
    message("IS_STATIC = static")
else(${IS_STATIC})
#    add_library(getndk SHARED IMPORTED)
#    set_target_properties(getndk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jinLibs/${CMAKE_ANDROID_ARCH_ABI}/libgetndk.so)
    #导入动态库 用这种方法 上面注释的方法行不通不知道什么原因
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")  # 相当于设置win平台环境变量
    message("IS_STATIC = share")
endif(${IS_STATIC})

# 导入子目录的
add_subdirectory(${CMAKE_SOURCE_DIR}/libget)

target_link_libraries(
        #编译阶段，链接到总库，
        # 静态库 .a相当于编译链接阶段把静态库里的代码Copy到总库ffmpegdemo里面，所以在我们打包成apk的时候是看不到静态库的。只有一个总库ffmpegdemo。
        #动态库 .so则不一 样，把对应方法的地址记录过来。 在运行的时候通过记录的地址寻找方法，所以动态库是不能删。也叫共享库
        native-lib
        ${log-lib}
        get
        getndk)


# ------------------------------------------------------todo 下面是打印日志和一些 CMake语法------------------------------------------------------

#[[
(无) = 重要传为笑谈上
STATUS = 非重要信息
WARNING 警告 会继续执行
AUTHOR_WARNING  警告，会继续执行
SEND_ERROR 错误，继续执行，但是会跳过生成步骤
FATAL_ERROR 错误，终止所有处理过程
]]
message("111sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "222sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "333sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "444sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "555sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "666sdfsldkfsldkfjlskdflsdlfds")
message("777sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "888sdfsldkfsldkfjlskdflsdlfds")
message(STATUS "999sdfsldkfsldkfjlskdflsdlfds")
message(${CMAKE_SOURCE_DIR}) # F:/studio/FFmpegDemo/app/src/main/cpp
message(${CMAKE_ANDROID_ARCH_ABI}) # armeabi-v7a
message(${CMAKE_CXX_FLAGS}) # -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -march=armv7-a -mthumb -Wformat -Werror=format-security   -LF:/studio/FFmpegDemo/app/src/main/cpp/armeabi-v7a
message(${CMAKE_C_FLAGS}) # C/C++: debug|armeabi-v7a :-g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -march=armv7-a -mthumb -Wformat -Werror=format-security

set(var 666)
message("var = ${var}")
# 在cmake所有的变量都是string类型 可以用set()和unset()来声明或移除一个变量
# 移除
unset(var)
message("un_var = ${var}")

set(list_var 1 2 3 4 5)
set(list_var2 "1;2;3;4;5")
message("list_var=${list_var}")  # 打印结果 ：list_var=1;2;3;4;5
message("list_var2=${list_var2}") # 打印结果： list_var2=1;2;3;4;5

# true 1 YES ON Y  非0的值     false（0 OFF  NO FALSE N IGNORE NOTFOUND）
set(`if_tap` OFF)
set(elseif_tap ON)

if (${if_tap})
    message("if")
elseif (${elseif_tap})
    message("elseif")
else (${if_tap})
    message("else")
endif (${if_tap}) # 可以不加

# 循环
set(a "") # 定义一个变量a，变设置默认值  “”
while (NOT a STREQUAL "xxx") # 这句话的意思是 a等不等于xxx      (a STREQUAL "xxx" 判断a是否等于xxx) 加个NOT 就是不等于
    set(a "${a}x") # 如果a不等于xxx则在a后面拼接一个x
    message(">>>>>> a = ${a}")
endwhile ()

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

# todo CMake自定义函数
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