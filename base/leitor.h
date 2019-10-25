/* Arquivo header do Leitor de arquivos 
 *
 */

#ifndef _LEITOR_H
#define _LEITOR_H

#include "../base/auxiliar.h"

//Esta função deve ser chamada antes de qualquer chamada de método da biblioteca
//Devemos passar um nome de arquivo para o leitor
void initLeitor(string nome);

//Esta função deve ser chamada após a utilização da biblioteca, para limpar a memória
void disposeLeitor();

//Esta função simplesmente imprime no sdtdout o token lido atualmente
void imprimeAtual();

//Lê e retorna o próximo token do arquivo
string proxLeitura();

//Retorna o último token lido, mas não lê o próximo
string getLeitura();

//Retorna true se duas s1 é igual ao último token lido e false caso contrário
Bool strigual(string s1);

#endif