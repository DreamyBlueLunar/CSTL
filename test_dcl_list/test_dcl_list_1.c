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
    
    my_entry_t *n1 = malloc(sizeof(my_entry_t));
    my_entry_t *n2 = malloc(sizeof(my_entry_t));
    my_entry_t *n3 = malloc(sizeof(my_entry_t));
    my_entry_t *n4 = malloc(sizeof(my_entry_t));
    my_entry_t *n5 = malloc(sizeof(my_entry_t));
    my_entry_t *n6 = malloc(sizeof(my_entry_t));
    n1->data = 5;
    n2->data = 4;
    n3->data = 3;
    n4->data = 2;
    n5->data = 1;
    n6->data = 12138;

    DCL_LIST_INSERT_HEAD(list, my_entry_t, n1, entries);
    DCL_LIST_INSERT_HEAD(list, my_entry_t, n2, entries);
    DCL_LIST_INSERT_HEAD(list, my_entry_t, n3, entries);
    DCL_LIST_INSERT_HEAD(list, my_entry_t, n4, entries);
    DCL_LIST_INSERT_HEAD(list, my_entry_t, n5, entries);
    DCL_LIST_INSERT_HEAD(list, my_entry_t, n6, entries);

    my_entry_t *n7 = malloc(sizeof(my_entry_t));
    my_entry_t *n8 = malloc(sizeof(my_entry_t));
    n7->data = 2000;
    n8->data = 2004;
    DCL_LIST_INSERT_AFTER(n3, n7, entries);
    DCL_LIST_INSERT_BEFORE(n3, n8, entries);

    my_entry_t *tmp;
    printf("===============1==============\n");
    DCL_LIST_FOREACH(tmp, list, entries) {
        printf("%d\n", tmp->data);
    }

    DCL_LIST_DELETE(n6, entries);
    DCL_LIST_DELETE(n7, entries);
    DCL_LIST_DELETE(n8, entries);

    printf("===============2==============\n");
    DCL_LIST_FOREACH(tmp, list, entries) {
        printf("%d\n", tmp->data);
    }

    DCL_LIST_SWAP_ENTRIES(n2, n3, my_entry_t, entries);
    DCL_LIST_SWAP_ENTRIES(n5, n1, my_entry_t, entries);
    DCL_LIST_SWAP_ENTRIES(n4, n2, my_entry_t, entries);
    printf("===============3==============\n");
    DCL_LIST_FOREACH(tmp, list, entries) {
        printf("%d\n", tmp->data);
    }

    DCL_LIST_DESTROY(list, my_entry_t, entries);

    return 0;
}