#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<math.h>
#include<time.h>
#include<string.h>
#define MAX 999

struct Node{
    Node *up,*down,*left,*right,*colPtr,*rowPtr;//指向row,col对应的行对象和列对象
    int row_num;//记录行数，行专属(从1开始)
    int col_elemCount;//记录该列元素个数，列专属
};

struct player{//玩家信息结点
    int m ;
    int s ;
    char name[20];
    int level ;
    player* next;
};


int row_size=593;//行数
int col_size = 324;//列数
int result[81];//存放结果行的栈
int index = 0;//栈指针
int sudoku[81] = {0};//存放数独
int time_start = 0;
int time_end = 0;//起始时间

void init(Node* head){
	head->left = head;
	head->right = head;
	head->up = head;
	head->down = head;

	for(int k = 0;k < row_size;k++){//创建行对象(头插)
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->up = head;
		newNode->down = head->down;
		newNode->left = newNode;
		newNode->right = newNode;
		newNode->down->up = newNode;
		head->down = newNode;
		newNode->row_num = row_size-k;
		newNode->col_elemCount = 0;//借用，作为标志
	}
}
void init_col(Node* head){
	/************初始化行列对象*******************/
	for(int j = 0;j < col_size;j++){//创建列对象(头插)
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->right = head->right;
		newNode->left = head;
		newNode->down = newNode;
		newNode->up = newNode;
		newNode->right->left = newNode;
		head->right = newNode;
		newNode->col_elemCount = 0;//列元素个数初始为0
	}
}

int main(){
    int player_res[81]={0};
    int choice;
    int** matrix;//存放数独的01矩阵
    int* answer;//存放答案
    player* easy;//容易难度排行
    player* normal;//简单难度排行
    player* hard;//困难难度排行
    player info ;//玩家信息

    Node* head =(Node*)malloc(sizeof(Node));
    //初始化行列对象，建立十字双向循环链表
    init(head);
    init_col(head);
}