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

/* 
    Recebe um ponteiro para arvore, um nó pai x que esta cheio 
    um marcador i de posição bem como um nó y filho.
    Leva a chave do meio do nó y para o nó pai x, e copia metade
    do conteudo e filhos do nó filho y para um novo no x caso não seja 
    um nó folha
*/
void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y)
{
    // Aloca memoria vem como inicializa o novo nó irmão Z
	bTreeNode* z = malloc(sizeof(bTreeNode));
	nodeInit(z,y->isLeaf,tree);
	z->noOfRecs = t-1;

    // Copia metade das chaves do nó y para o nó z
	int j;
	for(j=0;j<t-1;j++)
	{
		z->recordArr[j] = y->recordArr[j+t];
	}

    // Caso não seja uma folha copia metade dos filhos 
    // do nó y para o nó z, além de esvaziar os ponteiros
    // nó y setando os filhos para -1
	if(!y->isLeaf)
	{
		for(j=0;j<t;j++)
		{
			z->children[j] = y->children[j+t];
            y->children[j+t] = -1; 
		}
	}
    // Seta numero de chaves para a metade
	y->noOfRecs = t-1;
    
    // Move todos filhos uma posição para frente 
	for(j=(x->noOfRecs); j >= i+1;j--)
	{
		x->children[j+1] = x->children[j];
	}
	
    // Seta o uma referencia que liga o 
    // novo nó x que foi criado para o nó x
	x->children[i+1] = z->pos;

    // Move todas chaves uma posição para frente 
	for(j=(x->noOfRecs) - 1; j >= i;j--)
	{
		x->recordArr[j+1] = x->recordArr[j];
	}
    // Seta a chave do meio do nó filho y para o pai x
	x->recordArr[i] = y->recordArr[t-1];
	x->noOfRecs++;

    // Salva as as alterações feitas em x, y e z
    writeFile(tree, x, x->pos);
    writeFile(tree, y, y->pos);
    writeFile(tree, z, z->pos);
	free(z);
}

/*
    Insere uma nova chave em um nó folha que não está cheio ainda
    e caso haja necessidade faz os splitChild em nós internos ele
    faz enquanto desce recursivamente.
*/
void insertNonFull(bTree* tree,bTreeNode* x,recordNode* record)
{	
    // Apos setar o contardor i para a ultima posição do nó x
    // caso o nó x for uma folha simplesmente vai deslocando 
    // todas as chaves deste nó x para que possa abrir expaço 
    // para a chave que vem da variavel record
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
        // Por fim salva as alterações feitas
        writeFile(tree, x, x->pos);
	}
    // Caso seja um nó interno, da mesma forma posiciona o 
    // ponteiro para a posição adequada da chave e faz split 
    // se se necessario, e continua com a chamada recursiva
	else
	{   
        // Posicionamento da chave no local certo 
		while((i>=0) && (record->key < x->recordArr[i]->key))
		{
			i=i-1;
		}

        // Faz um seek para trazer o no que devera ser feita a
        // chamada recursiva 
		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, x->children[i+1]);
        
        // Performa um split caso nó esteja cheio 
        // e verifica se a chave inserida é maior que a ultima do 
        // nó para que possa ser inserida 
		if(childAtPosi->noOfRecs == (2*t-1))
		{
			splitChild(tree,x,i+1,childAtPosi);
			if( x->recordArr[i+1]->key < record->key){
				i++;
			}
		}

        // Le novamente e faz chamada recursiva para continar 
        // buscando onde inserir a chave
        readFile(tree, childAtPosi, x->children[i+1]);
		insertNonFull(tree,childAtPosi,record);

		free(childAtPosi);
	}
}

/*
    Principal metodo a ser chamado quando uma inserção deve ser feita
*/
void insert(bTree* tree,recordNode* record)
{
    // Caso a arvore esteja sem nenhum registro 
	if(tree->nextPos == 0) 
	{
		tree->root = tree->nextPos;
        // Aloca memoria e cria um novo nó onde seta a chave a ser inserida
		bTreeNode* firstNode = malloc(sizeof(bTreeNode));
		nodeInit(firstNode,true,tree);
		firstNode->recordArr[0] = record;
		(firstNode->noOfRecs)++;

        writeFile(tree, firstNode, firstNode->pos);

		free(firstNode);
		return;
	}
    // Caso a raiz já tenha sido criada 
	else
	{
        // Le o a arvore que com base no ponteiro passado 
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);
        
        // Se a raiz está cheia ou seja 2*t-1 que é a regra da arvore b
		if(rootCopy->noOfRecs == (2*t-1))
		{
            // Criamos uma nova raiz e a altura da arvore crece em +1 de profundidade
			bTreeNode* newRoot = malloc(sizeof(bTreeNode));
			nodeInit(newRoot,false,tree);
			newRoot->children[0] = tree->root;

            // Metodo split child criado passando a raiz antiga e a nova
            // dessa forma dividindo os conteudos para considerando a raiz antiga
            // e a nova raiz como sendo o nó pai
			splitChild(tree,newRoot,0,rootCopy);


            // Contador vai até a posição que a chave a ser inserida deve ficar
			int i=0;
			if(newRoot->recordArr[0]->key < record->key){
				i++;
			}

            // E feito um seek para que um novo nó seja chamado para memoria 
            // principal e seja inserido a chave atravez de uma chamada recursiva
			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, newRoot->children[i]);
			insertNonFull(tree,childAtPosi,record);

            // Nova raiz é setada na arvore
			tree->root = newRoot->pos;

			free(newRoot);
            free(childAtPosi);
		}
        // Caso nó não esteje cheio uma chamada para a função insert not full e feita
		else
		{
			insertNonFull(tree,rootCopy,record);
		}
		free(rootCopy);
	}
}
/*
    Percorre toda a arvore de forma como se fosse uma 
    pre order traversal 
*/
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
/*
    Mostra conteudo de um nó, entre suas chaves e filhos
*/
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

