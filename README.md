# clustering-web-documents

Exemplo de execução
https://www.youtube.com/watch?v=xviE-Darn48&feature=youtu.be

## Execução

O data set precisa estar na pasta files/

g++ main.cpp -std=c++11 -DSOL -DLIST; ./a.out


-DSOL e -DLIST são opcionais

-DSOL exibe a quantidade de páginas por cluster
-DLIST exibe o grafo que representa o documento (ajuda para buscar os documentos)


**Para salvar os resultados basta usar o comando**

g++ main.cpp -std=c++11 -DSOL -DLIST; ./a.out > out.txt

**Para fins de teste!**
Não criamos argumentos para mudar a quantidade do K e do tamanho máximo de nós dos grafos, mas para fazer isso basta alterar as constantes K e G, respectivamente, no main.cpp.