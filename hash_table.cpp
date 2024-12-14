#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// ��ϣ����,����ѧ¥����ת��Ϊ��ϣ���е�����
int hash(const char *str)
{
    // ����ѧ¥����ת��Ϊ��ϣ���е�����,�����кܶ��ѧ¥�Ŀ�ͷ����ͬ��,������Ҫ���д���
    // ͨ����ѯ����,�Ҳ��õ���BKDRHash�㷨,����ѧ¥����ת��Ϊ��ϣ���е�����,seed����Ϊ131,�����ķֲ�����Ӿ���
    // ��ʵ���Ǽ򵥳˷�ɢ�з�,���ַ����е�ÿ���ַ�ת��Ϊ����,Ȼ�����seed,���ȡģ
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return hash % TABLE_SIZE;

    return (int)(str[0] - 'A') % TABLE_SIZE;
}

// �������Щ��������϶�����ͨ�����͸���ճ��Ȼ����õ�

// ��ʼ����ϣ��
HashTable *createHashTable()
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (ht == NULL)
    {
        printf("�ڴ����ʧ��\n");
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        ht->table[i] = NULL;
    }
    return ht;
}

// �����ֵ�Ե���ϣ��
void insert(HashTable *ht, char *key, BuildingNode *building_node)
{
    int index = hash(key);
    KeyValuePair *kvp = (KeyValuePair *)malloc(sizeof(KeyValuePair));
    strncpy(kvp->building, key, MAX_BUILDING_NAME - 1);
    kvp->building_node = building_node;
    kvp->next = NULL;

    // �����λ��Ϊ�գ���ֱ�ӽ���ֵ�Բ���
    if (ht->table[index] == NULL)
    {
        ht->table[index] = kvp;
    }
    else
    { // ���򣬽���ֵ����ӵ�����ĩβ
        KeyValuePair *curr = ht->table[index];
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = kvp;
    }
}

// ���Ҽ���Ӧ��ֵ
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
    return NULL; // û���ҵ�
}

// ���ٹ�ϣ��
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
