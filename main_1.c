#define FIFO_FILE "MYFIFO"
#define COMPRIMENTO_MENSAGEM 40
#define TAMANHO_NOME 20
#define USER_LEVEL_PERMISSIONS 0666
#define TEM_ERRO -1
#define NUM_CARROS 9
#define MASTER_FIFO "MASTER.in"
#define COMPRIMENTO_MENSAGEM_MASTER 9

/******************************************************************************
 *                                                                            *
 *	Includes                                                                  *
 *                                                                            *
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

/******************************************************************************
 *                                                                            *
 *	Estrutura de Dados                                                        *
 *                                                                            *
 ******************************************************************************/
typedef struct StatusCarro{
	short id;					/* Numero de carro */
    int volta;              /* Volta na pista */
    int setor;
    float tempo;
    int combustivel;
    int pneus;
    char tipo_pneu[5];          /* tipo pneu */
} Status;

/******************************************************************************
 *                                                                            *
 *  FUNCAO imprimeResultados                                                  *
 *                                                                            *
 *  Imprime resultados da corrida                                             *
 *                                                                            *
 ******************************************************************************/
void imprimeResultados(Status tab[])
{
    printf ("\tID\tVolta\tTempo\n");
    for (int t = 0; t < NUM_CARROS; t++) 
    {
        printf ("\t%d\t%d\t%f\n", tab[t].id, tab[t].volta, tab[t].tempo);
    }
}

/******************************************************************************
 *                                                                            *
 *  FUNCAO escreveTC                                                          *
 *                                                                            *
 *  imprime uma mensagem do FIFO cujo nome e definido pelo numero do carro    *
 *  Parametro num_carro - Numero que identifica o automovel                   *
 *                                                                            *
 ******************************************************************************/
void escreveTC(int fd, const char *mensagem) {
	//printf("escreveTC\n");
	ssize_t bytes_written = write(fd, mensagem, strlen(mensagem));
	if (bytes_written == -1) {
		perror("Erro ao escrever no fifo");
		close(fd);
		exit(EXIT_FAILURE);
	}
}
/******************************************************************************
 *                                                                            *
 *  FUNCAO LerMensagens                                                       *
 *                                                                            *
 *  Esta funcao le, espera e imprime uma mensagem do FIFO                     *
 *  Parametro sessao - Numero que identifica o automovel*
 *                                                                            *
 ******************************************************************************/
