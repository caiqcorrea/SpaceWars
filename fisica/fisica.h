/* INTERFACE PARA A BIBLIOTECA FÍSICA PARA O JOGO SPACE WARS
 *
 * Esta biblioteca foi criada especificamente para o jogo Space Wars,
 * Sua base são as três structs: Nave, Projétil e Planeta, que devem
 * se encontrar instanciadas dentro dos arrays: naves, projs e planetas.
 * 
 * Decidimos implementar uma struct mais básica chamada de Objeto, que existirá
 * dentro de cada uma das structs mais importantes. Ela possuirá inicialmente
 * os atributos: massa, posição e velocidade, mas a ideia é que todos os
 * atributos compartilhados pelos três structs estejam dentro de Objeto (como,
 * por exemplo, o raio).
 * Dessa forma, podemos criar funções genéricas que recebem apenas Objetos ao
 * invés de três (ou mais) funções que fazem a mesma coisa, mas uma para cada
 * tipo de struct.
 * 
 * Todos os calculos feitos são baseados na Teoria da Gravitação Universal de
 * Isaac Newton.
 * https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
 * É sabido que determinar posições exatas de orbitas complexas (como as do jogo) 
 * é um problema em aberto na matemática e na física, então o professor nos
 * recomendou usar o método numérico de Runge-Kutta.
 * 
 * Convenções de nomenclatura:
 *		Array: Variáveis do tipo v[MAX]
 *		Vetor: Estrutura vet2D, representa um vetor em R²
 */
 
 #ifndef _FISICA_H
 #define _FISICA_H
 
 #include "vetores.h"
 #include "../auxiliar.h"

/*--------------- M A C R O S ---------------*/

#define G 6.67e-11		  //Constante gravitacional
#define RAIO_COLISAO 10.0 //Raio utilizado para checar colisao com a nave
#define SIZE_X 1e10		  //Tamanho da horizontal da tela
#define SIZE_Y 1e10		  //Tamanho da vertical da tela
#define MAX_PROJ 100	  //Número máximo de projéteis
#define MAX_NAVES 2		  //Número de naves
#define MAX_PLANETAS 1	//Número de planetas

#define NUM_TIPO_OBJ 3 //Número de tipos de objeto
#define MAX_OBJ { MAX_PLANETAS, MAX_NAVES, MAX_PROJ } //Array contendo o número máximo de cada tipo de objeto

/*--------------- E S T R U T U R A S ---------------*/

/* Struct Objeto possui uma massa, posição e velocidade. 
 * Ele é um objeto genérico que deve estar dentro de outros structs importantes
 * do programa, como as naves, projetéis e planetas.
 */
typedef struct
{
	double m; //massa
	vet2D p;  //posição
	vet2D v;  //velocidade
} Objeto;

/* Struct Nave contém um objeto próprio e um nome que é o nome da nave.
 *
 */
typedef struct
{
	Objeto o;
	string nome;
} Nave;

/* Struct Projetil possui um objeto e um tempoRestante na tela.
 * A cada quadro, o tempo retante é decrementado de dt e ao chegar em zero,
 * devemos exclui-lo do array de projéteis.
 */
typedef struct
{
	Objeto o;
	double tempoRestante;
} Projetil;

/* Struct Planeta possui, além de um objeto, um raio que deve
 * ser usado para cálculos de gravidade e para checar colisões
 */
typedef struct
{
	Objeto o;
	double raio;
} Planeta;

/* Defines que facilitam na hora de acessar um campo dos objetos que estão dentro
 * das structs. Basta fazer naves[0].vel para obter a velocidade da nave 0.
 * Os defines funcionam também caso tenhamos um ponteiro para um struct.
 * Basta fazer ponteiro->vel
 */
#define vel o.v  // Macro para a velocidade de um objeto
#define mass o.m // Macro para a massa de um objeto
#define pos o.p  // Macro para a posição de um objeto

/* Um enum com os tipos de objetos possíveis.
 * Serve para fazermos referência a qual dos três arrays estamos falando.
 * Por exemplo, uma função x com parâmetros um Objeto e um TipoObj. Dessa forma, com uma só função,
 * nós podemos fazer o mesmo tipo de tarefa aplicada a qualquer um dos três tipos de objeto.
 */
