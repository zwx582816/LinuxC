/*
 * 顺序表的各种操作
 * */
#include <stdio.h>
#include <stdlib.h>
#include "seqlist.h"

/*
 *L->data[i](0<= i <= L->last)
 * */
seqlist_t *create_seqlist(void)  //创建
{
    seqlist_t *L = NULL; 
    L = (seqlist_t *)malloc(sizeof(seqlist_t));//为数据表创建空间
    if (L == NULL)
    {
        perror("malloc");
        return NULL;
    }
    L->last = -1;  //表为空时，为-1
    return L;
}
void clear_seqlist(seqlist_t *L)   //释放
{
    if (L == NULL)
    {
        puts("seqlist_t *L is NULL");
        return;
    }
    free(L);
}

int is_empty_seqlist(seqlist_t *L)  //是否为空
{
    if (L == NULL)
    {
        puts("seqlist_t *L is NULL");
        return -1;
    }
    return (L->last == -1);
}
int is_full_seqlist(seqlist_t *L)  //是否为满
{
    if (L == NULL)
    {
        puts("seqlist_t *L is NULL");
        return -1;
    }
    return (L->last == MAXSIZE-1);
}
/*
 *ClearSqlist(sqlist *l)   L->last = -1
 * */
void set_empty_seqlist(seqlist_t *L)  //清空
{
    if (L == NULL)
    {
        puts("seqlist_t *L is NULL");
    }
    L->last = -1;
}
/*
 *Length（seqlist_t *L）  return（L->last+1）
 * */
int get_length_seqlist(seqlist_t *L)  //长度
{
    if (L == NULL)
    {
        puts("seqlist_t *L is NULL");
        return -1;
    }
    return (L->last+1);
}
void show_seqlist(seqlist_t *L)   //显示
{
    int i = 0;
    if (L == NULL)
    {
        puts("seqlist_t *L is NULL");
        return;
    }
    for (i = 0; i<=L->last; i++)
        printf("L->data[%d] = %d\n",i,L->data[i]);
}
/*
 *算法思路：若表存在空闲空间，且参数i满足0<=i<=L->last+1 ,则可正常插入，插入前，将 *表中（L->data[L->last]~L->data[i])部分顺序下移一个位置，然后将x插入L->data[i]即可
 * */
int insert_seqlist(seqlist_t *L,data_t x,int pos)
{
    int i = 0;
    if((is_full_seqlist(L))||(pos < 0)||(pos>L->last+1))   
    {
        puts("inputs argv is invalid");
        return -1;
    }
    for (i=L->last; i>=pos; i--)
    {
        L->data[i+1] = L->data[i];
    }
    L->data[pos] = x;
    L->last++;
    return 0;
}
/*
*算法思路：且参数i满足0<=i<=L->last,将表中（L->data[i+1]~L->data[L->last])部分顺序*上移一个位置，覆盖L->data[i]
**/ 
int delete_seqlist(seqlist_t *L,int pos)
{
    int i = 0;
    if ((pos < 0)||(pos > L->last))
    {
        puts("inputs argv is invalid");
        return -1;
    }
    for (i = pos; i < get_length_seqlist(L); i++)
        L->data[i] = L->data[i+1];
    L->last--;
    return 0;
}
/*
 *GetSqlist（seqlist_t *L ,int i） return L->data[i]
 * */
int change_seqlist(seqlist_t *L,data_t x,int pos)
{
    if ((pos < 0)||(pos > L->last))
    {
        puts("inputs argv is invalid");
        return -1;
    }
    L->data[pos] = x;
    return 0;
}
/*
 * 算法思路：设一扫描变量i(i=0),判断当前表中的元素a[i]是否等于x，若相等，则返回当前i值（表明x落在表的第i位置）；否则i+1，继续比较，若表中无一元素与x相等，则返回-1
 * */
int search_seqlist(seqlist_t *L,data_t x)
{
    int i = 0;
    for(i = 0; i <= L->last; i++)
    {
        if (L->data[i] == x)
            return i;
    }
    return -1;
}
