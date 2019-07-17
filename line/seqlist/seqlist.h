#ifndef __SEQLIST_H_
#define __SEQLIST_H_

#define MAXSIZE 100

typedef int data_t;
typedef struct {
    data_t data[MAXSIZE];//存储空间
    int last; //表尾指针
}seqlist_t; 

seqlist_t *create_seqlist(void);   //创建
void clear_seqlist(seqlist_t *L);   //释放

int is_empty_seqlist(seqlist_t *L);  //是否为空
int is_full_seqlist(seqlist_t *L);  //是否为满
void set_empty_seqlist(seqlist_t *L);  //清空
int get_length_seqlist(seqlist_t *L);  //长度
void show_seqlist(seqlist_t *L);   //显示
/*
 * 顺序表的增删改查
 * */
int insert_seqlist(seqlist_t *L,data_t x,int pos);
int delete_seqlist(seqlist_t *L,int pos);
int change_seqlist(seqlist_t *L,data_t x,int pos);
int search_seqlist(seqlist_t *L,data_t x);

#endif

