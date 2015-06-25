#ifndef BUZON_H
#define BUZON_H

// System
#include <sys/ipc.h>
#include <sys/msg.h>
// Errors
#include <errno.h>
#include <stdio.h>
//String C
#include <string.h>

// Definiciones
#define KEY 0xB22214		// Llave del buzón
#define PERMISSIONS 0600	// Permisos de lectura y escritura del dueño del proceso
#define MAX_CHAR 64

/**
* @class Buzon
* @brief Proporciona una interfaz para utilizar buzones del S.O
* @author Kevin Delgado Sandí
* @version 2.0
*/
class Buzon {
public:
	Buzon();

	/** @brief Método que se encarga de enviar al buzón un mensaje.
	 * @param mensaje Cadena de texto a enviar por el buzón
	 * @param cantidad LLevará el número de etiquetas del archivo procesado
	 * @param type Identificador con el que se enviará el mensaje, por defecto se utiliza 1
	 * @param flag Se pondrán las banderas para enviar el mensaje.
	 * El valor por defecto es {@code IPC_NOWAIT}, en el cual envia el mensaje y continua procesando,
	 * 0 es de la manera tradicional, en la que espera hasta que el receptor obtenga el mensaje para continuar
	 * @return 0 si la operación se relizó exitosamente, -1 en caso contrario. */
	int enviar(const char* mensaje, const int cantidad, const long type = 1, const int flag = IPC_NOWAIT);

	/** @brief Método que se encarga de recibir mensajes del buzón.
	 * @param mensaje Cadena de texto donde se pasará el mensaje
	 * @param tamanio Longitud máxima de la cadena de texto
	 * @param type Identificador con el que se recibirá el mensaj.
	 * @param flag Se pondrán las banderas para enviar el mensaje.
	 * El valor por defecto es {@code IPC_NOWAIT}, en el cual envia el mensaje y continua procesando,
	 * 0 es de la manera tradicional, en la que espera hasta que el receptor obtenga el mensaje para continuar
	 * @return 0 si la operación se relizó exitosamente, -1 en caso contrario */
	int recibir(char* mensaje, const int tamanio, int& cantidad, const long type = 1, const int flag = IPC_NOWAIT);

	/** @brief Método que hace el trabajo del destructor.
	 *	Se debe hacer así, llamándolo explicitamente, ya que llamar al destructor
	 * puede ocasionar errores al tratar de llamarlo en más de 2 ocasiones,
	 * como frecuentemente ocurre en esta clase.
	 * @return 0 si la operación se relizó exitosamente, -1 en caso contrario */
	int destroy();

private:
	int id; // Identificador del buzon

	struct msgbuf {
		long mtype;
		char mensaje[MAX_CHAR]; // valor máximo de el mensaje es de 4056 bytes
		int cantidad;
	};
};
#endif // BUZON_H
