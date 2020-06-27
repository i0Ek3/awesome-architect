# Protobuf 解析

> Protobuf 即是 Google Protocol Buffer 的简称，是 Google 公司内部的混合语言数据标准，主要用于 RPC 系统和持续数据存储系统。

Protocol Buffers 是一种轻便高效的结构化数据存储格式，可以用于结构化数据串行化，或者说序列化。它很适合做数据存储或 RPC 数据交换格式。可用于通讯协议、数据存储等领域的语言无关、平台无关、可扩展的序列化结构数据格式。

## 优缺点

- 优点
    - 简单
    - 快速的反序列化
    - 体积更小
    - 向后兼容
    - 语义更清晰
- 缺点
    - 功能简单，无法表示复杂的概念
    - 通用性不佳
    - 不适合用来对基于文本的标记文档建模

## message

在 proto 中，所有结构化的数据都被称为 message。

- 分配字段编号
- 保留字段
- 默认字段规则
- 枚举
- 枚举中的保留值
- 嵌套
- 枚举不兼容性
- 更新 message
- 未知字段：未知数字段是 protocol buffers 序列化的数据，表示解析器无法识别的字段
- Map 类型
- JSON Mapping

## Service

## 命名规范

类似 C++ 中的代码命名风格。

## 编码原理

- Message Structure 编码
- Signed Integers 编码
- Non-varint Numbers
- 字符串
- 嵌入式 message
- Optional 和 Repeated 的编码
- Packed Repeated Fields
- Field Order


## Ref

- [https://halfrost.com/protobuf_encode/](https://halfrost.com/protobuf_encode/)
- [https://www.ibm.com/developerworks/cn/linux/l-cn-gpb/](https://www.ibm.com/developerworks/cn/linux/l-cn-gpb/)

