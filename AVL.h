#ifndef AVL_H
#define AVL_H

#define MAX_BUILDING_NAME 50  // 教学楼名称最大长度
#define MAX_CLASSROOM_NAME 50 // 教室名称最大长度

// 头文件中出现了循环依赖
struct HashTable;

// 教室节点结构
typedef struct Classroom
{
    int id;                           // 教室ID
    char building[MAX_BUILDING_NAME]; // 教学楼
    char name[MAX_CLASSROOM_NAME];    // 教室名称
    char type[20];                    // 教室类型（多媒体/实验室等）
    int capacity;                     // 容量
    int is_scheduled;                 // 是否排课
    int height;                       // AVL树节点高度
    struct Classroom *left;           // 左子节点
    struct Classroom *right;          // 右子节点
} Classroom;

// 全局变量声明
extern Classroom *root;
extern HashTable *buildingHash;

// AVL树相关函数声明
Classroom *createClassroom(int id, const char *building, const char *name, const char *type, int capacity, int is_scheduled);
Classroom* insertClassroom(Classroom* node, Classroom* newRoom);
void addClassroomToBuilding(HashTable* ht, Classroom* room);
Classroom* findClassroomById(Classroom* node, int id);
void printClassroomsInBuilding(HashTable* ht, const char* building);
void showMenu();
void importFromCSV(const char* filename);
int getHeight(Classroom* node);
int getBalanceFactor(Classroom* node);
void updateHeight(Classroom* node);
Classroom* rightRotate(Classroom* y);
Classroom* leftRotate(Classroom* x);
Classroom* insertAVL(Classroom* node, Classroom* newRoom);
Classroom* findMin(Classroom* node);
Classroom* deleteAVL(Classroom* root, int id);
void destroyAVL(Classroom *node);
void printUnscheduledClassrooms(HashTable *ht);
#endif 