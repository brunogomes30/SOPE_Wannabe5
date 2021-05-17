## SOPE T5G3

#### Estrutura do Código


O projeto está dividido em cinco source files: log.c, utils.c, linkedList.c, communication.c e client.c.

##### log

Módulo responsável por efetuar os registos correspondentes aos eventos do __client__.

A função **writeLog** é chamada sempre que for necessário registar a ocorrência de um evento. Recebendo uma **Message** e uma **Operation**, isto é, uma das fases da operação de cada processo/thread a executar, imprime para a saída padrão a linha com a informação correspondente e exigida. As diferentes Operations foram guardadas num enum.

##### utils

Módulo que reune um conjunto de funções úteis para certos momentos do programa.

A função **getRandomNumber** tal como o nome explicita retorna um número aleatório entre os limites inferiores e superiores indicados.  É usada no nosso código em duas situações: em primeiro lugar é através desta função que se determina o intervalo de tempo entre a criação de novas threads, sendo que este intervalo pode variar entre 10000 us e 50000 us; a segunda utilização desta função define o **tskload** do pedido que varia entre 1 e 9. Na realidade existe, também neste ficheiro, uma função **getTaskRandom** que invoca a **getRandomNumber** nos limites exigidos. 

 A função **isNumber** auxilia a deteção de um válido parâmetro de tempo aquando da execução do programa.  
 
A função **FIFOexists** verifica a existência de uma fifo , tal como o nome indica, recorrendo à função access.

A função **createFIFO** cria um fifo com o nome passado como argumento, com todas as permissões. 

A função **deleteFIFO** apaga-a eliminando o nome do ficheiro correspondente e libertando a memória utilizada. 

Estas últimas 3 funções são usadas recorrentemente ao longo do programa, quando trabalhando com diferentes threads.

##### linkedList

Este módulo é uma possível implementação de uma linkedList usada para armazenar ao longo do programa, as threads por ele criadas e a sua informação.

Cada linkedListElement guarda a thread correspondente e o elemento da lista seguinte. 

A lista é inicializada pela função **initLinkedList** que coloca na lista a primeira thread a correr. Sempre que for necessário adicionar um elemento hà lista chama-se a função **addElement** com a nova thread criada. No fim da utilização da linkedList é necessário libertar toda a memória requisitada com a função **freeLinkedList**. Esta estrutura revelou-se útil e necessária para guardar todas as threads em execução, para, no término do programa se possa juntar e terminar todas as threads esperadas. 

##### communication

A quando da criação de uma thread é necessário especificar a sua função a executar, assim, é neste módulo que ela se encontra já que este é responsável por toda a comunicação entre o client e server.

A **thread_func** é responsável pelo ciclo de ações exigidas na interação client-server. Passando como argumento a informação relativa ao _publicFIFO_ e ao id da thread correspondente, numa estrutura auxiliar que denominamos **ClientThreadArgs** o client está apto a inciar a comunicação. 

Inicializa em primeiro lugar a estrutura _Message_ com a informação exigida, para que esta seja passada corretamente ao servidor. Seguidamente, envia esta mensagem pela _publicFIFO_ ao servidor, fazendo um pedido (**IWANT**). Este registo é então efetuado no stdout. Posteriormente, cria-se uma fifo privada onde se aguardará pela resposta do servidor, na função **getServerResponse**. 

Aqui a função readFromFIFO é chamada e retorna 0 quando é possível ler da _privatefifo_ e -1 quando não é. A estratégia nesta função é tentar abrir e ler a resposta _Message_ do ficheiro enquanto o client não der timeout. É também nesta função verificado se a _publicFIFO_ ainda existe pois se não existir é necessário atualizar **serverClosed** para parar de criar threads. Após esta função, temos, então, dois cenários. No primeiro, é necessário verificar o valor com que vem o _tskres_ do server, sendo que no caso de vir a -1, o registo a ser efetuado é de **CLOSD**. No outro, o registo é de **GOTRS**. Note-se que a partir do momento que o valor de uma resposta do server vem a -1, esta é a indicação de que o servidor fechou, assim é atualizada uma variável _serverClosed_ útil para terminar a criação de threads no client. No caso de não ser possível ler da fifo o registo efetuado é de **GAVUP**. No fim de **getServerResponse** elimina-se a fifo privada.

##### client

É neste módulo que se encontra nossa função main. Inicialmente, corremos uma verificação dos argumentos para a execução do programa. Posteriormente, espera-se que seja aberta a fifo pública. Quando esta for aberta, e enquanto o tempo de execução do client ainda não tiver sido ultrapassado, entre cada intervalo de tempo semi-aleatório, entre 10000 us e 50000 us, é criada uma nova thread onde são passado todos os parâmetros e cada thread é adicionada a linkedList previamente inicializada. Quando o tempo termina, todas as fifos são fechadas e as threads também. No fim desalocada toda a memória utilizada. 


#### Contribuição Individual

Beatriz Aguiar: 25%
Bruno Gomes: 25%
Matilde Oliveira: 25%
Rodrigo Tuna: 25%