/*
    Ideia Geral é a partir de da posição em um nó ir 
    ou posicionando ele até a posição correta ou 
    busca recursivamente em um nivel abaixo 
    e retorna o nó que esta sendo buscado 
*/
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root) {
    int i = 0;
    
    // Incrementa contador até a posição correnta que a chave 
    // possivelmente pode estar
    while(i < root->noOfRecs && key > root->recordArr[i]->key)
        i++;
    
    // Se o contador menor que o tamanho do nó e se a chave em
    // questão que o ponteiro parou é a chave buscada retorna ela 
    if(i < root->noOfRecs && key == root->recordArr[i]->key)
        return root->recordArr[i];
    
    // Retorna nulo caso esteja na raiz pois não há mais possibilidade
    // de o nó buscado estar em um nivel abaixo na arvore 
    else if(root->isLeaf) {
        return NULL;
    }
    // Caso contrario continuo buscando no nó filho respectivo ao 
    // contador atual de forma recursiva. Para isso ele chama o metodo
    // para que o proximo nó seja trazido para a memória principal
    else {
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, root->children[i]);

        recordNode* found = searchRecursive(tree, key, childAtPosi);
        free(childAtPosi);
        return found;
    }
}

/* 
    Função inicial que chama a função recursiva de busca
    e retorna um resultado 
*/
recordNode* search(bTree* tree, int key) {
    // Aloca nó raiz para inicialização da busca e
    // lê o arquivo de armazenamento de registros 
    bTreeNode* root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    // Inicia a busca de maneira recursiva passando a raiz 
    // como inicio
    recordNode* result = searchRecursive(tree, key, root);
    free(root);
    return result;
     
}

/*
    Move o contador idx ate a posicão mais conveniente para achar a chave k
*/
int findKey(bTreeNode* node, int k) {
    int idx=0;
    while (idx < node->noOfRecs && node->recordArr[idx]->key < k)
        ++idx;
    return idx;
}

/*
    Remove a chave e decrementa pelo processo 
    de mover todos as chaves a frente uma posição 
    para traz e decrementa o numero de nós.
*/
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx) {
    for (int i=idx+1; i<node->noOfRecs; ++i){
	    node->recordArr[i-1] = node->recordArr[i];
    }
    node->noOfRecs--;
}

/*
    Método que trata dos corner cases do caso de ter que deletar um
    nó interno, estes corner cases são os seguintes 
        -> 2.a Se o filho y que precede k do nó x tem pelo menos t chaves
        -> 2.b Simetricamente, se o filho z que segue k do nó x tem pelo menos t chaves
        -> 2.c Caso contrário, se ambos y e z possuem apenas t-1 chaves
*/
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx) {
    
    // Seta a chave atual com base no indice passado 
    int k = node->recordArr[idx]->key;
    
    // Aloca nós para o filho do node no parametro e 
    // para seu irmão 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    // Por meio de uma leitura seta o nó filho de nó passado no parametro 
    // e o nó irmão do filho respectivo 
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]); 
 
    // caso 2.a : nó interno, filho esquerdo (child) tenha t chaves
    //   primeiro a chave predecessora (pred) a com base no index do filho esquerdo(idx)    
    //   substitui a ultima chave do nó pai(child) pela chave predecessora no filho 
    //   por fim deletamos a chave predecessora do nó filho 
    if (child->noOfRecs >= t) {
        recordNode* pred = getPred(tree, node, idx);
        node->recordArr[idx] = pred;
        removeNode(tree, child, pred->key); 
    }
    // Caso 2.b : nó interno, filho direito (sibling) tem t chaves
    //   primeiro pega a chave sucessora ao index (idx) por meio do getSucc
    //   na sequencia substitui no nó pai (node) a chave sucessora
    //   por fim deleta a chave sucessroa do nó irmao direito(sibbling)
    else if  (sibling->noOfRecs >= t)
    {
        recordNode* succ = getSucc(tree, node, idx);
        node->recordArr[idx] = succ;
        removeNode(tree, sibling, succ->key); 
    }
    // Caso 2.c : nó interno, ambos filhos tem t-2 chaves 
    //   Chama o merge para copiar todo conteudo do nó a direita (child) da chave
    //   a ser deletada para a o nó a esquerda que sera obtido a partir de idx.
    //   Também este mesmo metodo merge trasfere a chave a ser deletada para o nó de baixo.
    //   Recursivamente chama o metodo removeNode que 
    //   simplesmente deleta o nó em questão da caso esteja na folha
    //   ou em recursão continua fazendo os merges ou outras operações necessarias    
    else {
        child = merge(tree, node, idx); 
        removeNode(tree, child, k);
	    return; 
    }
    
    // Alterações são escritas nos nós que tiveram operações realizadas 
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}

