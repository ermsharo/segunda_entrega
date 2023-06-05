#include "headers.h"


/*
    A partir de um char que define o nome do arquivo e 
    um boolean que define a sobre sobre escrita de um aquivo 
    chamado tree.dat quando verdadeiro, uma arvore b é intanciada.
    
    Quando mode é falso a definição de parametros como raiz ( root ) 
    e proxima posição ( nextPos ) são iniciadas com 0, bem como 
    o nome do arquivo passado por parametro é considerado para o 
    armazenamento dos dados da arvore, e aberto em modo de escrita.
    
    Quando mode é verdadeiro o arquivo de armazenamento prévio é considerado 
     "tree.dat", e este arquivo e aberto em modo de leitura
    entretato o nome do novo aquivo que é considerado para armazenamento atual

    No final é retornado um ponteiro para a arvore B* criada 
*/
bTree* createTree(char* fileName,bool mode)
{
	bTree* tree = malloc(sizeof(bTree));
    
    // Caso seja um arquivo totalmente novo 
    // mode será = False
	if(!mode) //new file
    {
        
        strcpy(tree->fileName,fileName);
        tree->fp = fopen(fileName,"w");
        fclose(tree->fp);

        tree->root = 0;
        tree->nextPos = 0;
    // Caso de um arquivo antigo ser considerado 
    } else {
        FILE *oldFile = fopen("tree.dat", "r");
        fread(tree, sizeof(bTree), 1, oldFile);
        fclose(oldFile);
    }
    // Novo arquivo é considerado como sendo o 
    // principal para armazenamento 
    tree->fp = fopen(fileName, "r+");
    return tree;
}

/*
    Inicializa nó da arvore B com base no ponteiro de 
    um nó já instanciado, em um booleando que é define se o
    nó é uma folha ou não, e por fim um ponteiro da arvore 
    de onde este nó que sera inicializado ira pertencer. 

    Os parametros do nó isLeaf, noOfRecords e pos são inicializados 
    respectivamente considerando que o nó está vazio, e a arvore 
    em questão e setada no nó. 

    O nó tambem tem seu tamanho inicializado como todas as posições 
    sendo setadas para -1. 

    e por fim o nó e retornado.
*/
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree)
{
	node->isLeaf = isLeaf;
	node->noOfRecs=0;
	node->pos = tree->nextPos;
	(tree->nextPos)++;
	int i;
	for (int i = 0; i < 2*t; ++i)
	{
		node->children[i] = -1;
	}
	return node;
}

/*
    Escreve no arquivo através dos ponteiros para a arvore b, o nó, 
    bem com um int para a posição.

    primeiramente é feita uma verificação para ver se a posição passada não é -1 
    pois dessa forma não se consegue calcular a posição para escrita 

    na segunda parte o SEEK é feito no arquivo de armazenamento 
    considerando os parametros passados para que a escrita seja feita no local correto 
    Após o SEEK a escrita (WRITE) é feita no arquivo por meio do campo fp da arvore
*/
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos) {
    // Verificação para chaves com posiçao -1
    if(pos == -1) {
        pos = ptr_tree->nextPos++;
    }

    // Atravez do calculo vai até o lugar para escrita 
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), 0);
    // Faz a escrita de acordo com o tamanho de um nó e o conteudo do nó 
    fwrite(p, sizeof(bTreeNode), 1, ptr_tree->fp);
    
}

/*
    Lê o arquivo através dos ponteiros para a arvore b, o nó, 
    bem com um int para a posição.

    o SEEK é feito no arquivo de armazenamento considerando os parametros 
    passados para que a leitura seja feita no local correto 
    Após o SEEK a escrita (READ) é feita no arquivo por meio do campo fp da arvore

*/
void readFile(bTree* ptr_tree, bTreeNode* p, int pos) {    
    // Atravez do calculo vai até o lugar para leitura
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
    // Faz a leitura de acordo com o tamanho de um nó e o conteudo do nó 
    fread(p, sizeof(bTreeNode), 1, ptr_tree->fp);
}