void * lerTelemetria(void * carro)
{	

	int fd = 0; 						/* Identificador do FIFO */
	char mensagem[COMPRIMENTO_MENSAGEM + 1]; 	/* Mensagem lida do FIFO */
	char filename[TAMANHO_NOME];   /* Nome da FIFO */
	int tamanho_msg = 0;				/* Tamanho em bytes da mensagem */
    int fd2 = 0;
	Status *car_stat = (Status* ) carro;

	/* Cria o nome da FIFO "FIFO" + numero do carro + ".out." */
    /* Mensagens serÃ£o transmitidas neste pipe */
	sprintf (filename, "CARRO%d.out", car_stat->id+1);

	/* Cria FIFO  */
	if (mkfifo(filename, USER_LEVEL_PERMISSIONS) == TEM_ERRO)
	{
		printf("Erro ao criar FIFO %s\n",filename);
	    pthread_exit(NULL);
	}
	else
	{
		//printf("FIFO %s criado com sucesso!\n",filename);
	}

	/* Abre FIFO e associa a um identificar "fd", permissoes so escrita */
    /* Mensagens serÃ£o recebidas neste pipe */
	fd2 = open(filename, O_WRONLY);

	if(fd2 == TEM_ERRO)
	{
		printf("Erro no OPEN de %s \n",filename);
		pthread_exit(NULL);
	}
	else
	{
		//printf("FIFO %s aberto com sucesso!\n",filename);
	}
    
	/* Cria o nome da FIFO "FIFO" + numero do carro + ".in." */
	sprintf (filename, "CARRO%d.in", car_stat->id+1);

	/* Cria FIFO  */
	if (mkfifo(filename, USER_LEVEL_PERMISSIONS) == TEM_ERRO)
	{
		printf("Erro ao criar FIFO %s\n",filename);
	    pthread_exit(NULL);
	}
	else
	{
		//printf("FIFO %s criado com sucesso!\n",filename);
	}
		

	/* Abre FIFO e associa a um identificar "fd", permissoes so leitura */
	fd = open(filename, O_RDONLY);

	if(fd == TEM_ERRO)
	{
		printf("Erro no OPEN de %s \n",filename);
		pthread_exit(NULL);
	}
    else
    {
        //printf("FIFO %s aberto com sucesso!\n",filename);
    }

    while (car_stat->volta != 10)
    {
        /* Ler mensagem da fifo */
        tamanho_msg = read(fd, mensagem, COMPRIMENTO_MENSAGEM);
		

        if (tamanho_msg == -1) {
            perror("Erro a ler do FIFO");
            close(fd);
            pthread_exit(NULL);
        }

        mensagem[tamanho_msg] = '\0';
        
        /* Imprime mensagem no ecra */
		if (tamanho_msg != 0)
		{
        	/* Retira palavra e numero da mensagem */
        	sscanf(mensagem, "| %hd | %d | %d | %f | %d | %d | %s |", 
            &car_stat->id, &car_stat->volta, &car_stat->setor, 
            &car_stat->tempo, &car_stat->combustivel, &car_stat->pneus, car_stat->tipo_pneu);
			
			printf("Msg: %s\n", mensagem);   
			if ((car_stat->pneus < 10) || (car_stat->combustivel < 10))
			{
				escreveTC(fd2, "BOX");
			}
		}
        
    }
    
    printf("Corrida terminou para carro numero %d\n", car_stat->id);
    
	/* Fecha FIFOs */
    close(fd2);
	close(fd);
    
    /* Apaga as FIFOS */
    sprintf (filename, "CARRO%d.out", car_stat->id+1);
    unlink(filename);
    sprintf (filename, "CARRO%d.in ", car_stat->id+1);
    unlink(filename);    
    pthread_exit(NULL);
    
}

/******************************************************************************
 *                                                                            *
 *  FUNCAO initTabela                                                         *
 *                                                                            *
 *  Inicializa o conteudo da tabela                                           *
 *                                                                            *
 ******************************************************************************/
void initTabela (Status tab[])
{
    for (int t = 0; t < NUM_CARROS; t++)
    {
        tab[t].id = t;
        tab[t].volta = 0;
        tab[t].setor = 0;
        tab[t].tempo = 0.0;
        tab[t].combustivel = 0;
        tab[t].pneus = 0;
        memset(tab[t].tipo_pneu, 0, 5);
    }
}

/******************************************************************************
 *                                                                            *
 *  FUNCAO threadMaster                                                       *
 *                                                                            *
 *  Inicio do programa                                                        *
 *                                                                            *
 ******************************************************************************/
void * threadMaster(void * threads_arg) {
    pthread_t* threads = (pthread_t*) threads_arg;
    char filename[TAMANHO_NOME];
    int fd;
    char mensagem[COMPRIMENTO_MENSAGEM_MASTER+1];

    /* Cria FIFO  */
	if (mkfifo(MASTER_FIFO, USER_LEVEL_PERMISSIONS) == TEM_ERRO)
	{
		printf("Erro ao criar FIFO %s\n",MASTER_FIFO);
	    pthread_exit(NULL);
	}
	else
	{
		//printf("FIFO %s criado com sucesso!\n",filename);
	}		

	/* Abre FIFO e associa a um identificar "fd", permissoes so leitura */
	fd = open(MASTER_FIFO, O_RDONLY);
    
    while(1) {
        int tamanho_msg = read(fd, mensagem, COMPRIMENTO_MENSAGEM_MASTER);
        if (tamanho_msg == -1) {
            perror("Erro a ler do FIFO");
            close(fd);
            pthread_exit(NULL);
        }

        mensagem[tamanho_msg] = '\0';
        
		if (tamanho_msg != 0) {
            int car_id = 0;
            sscanf(mensagem, "despiste%d", &car_id);
            printf("Carro %d despistou-se\n", car_id);            
            pthread_cancel(threads[car_id-1]);

            /* Apaga as FIFOS */
            sprintf (filename, "CARRO%d.out", car_id);
            unlink(filename);
            sprintf (filename, "CARRO%d.in ", car_id);
            unlink(filename);  
        }
    }
}