/*
    Trata de todos os corner cases da remoção 
    engloba os casos 1, 2 e 3
*/
void removeNode(bTree* tree, bTreeNode* node, int k) {

    // Chama o metodo findKey que posiciona o contador k onde
    // a chave precisa ser eliminada
    int idx = findKey(node, k);

    
    if (idx < node->noOfRecs && node->recordArr[idx]->key == k) {
        // Caso 1: Se a chave k está no nó x e x é uma folha, 
        // exclua a chave k de x.
        if (node->isLeaf){
	        removeFromLeaf(tree, node, idx);
        // Caso 2: Se a chave k está no nó x e x é um nó interno:
        } else {
            removeFromNonLeaf(tree, node, idx);
        }
        // Escreve arquivo para que as alterações sejam salvas
	    writeFile(tree, node, node->pos);
    }
    else {
       
        if (node->isLeaf) {
		    return false;
       	}
 
        bool flag = idx == node->noOfRecs;
 
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

/*
    Obtem uma chave predecessora a uma chave em idx
*/
recordNode* getPred(bTree* tree, bTreeNode *node, int idx) {

    // Aloca e lê o que sera o registro do nó filho onde esta a chave predecessora
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);

    // Enquanto current não é folha lê chama o filho para memoria
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[curr->noOfRecs]);
    }

    // Salva posição mais a direita do nó curr que é o predecessor
    recordNode* result = curr->recordArr[curr->noOfRecs-1];
    free(curr);
    return result;
}

/*
    Obtem uma chave Sucessora a uma chave em idx
*/
recordNode* getSucc(bTree* tree, bTreeNode *node, int idx) {
    
    // Aloca e lê o que sera o registro do nó filho onde esta a chave predecessora
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx+1]); 

    // Enquanto current não é folha lê chama o filho para memoria
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[0]);
    }
 
     // Salva posição mais a esquerda do nó curr que é a chave sucessora 
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
 
/*
    Operação utilizada quando temos que deletar uma chave que 
    serve de raiz para dois nós e estes nós podem ser mesclados 
*/ 
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx) {

    // Aloca memoria para o filho esquerdo e o irmão direito deste filho 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    // Seta os conteudos com base em 2 SEEKs para o filho e o irmao deste
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    // coloca a chave pai que sera deletada
    // no filho esquerdo
    child->recordArr[t-1] = node->recordArr[idx];
    
    // Percorre todo o irmão direito copiando o conteudo 
    // das chaves para o filho esquerdo 
    for (int i=0; i<sibling->noOfRecs; ++i)
        child->recordArr[i+t] = sibling->recordArr[i];
    
    // Caso não seja folha copia também todos os ponteiros para 
    // os filhos do irmão a direita
    if (!child->isLeaf) {
        for(int i=0; i<=sibling->noOfRecs; ++i)
            child->children[i+t] = sibling->children[i];
    }

    // Exclusão no chave desejada no nó pai 
    // Desloca todas chaves do nó pai (node) uma posicão 
    // para esquerda, pois a chave de cima mudou de lugar 
    // e foi para o filho.
    for (int i=idx+1; i<node->noOfRecs; ++i) 
        node->recordArr[i-1] = node->recordArr[i];
    
    // Exclusão da chave desejada do nó filho. Faz o mesmo 
    // processo do for de cima
    for (int i=idx+2; i<=node->noOfRecs; ++i) 
        node->children[i-1] = node->children[i];

    // Atualiza calores de controle 
    node->children[node->noOfRecs] = -1; 
    child->noOfRecs += sibling->noOfRecs+1;
    node->noOfRecs--;
    
    // Caso seja o nó pai fique vazio, seta a raiz para ser ele
    if(node->noOfRecs == 0) {
        tree->root = node->children[0];
    }

    // Escreve todas modificações feitas
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(sibling);

    return child;
}

/*
    Metodo inicial para exclusão de nó de uma arvore 
*/
bool removeFromTree(bTree* tree, int key) {

    // Aloca nó raiz e seta infomações de acordo com o arquivo que é lido
    bTreeNode *root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    // primeiramente verifica se a chave a ser excluido existe na arvore
    // por meio de uma busca
    bool found = search(tree, key);
    if(found);
        // se a chave existe na arvore esta pode ser excluida por meio 
        // de um metodo auxiliar para tratar de todos os corner cases
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