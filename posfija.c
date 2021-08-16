#include "posfija.h"
#include <math.h>

PILA* crear_pila()
{
    PILA* nueva_pila = (PILA*) malloc(sizeof(PILA));
    nueva_pila -> head = nueva_pila -> tail = NULL;
    return nueva_pila;
}

NODO* crear_nodo(int prioridad, char caracter, double valor, bool operador)
{
    NODO *nuevo_nodo  = (NODO*) malloc(sizeof(NODO));
    nuevo_nodo -> siguiente = NULL;
    nuevo_nodo -> prioridad = prioridad;
    nuevo_nodo -> caracter = caracter;
    nuevo_nodo -> valor = valor;
    nuevo_nodo -> operador = operador;
    return nuevo_nodo;
}

bool push(PILA *pila, int prioridad, char caracter, double valor, bool operador)
{
    NODO* nuevo = crear_nodo(prioridad, caracter, valor, operador);
    
    if (pila -> num == 0)
    {
        pila -> head = pila -> tail = nuevo;
    }
    else
    {
        nuevo -> siguiente = pila -> head;
        pila -> head = nuevo;
    }
    pila -> num++;
    
    return true;
}

bool pop(PILA *pila)
{
    if (pila -> num == 0) return false;
    
    NODO *temporal = pila -> head;
    
    if (pila -> num == 1)
    {
        pila -> head = pila -> tail = NULL;
    }
    else
        pila -> head = pila -> head -> siguiente;
    
    temporal -> siguiente = NULL; 
    free(temporal);
    pila -> num--;
    
    return true;
}

bool encolar(COLA* cola, int prioridad, char caracter, double valor, bool operador)
{
    NODO* temporal = crear_nodo(prioridad, caracter, valor, operador);

    if (cola -> num == 0)
    {
        cola -> head = cola -> tail = temporal;
        cola -> num++;
    }
    else
    {
        cola -> tail -> siguiente = temporal;
        cola -> tail = temporal;
        cola -> num++;
    }

    return true;

}

bool es_vacia(PILA *pila){
    if(pila->head == NULL && pila->tail == NULL) return true;
    return false;
}

bool vaciar(PILA *pila){
    if(es_vacia(pila)) return false;
    NODO* temporal = pila->head;
    while(temporal != NULL){
        pila->head = temporal->siguiente;
        temporal -> siguiente = NULL;
        free(temporal);
        temporal = pila->head;
    }
    pila->head = pila->tail = NULL;
    pila->num = 0;
    return true;
}

void eliminar_pila(PILA *pila){
    if(pila == NULL) return;
    if(!es_vacia(pila)){
        vaciar(pila);
    }
    free(pila);
    pila = NULL;
}

void imprimir_cola(COLA* cola)
{
    NODO *temporal = cola -> head;

    if (es_vacia(cola))
    {
        printf("NULL");
        return;
    }

    if (cola -> num > 1)
    {
        while(temporal -> siguiente != NULL)
        {
            if (temporal -> operador == false)
            {
                // Si el valor no tiene parte decimal se imprime sólo el entero
                if (fmod(temporal -> valor, 1) == 0)
                    printf("%.0f ", temporal -> valor);
                else
                    printf("%.2f ", temporal -> valor);
            }
            else
                printf("%c ", temporal -> caracter);
            
            temporal = temporal -> siguiente;
        }
    } // El último elemento de una cadena posfija siempre es un operador
    printf("%c\n", temporal -> caracter);
}
void escribir_Cola_Archivo(COLA* cola){
  NODO *temporal = cola -> head;
  FILE* flujo_posfija = fopen("posfija.txt", "w");
        

    if (es_vacia(cola))
    {
        printf("NULL");
        return ;
    }
    if (cola -> num > 1)
    {
        while(temporal -> siguiente != NULL)
        {
            if (temporal -> operador == false)
            {
                // Si el valor no tiene parte decimal se imprime sólo el entero
                if (fmod(temporal -> valor, 1) == 0)
                    fprintf(flujo_posfija,"%.0f ", temporal -> valor);
                else
                    fprintf(flujo_posfija,"%.2f ", temporal -> valor);
            }
            else
                fprintf(flujo_posfija,"%c ", temporal -> caracter);
            
            temporal = temporal -> siguiente;
        }
    } // El último elemento de una cadena posfija siempre es un operador
    fprintf(flujo_posfija,"%c\n", temporal -> caracter);
  
  fclose(flujo_posfija);
}
int determinar_prioridad(char caracter)
{
    int prioridad = 0;
    
    switch (caracter)
    {
        case '(':
            prioridad = 0;
            break;
        case ')':
            prioridad = 0;
            break;
        case '+':
            prioridad = 1;
            break;
        case '-':
            prioridad = 1;
            break;
        case '*':
            prioridad = 2;
            break;
        case '/':
            prioridad = 2;
            break;
        case '^':
            prioridad = 3;
            break;
        default:
            break;
    }

    return prioridad;
}

void evaluar_cadena(COLA* cola)
{   
    // Pila que almacenara los valores
    PILA* evaluacion = crear_pila();
    // Guarda valoor para los operandos
    double operando_auxiliar = 0;

    for (NODO* auxiliar = cola -> head; auxiliar -> siguiente!= NULL; auxiliar = auxiliar -> siguiente)
    {
        // Si es un número se agrega a la pila
        if (auxiliar -> operador == false)
        {
            push(evaluacion, auxiliar -> prioridad, auxiliar -> caracter, auxiliar -> valor, auxiliar -> operador);
        }
        // Si es un operador, se efectua la operacion
        else 
        {
            // Se guarda el valor del segundo operando
            operando_auxiliar = evaluacion -> head -> valor;
            // Se elimina de la pila
            pop(evaluacion);

            // Se cambia el valor del primer operando segun la operacion
            switch (auxiliar -> caracter)
            {
                case '+':
                    evaluacion -> head -> valor += operando_auxiliar;
                    break;
                case '-':
                    evaluacion -> head -> valor -= operando_auxiliar;
                    break;
                case '*':
                    evaluacion -> head -> valor = (evaluacion -> head -> valor)*(operando_auxiliar);
                    break;
                case '/':
                    evaluacion -> head -> valor = (evaluacion -> head -> valor) / (operando_auxiliar);
                    break;
                case '^':
                    evaluacion -> head -> valor = pow(evaluacion -> head -> valor, operando_auxiliar);
                    break;
            }
        }
    }

    // Si el resultado no cuenta con parte decimal se imprime sólo el entero
    if (fmod(evaluacion -> head -> valor, 1) == 0)
        printf("\nLa evaluacion de la cadena posfija es: %.0f\n", evaluacion -> head -> valor);
    else
        printf("\nLa evaluacion de la cadena posfija es: %.2f\n", evaluacion -> head -> valor);

    eliminar_pila(evaluacion);

}