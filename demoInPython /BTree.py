class Node:
    def __init__(self, leaf=False):
        self.keys = []
        self.children = []
        self.leaf = leaf


class BTree():
    def __init__(self, t):
        self.root = Node(True)
        self.t = t

    # recebemos uma chave chamada key que e a que buscamos
    def search(self, key, node=None):

        # verifico se o nó atual for 
        # nulo seto como sendo a raiz
        # caso contrario seto como o 
        # no que veio por parametro 
        node = self.root if node == None else node

        # zera contador para percorrer o no corrente da arvore
        i = 0

        # enquanto o contador menor que o tamanho do nó 
        # e a chave buscada maior que a chave 
        # atual percorrida.
        # continuo incrementando o contador
        # para visitar outro registro.
        while i < len(node.keys) and key > node.keys[i]:
            i += 1

        # se o contador menor que o tamnho do nó 
        # e a chave buscada igual a chave atual retorna o nó e o contador 
        if i < len(node.keys) and key == node.keys[i]:
            return (node, i)
        # caso contrario se o no atual for uma folha retorno nulo 
        elif node.leaf:
            return None
        # caso contrario continuo buscando no nó filho 
        # respectivo ao contador atual de forma recursiva
        else:
            return self.search(key, node.children[i])
        
    def insert(self, k):
        t = self.t
        root = self.root 

        # primeiro checa se a raiz esta cheia     
        if len(root.keys) == ( 2* t ) - 1 :

            # caso cheia craimos um nó vazio e inserimos na b-tree
            # a altura dela cresce em 1
            new_root = Node()

            # setamos a raiz para o nó novo 
            self.root = new_root

            # adcionamos o valor da nova raiz para ser pai
            # da raiz antiga
            new_root.children.insert(0, root)

            # chamamos o split para fazer com que a raiz antiga 
            # fique com dois nós 
            self.split_child(new_root, 0)

            # chamamos o insert_non full
            self.insert_non_full(root,k)
        else:
            self.insert_non_full(root,k)

    # Basicamente temos dois casos, quando estamos em um nó folha 
    # (que é onde devemos inserir as chaves)
    # e quando ainda não estamos em um nó folha 
    # que devemos continuar fazendo os splits child conforme necessario 
    # e recursivamente buscar pela folha
    def insert_non_full(self, x, k):
        t = self.t 
        
        # setamos o contador i para ser o ultimo elemento do nó de chaves
        i = len(x.keys) - 1

        if x.leaf:
            # adcionamos ao final uma chave nula para termos onde almentar o nó
            x.keys.append(None)

            # vai iterando de traz pra frente no nó de chaves 
            # ao mesmo tempo que vai deslocando todas as chave 1 posicao pra frente
            # quando encontrar o local corrento de inserir a chave k para 
            while i >= 0 and k < x.keys[i]:
                x.keys[i + 1] = x.keys[i]
                i -= 1
            
            # insere a chave k no lugar certo 
            x.keys[i + 1] = k
        else :
            # primeiro escaneamos o nó de chaves para buscar 
            # a posição correta de inserção do k
            while i >= 0 and k < x.keys[i]:
                i -= 1
            i += 1

            # se nos depararmos com um nó cheio fazemos 
            # a operação de split nele 
            if len(x.children[i].keys) == ( 2 * t ) - 1 :
                self.split_child(x, i)
                if k > x.keys[i]:
                    i += 1
            
            # continuamos buscando recursivamente o lugar de inserir
            # até que cheguemos numa folha
            self.insert_non_full(x.children[i], k)

    # recebemos um no pai x e um indice i para o filho cheio
    def split_child(self, x, i):
        t = self.t

        # y e o filho cheio de x
        y = x.children[i]

        # cria um novo nó z e adiciona ele la lista dos filhos de x
        z = Node(y.leaf)
        x.children.insert(i + 1, z)

        # insere a mediana do filho chei o y em x 
        x.keys.insert(i, y.keys[t - 1])
        
        # divide as chaves de y na metade com exeção 
        # do nó mediano, e insere detro das chaves de 
        # z e y 
        z.keys = y.keys[t: (2 * t) - 1]
        y.keys = y.keys[0: t - 1]

        # se y não é folha, rearranjamos os filhos de y para y e z
        if not y.leaf:
            z.children = y.children[t: 2 * t]
            y.children = y.children[0: t - 1]

    def delete(self, x, k):
        t = self.t
        i = 0
        
        # Move o contador até o ponteiro correto 
        while i < len(x.keys) and k > x.keys[i]:
            i += 1
        if x.leaf:
            # caso esteja na folha e achou a chave correta
            # elimina a chave em questão
            if i < len(x.keys) and x.keys[i] == k:
                x.keys.pop(i)
            return

        # Caso 2 : nó interno e achamos a chave K
        if i < len(x.keys) and x.keys[i] == k:
            return self.delete_internal_node(x, k, i)
        # Continuando recursão e nó valido de tamanho t
        elif len(x.children[i].keys) >= t:
            self.delete(x.children[i], k)

        # Caso durante a busca recursiva pelo nó a ser deletado 
        # encontramos algum nó que seja diferente de t-1 chaves
        else:
            if i != 0 and i + 2 < len(x.children):
                # caso 3 a : nó somente tem t-1 chaves mas irmão tem t 
                # (Filho a ESQUERDA da chave tem t chaves)
                if len(x.children[i - 1].keys) >= t:
                    self.delete_sibling(x, i, i - 1)
                # caso 3 a : nó somente tem t-1 chaves mas irmão tem t 
                # (Filho a DIREITA da chave tem t chaves)
                elif len(x.children[i + 1].keys) >= t:
                    self.delete_sibling(x, i, i + 1)
                # caso 3 b : nó na recursão tem só t-1 chaves
                else:
                    self.delete_merge(x, i, i + 1)
            elif i == 0:
                # caso 3 a : nó somente tem t-1 chaves mas irmão tem t 
                # (Filho a DIREITA da chave tem t chaves)
                if len(x.children[i + 1].keys) >= t:
                    self.delete_sibling(x, i, i + 1)
                # caso 3 b : nó na recursão tem só t-1 chaves
                else:
                    self.delete_merge(x, i, i + 1)
            elif i + 1 == len(x.children):
                # caso 3 a : nó somente tem t-1 chaves mas irmão tem t 
                # (Filho a ESQUERDA da chave tem t chaves)
                if len(x.children[i - 1].keys) >= t:
                    self.delete_sibling(x, i, i - 1)
                # caso 3 b : nó na recursão tem só t-1 chaves
                else:
                    self.delete_merge(x, i, i - 1)
            self.delete(x.children[i], k)

    def delete_internal_node(self, x, k, i):
        t = self.t
        # caso 1 : simples deleção da folha 
        if x.leaf:
            if x.keys[i] == k:
                x.keys.pop(i)
            return
        # caso 2 a : nó interno, filho esquerdo tem t chaves
        #   deletamos o predecessor, pegamos o elemento 
        #   sucessor e substistuimos no nó atual deletado 
        if len(x.children[i].keys) >= t:
            x.keys[i] = self.delete_predecessor(x.children[i])
            return
        # caso 2 b : nó interno, filho direito tem t chaves
        #   deletamos o sucessor, pegamos o elemento predecessor  
        #   deletado no filho e substituimos no nó atual que 
        #   é pra ser deletado 
        elif len(x.children[i + 1].keys) >= t:
            x.keys[i] = self.delete_successor(x.children[i + 1])
            return
        # caso 2 c : nó interno, ambos filhos tem t-2 chaves 
        #   Chama o merge para copiar todo conteudo do nó a direita da chave
        #   a ser deletada para a o nó a esquerda. Também este mesmo metodo 
        #   merge trasfere a chave a ser deletada para o nó de baixo.
        #   Recursivamente chama o metodo delete_internal_node que 
        #   simplesmente deleta o nó em questão da caso esteja na folha
        #   ou em recursão continua fazendo os merges ou outras operações necessarias
        else:
            self.delete_merge(x, i, i + 1)
            self.delete_internal_node(x.children[i], k, self.t - 1)

    def delete_predecessor(self, x):
        if x.leaf:
            return x.keys.pop()
        n = len(x.keys) - 1
        if len(x.children[n].keys) >= self.t:
            self.delete_sibling(x, n + 1, n)
        else:
            self.delete_merge(x, n, n + 1)
        self.delete_predecessor(x.children[n])

    def delete_successor(self, x):
        if x.leaf:
            return x.keys.pop(0)
        if len(x.children[1].keys) >= self.t:
            self.delete_sibling(x, 0, 1)
        else:
            self.delete_merge(x, 0, 1)
        self.delete_successor(x.children[0])

    # x nó pai
    # i ponteiro para filho da esquerda da chava a ser deletada
    # j ponteiro para filho da direita da chava a ser deletada
    def delete_merge(self, x, i, j):
        # seta ponteiro para filho da esquerda
        cnode = x.children[i]
        
        if j > i:
            # setando nó da direita 
            rsnode = x.children[j]

            # coloca o nó pai que sera deletado 
            # no filho esquerdo
            cnode.keys.append(x.keys[i])

            # vai percorrendo pelo filho da direita 
            # copiando todo conteudo ( chaves e ponteiros ) 
            # para o filho da esquerda
            for k in range(len(rsnode.keys)):
                # copiando as chaves 
                cnode.keys.append(rsnode.keys[k])
                # copiando os ponteiros dos filhos 
                if len(rsnode.children) > 0:
                    cnode.children.append(rsnode.children[k])
            # utilizado para pegar o ultimo ponteiro restante 
            # pois tem t+1 ponteiros 
            if len(rsnode.children) > 0:
                cnode.children.append(rsnode.children.pop())
            new = cnode
            # exclusão da chave a ser deletada do nó pai
            x.keys.pop(i)
            # tira a referencia do nó x pai para o filho a esquerda 
            # da chave deletada
            x.children.pop(j)
        else:
            lsnode = x.children[j]
            lsnode.keys.append(x.keys[j])
            for i in range(len(cnode.keys)):
                lsnode.keys.append(cnode.keys[i])
                if len(lsnode.children) > 0:
                    lsnode.children.append(cnode.children[i])
            if len(lsnode.children) > 0:
                lsnode.children.append(cnode.children.pop())
            new = lsnode
            x.keys.pop(j)
            x.children.pop(i)

        # caso seja a raiz e está esteja vazia substitui
        if x == self.root and len(x.keys) == 0:
            self.root = new

    def delete_sibling(self, x, i, j):
        # coloca chave a ser deletada no nó de c
        cnode = x.children[i]
        if i < j:
            # seta o nó rs como sendo o irmão tera valores alterados
            rsnode = x.children[j]
            # descendo a chave a ser deletada no nó filho c
            cnode.keys.append(x.keys[i])
            # subindo a chave do nó irmão rs para o nó pai
            x.keys[i] = rsnode.keys[0]
            if len(rsnode.children) > 0:
                cnode.children.append(rsnode.children[0])
                rsnode.children.pop(0)
            # tiramos chave que subio do nó irmão rs
            rsnode.keys.pop(0)
        else:
            lsnode = x.children[j]
            cnode.keys.insert(0, x.keys[i - 1])
            x.keys[i - 1] = lsnode.keys.pop()
            if len(lsnode.children) > 0:
                cnode.children.insert(0, lsnode.children.pop())
                    
    def print_tree(self, x, level=0):
        print(f'Level {level}', end=": ")

        for i in x.keys:
            print(i, end=" ")

        print()
        level += 1

        if len(x.children) > 0:
            for i in x.children:
                self.print_tree(i, level)

    
