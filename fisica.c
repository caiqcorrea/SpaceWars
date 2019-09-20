/* BIBLIOTECA FÍSICA PARA O JOGO SPACE WARS
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

#include "fisica.h"

/*--------------- I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S ---------------*/

vet2D Forca(Objeto o1, Objeto o2)
{
	vet2D aux = sub(o1.p, o2.p);
	if(norma(aux) == 0) return NULL_VET;

	return mult(-G * o1.m * o2.m / pow(norma(aux), 2), versor(aux));
}

void IncVel(vet2D F, Objeto *o)
{
	o->v = soma(o->v, mult((dt / (o->m)), F));
}

void IncPos(Objeto *o)
{
	o->p = soma(o->p, mult(dt, o->v));

	//Casos em que o objeto sai da tela: trazemos ele de volta do lado oposto
	while (o->p.x > SIZE_X / 2)
		o->p.x -= SIZE_X;

	while (o->p.x < -SIZE_X / 2)
		o->p.x += SIZE_X;

	while (o->p.y > SIZE_Y / 2)
		o->p.y -= SIZE_Y;

	while (o->p.y < -SIZE_Y / 2)
		o->p.y += SIZE_Y;
}

vet2D CalculaForcaSobre(Objeto o)
{
	vet2D F = NULL_VET;
	int i;
	TipoObj tipo;
	
	for(tipo = 0 ; tipo < NUM_TIPO_OBJ ; tipo++)
		for (i = 0; i < tot_obj[tipo]; i++)
			F = soma(F, Forca(o, naves[i].o));

	return F;
}

double distanciaEntre(Objeto o1, Objeto o2)
{
	return norma(sub(o1.p, o2.p));
}

Bool checaColisaoEntre(Objeto o1, Objeto o2)
{
	return (distanciaEntre(o1, o2) < RAIO_COLISAO);
}

Objeto *getObjeto(TipoObj tipo, int indice)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else switch (tipo)
	{
	case PLANETA:
		return &(planetas[indice].o);
			
	case NAVE:
		return &(naves[indice].o);
			
	case PROJETIL:
		return &(projs[indice].o);
			
	default:
		throwException("getObjeto", "tipo nao identificado", var_type_undefined_exception);
	}
	
	return NULL; //Só para o compilador não reclamar
}
void setObjeto(TipoObj tipo, int indice, Objeto o)
{
	if (!(0 <= indice && indice < tot_obj[tipo]))
		throwException("getObjeto", "indexOutOfRangeException", index_out_of_range_exception);

	else switch (tipo)
	{
	case PLANETA:
		planetas[indice].o = o;
		break;

	case NAVE:
		naves[indice].o = o;
		break;

	case PROJETIL:
		projs[indice].o = o;
		break;

	default:
		throwException("getObjeto", "tipo nao identificado", var_type_undefined_exception);
	}
}

void AtualizaObjeto(Objeto *o)
{
	IncPos(o);
	IncVel(CalculaForcaSobre(*o), o);
}

void AtualizaObjetos()
{
	int i;
	//Planetas não precisam ser atualizados (pelo menos na versão atual)
	TipoObj tipo;
	//Planetas não precisam ser atualizados (pelo menos na versão atual)
	for(tipo = 0 ; tipo < NUM_TIPO_OBJ ; tipo++)
		for (i = 0; i < tot_obj[tipo]; i++)
			if(tipo != PLANETA) AtualizaObjeto(getObjeto(tipo, i));
}

void ReduzTempoProj(Projetil *p)
{
	p->tempoRestante -= dt;
}

/* Essa função foi criada para facilitar a edição de código caso queiramos, no futuro,
 * mudar a forma de reduzir o tempo dos projéteis (como reduzir apenas de alguns e de outros não)
 */
void ReduzTempoProjs()
{
	int i;
	for (i = 0; i < tot_obj[PROJETIL]; i++)
		ReduzTempoProj(&(projs[i]));
}

Bool VerificaSeProjMorreu(Projetil p)
{
	return p.tempoRestante <= 0;
}

/* ESTA FUNÇÃO RECEBERÁ ATUALIZAÇÕES NO FUTURO:
 *		Remover o projétil caso ele morreu (fazer uma função remove de array)
 *		Checar colisão (EP2)
 */
Bool AtualizaJogo()
{
	//int i, j;
	//Primeiro atualizamos a posição e velocidade de todos os objetos
	AtualizaObjetos();
	//Depois, devemos reduzir o tempo de todos os projéteis
	ReduzTempoProjs();
	//Verificamos se algum projétil sumiu e removemos-o se sim
	/*for (i = 0; i < tot_projs; i++)
	{
		if (VerificaSeProjMorreu(projs[i]))
		{ //Remove do array (FAZER FUTURAMENTE UMA FUNÇÃO À PARTE)
			if (i != tot_projs - 1)
			{
				for (j = i; j < tot_projs - 1; j++)
					projs[j] = projs[j + 1];
			}
			tot_projs--;
		}
	}*/
	//VERIFICAR COLISÃO (EP2)

	tRestante -= dt;

	return (tRestante > 0);
}

void freeAll(){
	int i;
	for(i = 0; i < tot_obj[NAVE]; i++)
		freeSafe(naves[i].nome);
}
