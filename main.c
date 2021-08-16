#include "posfija.h"

void guardar_CadenaPosfija(COLA *Posfija){
        char answer;
      printf("\n¿Deseas guardar la cadena posfija en un archivo?[s/n]\n");
      answer = getchar();
      if(answer == 's'){
        printf("Guardando cadena posfija");
        escribir_Cola_Archivo(Posfija);
      }
      else{
        printf("Cadena posfija no fue guardada");
      }
}

int main(int argc, char *argv[])
{
    //Asignamos el arreglo que guardará las cadena y le asignamos valor
    char infija[150];
    int commsCase = 0;
    // Para los casos en que se debe leer desde el archivo
    if (argc == 1)
    {
        // Abrimos flujo para tomar los datos del archivo
        FILE* flujo_infija = fopen("infija.txt", "r");

        if (flujo_infija == NULL)
        {
            printf("No se encontró el archivo infija.txt\n");
            return 0;
        }

        //Usamos un while para leer los caracteres
        int i = 0;
        while(feof(flujo_infija) == 0)
        {
            fscanf(flujo_infija, "%c", &infija[i]);
            i++;
        }

        printf("La cadena infija en el archivo es: %s\n", infija);
        fclose(flujo_infija);

    }    
    else{ //Para los casos en que la cadena se paso por linea de comandos
        commsCase = 1;
        strcpy(infija, "8+1");
    }

    //Para los casos en que al cadena esta en un archivo de texto
    
    //Auxiliares para la creacion de los nodos
    char operador;
    int prioridad;
    double valor = 0;
    //auxiliar para guardar cadena
	  PILA* Operadores = crear_pila();
    COLA* Posfija = crear_pila();

    //Comenzamos a recorrer la cadena
    for (int i = 0; i < strlen(infija); i++)
    {
        // Si hay espacio en la cadena original, estos no se procesan
        while(isspace(infija[i]))
            i++;
        // Los numeros pasan directo a la cadena posfija
        if (isdigit(infija[i]))
        {
            // Para tomar todos los dígitos que componen un numero
            while (isdigit(infija[i]))
            {
                // Se acomoda segun su posicion en notacion 10
                valor = valor*10;
                // Obtenemos el valor numérico del caracter a las unidades
                valor += (infija[i] - '0');
                i++;
            }
            //Una vez con el número completo, ese valor se guarda en el nodo
            encolar(Posfija, 0, '0', valor, false);
            //Devolvemos el valor a 0
            valor = 0;
        }
        
        // Se vuelven a eliminar los espacio de la cadena
        while(isspace(infija[i]))
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

    evaluar_cadena(Posfija);
    guardar_CadenaPosfija(Posfija);
	  eliminar_pila(Operadores);
	  eliminar_pila(Posfija);

    return 0;
}

