#include <stdio.h>

#include <stdlib.h>

#include <string.h>

// 原有宏定义保留

#define MAX_STR_LEN 128

// 新增Trie树相关宏定义

#define MAX_CHAR_CNT 26 // 假设只存储小写英文字母，若支持中文可修改为对应编码范围，此处以通用字母为例，可扩展

// -------------------------- 原有链表结构保留 --------------------------
typedef struct Node {
    char data[MAX_STR_LEN];
    struct Node* next;
} Node, *LinkList;

// 原有链表函数保留（初始化、销毁、创建、查找、打印、菜单）
void InitList(LinkList *L) {
    *L = (Node*)malloc(sizeof(Node));
    if (*L == NULL) {
        printf("内存分配失败!\n");
        exit(1);
    }
    (*L)->next = NULL;
}

void DestroyList(LinkList L) {
    Node* P = L->next;
    while (P != NULL) {
        Node* q = P->next;
        free(P);
        P = q;
    }
    L->next = NULL;
}

void CreateList(LinkList L) {
    DestroyList(L);
    int n;
    printf("请输入链表元素个数:");
    scanf("%d", &n);
    if (n <= 0) {
        printf("输入有误!元素个数必须大于0。\n");
        return;
    }
    Node* tail = L;
    printf("请输入%d个字符串(用空格分隔):\n", n);
    for (int i = 0; i < n; i++) {
        char val[MAX_STR_LEN];
        scanf("%s", val);
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {
            printf("内存分配失败!\n");
            exit(1);
        }
        strcpy(newNode->data, val);
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
    printf("链表创建完成!\n");
}

Node* LinkSearch(LinkList L, char target[]) {
    Node* P = L->next;
    int pos = 1;
    while (P != NULL) {
        if (strcmp(P->data, target) == 0) {
            printf("查找成功!目标值%s是链表的第%d个节点,节点地址%p\n", target, pos, P);
            return P;
        }
        P = P->next;
        pos++;
    }
    printf("查找失败!目标值%s不在链表中。\n", target);
    return NULL;
}

void PrintList(LinkList L) {
    if (L->next == NULL) {
        printf("当前链表为空,请先创建链表!\n");
        return;
    }
    Node* P = L->next;
    printf("当前链表元素为:");
    while (P != NULL) {
        printf("%s ", P->data);
        P = P->next;
    }
    printf("\n");
}

void ShowMenu() {
    printf("\n===== 链表操作菜单 =====\n");
    printf("1. 创建新链表(尾插法)\n");
    printf("2. 按值查找元素\n");
    printf("3. 打印当前链表\n");
    printf("4. 前缀搜索词汇(基于Trie树)\n"); // 新增菜单选项
    printf("0. 退出程序\n");
    printf("========================\n");
    printf("请输入你的选择:");
}

// -------------------------- 新增Trie树结构和函数 --------------------------
// Trie树节点定义
typedef struct TrieNode {
    int isEnd; // 标记是否为一个词汇的结尾
    struct TrieNode* children[MAX_CHAR_CNT]; // 子节点指针数组
} TrieNode;

// 创建新Trie节点
TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEnd = 0;
    for (int i = 0; i < MAX_CHAR_CNT; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// 插入词汇到Trie树
void trieInsert(TrieNode* root, char* word) {
    TrieNode* p = root;
    for (int i = 0; word[i] != '\0'; i++) {
        // 转小写，处理大小写不敏感；若需要中文可修改为对应索引计算方式
        int c = tolower(word[i]) - 'a';
        if (c < 0 || c >= MAX_CHAR_CNT) continue; // 跳过超出范围的字符
        if (p->children[c] == NULL) {
            p->children[c] = createTrieNode();
        }
        p = p->children[c];
    }
    p->isEnd = 1; // 标记词汇结束
}

// 深度优先搜索：收集所有以当前前缀为开头的词汇
void dfsCollect(TrieNode* node, char* currentPrefix, char result[][MAX_STR_LEN], int* cnt) {
    if (node->isEnd) {
        strcpy(result[(*cnt)++], currentPrefix);
    }
    for (int i = 0; i < MAX_CHAR_CNT; i++) {
        if (node->children[i] != NULL) {
            char nextChar = 'a' + i;
            int len = strlen(currentPrefix);
            currentPrefix[len] = nextChar;
            currentPrefix[len + 1] = '\0';
            dfsCollect(node->children[i], currentPrefix, result, cnt);
            currentPrefix[len] = '\0'; // 回溯
        }
    }
}

// 在Trie树中按前缀搜索，返回匹配结果数量
int trieSearchPrefix(TrieNode* root, char* prefix, char result[][MAX_STR_LEN]) {
    TrieNode* p = root;
    int cnt = 0;
    // 先定位到前缀的最后一个节点
    for (int i = 0; prefix[i] != '\0'; i++) {
        int c = tolower(prefix[i]) - 'a';
        if (c < 0 || c >= MAX_CHAR_CNT || p->children[c] == NULL) {
            return 0; // 前缀不存在
        }
        p = p->children[c];
    }
    char current[MAX_STR_LEN] = {0};
    strcpy(current, prefix);
    dfsCollect(p, current, result, &cnt);
    return cnt;
}

// 销毁Trie树
void destroyTrie(TrieNode* root) {
    if (root == NULL) return;
    for (int i = 0; i < MAX_CHAR_CNT; i++) {
        destroyTrie(root->children[i]);
    }
    free(root);
}

// 将链表中所有词汇插入Trie树，构建Trie树
TrieNode* buildTrieFromList(LinkList L) {
    TrieNode* root = createTrieNode();
    Node* p = L->next;
    // 遍历链表每个字符串，直接插入（若需要分词可在此对字符串分词后插入，示例中直接将每个字符串作为分词结果）
    while (p != NULL) {
        trieInsert(root, p->data);
        p = p->next;
    }
    return root;
}

// -------------------------- 修改后main函数 --------------------------
int main() {
    LinkList L;
    int choice;
    TrieNode* trieRoot = NULL; // Trie树根节点
    InitList(&L);
    while (1) {
        ShowMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                CreateList(L);
                // 链表重建后重新构建Trie树
                if (trieRoot != NULL) {
                    destroyTrie(trieRoot);
                }
                trieRoot = buildTrieFromList(L);
                break;
            case 2:
                if (L->next == NULL) {
                    printf("链表为空,请先创建链表!\n");
                    break;
                }
                char target[MAX_STR_LEN];
                printf("请输入要查找的字符串:");
                scanf("%s", target);
                LinkSearch(L, target);
                break;
            case 3:
                PrintList(L);
                break;
            case 4: { // 新增前缀搜索功能
                if (L->next == NULL) {
                    printf("链表为空,请先创建链表!\n");
                    break;
                }
                if (trieRoot == NULL) {
                    trieRoot = buildTrieFromList(L);
                }
                char prefix[MAX_STR_LEN];
                printf("请输入要搜索的前缀:");
                scanf("%s", prefix);
                char result[100][MAX_STR_LEN]; // 最多存储100个匹配结果
                int cnt = trieSearchPrefix(trieRoot, prefix, result);
                if (cnt == 0) {
                    printf("未找到以%s开头的词汇\n", prefix);
                } else {
                    printf("找到%d个以%s开头的词汇:\n", cnt, prefix);
                    for (int i = 0; i < cnt; i++) {
                        printf("%d. %s\n", i+1, result[i]);
                    }
                }
                break;
            }
            case 0:
                printf("退出,感谢使用!\n");
                DestroyList(L);
                if (trieRoot != NULL) {
                    destroyTrie(trieRoot);
                }
                exit(0);
            default:
                printf("选项有误,请输入0-4之间的有效选项\n");
                break;
        }
    }
    return 0;
}
