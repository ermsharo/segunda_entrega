#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// 1 , AFG , DIST , 93 , 18
struct rec
{
	int key;
	// rest
	char country[5];
	char grate[10];
	int score;
	int rate;
};
typedef struct rec recordNode;

/*
	Estrura base para no da arvore B
	isLeaf : boleano para indicar se é nó folha ou não
	pos : ponteiro para a posição atual do chave no nó

	recordNode : e definida de acordo com o t que foi 
	setado na arvore seguindo a regra da arvore b de 2 * t - 1

	children : da mesma forma e setado a partir do t passado 
	respeitando a definição da arvore b de ter 2*t filhos 

*/
struct bTreeNode
{
	bool isLeaf; 
	int pos; 
	int noOfRecs;
	recordNode* recordArr[2 * t - 1];
	int children[2 * t]; 
};

/* 
TODO : definir comentario para essa struct 
*/
typedef struct bTreeNode bTreeNode;

/*
	Estrutura de definição da arvore b
	fileName : campo para guardar o nome do arquivo 
	fp : guarda o um ponteiro para o arquivo de fato 
	root : define o indice da raiz (TODO : verificar corretude)
	nextPos :  define o proxima posição que alguma inserção deve 
	considerar para inserir uma chave
*/
struct tree
{
	char fileName[20];
	FILE* fp;
	int root;
	int nextPos;
};
typedef struct tree bTree;

bTree* createTree(char* fileName,bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree,recordNode* record);
void delete(bTree* tree,int key);
void traverse(bTree* tree, int root);
void dispNode(bTreeNode* node);
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);


void enterData(recordNode* record, int id_num, char country[], char Grate[], int Score, int Rate);
recordNode* getData(char *filepath, int len);
recordNode* search(bTree* tree, int key);
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
recordNode *getSucc(bTree* tree, bTreeNode *node, int idx);
recordNode *getPred(bTree* tree, bTreeNode *node, int idx);
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx);
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx);
void removeNode(bTree* tree, bTreeNode* node, int k);
int findKey(bTreeNode* node, int k);
