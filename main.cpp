#include "AVL.h"
#include "hash_table.h"
#include <stdio.h>

Classroom *root = NULL;
HashTable *buildingHash = NULL;
void showMenu();
void importFromCSV(const char *filename);

int main()
{
    buildingHash = createHashTable();
    int choice;

    do
    {
        showMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            // 添加教室
            int id, capacity, is_scheduled;
            char building[MAX_BUILDING_NAME];
            char name[MAX_CLASSROOM_NAME];
            char type[20];

            printf("输入教室ID: ");
            scanf("%d", &id);
            printf("输入教学楼: ");
            scanf("%s", building);
            printf("输入教室名称: ");
            scanf("%s", name);
            printf("输入教室类型: ");
            scanf("%s", type);
            printf("输入容量: ");
            scanf("%d", &capacity);
            printf("是否排课(1是/0否): ");
            scanf("%d", &is_scheduled);

            Classroom *newRoom = createClassroom(id, building, name,
                                                 type, capacity, is_scheduled);
            root = insertAVL(root, newRoom);
            addClassroomToBuilding(buildingHash, newRoom);
            printf("教室添加成功！\n");
            break;
        }
        case 2:
        {
            // 查找教室
            int id;
            printf("输入要查找的教室ID: ");
            scanf("%d", &id);

            Classroom *room = findClassroomById(root, id);
            if (room)
            {
                printf("\n找到教室:\n");
                printf("ID: %d\n教学楼: %s\n教室: %s\n类型: %s\n容量: %d\n",
                       room->id, room->building, room->name,
                       room->type, room->capacity);
            }
            else
            {
                printf("未找到ID为%d的教室\n", id);
            }
            break;
        }
        case 3:
        {
            // 查看教学楼的教室
            char building[MAX_BUILDING_NAME];
            printf("输入教学楼名称: ");
            scanf("%s", building);
            printClassroomsInBuilding(buildingHash, building);
            break;
        }
        case 4:
        {
            // 导入CSV数据
            printf("请输入CSV文件路径: ");
            char filename[256];
            scanf("%s", filename);
            importFromCSV(filename);
            break;
        }
        case 0:
            printf("感谢使用！再见！\n");
            break;
        default:
            printf("无效的选择，请重试\n");
        }
    } while (choice != 0);

    // 内存释放
    destroyHashTable(buildingHash);
    destroyAVL(root);
    printf("内存清理完成\n");

    return 0;
}