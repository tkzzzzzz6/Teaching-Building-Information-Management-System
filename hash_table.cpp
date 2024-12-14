#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// 哈希函数,将教学楼名称转换为哈希表中的索引
int hash(const char *str)
{
    // 将教学楼名称转换为哈希表中的索引,但是有很多教学楼的开头是相同的,所以需要进行处理
    // 通过查询资料,我采用的是BKDRHash算法,将教学楼名称转换为哈希表中的索引,seed设置为131,处理后的分布会更加均匀
    // 其实就是简单乘法散列法,将字符串中的每个字符转换为数字,然后乘以seed,最后取模
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return hash % TABLE_SIZE;

    return (int)(str[0] - 'A') % TABLE_SIZE;
}

// 后面的这些代码基本上都是我通过博客复制粘贴然后改用的

// 初始化哈希表
HashTable *createHashTable()
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (ht == NULL)
    {
        printf("内存分配失败\n");
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        ht->table[i] = NULL;
    }
    return ht;
}

// 插入键值对到哈希表
void insert(HashTable *ht, char *key, BuildingNode *building_node)
{
    int index = hash(key);
    KeyValuePair *kvp = (KeyValuePair *)malloc(sizeof(KeyValuePair));
    strncpy(kvp->building, key, MAX_BUILDING_NAME - 1);
    kvp->building_node = building_node;
    kvp->next = NULL;

    // 如果该位置为空，则直接将键值对插入
    if (ht->table[index] == NULL)
    {
        ht->table[index] = kvp;
    }
    else
    { // 否则，将键值对添加到链表末尾
        KeyValuePair *curr = ht->table[index];
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = kvp;
    }
}

// 查找键对应的值
BuildingNode *FindBuildingNode(HashTable *ht, const char *key)
{
    int index = hash(key);
    KeyValuePair *curr = ht->table[index];
    while (curr != NULL)
    {
        if (strcmp(curr->building, key) == 0)
        {
            return curr->building_node;
        }
        curr = curr->next;
    }
    return NULL; // 没有找到
}

// 销毁哈希表
void destroyHashTable(HashTable *ht)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        KeyValuePair *curr = ht->table[i];
        while (curr != NULL)
        {
            KeyValuePair *temp = curr;
            curr = curr->next;
            free(temp->building);
            free(temp);
        }
    }
    free(ht);
}
