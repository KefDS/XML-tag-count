#include "Buzon.h"

Buzon::Buzon() {
	id = msgget(KEY, IPC_CREAT | PERMISSIONS);
	if(-1 == id){
		perror("Error al inciar el buzón: ");
	}
}

int Buzon::enviar(const char* mensaje, const int cantidad, const long type, const int flag) {
	struct msgbuf st_mensaje;
	st_mensaje.mtype = type;
	strcpy(st_mensaje.mensaje,mensaje);
	st_mensaje.cantidad = cantidad;
	int status = msgsnd(id, &st_mensaje, (sizeof(struct msgbuf)-sizeof(long)), flag);
	if(-1 == status){
		perror("Error al enviar el mensaje: ");
		return -1;
	}
	return 0;
}

int Buzon::recibir(char* mensaje, const int tamano, int& cantidad, const long type, const int flag) {
	struct msgbuf st_mensaje;
	int status = msgrcv(id, &st_mensaje, sizeof(struct msgbuf)-sizeof(long), type, flag);
	if(-1 == status){
		perror("Error al recibir el mensaje");
		return -1;
	}
	else{
		strncpy(mensaje, st_mensaje.mensaje, tamano);
		cantidad = st_mensaje.cantidad;
	}
	return 0;
}

int Buzon::destroy(){
	int status = msgctl(id, IPC_RMID, NULL);
	if(-1 == status){
		perror("Error al eliminar el proceso");
		return -1;
	}
	return 0;
}