typedef enum
{
	NAVE,
	PLANETA,
	PROJETIL
} TipoObj;

/*--------------- V A R I Á V E I S   G L O B A I S ---------------*/

extern int tot_obj[NUM_TIPO_OBJ];		//O número de objetos de cada tipo

/* Armazenaremos todos os objetos na tela através de arrays globais,
 * um para cada tipo: Nave, Planeta e Projetil.
 * Os tamanhos desses arrays estão definidos no começo desse arquivo e podem ser
 * editados hard-coded (quem sabe futuramente em tempo de execução)
 */
extern Nave naves[MAX_NAVES];			//O array que contém as duas naves dos jogadores
extern Planeta planetas[MAX_PLANETAS]; //O array que contém o planeta central
extern Projetil projs[MAX_PROJ];		//O que array que contém os projéteis que estão atualmente na tela

#define TERRA planetas[0] //Como só há um planeta, vamos chamá-lo de TERRA

//Intervalo de tempo da simulacao, lido no arquivo principal. Por padrão, é 0.01
extern double dt;

//Tempo restante de simulação;
extern double tRestante;

/*--------------- F U N Ç Õ E S ---------------*/

/* FUNÇÕES QUE UTILIZAM APENAS OBJETOS */

//Dados dois objetos, retorna a força gravitacional exercida entre o1 e o2.
//O vetor retornado deve ser aplicado em o1.
vet2D Forca(Objeto o1, Objeto o2);

//Dado um objeto e uma forca, incrementa a velocidade desse objeto
void IncVel(vet2D F, Objeto *o);

//Dado um objeto e uma velocidade, incrementa a posicao desse objeto
void IncPos(Objeto *o);

//Dados dois objetos, retorna a distância entre eles
double distanciaEntre(Objeto o1, Objeto o2);

//Dados dois objetos, retorna TRUE se eles colidiram e FALSE caso contrário.
Bool checaColisaoEntre(Objeto o1, Objeto o2);

/* FUNÇÕES DE INTERFACE ENTRE NAVES, PLANETAS E PROJETEIS E SEUS OBJETOS */

//Dados um tipo de objeto e um índice do vetor onde esse objeto se encontra a função retorna um apontador para este.
//A função verifica se o índice está dentro dos limites
Objeto *getObjeto(TipoObj tipo, int indice);

//Dados um tipo de objeto, um índice do vetor referente ao tipo desse objeto e o objeto em si,
//a função sobrescreve o array no índice i com o objeto o.
//A função verifica se o índice está dentro dos limites
void setObjeto(TipoObj tipo, int indice, Objeto o);

/* FUNÇÕES QUE ITERAM SOBRE TODOS OS OBJETOS EM JOGO */

//Recebe um objeto e calcula a força gerada sobre ele pelos outros objetos.
vet2D CalculaForcaSobre(Objeto o);

//Dado um objeto, atualiza suas propriedades
void AtualizaObjeto(Objeto *o);

//Atualiza todos os objetos (Naves, Projéteis e Planetas)
void AtualizaObjetos();

/* FUNÇÕES QUE ITERAM SOBRE OS PROJÉTEIS*/

//Dado um projétil, reduz seu tempo de vida de acordo com dt
void ReduzTempoProj(Projetil *p);

//Método responsável por reduz o tempo de vida de todos os projéteis
void ReduzTempoProjs();

//Dados um projétil, retorna TRUE se ele está morto e FALSE caso contrário
Bool VerificaSeProjMorreu(Projetil p);

/* OUTRAS FUNÇÕES 	*/

//Atualiza o estado atual do jogo
//Este método deve:
//	Atualizar as posições de todos os objetos em tela
//	Verificar colisão entre dois quaisquer objetos
//		Se isso acontecer, deve tomar a ação correta dependendo do tipo de objeto que colidiu
//	Verifica se os projéteis ainda estão com tempo de vida positivo
//		Se isso não acontece, deve remover o projétil do array
//
//A função devolve TRUE se o jogo continua e FALSE se o jogo terminou por qualquer motivo
//(Pode ser que mudemos o retorno no futuro)
Bool AtualizaJogo();

//Esta função dá free em todas as alocações de memória relacionadas aos três arrays da biblioteca física
void freeAll();

#endif
