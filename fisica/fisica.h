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

#include "../base/vetores.h"
#include "../base/auxiliar.h"
#include "../graficos/graficos.h"

/*--------------- M A C R O S ---------------*/

#define G 6.67e-11 //Constante gravitacional

#define SIZE_X_FIS 1e5						 //Tamanho da horizontal da tela
#define SIZE_Y_FIS (SIZE_X_FIS * SIZE_RATIO) //Tamanho da vertical da tela

#define MAX_PROJ 100	//Número máximo de projéteis
#define MAX_NAVES 2		//Número de naves
#define MAX_PLANETAS 1  //Número de planetas
#define MAX_BOOSTERS 10 //Número máximo de boosters

#define RAIO_PROJS 1000
#define RAIO_NAVES 1000
#define RAIO_BOOSTER 1000
#define MAX_HP 10 //Número máximo de pontos de vida (as naves começam com este valor) (por enquanto é uma constante)

#define MAX_OBJ                                         \
	{                                                   \
		MAX_NAVES, MAX_PLANETAS, MAX_PROJ, MAX_BOOSTERS \
	}
//Array contendo o número máximo de cada tipo de objeto
//MAX_OBJ[NAVE] := MAX_NAVES e assim por diante

/*--------------- E S T R U T U R A S ---------------*/

/* Struct Objeto possui uma massa, posição, velocidade e raio. 
 * Ele é um objeto genérico que deve estar dentro de outros structs importantes
 * do programa, como as naves, projetéis e planetas.
 */
typedef struct
{
	double m; //massa
	double r; //raio
	vet2D p;  //posição
	vet2D v;  //velocidade
	Sprite s; //sprite
} Objeto;

/* Struct Projetil possui um objeto e um tempoRestante na tela.
 * A cada quadro, o tempo retante é decrementado de dt e ao chegar em zero,
 * devemos exclui-lo do array de projéteis.
 */
typedef struct
{
	Objeto o;
	double tempoRestante; //O tempo de vida restante do projétil (ele some quando o tempo vai a zero)
	int dano;			  //A quantidade de pontos de vida que o projétil tira se acertar
} Projetil;

/* Struct Booster.
 * Esta struct é "dupla", pois ao mesmo tempo que ela é um objeto na tela (antes de uma nave colidir com o
 * booster), também é uma propriedade de cada nave.
 * 
 * Boosters aparecem aleatoriamente na tela e podem ser "pegados" pelas naves para adicionar algum
 * efeito adicional à jogatina, como tiros mais rápidos, escudo, etc.
 * 
 * Ao mesmo tempo, a nave possui uma struct boosterAtual que é utilizada para saber qual é o estado atual
 * da nave. Ou seja: como serão seus tiros? qual a cadência deles? são a vida atual da nave? entre outras
 * várias perguntas.
 * 
 * Atualmente, um booster conta com as seguintes propriedades:
 * 		Vida adicional: quando a nave toma um tiro, perde o que tem na vida adicional (se ela não for 0)
 * 		Cadência: a cadência de tiros da nave é determinada por este valor
 * 		Projetil: o booster tem um projétil dentro de si, determinando o dano da nave
 * 		
 */
typedef struct
{
	string nome; //Nome do booster
	Objeto o;
	double tempoRestanteTela; //Tempo restante do booster na tela, ao chegar em 0, removemos ele do array
	double tempoRestanteNave; //Tempo restante do booster após a nave pegá-lo
	int vidaAdicional;
	int cadencia;
	Projetil proj;
} Booster;

/* Struct Nave contém um objeto próprio e um nome que é o nome da nave.
 *
 */
typedef struct
{
	string nome; //Nome da nave
	Objeto o;
	int HP;				  //Pontos de vida
	Booster boosterAtual; //Booster atual da nave (por padrão é o booster padrão)
} Nave;

/* Struct Planeta, por enquanto, possui apenas um objeto
 */
typedef struct
{
	Objeto o;
} Planeta;

/* Defines que facilitam na hora de acessar um campo dos objetos que estão dentro
 * das structs. Basta fazer naves[0].vel para obter a velocidade da nave 0.
 * Os defines funcionam também caso tenhamos um ponteiro para um struct.
 * Basta fazer ponteiro->vel
 */
#define vel o.v	// Macro para a velocidade de um objeto
#define mass o.m   // Macro para a massa de um objeto
#define pos o.p	// Macro para a posição de um objeto
#define radius o.r // Macro para o raio de um objeto
#define spr o.s // Macro para o sprite de um objeto

/* Um enum com os tipos de objetos possíveis.
 * Serve para fazermos referência a qual dos três arrays estamos falando.
 * Por exemplo, uma função x com parâmetros um Objeto e um TipoObj. Dessa forma, com uma só função,
 * nós podemos fazer o mesmo tipo de tarefa aplicada a qualquer um dos três tipos de objeto.
 */
typedef enum
{
	NAVE,
	PLANETA,
	PROJETIL,
	BOOSTER,
	NUM_TIPO_OBJ
} TipoObj;

/*--------------- V A R I Á V E I S   G L O B A I S ---------------*/

extern int tot_obj[NUM_TIPO_OBJ]; //O número de objetos de cada tipo

