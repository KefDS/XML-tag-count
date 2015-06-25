#ifndef SEMAFORO_H
#define SEMAFORO_H

// System
#include <sys/ipc.h>
#include <sys/sem.h>
//Errors
#include <errno.h>
#include <stdio.h>

// Definiciones
#define KEY 0xB22214		// Valor de la llave del recurso
#define PERMISSIONS 0600	//Esta configuración de permisos de lectura & escritura a el dueño de los procesos

/**
* @class Semaforo
* @brief Proporciona una interfaz para utilizar semáforos
* @author Kevin Delgado Sandí
* @version 1.0
*/
class Semaforo {
public:
	Semaforo(int valorInicial = 0);

	/** @brief Método que indica a los demás dependientes del semáforo puede realizar su labores
	 * @sa wait()
	 * @return 0 si la operación se relizó exitosamente, -1 en caso contrario */
	int signal();

	/** @brief Método que funciona para detenerse hasta que otro proceso o hijo realice el método signal
	 * @sa signal()
	 * @return 0 si la operación se relizó exitosamente, -1 en caso contrario */
	int wait();

	/** @brief Método que hace el trabajo del destructor.
	 *	Se debe hacer así, llamándolo explicitamente, ya que puede ocasionar errores al tratar de llamarlo en más de 2 ocasiones,
	 *	como frecuentemente ocurre en esta clase
	 * @return 0 si la operación se relizó exitosamente, -1 en caso contrario */
	int destroy(); //Funciona como destructor, ya que se debe invocar explícitamente para no cuasar errores

private:
	int id;
};
#endif // SEMAFORO_H
