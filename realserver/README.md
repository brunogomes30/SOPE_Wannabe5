## SOPE T5G3

#### Estrutura do Código


O projeto está dividido em seis source files: log.c, utils.c, linkedList.c, queue.c, communication.c e server.c.

##### log

Módulo responsável por efetuar os registos correspondentes aos eventos do __server__.

A função **writeLog** é chamada sempre que for necessário registar a ocorrência de um evento. Recebendo uma **Message** e uma **Operation**, isto é, uma das fases da operação de cada processo/thread a executar, imprime para a saída padrão a linha com a informação correspondente e exigida. As diferentes Operations foram guardadas num enum.

##### utils

Módulo que reune um conjunto de funções úteis para certos momentos do programa.

A função **isNumber** auxilia a deteção de um válido parâmetro de tempo aquando da execução do programa.  
 
A função **FIFOexists** verifica a existência de uma fifo , tal como o nome indica, recorrendo à função access.

A função **createFIFO** cria um fifo com o nome passado como argumento, com todas as permissões. 

A função **deleteFIFO** apaga-a eliminando o nome do ficheiro correspondente e libertando a memória utilizada. 

Estas últimas 3 funções são usadas recorrentemente ao longo do programa, quando trabalhando com diferentes threads.

##### linkedList

Este módulo é uma possível implementação de uma linkedList usada para armazenar ao longo do programa, as threads por ele criadas e a sua informação.

Cada linkedListElement guarda a thread correspondente e o elemento da lista seguinte. 

A lista é inicializada pela função **initLinkedList** que coloca na lista a primeira thread produtora a correr. Sempre que for necessário adicionar um elemento hà lista chama-se a função **addElement** com a nova thread produtora criada. No fim da utilização da linkedList é necessário libertar toda a memória requisitada com a função **freeLinkedList**. Esta estrutura revelou-se útil e necessária para guardar todas as threads em execução, para, no término do programa se possa juntar e terminar todas as threads esperadas. 

##### queue

Este módulo é uma possível implementação de uma estrutura do tipo FIFO que simula o armazém e guarda os pedidos para dar resposta ao client.

Cada elemento da queue, **Node**, guarda a **Message**. 

A lista é inicializada pela função **initQueue** que inicializa os semaforos responsáveis pelo controlo de elementos no armazém, _empty_ a zeros e _full_ com o número máximo especificado pelo utilizador. Sempre que for necessário adicionar um elemento hà lista chama-se a função **push**, que com recurso ao semáforo _full_, fica em espera até haver espaço no armazém para este novo elemento ser adicionado. A função **pop** retira o primeiro elemento da queue e fica em espera por sua vez se está estiver vazia, com recurso ao semáforo _empty_. A quando da inserção e remoção de um elemento da queue é sempre atualizado o semáforo respetivo, _empty_ e o _full_. 

No fim da utilização da queue é necessário libertar toda a memória requisitada e destruir o uso dos semáforos com a função **destroyQueue**. 

##### communication

A quando da criação de uma thread é necessário especificar a sua função a executar, assim, é neste módulo que ela se encontra já que este é responsável por toda a comunicação entre o client e server.

A **thread_func** é responsável pelo ciclo de ações exigidas na interação client-server pelas threads produtoras do server. Passando como argumento a message recebida vinda do client. Se o server não estiver fechado esta função realiza a _task_ e dá-se writeLog da operação **TSKEX**. Depois adiciona-se esta ao armazém terminando assim a utilização da thread.  

A thread consumidora têm a sua única função **thread_consumer** que enquanto o armazém não estiver vazio e enquanto ainda houver threads produtoras a correr retira uma mensagem do armazém para dar writeLog da mensagem respetiva. Se for impossível escrever na _publicFIFO_, na **writeToFIFO**, significa que o client deu timeout e por isso o registo a ser efetuado é **FAILD**. No caso da _publicFIFO_ ainda existir verifica-se se o resultado da _task_ e se esta não tiver sido executada dá-se o registo **2LATE** pois o server não conseguiu executar o pedido no caso de sucesso dá-se **TSKDN**.

##### server

É neste módulo que se encontra nossa função main. Inicialmente, corremos uma verificação dos argumentos para a execução do programa.

Posteriormente, abre-se a fifo pública, inicializa-se a thread consumidora e a queue de _Messages_. 

Enquanto o servidor não fechar, isto é o sinal de alarm ser enviado porque o tempo deste ultrapassou, são criadas as threads produtoras (que são sucessivamente adicionadas à _linkedList_ para posteriormente se juntarem) que recebem como argumento o pedido vindo do cliente (que é lido da fifo pública) e efetuado o respetivo registo de **RECVD**.

Quando o tempo do servidor termina dá-se unlink da fifo pública, todas as threads produtoras são fechadas, atualizando o sistemas que todas estas terminaram e para que não seja permitido enviar mais pedidos. Dando-se por fim _join_ da thread consumidora quando esta percorrer todos os pedidos ainda em falta. No fim é desalocada toda a memória utilizada e destroídas as fifos, _queue_ e _linkedList_. 

#### Contribuição Individual

Beatriz Aguiar: 25%
Bruno Gomes: 25%
Matilde Oliveira: 25%
Rodrigo Tuna: 25%
