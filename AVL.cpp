#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"
#include "hash_table.h"
// #include "hash_table.cpp"

/* AVL�������ϻ���BST��,ֻ�����ڲ����ɾ���ڵ�ʱ,��Ҫ����ƽ�����,��ȷ�����ĸ߶�ƽ��,�Ӷ���֤AVL���Ĳ�ѯЧ��
AVL���Ĳ����ɾ��������BST������,
 */

// �����½��ҽڵ�
Classroom *createClassroom(int id, const char *building, const char *name,
                           const char *type, int capacity, int is_scheduled)
{
    Classroom *newRoom = (Classroom *)malloc(sizeof(Classroom));
    newRoom->id = id;
    strncpy(newRoom->building, building, MAX_BUILDING_NAME - 1);
    strncpy(newRoom->name, name, MAX_CLASSROOM_NAME - 1);
    strncpy(newRoom->type, type, 19);
    newRoom->capacity = capacity;
    newRoom->is_scheduled = is_scheduled;
    newRoom->left = newRoom->right = NULL;
    newRoom->height = 1;
    return newRoom;
}

// ��BST�в������
Classroom *insertClassroom(Classroom *node, Classroom *newRoom)
{
    if (node == NULL)
    {
        return newRoom;
    }

    if (newRoom->id < node->id)
    {
        node->left = insertClassroom(node->left, newRoom);
    }
    else if (newRoom->id > node->id)
    {
        node->right = insertClassroom(node->right, newRoom);
    }

    return node;
}

// �ڹ�ϣ������ӽ��ҵ���ѧ¥
void addClassroomToBuilding(HashTable *ht, Classroom *room)
{
    int index = hash(room->building);
    BuildingNode *current = FindBuildingNode(ht, room->building);

    // ���ҽ�ѧ¥�ڵ�
    if (current != NULL)
    {
        // �ҵ���ѧ¥����ӽ���
        if (current->classroom_count >= current->capacity)
        {
            current->capacity *= 2;
            current->classrooms = (Classroom **)realloc(current->classrooms,
                                                        current->capacity * sizeof(Classroom *));
        }
        current->classrooms[current->classroom_count++] = room;
        return;
    }

    // �����µĽ�ѧ¥�ڵ�
    BuildingNode *newBuilding = (BuildingNode *)malloc(sizeof(BuildingNode));
    strncpy(newBuilding->name, room->building, MAX_BUILDING_NAME - 1);
    newBuilding->capacity = 10;
    newBuilding->classroom_count = 0;
    newBuilding->classrooms = (Classroom **)malloc(newBuilding->capacity * sizeof(Classroom *));
    newBuilding->classrooms[newBuilding->classroom_count++] = room;

    // ��ӵ���ϣ��
    KeyValuePair *newPair = (KeyValuePair *)malloc(sizeof(KeyValuePair));
    strncpy(newPair->building, room->building, MAX_BUILDING_NAME - 1);
    newPair->building_node = newBuilding;
    newPair->next = ht->table[index];
    ht->table[index] = newPair;
}

// ����ID���ҽ���
Classroom *findClassroomById(Classroom *node, int id)
{
    if (node == NULL || node->id == id)
    {
        return node;
    }

    if (id < node->id)
    {
        return findClassroomById(node->left, id);
    }
    return findClassroomById(node->right, id);
}

// �����ض���ѧ¥�����н���
void printClassroomsInBuilding(HashTable *ht, const char *building)
{
    int index = hash(building);
    BuildingNode *current = FindBuildingNode(ht, building);

    while (current != NULL)
    {
        if (strcmp(current->name, building) == 0)
        {
            printf("\n��ѧ¥ %s �Ľ����б�:\n", building);
            for (int i = 0; i < current->classroom_count; i++)
            {
                printf("ID: %d, ����: %s, ����: %s, ����: %d\n",
                       current->classrooms[i]->id,
                       current->classrooms[i]->name,
                       current->classrooms[i]->type,
                       current->classrooms[i]->capacity);
            }
            return;
        }
        current = current->next;
    }
    printf("δ�ҵ���ѧ¥: %s\n", building);
}