def delete_example():
    first_leaf = Node(True)
    first_leaf.keys = [1, 9]

    second_leaf = Node(True)
    second_leaf.keys = [17, 19, 21]

    third_leaf = Node(True)
    third_leaf.keys = [23, 25, 27]

    fourth_leaf = Node(True)
    fourth_leaf.keys = [31, 32, 39]

    fifth_leaf = Node(True)
    fifth_leaf.keys = [41, 47, 50]

    sixth_leaf = Node(True)
    sixth_leaf.keys = [56, 60]

    seventh_leaf = Node(True)
    seventh_leaf.keys = [72, 90]

    root_left_child = Node()
    root_left_child.keys = [15, 22, 30]
    root_left_child.children.append(first_leaf)
    root_left_child.children.append(second_leaf)
    root_left_child.children.append(third_leaf)
    root_left_child.children.append(fourth_leaf)

    root_right_child = Node()
    root_right_child.keys = [55, 63]
    root_right_child.children.append(fifth_leaf)
    root_right_child.children.append(sixth_leaf)
    root_right_child.children.append(seventh_leaf)

    root = Node()
    root.keys = [40]
    root.children.append(root_left_child)
    root.children.append(root_right_child)

    B = BTree(3)
    B.root = root
    print('\n--- Original B-Tree ---\n')
    B.print_tree(B.root)

    print('\n--- Case 1: DELETED 21 ---\n')
    B.delete(B.root, 21)
    B.print_tree(B.root)

    print('\n--- Case 2a: DELETED 30 ---\n')
    B.delete(B.root, 30)
    B.print_tree(B.root)

    print('\n--- Case 2b: DELETED 27 ---\n')
    B.delete(B.root, 27)
    B.print_tree(B.root)

    print('\n--- Case 2c: DELETED 22 ---\n')
    B.delete(B.root, 22)
    B.print_tree(B.root)

    print('\n--- Case 3b: DELETED 17 ---\n')
    B.delete(B.root, 17)
    B.print_tree(B.root)

    print('\n--- Case 3a: DELETED 9 ---\n')
    B.delete(B.root, 9)
    B.print_tree(B.root)


def insert_and_search_example():
    B = BTree(3)
    
    for i in range(10):
        B.insert(i)

        B.print_tree(B.root)
        print()

    keys_to_search_for = [2, 9, 11, 4]
    for key in keys_to_search_for:
        if B.search(key) is not None:
            print(f'{key} is in the tree')
        else:
            print(f'{key} is NOT in the tree')


def main():
    print('\n--- INSERT & SEARCH ---\n')
    insert_and_search_example()

    #ðdelete_example()

main()