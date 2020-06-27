# CMake

CMake 是一种跨平台的自动化构建的编译工具，比 make 更为高级，使用起来要方便得多。

CMake 主要是编写 CMakeLists.txt 文件的，然后用 cmake 命令将 CMakeLists.txt 文件转化为 make 所需要的 makefile 文件，最后用 make 命令编译源码生成可执行程序或共享库。

## 基本步骤

- `cmake`
- `make`

其中，`cmake` 指向 CMakeLists.txt 所在的目录。`make` 则根据生成的 makefile 文件编译程序。

## CMakeLists.txt 编写

该文件必须与主文件在同一个目录下。

### 单个源文件

```C
# cmake 最低版本 x.x
cmake_minimum_required (VERSION x.x)

# 项目信息
project (projectname)

# 指定生成目标
add_executable (target x.cpp)
```

之后便可以在当前目录运行 `cmake .`，然后在 `make` 一下，便会得到名为 `target` 的可执行目标文件。

上面是针对单文件的，对于可以手数得清的多个文件而言，我们只需要在最后一行 add_executable 中加上需要编译的源文件即可。但对于超级多的源文件，直接使用 aux_source_directory 是更好的选择。

### 多个源文件

```C
# 多个源文件，单个文件夹
cmake_minimum_required (VERSION x.x)
project (projectname)
aux_source_directory(/target_folder DIR_SRCS) # DIR_SRCS 可以更换为其他 arg
add_executable (target ${DIR_SRCS})

# 多个源文件，多个文件夹
cmake_minimum_required (VERSION x.x)
project (projectname)
aux_source_directory(/target_folder DIR_SRCS)
add_subdirectory(subfolder_name) # 添加子目录
add_executable (target x.cpp)
target_link_libraries(target link_lib_name) # 添加链接库
```

### 自定义编译选项

具体请参考[这里](https://www.hahack.com/codes/cmake/)。

## Ref

- [https://www.ibm.com/developerworks/cn/linux/l-cn-cmake/](https://www.ibm.com/developerworks/cn/linux/l-cn-cmake/)
- [https://juejin.im/post/5a6f32e86fb9a01ca6031230](https://juejin.im/post/5a6f32e86fb9a01ca6031230)
- [https://www.hahack.com/codes/cmake/](https://www.hahack.com/codes/cmake/)

