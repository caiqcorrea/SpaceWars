/* Esta é uma biblioteca genérica para vetores 2D.
 * A princípio, ela pode ser utilizada para quaisquer fins.
 */

#include "vetores.h"

#include <math.h>

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

double anguloX(vet2D v)
{
	double a = acos(v.x / norma(v));
	if (v.y >= 0)
		return a;
	else
		return 2 * M_PI - a;
}

double anguloY(vet2D v)
{
	double a = acos(escalar(v, J_VET) / norma(v));
	if (v.x >= 0)
		return a;
	else
		return 2 * M_PI - a;
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

vet2D multiplicaPorMatriz(mat2D M, vet2D v)
{
	return soma(mult(v.x, M.v1), mult(v.y, M.v2));
}

vet2D rotaciona(vet2D x, double angulo)
{
	mat2D R = {{cos(angulo), -sin(angulo)}, {sin(angulo), cos(angulo)}};
	return multiplicaPorMatriz(R, x);
}
