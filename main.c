#include "posfija.h"

int main(int argc, char *argv[])
{
    //Iniciamos el arreglo que guaradrá el valor de la cadena infija
    char infija[150];

    // Abrimos flujo para tomar los datos del archivo
    FILE* flujo_infija = fopen("infija.txt", "r");

    if (flujo_infija == NULL)
    {
        printf("No se encontró el archivo %s\n", "infija.txt");
        return 0;
    }

    //Usamos un while para leer los caracteres
    int pos = 0;
    while(feof(flujo_infija) == 0)
    {
        fscanf(flujo_infija, "%c", &infija[pos]);
        pos++;
    }

    printf("La cadena infija en el archivo es: %s\n", infija);
    fclose(flujo_infija);    
    
    //Auxiliares para la creacion de los nodos
    char operador;
    int prioridad;
    //auxiliar para guardar cadena
	PILA* Operadores = crear_pila();
    COLA* Posfija = crear_pila();

    //Comenzamos a recorrer la cadena
    for (int i = 0; i < strlen(infija); i++)
    {
        // Si hay espacio en la cadena original, estos no se procesan
        while(isblank(infija[i]))
            i++;
        // Las variables pasan directo a la cadena posfija
        if (isalnum(infija[i]))
        {
            //Una vez con el número completo, ese valor se guarda en el nodo
            encolar(Posfija, 0, infija[i], 0, false);
            i++;
        }
        
        // Se vuelven a omitir los espacio de la cadena
        while(isblank(infija[i]))
            i++;
        
        // Pasamos a evaluar los operadores

        //Determinamos la prioridad
        operador = infija[i];
        prioridad = determinar_prioridad(operador);

        if (es_vacia(Operadores))
        {
            push(Operadores, prioridad, operador, 0, true);
        }
        // El parentesis de apertura se coloca sin medir prioridad
        else if (operador == '(')
        {
            push(Operadores, prioridad, operador, 0, true);
        }
        // En los parentesis de cierra vaciamos la pila
        else if (operador == ')')
        { // Vaciamos los operadores hasta encontrar un "("
            while (Operadores->head->caracter != '(')
            {
                encolar(Posfija, Operadores->head->prioridad, Operadores->head->caracter, 0, true);
                pop(Operadores);
            }
            // Eliminamos el "("
            pop(Operadores);
        }
        else // Colocamos los casos en los que se validan operadores
        {    // Si tiene mayor prioridad se agrega directamente
            if (prioridad > Operadores->head->prioridad)
            {
                push(Operadores, prioridad, operador, 0, true);
            }
            /* En los casos de mayor o igual prioridad se sacan
                operadores hasta encontrae uno con mayor prioridad*/
            else
            {
                while (prioridad <= Operadores->head->prioridad)
                {
                    encolar(Posfija, Operadores->head->prioridad, Operadores->head->caracter, 0, true);
                    pop(Operadores);
                    // Si la lista se vacia, terminamos el ciclo
                    if (es_vacia(Operadores))
                        break;
                }
                // Una vez fuera los operadores mayores introducimos el nuevo
                push(Operadores, prioridad, operador, 0, true);
            }
        }
    }

    // Se sacan los operadores restantes de la pila
    while (!es_vacia(Operadores))
    {
        encolar(Posfija, Operadores->head->prioridad, Operadores->head->caracter, 0, true);
        pop(Operadores);
    }

    printf("\nLa cadena posfija es: ");
    imprimir_cola(Posfija);

    // El usuario elige acción a realziar con la cadena
    int eleccion = 0;
    printf("\nQue accion quieres realizar?\n(1) Guardar cadena\n(2) Evaluar Cadena\n(3) Guardar y evaluar\n");
    scanf("%d", &eleccion);

    if (eleccion == 1 || eleccion == 3)
        guardar_cadena(Posfija);
    if (eleccion == 2 || eleccion == 3)
        evaluar_cadena(Posfija);
    
	eliminar_pila(Operadores);
	eliminar_pila(Posfija);

    return 0;
}