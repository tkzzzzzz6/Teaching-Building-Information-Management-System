# 校园教室资源管理系统

这是一个基于C语言实现的校园教室资源管理系统,使用AVL树和哈希表作为核心数据结构,实现了教室信息的高效管理和查询。

## 项目结构

```
.
├── main.cpp                # 主程序入口
├── AVL.h                  # AVL树相关声明
├── AVL.cpp                # AVL树实现
├── hash_table.h           # 哈希表相关声明
├── hash_table.cpp         # 哈希表实现
├── data/                  # 数据文件目录
│   └── 2024年12月10日上午（第1讲）有课的教室.csv  # 示例数据
└── README.md              # 项目说明文档
```

## 核心数据结构

### 1. AVL树
- 用于存储和管理教室信息
- 每个节点包含教室的完整信息(ID、名称、类型、容量等)
- 通过教室ID作为键值进行平衡树的构建
- 保证了O(log n)的查询效率

```c
typedef struct Classroom {
    int id;                           // 教室ID
    char building[MAX_BUILDING_NAME]; // 教学楼
    char name[MAX_CLASSROOM_NAME];    // 教室名称
    char type[20];                    // 教室类型
    int capacity;                     // 容量
    int is_scheduled;                 // 是否排课
    int height;                       // AVL树节点高度
    struct Classroom *left;           // 左子节点
    struct Classroom *right;          // 右子节点
} Classroom;
```

### 2. 哈希表
- 用于快速查找特定教学楼的所有教室
- 使用教学楼名称作为键
- 采用链地址法处理哈希冲突
- 提供O(1)的平均查找时间复杂度

```c
typedef struct HashTable {
    KeyValuePair *table[TABLE_SIZE];  // 哈希表数组
} HashTable;
```

## 主要算法

### 1. AVL树平衡算法
- 左旋转(LL旋转)
- 右旋转(RR旋转)
- 左右旋转(LR旋转)
- 右左旋转(RL旋转)

### 2. 哈希算法
- 使用BKDR哈希函数(seed=131)
- 通过链地址法解决冲突

## 核心功能

1. 教室管理
   - 添加新教室
   - 按ID查找教室
   - 删除教室信息

2. 教学楼管理
   - 查看特定教学楼的所有教室
   - 教室信息的批量导入

3. 数据导入
   - 支持从CSV文件导入教室数据

## 时间复杂度分析

1. 查找操作
   - 按ID查找教室: O(log n)
   - 查找特定教学楼的教室: O(1)

2. 插入操作
   - 向AVL树插入新节点: O(log n)
   - 向哈希表插入新记录: O(1)

3. 删除操作
   - 从AVL树删除节点: O(log n)

## 编译和运行

### 编译
```bash
g++ main.cpp AVL.cpp hash_table.cpp -o classroom_management
```

### 运行
```bash
classroom_management.exe
```

## 使用说明

1. 主菜单选项:
   - 1: 添加教室
   - 2: 查找教室(按ID)
   - 3: 查看教学楼的所有教室
   - 4: 导入CSV数据
   - 0: 退出

2. 添加教室时需要输入:
   - 教室ID
   - 教学楼名称
   - 教室名称
   - 教室类型
   - 容量
   - 是否排课(1是/0否)

3. CSV数据导入格式:
   ```csv
   id,校区,教室,类型,是否排课,容量,教学楼
   ```

## 注意事项

1. 教室ID必须唯一
2. 导入CSV文件时请确保格式正确
3. 程序退出时会自动清理内存

## 未来改进方向

1. 添加更多查询条件(如按容量查询)
2. 实现数据持久化存储
3. 添加用户权限管理
4. 优化内存使用
5. 添加图形用户界面

## 作者

[tkzzzzzz6]

## 许可证

This project is licensed under the MIT License - see the LICENSE file for details