/*
 * 双向循环链表接口测试
 */

#include "../dcl_list.h"
#include <stdio.h>

// 定义链表元素类型  
typedef struct my_entry {  
    int data;  
    DCL_LIST_ENTRY(my_entry) entries; // 使用LIST_ENTRY宏定义双向链表元素  
} my_entry_t;  

DCL_LIST_HEAD(dcl_list, my_entry);
typedef struct dcl_list dcl_list_t;

int main(void) {
    dcl_list_t list;
    DCL_LIST_INIT(list, my_entry_t, entries);
    int idx = 0;
    
    my_entry_t *node = NULL;
    for (int i = 499; i >= 0; i --) {
        node = malloc(sizeof(my_entry_t));
        if (NULL == node) {
            printf("failed to create newNode.\n");
            exit(-1);
        }
        node->data = i;
        DCL_LIST_INSERT_HEAD(list, my_entry_t, node, entries);
    }
#if 0
    printf("After inserting 500 nodes:\n");
    int idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif

    my_entry_t *mark = NULL;
    DCL_LIST_FOREACH(mark, list, entries) {
        if (499 == mark->data) {
            break;
        }
    }
    for (int i = 1099; i >= 1000; i --) {
        node = malloc(sizeof(my_entry_t));
        if (NULL == node) {
            printf("failed to create newNode.\n");
            exit(-1);
        }
        node->data = i;
        DCL_LIST_INSERT_AFTER(mark, node, entries);
    }
#if 0
    printf("After inserting 100 nodes after the node whose value is 499:\n");
    idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif

    mark = NULL;
    DCL_LIST_FOREACH(mark, list, entries) {
        if (0 == mark->data) {
            break;
        }
    }
    for (int i = 1199; i >= 1100; i --) {
        node = malloc(sizeof(my_entry_t));
        if (NULL == node) {
            printf("failed to create newNode.\n");
            exit(-1);
        }
        node->data = i;
        DCL_LIST_INSERT_AFTER(mark, node, entries);
    }
#if 0
    printf("After inserting 100 nodes after the node whose value is 0:\n");
    idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif

    mark = NULL;
    DCL_LIST_FOREACH(mark, list, entries) {
        if (299 == mark->data) {
            break;
        }
    }
    for (int i = 1299; i >= 1200; i --) {
        node = malloc(sizeof(my_entry_t));
        if (NULL == node) {
            printf("failed to create newNode.\n");
            exit(-1);
        }
        node->data = i;
        DCL_LIST_INSERT_BEFORE(mark, node, entries);
    }
#if 0
    printf("After inserting 100 nodes before the node whose value is 299:\n");
    idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif

    my_entry_t *cur = DCL_LIST_NEXT(DCL_LIST_FIRST(list), entries);
    my_entry_t *prev = NULL;
    while (cur != DCL_LIST_FIRST(list)) {
        prev = cur;
        cur = DCL_LIST_NEXT((cur), entries);
        if (0 == prev->data % 2) {
            DCL_LIST_DELETE((prev), entries);
        }
    }
#if 0
    printf("After deleting all nodes whose value is even:\n");
    idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif

    my_entry_t *i, *j;
    DCL_LIST_FOREACH(i, list, entries) {
        if (299 == i->data) {
            break;
        }
    }
    DCL_LIST_FOREACH(j, list, entries) {
        if (499 == j->data) {
            break;
        }
    }
    DCL_LIST_SWAP_ENTRIES(i, j, my_entry_t, entries);
    DCL_LIST_FOREACH(i, list, entries) {
        if (399 == i->data) {
            break;
        }
    }
    DCL_LIST_FOREACH(j, list, entries) {
        if (401 == j->data) {
            break;
        }
    }
    DCL_LIST_SWAP_ENTRIES(i, j, my_entry_t, entries);
#if 1
    printf("After swapping 299 and 499, 399 and 401:\n");
    idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif
    DCL_LIST_SWAP_ENTRIES(j, i, my_entry_t, entries);
#if 1
    printf("After swapping 399 and 401 again:\n");
    idx = 0;
    DCL_LIST_FOREACH(node, list, entries) {
        printf("node#%d's data: %d\n", idx, node->data);
        idx ++;
    }
    printf("\n");
#endif
    DCL_LIST_DESTROY(list, my_entry_t, entries);

    return 0;
}