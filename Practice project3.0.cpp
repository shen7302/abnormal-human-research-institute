#include <stdio.h>

#include <stdlib.h>

#include <string.h>



#define MAX_STR_LEN 128

// ----------修改部分：链表节点结构----------
typedef struct Node {
    char data[MAX_STR_LEN]; // 数据域从int改为字符串类型
    struct Node* next;      // 指针域保留原有逻辑，指向下一个节点
} Node, *LinkList;

// 初始化链表(创建头节点)
void InitList(LinkList *L) {
    *L = (Node*)malloc(sizeof(Node)); // 头节点（不存储数据）
    if (*L == NULL) {
        printf("内存分配失败!\n");
        exit(1);
    }
    (*L)->next = NULL; // 头节点的next初始化为NULL
}

// 销毁链表（释放所有节点内存，重置为空链表）
void DestroyList(LinkList L) {
    Node* p = L->next;
    while (p != NULL) {
        Node* q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
}

// 创建链表（尾插法，输入元素）
void CreateList(LinkList L) {
    // 如果已经有链表，先释放原有内容避免泄漏
    DestroyList(L);

    int n;
    printf("请输入链表元素个数：");
    scanf("%d", &n);
    if (n <= 0) {
        printf("输入有误!元素个数必须大于0。\n");
        return;
    }

    Node* tail = L; // 尾指针，指向当前链表末尾
    printf("请输入%d个字符串（用空格分隔）：", n);
    for (int i = 0; i < n; i++) {
        char val[MAX_STR_LEN];
        scanf("%s", val);
        Node* newNode = (Node*)malloc(sizeof(Node)); // 创建新节点
        if (newNode == NULL) {
            printf("内存分配失败!\n");
            exit(1);
        }
        strcpy(newNode->data, val); // 修改：字符串拷贝替代直接赋值
        newNode->next = NULL;
        tail->next = newNode; // 尾节点的next指向新节点
        tail = newNode;        // 尾指针移动到新节点
    }
    printf("链表创建完成!\n");
}

// 链表查找（按值查找）
Node* LinkSearch(LinkList L, char target[]) {
    Node* p = L->next; // 从第一个节点（头节点的下一个）开始遍历
    int pos = 1; // 新增计数，输出位置更直观
    while (p != NULL) {
        if (strcmp(p->data, target) == 0) { // 修改：字符串比较替代==判断
            printf("查找成功!目标值%s是链表的第%d个节点，节点地址：%p。\n", target, pos, p);
            return p; // 找到，返回节点指针
        }
        p = p->next;
        pos++;
    }
    printf("查找失败!目标值%s不在链表中。\n", target);
    return NULL; // 未找到
}

// 打印当前链表所有元素
void PrintList(LinkList L) {
    if (L->next == NULL) {
        printf("当前链表为空，请先创建链表!\n");
        return;
    }
    Node* p = L->next;
    printf("当前链表元素为：");
    while (p != NULL) {
        printf("%s ", p->data); // 修改：字符串格式输出替代%d
        p = p->next;
    }
    printf("\n");
}

// 显示菜单
void ShowMenu() {
    printf("\n===== 链表操作菜单=====\n");
    printf("1. 创建新链表（尾插法）\n");
    printf("2. 按值查找元素\n");
    printf("3. 打印当前链表\n");
    printf("0. 退出程序\n");
    printf("======================\n");
    printf("请输入你的选择：");
}

int main() {
    LinkList L;
    int choice;
    char target[MAX_STR_LEN];
    // 提前初始化链表
    InitList(&L);

    while (1) {
        ShowMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                CreateList(L);
                break;
            case 2:
                if (L->next == NULL) {
                    printf("当前链表为空，请先创建链表!\n");
                    break;
                }
                printf("请输入要查找的字符串：");
                scanf("%s", target);
                LinkSearch(L, target);
                break;
            case 3:
                PrintList(L);
                break;
            case 0:
                printf("程序退出，感谢使用!\n");
                // 退出前释放所有内存
                DestroyList(L);
                free(L);
                exit(0);
            default:
                printf("输入选项有误，请输入0-3之间的有效选项\n");
                break;
        }
    }
    return 0;
}
