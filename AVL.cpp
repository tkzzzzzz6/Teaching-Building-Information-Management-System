#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"
#include "hash_table.h"
// #include "hash_table.cpp"

/* AVL树本质上还是BST树,只不过在插入和删除节点时,需要进行平衡操作,以确保树的高度平衡,从而保证AVL树的查询效率
AVL树的插入和删除操作与BST树类似,
 */

// 创建新教室节点
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

// 在BST中插入教室
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

// 在哈希表中添加教室到教学楼
void addClassroomToBuilding(HashTable *ht, Classroom *room)
{
    int index = hash(room->building);
    BuildingNode *current = FindBuildingNode(ht, room->building);

    // 查找教学楼节点
    if (current != NULL)
    {
        // 找到教学楼，添加教室
        if (current->classroom_count >= current->capacity)
        {
            current->capacity *= 2;
            current->classrooms = (Classroom **)realloc(current->classrooms,
                                                        current->capacity * sizeof(Classroom *));
        }
        current->classrooms[current->classroom_count++] = room;
        return;
    }

    // 创建新的教学楼节点
    BuildingNode *newBuilding = (BuildingNode *)malloc(sizeof(BuildingNode));
    strncpy(newBuilding->name, room->building, MAX_BUILDING_NAME - 1);
    newBuilding->capacity = 10;
    newBuilding->classroom_count = 0;
    newBuilding->classrooms = (Classroom **)malloc(newBuilding->capacity * sizeof(Classroom *));
    newBuilding->classrooms[newBuilding->classroom_count++] = room;

    // 添加到哈希表
    KeyValuePair *newPair = (KeyValuePair *)malloc(sizeof(KeyValuePair));
    strncpy(newPair->building, room->building, MAX_BUILDING_NAME - 1);
    newPair->building_node = newBuilding;
    newPair->next = ht->table[index];
    ht->table[index] = newPair;
}

// 根据ID查找教室
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

// 查找特定教学楼的所有教室
void printClassroomsInBuilding(HashTable *ht, const char *building)
{
    int index = hash(building);
    BuildingNode *current = FindBuildingNode(ht, building);

    while (current != NULL)
    {
        if (strcmp(current->name, building) == 0)
        {
            printf("\n教学楼 %s 的教室列表:\n", building);
            for (int i = 0; i < current->classroom_count; i++)
            {
                printf("ID: %d, 教室: %s, 类型: %s, 容量: %d\n",
                       current->classrooms[i]->id,
                       current->classrooms[i]->name,
                       current->classrooms[i]->type,
                       current->classrooms[i]->capacity);
            }
            return;
        }
        current = current->next;
    }
    printf("未找到教学楼: %s\n", building);
}

// 主菜单
void showMenu()
{
    printf("\n=== 校园教室资源管理系统 ===\n");
    printf("1. 添加教室\n");
    printf("2. 查找教室(按ID)\n");
    printf("3. 查看教学楼的所有教室\n");
    printf("4. 导入CSV数据\n");
    printf("0. 退出\n");
    printf("请选择操作: ");
}

// 从CSV文件导入数据
void importFromCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    char line[256];
    // 跳过标题行
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {
        int id;
        char campus[50], classroom[50], type[50], scheduled[10], building[50];
        int capacity;

        // 解析CSV行
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d,%[^\n]",
               &id, campus, classroom, type, scheduled,
               &capacity, building);

        // 创建并插入教室节点
        Classroom *newRoom = createClassroom(id, building, classroom,
                                             type, capacity,
                                             strcmp(scheduled, "是") == 0);
        root = insertAVL(root, newRoom);
        addClassroomToBuilding(buildingHash, newRoom);
    }

    fclose(file);
    printf("数据导入完成！\n");
}

// 获取节点高度
int getHeight(Classroom *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

// 获取平衡因子
int getBalanceFactor(Classroom *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// 更新节点高度
void updateHeight(Classroom *node)
{
    if (node == NULL)
        return;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// 右旋转
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

// 左旋转
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

// AVL树插入函数
Classroom *insertAVL(Classroom *node, Classroom *newRoom)
{
    // 执行标准BST插入
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
        return node; // 重复ID不插入

    // 更新节点高度
    updateHeight(node);

    // 获取平衡因子
    int balance = getBalanceFactor(node);

    // 左左情况
    if (balance > 1 && newRoom->id < node->left->id)
        return rightRotate(node);

    // 右右情况
    if (balance < -1 && newRoom->id > node->right->id)
        return leftRotate(node);

    // 左右情况
    if (balance > 1 && newRoom->id > node->left->id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // 右左情况
    if (balance < -1 && newRoom->id < node->right->id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// 查找AVL树中的最小值节点
Classroom *findMin(Classroom *node)
{
    if (node == NULL)
        return NULL;
    while (node->left != NULL)
        node = node->left;
    return node;
}

// AVL树删除函数
Classroom *deleteAVL(Classroom *root, int id)
{
    if (root == NULL)
        return root;

    // 执行标准BST删除
    if (id < root->id)
        root->left = deleteAVL(root->left, id);
    else if (id > root->id)
        root->right = deleteAVL(root->right, id);
    else
    {
        // 节点找到，执行删除
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
            // 有两个子节点的情况
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

    // 更新高度
    updateHeight(root);

    // 检查平衡因子并进行旋转
    int balance = getBalanceFactor(root);

    // 左左情况
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // 左右情况
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // 右右情况
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // 右左情况
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
