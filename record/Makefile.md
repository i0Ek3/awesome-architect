# Makefile

Makefile 可以理解为是一个自动化编译的脚本文件，其规定了那些文件需要编译，按照什么样的规则来编译，你只需要输入一个 `make` 命令即可。

## 规则

```Makefile
target ... : prerequisites ...
    command
    ...
    ...
```

其中，target 为一个目标文件，prerequisites 为依赖文件，command 为生成规则。

## 示例

```Makefile
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o

edit : $(objects)
    cc -o edit $(objects)

$(objects) : defs.h
kbd.o command.o files.o : command.h
display.o insert.o search.o files.o : buffer.h

.PHONY : clean
clean :
    rm edit $(objects)
```

未完待续...


## Ref

- [http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/](http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/)
- [https://seisman.github.io/how-to-write-makefile/introduction.html](https://seisman.github.io/how-to-write-makefile/introduction.html)
- [https://www.gnu.org/software/make/manual/html_node/index.html#SEC_Contents](https://www.gnu.org/software/make/manual/html_node/index.html#SEC_Contents)