/*
    Setter para que todos os dados respectivos da chave de registro 
    ( id_num, country, grate, score, rate ) no nó passado para que esta 
    operação de escrita seja feita

    stripy : strcpy (variável, string); Essa função copia o valor da string
    na segunda posição do argumento para uma variável string na primeira posição.

    fonte : https://www.unicamp.br/fea/ortega/info/aula09.htm#:~:text=Para%20armazenar%20uma%20string%20literal,vari%C3%A1vel%20string%20na%20primeira%20posi%C3%A7%C3%A3o.
*/
void enterData(recordNode* record, int id_num, char country[], char grate[], int score, int rate) {
    
    record->key = id_num;
    strcpy(record->country, country);
    strcpy(record->grate, grate);
    record->score = score;
    record->rate = rate;
    
    return;
}

/*
   Pega dados de um arquivo já existente que tem seu caminho definido 
   pelo caracter filepath, e tem o tamanho definido pela variavel len

   Um nó é alocado para a o manuseio do arquivo na memoria principal 
   este nó é alocado com base no tamanho do nó de uma arvore multiplicado 
   pelo tamanho do arquivo passado, após isso em um loop while o arquivo 
   atravez de um fscanf é percorrido para que os dados sejam coletados em 
   todas linhas do arquivo


*/
recordNode* getData(char *filepath, int len) {
    
    // Alocamento de um espaço de memoria principal 
    // que tenha o mesmo tamanho do arquivo 
    recordNode *recordArr = malloc(sizeof(recordNode)*len);

    // Inicialização das variaveis correnpontes ao nó 
    char delim = ','; char line[256];
    int file_no = 0;
    int i,key,score,rate;
    char country[255];
    char grate[255];
    
    // Arquivo atravez do caminho passado é aberto 
    FILE *inpFile = fopen(filepath, "r");
    
    // Percorre todas as linhas do arquivo convertendo os registros 
    // em um nó no array de nós. a converção é feita pelo metodo enterData
    while(file_no < len && fscanf(inpFile, "%d,%[^,],%[^,],%d,%d", &key, country, grate, &score, &rate)) {
        enterData(&recordArr[file_no], key, country, grate, score, rate);
        file_no++;
    }

    return recordArr;
}

void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y)
{
	bTreeNode* z = malloc(sizeof(bTreeNode));
	nodeInit(z,y->isLeaf,tree);
	z->noOfRecs = t-1;

	int j;
	for(j=0;j<t-1;j++)
	{
		z->recordArr[j] = y->recordArr[j+t];
	}

	if(!y->isLeaf)
	{
		for(j=0;j<t;j++)
		{
			z->children[j] = y->children[j+t];
            y->children[j+t] = -1; 
		}
	}
	y->noOfRecs = t-1;

	for(j=(x->noOfRecs); j >= i+1;j--)
	{
		x->children[j+1] = x->children[j];
	}
	
	x->children[i+1] = z->pos;

	for(j=(x->noOfRecs) - 1; j >= i;j--)
	{
		x->recordArr[j+1] = x->recordArr[j];
	}
	x->recordArr[i] = y->recordArr[t-1];
	x->noOfRecs++;

    writeFile(tree, x, x->pos);
    writeFile(tree, y, y->pos);
    writeFile(tree, z, z->pos);
	free(z);
}

void insertNonFull(bTree* tree,bTreeNode* x,recordNode* record)
{	
	int i = (x->noOfRecs)-1;
	if(x->isLeaf == true)
	{
		while((i>=0) && (record->key < x->recordArr[i]->key))
		{
			x->recordArr[i+1] = x->recordArr[i];
			i--;
		}
		x->recordArr[i+1] = record;
		(x->noOfRecs)++;

        writeFile(tree, x, x->pos);
	}
	else
	{
		while((i>=0) && (record->key < x->recordArr[i]->key))
		{
			i=i-1;
		}
		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, x->children[i+1]);
        
		if(childAtPosi->noOfRecs == (2*t-1))
		{
			splitChild(tree,x,i+1,childAtPosi);
			if( x->recordArr[i+1]->key < record->key){
				i++;
			}
		}

        readFile(tree, childAtPosi, x->children[i+1]);
		insertNonFull(tree,childAtPosi,record);

		free(childAtPosi);
	}
}