// ���˵�
void showMenu()
{
    printf("\n=== У԰������Դ����ϵͳ ===\n");
    printf("1. ��ӽ���\n");
    printf("2. ���ҽ���(��ID)\n");
    printf("3. �鿴��ѧ¥�����н���\n");
    printf("4. ����CSV����\n");
    printf("0. �˳�\n");
    printf("��ѡ�����: ");
}

// ��CSV�ļ���������
void importFromCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("�޷����ļ� %s\n", filename);
        return;
    }

    char line[256];
    // ����������
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        int id;
        char campus[50], classroom[50], type[50], scheduled[10], building[50];
        int capacity;

        // ����CSV��
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d,%[^\n]",
               &id, campus, classroom, type, scheduled,
               &capacity, building);

        // ������������ҽڵ�
        Classroom *newRoom = createClassroom(id, building, classroom,
                                             type, capacity,
                                             strcmp(scheduled, "��") == 0);
        root = insertAVL(root, newRoom);
        addClassroomToBuilding(buildingHash, newRoom);
    }

    fclose(file);
    printf("���ݵ�����ɣ�\n");
}

// ��ȡ�ڵ�߶�
int getHeight(Classroom *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

// ��ȡƽ������
int getBalanceFactor(Classroom *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// ���½ڵ�߶�
void updateHeight(Classroom *node)
{
    if (node == NULL)
        return;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// ����ת
Classroom *rightRotate(Classroom *y)
{
    Classroom *x = y->left;
    Classroom *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// ����ת
Classroom *leftRotate(Classroom *x)
{
    Classroom *y = x->right;
    Classroom *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// AVL�����뺯��
Classroom *insertAVL(Classroom *node, Classroom *newRoom)
{
    // ִ�б�׼BST����
    if (node == NULL)
    {
        newRoom->height = 1;
        return newRoom;
    }

    if (newRoom->id < node->id)
        node->left = insertAVL(node->left, newRoom);
    else if (newRoom->id > node->id)
        node->right = insertAVL(node->right, newRoom);
    else
        return node; // �ظ�ID������

    // ���½ڵ�߶�
    updateHeight(node);

    // ��ȡƽ������
    int balance = getBalanceFactor(node);

    // �������
    if (balance > 1 && newRoom->id < node->left->id)
        return rightRotate(node);

    // �������
    if (balance < -1 && newRoom->id > node->right->id)
        return leftRotate(node);

    // �������
    if (balance > 1 && newRoom->id > node->left->id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // �������
    if (balance < -1 && newRoom->id < node->right->id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// ����AVL���е���Сֵ�ڵ�
Classroom *findMin(Classroom *node)
{
    if (node == NULL)
        return NULL;
    while (node->left != NULL)
        node = node->left;
    return node;
}

// AVL��ɾ������
Classroom *deleteAVL(Classroom *root, int id)
{
    if (root == NULL)
        return root;

    // ִ�б�׼BSTɾ��
    if (id < root->id)
        root->left = deleteAVL(root->left, id);
    else if (id > root->id)
        root->right = deleteAVL(root->right, id);
    else
    {
        // �ڵ��ҵ���ִ��ɾ��
        if (root->left == NULL || root->right == NULL)
        {
            Classroom *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }
            free(temp);
        }
        else
        {
            // �������ӽڵ�����
            Classroom *temp = findMin(root->right);
            root->id = temp->id;
            strcpy(root->building, temp->building);
            strcpy(root->name, temp->name);
            strcpy(root->type, temp->type);
            root->capacity = temp->capacity;
            root->is_scheduled = temp->is_scheduled;
            root->right = deleteAVL(root->right, temp->id);
        }
    }

    if (root == NULL)
        return root;

    // ���¸߶�
    updateHeight(root);

    // ���ƽ�����Ӳ�������ת
    int balance = getBalanceFactor(root);

    // �������
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // �������
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // �������
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // �������
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void destroyAVL(Classroom *node)
{
    if (node == NULL)
        return;
    destroyAVL(node->left);
    destroyAVL(node->right);
    free(node);
}
