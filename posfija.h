#ifndef PILA_H
#define PILA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

typedef struct _nodo NODO;

struct _nodo{
    bool operador;
    char caracter;
    double valor;
    int prioridad;
    NODO* siguiente;
};

struct _pila{
    NODO* head;
    NODO* tail;
    int num;
};

typedef struct _pila PILA;
typedef struct _pila COLA;

PILA* crear_pila();
NODO* crear_nodo(int prioridad, char caracter, double valor, bool operador);
bool push(PILA* pila, int prioridad, char caracter, double valor, bool operador);
bool pop(PILA* pila);
bool encolar(COLA* cola, int prioridad, char caracter, double valor, bool operador);
bool es_vacia(PILA *pila);
bool vaciar(PILA *pila);
void eliminar_pila(PILA *pila);
int determinar_prioridad(char caracter);
void imprimir_cola(COLA* cola);
void evaluar_cadena(COLA* cola);
bool guardar_cadena(COLA* cola);

#endif