void insert(bTree* tree,recordNode* record)
{
	if(tree->nextPos == 0) 
	{
		tree->root = tree->nextPos;

		bTreeNode* firstNode = malloc(sizeof(bTreeNode));
		nodeInit(firstNode,true,tree);
		firstNode->recordArr[0] = record;
		(firstNode->noOfRecs)++;

        writeFile(tree, firstNode, firstNode->pos);

		free(firstNode);
		return;
	}
	else
	{
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);

		if(rootCopy->noOfRecs == (2*t-1))
		{
			bTreeNode* newRoot = malloc(sizeof(bTreeNode));
			nodeInit(newRoot,false,tree);
			newRoot->children[0] = tree->root;

			splitChild(tree,newRoot,0,rootCopy);

			int i=0;
			if(newRoot->recordArr[0]->key < record->key){
				i++;
			}
			
			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, newRoot->children[i]);
			insertNonFull(tree,childAtPosi,record);

			tree->root = newRoot->pos;
            
            

			free(newRoot);
            free(childAtPosi);
		}
		else
		{
			insertNonFull(tree,rootCopy,record);
		}
		free(rootCopy);
	}
}

void traverse(bTree* tree, int root) {
    
    if(-1 == root) {    
        return;
    }

    bTreeNode *toPrint = malloc(sizeof(bTreeNode));
    readFile(tree, toPrint, root);
    dispNode(toPrint);

    for(int i = 0; i < 2*t; i++) {
        traverse(tree, toPrint->children[i]);
    }

    free(toPrint);
}

void dispNode(bTreeNode* node)
{
	printf("Position in node:%d\n",node->pos );
    printf("Number of Records:%d\n",node->noOfRecs );
	printf("Is leaf?:%d\n",node->isLeaf );
	printf("Keys:\n");
	for(int i = 0; i < node->noOfRecs; i++)
	{
		printf("%d ", node->recordArr[i]->key);
	}
	printf("\n");
	printf("Links:\n");
	for (int i = 0; i < 2*t; ++i)
	{
		printf("%d ",node->children[i] );
	}
	printf("\n");
    printf("\n");
}

recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root) {
    int i = 0;
    
    while(i < root->noOfRecs && key > root->recordArr[i]->key)
        i++;
    
    
    if(i < root->noOfRecs && key == root->recordArr[i]->key)
        return root->recordArr[i];
    
    
    else if(root->isLeaf) {
        return NULL;
    }
    else {
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, root->children[i]);

        recordNode* found = searchRecursive(tree, key, childAtPosi);
        free(childAtPosi);
        return found;
    }
}

recordNode* search(bTree* tree, int key) {
    
    bTreeNode* root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    recordNode* result = searchRecursive(tree, key, root);
    free(root);
    return result;
     
}

int findKey(bTreeNode* node, int k) {
    int idx=0;
    while (idx < node->noOfRecs && node->recordArr[idx]->key < k)
        ++idx;
    return idx;
}

void removeFromLeaf (bTree* tree, bTreeNode *node, int idx) {
    for (int i=idx+1; i<node->noOfRecs; ++i){
	    node->recordArr[i-1] = node->recordArr[i];
    }
    node->noOfRecs--;
}
 
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx) {
 
    int k = node->recordArr[idx]->key;
    
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]); 
 
    
    
    if (child->noOfRecs >= t) {
        recordNode* pred = getPred(tree, node, idx);
        node->recordArr[idx] = pred;
        removeNode(tree, child, pred->key); 
    }
 
    
    else if  (sibling->noOfRecs >= t)
    {
        recordNode* succ = getSucc(tree, node, idx);
        node->recordArr[idx] = succ;
        removeNode(tree, sibling, succ->key); 
    }
 
        else {
        child = merge(tree, node, idx); 
        removeNode(tree, child, k);
	    return; 
    }
    
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}


void removeNode(bTree* tree, bTreeNode* node, int k) {

    int idx = findKey(node, k);
    if (idx < node->noOfRecs && node->recordArr[idx]->key == k) {
        if (node->isLeaf){
	        removeFromLeaf(tree, node, idx);
        } else {
            removeFromNonLeaf(tree, node, idx);
        }
        
	    writeFile(tree, node, node->pos);
    }
    else {
       
        if (node->isLeaf) {
		    return false;
       	}
 
        bool flag = idx==node->noOfRecs;
 

        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[idx]);

        if (childAtPosi->noOfRecs < t) {
            fill(tree, node, idx);
            readFile(tree, childAtPosi, node->children[idx]);
        }

        if (flag && idx > node->noOfRecs) {
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[idx-1]);
            removeNode(tree, sibling, k);

            writeFile(tree, sibling, sibling->pos);
            free(sibling);
        }
        else {
            removeNode(tree, childAtPosi, k);
        }
        
        writeFile(tree, childAtPosi, childAtPosi->pos);
        free(childAtPosi);
    }
}