/******************************************************************************
 *                                                                            *
 *  FUNCOES chamadas por sinais                                               *
 *                                                                            *
 ******************************************************************************/
void terminaCorridaAbruptamente() {
    char filename[TAMANHO_NOME];

    printf("Fim da corrida por tempo esgotado\n");
    for (int i=0; i < NUM_CARROS; i++) {
        /* Apaga as FIFOS */
        sprintf (filename, "CARRO%d.out", i+1);
        unlink(filename);
        sprintf (filename, "CARRO%d.in ", i+1);
        unlink(filename);  
    }
    unlink(MASTER_FIFO);
    exit(0);
}

void bandeiraAmarelaLevantada(){
    printf("Bandeira amarela levantada\n");
}

void bandeiraAmarelaRecolhida(){
    printf("Bandeira amarela recolhida\n");
}


/******************************************************************************
 *                                                                            *
 *  FUNCAO main                                                               *
 *                                                                            *
 *  Inicio do programa                                                        *
 *                                                                            *
 ******************************************************************************/
int main(int argc, char **argv)
{
	pthread_t threads[NUM_CARROS]; /*id threads para cada carro*/
	Status tabela[NUM_CARROS]; /* Tabela com status dos carros */
    char comando[COMPRIMENTO_MENSAGEM]; /* Comando para ativar o cliente*/
    // buffer[1000];
    int rc = 0;
    setvbuf(stdout, NULL, _IOLBF, 1024);

    if (argc >= 2)
	{
		sprintf (comando, "bash testscript.sh %d %s 2>&1", getpid(), argv[1]);
        FILE *pipe = popen(comando, "r");
        if (pipe == NULL)
        {
            printf("Erro no comando system erro %d\n", errno);
            perror ("Erro :");
            
        }
        else
        {
            //printf ("System nao deu erro\n");
        }

	 /* Para debug no pandora   
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) 
        {
            printf("%s", buffer);
        }
     */
    }

    signal(SIGUSR1, bandeiraAmarelaLevantada);
    signal(SIGUSR2, bandeiraAmarelaRecolhida);
	
     /** LanÃ§ando as threads */
    for (int t = 0; t < NUM_CARROS; t++) 
    {
        initTabela(tabela);
        rc = pthread_create(&threads[t], NULL, lerTelemetria, (void *) &tabela[t]);
        if (rc) 
        {
            fprintf(stderr, "Erro a crear thread %d; erro %d\n", t, rc);
            return 1;
        }
    }

    pthread_t master;
    rc = pthread_create(&master, NULL, threadMaster, (void*) threads);

    signal(SIGALRM, terminaCorridaAbruptamente);
    alarm(15);

    
    for (int t = 0; t < NUM_CARROS; t++) 
    {
        rc = pthread_join(threads[t], NULL);
        if (rc) 
        {
            fprintf(stderr, "Erro no join da thread %d; erro %d\n", t, rc);
            return 1;
        }
    }

    imprimeResultados(tabela);

    pthread_cancel(master);
    unlink(MASTER_FIFO);
    
    /* Espera 1 segundo para garantir que client tambem termina */
    sleep(1);

}