/* Armazenaremos todos os objetos na tela através de arrays globais,
 * um para cada tipo: Nave, Planeta e Projetil.
 * Os tamanhos desses arrays estão definidos no começo desse arquivo e podem ser
 * editados hard-coded (quem sabe futuramente em tempo de execução)
 */
extern Nave naves[MAX_NAVES];		   //O array que contém as duas naves dos jogadores
extern Planeta planetas[MAX_PLANETAS]; //O array que contém o planeta central
extern Projetil projs[MAX_PROJ];	   //O array que contém os projéteis que estão atualmente na tela
extern Booster boosters[MAX_BOOSTERS]; //O array que contém os booster que estão atualmente na tela

#define TERRA planetas[0] //Como só há um planeta, vamos chamá-lo de TERRA

//Intervalo de tempo da simulacao, lido no arquivo principal. Por padrão, é 0.001
extern double dt;

//Tempo restante de simulação;
extern double tRestante;

/*--------------- F U N Ç Õ E S ---------------*/

//Função que inicializa a biblioteca
void inicializaFisica();

/* FUNÇÕES QUE UTILIZAM APENAS OBJETOS */

//Dados dois objetos, retorna a força gravitacional exercida entre o1 e o2.
//O vetor retornado deve ser aplicado em o1.
vet2D Forca(Objeto o1, Objeto o2);

//Dado um objeto e uma forca, incrementa a velocidade desse objeto
void IncVel(vet2D F, Objeto *o);

//Dado um objeto e uma velocidade, incrementa a posicao desse objeto
void IncPos(Objeto *o);

//Dados dois objetos, retorna a distância entre eles
double DistanciaEntre(Objeto o1, Objeto o2);

/* FUNÇÕES DE INTERFACE ENTRE NAVES, PLANETAS E PROJETEIS E SEUS OBJETOS */

//Dados um tipo de objeto e um índice do vetor onde esse objeto se encontra a função retorna um apontador para este.
//A função verifica se o índice está dentro dos limites
Objeto *GetObjeto(TipoObj tipo, int indice);

//Dados um tipo de objeto, um índice do vetor referente ao tipo desse objeto e o objeto em si,
//a função sobrescreve o array no índice i com o objeto o.
//A função verifica se o índice está dentro dos limites
void SetObjeto(TipoObj tipo, int indice, Objeto o);

//Função booleana que diz se um objeto o1 e um o2 são o mesmo (i.e., se estão alocados na mesma memória)
Bool ObjetoDuplicado(Objeto o1, Objeto o2);

//Função booleana que diz se dois objetos possuem as mesmas caracteristicas
Bool ObjetoIgual(Objeto o1, Objeto o2);

/* FUNÇÕES QUE ITERAM SOBRE TODOS OS OBJETOS EM JOGO */

//Recebe um objeto e calcula a força gerada sobre ele pelos outros objetos.
vet2D CalculaForcaSobre(Objeto o);

//Dado um objeto, atualiza suas propriedades
void AtualizaObjeto(Objeto *o);

//Atualiza todos os objetos (Naves, Projéteis e Planetas)
void AtualizaObjetos();

//Rotaciona o sprite de um objeto baseado em sua velocidade
void giraObjetoVel(Objeto *o);

/* FUNÇÕES QUE ITERAM SOBRE OS PROJÉTEIS*/

//Dado um projétil, reduz seu tempo de vida de acordo com dt
void ReduzTempoProj(Projetil *p);

//Método responsável por reduz o tempo de vida de todos os projéteis
void ReduzTempoProjs();

//Dados um projétil, retorna TRUE se ele está morto e FALSE caso contrário
Bool VerificaSeProjMorreu(Projetil p);

//Dado um índice que representa uma posição do array de projéteis
//a função remove este projétil.
void RemoveProj(int index);

/* FUNÇÕES SOBRE COLISÃO */
//Função responsável por checar todas as possíveis colisões entre todos os objetos em jogo
//Se um projétil colide com um planeta ou outro projétil, ele apenas desaparece
//Se um projétil colide com uma nave, a nave deve perder pontos de vida
//Se uma nave colide com um planeta ou com outra nave, ela explode (pontos de vida vão a zero)
void ChecaTodasColisoes();

//Dado um objeto o, verifica se ele colidiu com qualquer outro objeto do cenário
Bool ChecaColisaoComTodos(Objeto o);

//Dados dois objetos, retorna TRUE se eles colidiram e FALSE caso contrário.
Bool ChecaColisaoEntre(Objeto o1, Objeto o2);

/* FUNÇÕES PARA VIDA DAS NAVES */
//Dados uma nave n e um valor de pontos de vida, decrementa a vida de n em valor
void DecrementaVida(Nave *n, int valor);

//Dados uma nave n e um valor de pontos de vida, incrementa a vida de n em valor
void IncrementaVida(Nave *n, int valor);

//Dada uma nave n, a função reduz seus pontos de vida a 0
void Destroi(Nave *n);

//Checa e retorna se a nave n ainda está viva
Bool EstaViva(Nave n);

//Checa se todas as naves estão vivas
Bool TodasEstaoVivas();

/* OUTRAS FUNÇÕES */

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

//Esta função dá free em todas as alocações de memória relacionadas aos quatro arrays da biblioteca física
void freeFisica();

#endif
