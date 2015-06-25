#ifndef LEEXML_H
#define LEEXML_H

#include "Buzon.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;

typedef map<string, int> mapTag;

/**
* @class LeeXML
* @brief Clase que contabiliza las etiquetas XML de un archivo.
* @author Kevin Delgado Sandí
* @version 2.0
*/
class LeeXML {

public:

	/** @param ruta Contiene la ruta del archivo a analizar
	 * @param type la clase enviará mensajes al buzon con ese id.
	 * @param mailbox apuntador al buzón donde se enviarán los mensajes.
	 *	Por defecto el apuntador del se inicializa con NULL */
	LeeXML(const string& ruta, const long type = 1, Buzon* mailbox = nullptr);
	
	~LeeXML ();

	/** @brief Toma la siguiente línea del archivo
	 * @return String con la siguiente línea por leer */
	string getNewLine();

	/** @brief Verifíca si es el final del archivo
	 * @return true si es el final del archivo, false en caso contrario */
	bool isEOF();

	/** @brief Inserta o suma en el diccionario la etiqueta
	 * @param str Contiene la cadena en la que se extraerán las etiquetas */
	void addEtiqueta(const string& str);

	/** @brief Imprime los resultados del análisis del archivo .xml */
	void printResults() const;

	/** @brief Imprime los resultados del análisis del archivo .xml */
	void sendResults();

private:
	/** @brief Flujo de entrada */
	ifstream entrada;

	/** @brief Apuntador con el buzón para mandar el mensaje */
	Buzon* mailBox;

	/** @brief Identificador del tipo de mensaje que enviará al buzón */
	const long type;

	/** @brief Diccionario que guardará las etiquetas */
	mapTag m_map;
};
#endif // LEEXML_H
