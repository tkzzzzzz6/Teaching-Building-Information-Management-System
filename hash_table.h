#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "AVL.h"

// 通过python代码统计分析教学楼数量为28个,考虑到后续新的教学楼可能会增加,所以这里我将哈希表大小设置为50
#define TABLE_SIZE 50 // 哈希表大小
#define HASH_SIZE 50          // 哈希表大小

// 哈希表中存储的键值对结构体,键为教学楼名称,值为教室列表
// 教学楼节点结构（用于哈希表）
typedef struct BuildingNode{
    char name[MAX_BUILDING_NAME]; // 教学楼名称
    Classroom **classrooms;       // 该教学楼的教室列表
    int classroom_count;          // 教室数量
    int capacity;                 // 列表容量
    struct BuildingNode *next;    // 处理哈希冲突时使用
} BuildingNode;

typedef struct KeyValuePair{
    char building[MAX_BUILDING_NAME];
    BuildingNode *building_node;
    struct KeyValuePair *next; // 指向下一个键值对的指针
} KeyValuePair;

// 哈希表结构体
typedef struct HashTable{
    KeyValuePair *table[TABLE_SIZE];
} HashTable;

// 哈希表相关函数声明
int hash(const char *str);
HashTable *createHashTable();
void insert(HashTable *ht,const char *key, BuildingNode *building_node);
BuildingNode *FindBuildingNode(HashTable *ht, const char *key);
void destroyHashTable(HashTable *ht);
void addClassroomToBuilding(HashTable* ht, Classroom* room);
void printClassroomsInBuilding(HashTable* ht, const char* building);

#endif