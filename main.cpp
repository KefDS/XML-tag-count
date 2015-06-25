#include "Buzon.h"
#include <iomanip>
#include <iostream>
#include <sys/ipc.h> // IPC operations
#include "LeeXML.h"
#include "Semaforo.h"
#include <string>
#include <sys/shm.h> // Memoria compartida
#include <cstring>
#include <stdlib.h> // exit()
#include <unistd.h> // fork()

#define BUFER_SIZE 100
#define KEY_i 501

using namespace std;

// Funcion Utilitaria
void procesa_xml (const char*,const long type = 1);
void procesa_xml_un_archivo (const char* ruta);

// Buzon para el paso de mensajes
Buzon myMailbox;

int main(int argc, char* argv[]) {

	if (argc <= 1) {
		cout << "Error: No hay direccion del archivo" << endl;
		return -1;
	}

	if (argc >= 3 && !strcmp(argv[1],"-t")) {
		for(int i = 2; i < argc; ++i) {
			/* Los hijos del proceso realizarán el interior del if,
			  *termina en exit(0), para que los hijos no realicen el for */
			if(!fork()) {
				procesa_xml(argv[i],i); // i representará el m_type con que enviará mensajes al buzón
				exit(0);
			}
		}

		// Semáforos para impresor-padre
		Semaforo mySemaphore;
		Semaforo sonSemaphore;

		struct tag {
			char tag_name[MAX_CHAR]; // Nombre de la etiqueta
			int cantidad_tag;		// Cantidad de la etiqueta
		};

		int share_mem_id = shmget(KEY, sizeof(tag)*BUFER_SIZE, 0700 | IPC_CREAT);	// Memoria compartida

		int share_mem_id_i = shmget(KEY_i, sizeof(int), 0700 | IPC_CREAT);				// Memoria compartida
		int* i_share = (int*) shmat( share_mem_id_i, NULL, 0 );
		*(i_share) = 0;

		/* Codigo de hijo impresor */
		if(!fork()) {
			const tag* const area = (tag*) shmat( share_mem_id , NULL, 0 );
			cout << "\nConteo de etiquetas de los archivos en orden alfabetico\n" << endl;
			cout << left << setw(28) << "Nombre de la etiqueta" << "Numero de veces" << endl;
			cout.fill('_'); cout << left << setw(42) << ' ' << '\n' << endl;

			mySemaphore.wait(); // Espera la primera llamada del padre
			while(*(i_share) != -1){
				for(int i = 0; i < *(i_share); ++i){
					cout.fill(' '); cout << left << setw(28) << area[i].tag_name << right << setw(8) << area[i].cantidad_tag << endl;
				}
				sonSemaphore.signal();	// Signal al proceso padre
				mySemaphore.wait();		// Se queda esperando a la proxima estructura a ejecutar
			}

			cout.fill('_'); cout << left << setw(43) << ' ' << endl;
			sonSemaphore.signal();
			exit(0);
		}

		/* Instrucciones que realizará el padre */

		tag tmp;

		// Vectores que se utilizarán para el ordenamiento alfabético de las estructuras
		tag tagVec[argc];
		bool  flagVec[argc];

		for(int i = 2; i < argc; ++i) {
			flagVec[i] = true;
		}

		// Llena los vectores con los primeros mensajes enviados por los hijos
		for(int i = 2; i < argc; ++i) {
			myMailbox.recibir(tmp.tag_name,MAX_CHAR,tmp.cantidad_tag,i,0);
			strcpy(tagVec[i].tag_name,tmp.tag_name);
			tagVec[i].cantidad_tag = tmp.cantidad_tag;
		}

		// Los mensajes de los hijos que tengan "END", indican que terminó su ejecución
		for(int i = 2; i < argc; ++i) {
			if(!strcmp(tagVec[i].tag_name,"END"))
				flagVec[i] = false;
		}

		tag* area = (tag*) shmat(share_mem_id , NULL, 0);
		char bandera = ' ';

		// Extrae la menor etiqueta y la envía a impresión
		bool finish = false;
		do {
			int index_menor_etiqueta = 2;
			for(int i = 3; i < argc; ++i) {
				if(flagVec[i]) {
					int comparacion = strcmp(tagVec[i].tag_name,tagVec[index_menor_etiqueta].tag_name);
					if( comparacion < 0 ) { // tagVec[i].tag_name es menor
						index_menor_etiqueta = i;
					}
					else if(comparacion == 0 && index_menor_etiqueta != i) {
						/* Como hay 2 etiquetas iguales, se pasará a sumar las etiqeutas a alguna de las 2
						 * luego, se pedirá la siguiente etiqueta (buzón) para que no hayan conflictos */
						tagVec[index_menor_etiqueta].cantidad_tag += tagVec[i].cantidad_tag;
						myMailbox.recibir(tmp.tag_name,MAX_CHAR,tmp.cantidad_tag,i,0);
						strcpy(tagVec[i].tag_name,tmp.tag_name);
						if(!strcmp(tagVec[i].tag_name,"END"))
							flagVec[i] = false;
						tagVec[i].cantidad_tag = tmp.cantidad_tag;
					}
				}
			}

			if(bandera != tagVec[index_menor_etiqueta].tag_name[1] && bandera != ' '){
				mySemaphore.signal();
				sonSemaphore.wait();
				*(i_share) = 0;
			}

			// Envía los datos a impresor
			strcpy(area[*i_share].tag_name,tagVec[index_menor_etiqueta].tag_name);
			area[*i_share].cantidad_tag = tagVec[index_menor_etiqueta].cantidad_tag;
			*(i_share) += 1;

			bandera = tagVec[index_menor_etiqueta].tag_name[1];

			// Al sacar el menor del vector, se llama a la siguiente etiqueta
			myMailbox.recibir(tmp.tag_name,MAX_CHAR,tmp.cantidad_tag,index_menor_etiqueta,0);
			strcpy(tagVec[index_menor_etiqueta].tag_name,tmp.tag_name);
			if(!strcmp(tagVec[index_menor_etiqueta].tag_name,"END"))
				flagVec[index_menor_etiqueta] = false;
			tagVec[index_menor_etiqueta].cantidad_tag = tmp.cantidad_tag;

			bool tmp = true;
			for(int i = 2; i < argc; ++i){
				if(flagVec[i])
					tmp = false;
			}

			if(tmp)
				finish = true;

		} while(!finish);

		mySemaphore.signal();
		sonSemaphore.wait(); // Envía el último estructura de etiquetas al impresor

		*(i_share) = -1;
		mySemaphore.signal();
		sonSemaphore.wait();
		shmdt(area);
		shmctl( share_mem_id, IPC_RMID, NULL );
		shmctl( share_mem_id_i, IPC_RMID, NULL );

		mySemaphore.destroy();
		sonSemaphore.destroy();
	}
	else {
		// Caso de un solo archivo
		procesa_xml_un_archivo(argv[1]);
	}
	myMailbox.destroy();
	return 0;
}

void procesa_xml (const char* ruta, const long type) {
	LeeXML myLector(ruta, type, &myMailbox);
	string str = myLector.getNewLine();

	while (!myLector.isEOF()) {
		myLector.addEtiqueta (str);
		str = myLector.getNewLine();
	}
	myLector.sendResults();
}

void procesa_xml_un_archivo (const char* ruta) {
	LeeXML myLector(ruta);
	string str = myLector.getNewLine();

	while (!myLector.isEOF()) {
		myLector.addEtiqueta (str);
		str = myLector.getNewLine();
	}
	myLector.printResults();
}
