#include "Semaforo.h"

Semaforo::Semaforo(int valorInicial) {
	// Necesario para un 4to argumento
	union semun {
		int              val;    /* Value for SETVAL */
		struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
		unsigned short  *array;  /* Array for GETALL, SETALL */
		struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
	};

	union semun X;
	X.val = valorInicial;

	id = semget(KEY, 1, IPC_CREAT | PERMISSIONS); //System call, que se encarga de crear un nuevo semáforo
	semctl(id, 0, SETVAL, X); // Modifica el valor inicial del semáforo
	if(-1 == id){
		perror("Error en la creacion del semaforo");
	}
}

int Semaforo::signal() {
	struct sembuf oper[] = {0,1,0};
	semop(id, oper, 1);
	if(-1 == id){
		perror("Error en Signal");
		return -1;
	}
	return 0;
}

int Semaforo::wait() {
	struct sembuf oper[] = {0,-1,0};
	semop(id, oper, 1);
	if(-1 == id){
		perror("Error en Wait");
		return -1;
	}
	return 0;
}

int Semaforo::destroy() {
	return semctl(id, 1, IPC_RMID);
}

