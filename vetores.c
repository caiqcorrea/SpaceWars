/* Esta é uma biblioteca genérica para vetores 2D.
 * A princípio, ela pode ser utilizada para quaisquer fins.
 */

#include "vetores.h"

/* I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S */

double norma(vet2D v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

vet2D mult(double a, vet2D v)
{
	v.x *= a;
	v.y *= a;
	return v;
}

vet2D divide(double a, vet2D v)
{
	v.x /= a;
	v.y /= a;
	return v;
}

vet2D versor(vet2D v)
{
	return divide(norma(v), v);
}

vet2D soma(vet2D v, vet2D w)
{
	v.x += w.x;
	v.y += w.y;
	return v;
}

vet2D sub(vet2D v, vet2D w)
{
	v.x -= w.x;
	v.y -= w.y;
	return v;
}

double escalar(vet2D v, vet2D w)
{
	return v.x * w.x + v.y * w.y;
}

double vetorial(vet2D v, vet2D w)
{
	return v.x * w.y - v.y * w.x;
}

void normaliza(vet2D *v)
{
	*v = versor(*v);
}

vet2D null_vet()
{
	vet2D nulo = {0, 0};
	return nulo;
}

vet2D i_vet()
{
	vet2D i = {1, 0};
	return i;
}

vet2D j_vet()
{
	vet2D j = {0, 1};
	return j;
}