recordNode* getPred(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);

    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[curr->noOfRecs]);
    }
        
    recordNode* result = curr->recordArr[curr->noOfRecs-1];
    free(curr);
    return result;
}
 
recordNode* getSucc(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx+1]); 
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[0]);
    }
 
    
    recordNode* result = curr->recordArr[0];
    free(curr);
    return result;
}
 

void fill(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *cPrev = malloc(sizeof(bTreeNode));
    bTreeNode *cSucc = malloc(sizeof(bTreeNode));

    readFile(tree, cPrev, node->children[idx-1]);
    readFile(tree, cSucc, node->children[idx+1]);
    
    if (idx!=0 && cPrev->noOfRecs>=t) {
        borrowFromPrev(tree, node, idx);
    }
 

    else if (idx!=node->noOfRecs && cSucc->noOfRecs>=t) {
        borrowFromNext(tree, node, idx);
    }
 
    else {
        if (idx != node->noOfRecs)
            merge(tree, node, idx);
        else
            merge(tree, node, idx-1);
    }

    free(cPrev);
    free(cSucc);

    return;
}
 
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx-1]);
    

    for (int i=child->noOfRecs-1; i>=0; --i)
        child->recordArr[i+1] = child->recordArr[i];
 
    if (!child->isLeaf) {
        for(int i=child->noOfRecs; i>=0; --i)
            child->children[i+1] = child->children[i];
    }
 
    child->recordArr[0] = node->recordArr[idx-1];
 
    if (!node->isLeaf) {
        child->children[0] = sibling->children[sibling->noOfRecs];
        sibling->children[sibling->noOfRecs] = -1; 
    }
 
    node->recordArr[idx-1] = sibling->recordArr[sibling->noOfRecs-1];
 
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);
    
    free(child);
    free(sibling);

    return;
}
 
void borrowFromNext(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    child->recordArr[(child->noOfRecs)] = node->recordArr[idx];
 
    if (!(child->isLeaf))
        child->children[(child->noOfRecs)+1] = sibling->children[0];
 
    node->recordArr[idx] = sibling->recordArr[0];
 
    for (int i=1; i<sibling->noOfRecs; ++i)
        sibling->recordArr[i-1] = sibling->recordArr[i];
 
    if (!sibling->isLeaf) {
        for(int i=1; i<=sibling->noOfRecs; ++i)
            sibling->children[i-1] = sibling->children[i];

        sibling->children[sibling->noOfRecs] = -1; //RESPOSTA:
    }
 
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);

    return;
}
 
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx) {

    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    child->recordArr[t-1] = node->recordArr[idx];
 
    for (int i=0; i<sibling->noOfRecs; ++i)
        child->recordArr[i+t] = sibling->recordArr[i];
 
    if (!child->isLeaf) {
        for(int i=0; i<=sibling->noOfRecs; ++i)
            child->children[i+t] = sibling->children[i];
    }

    for (int i=idx+1; i<node->noOfRecs; ++i)
        node->recordArr[i-1] = node->recordArr[i];
 
    for (int i=idx+2; i<=node->noOfRecs; ++i) 
        node->children[i-1] = node->children[i];
    node->children[node->noOfRecs] = -1; 
    child->noOfRecs += sibling->noOfRecs+1;
    node->noOfRecs--;
 
    if(node->noOfRecs == 0) {
        tree->root = node->children[0];
    }

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(sibling);

    return child;
}

bool removeFromTree(bTree* tree, int key) {
    bTreeNode *root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    bool found = search(tree, key);
    if(found);
        removeNode(tree, root, key); 

    free(root);
    return found;
}

void hardPrint(bTree* tree) {
    bTreeNode* lido = (bTreeNode*) malloc(sizeof(bTreeNode));
    for(int i = 0; i < tree->nextPos; i++) {
        fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET);
        fread(lido, sizeof(bTreeNode), 1, tree->fp);

        if(lido->isLeaf <= 1)
            dispNode(lido);
        else
            printf("ERRO: isLeaf = %i\n\n", lido->isLeaf);
    }

    free(lido);
}

void doublePrint(bTree* tree) {
    printf("=================");
    printf("\nTraverse\n");
    traverse(tree, tree->root);

    printf("=================");
    printf("\nHard print\n");
    hardPrint(tree);
}