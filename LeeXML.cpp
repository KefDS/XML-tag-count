#include "LeeXML.h"

LeeXML::LeeXML (const string& ruta, const long type, Buzon* mailbox)
	: mailBox(mailbox)
	, type(type)
{
	entrada.open (ruta.c_str()); // Abre el archivo
}

LeeXML::~LeeXML() {
	entrada.close(); //Cierra el archivo
}

string LeeXML::getNewLine() {
	string strTmp = "";

	// Si no hay llegado al final del archivo
	if (!isEOF()) {
		getline (entrada, strTmp);
	}

	return strTmp;
}

bool LeeXML::isEOF() {
	return entrada.eof();
}

void LeeXML::addEtiqueta (const string& str) {
	int largo = str.length();

	string tag = "";
	int i = 0;

	while (i < largo) { // Analiza toda la cadena
		if (str[i] == '<') {
			if (i+1 < largo && str[i+1] != ' ') { // Sí después de "<" hay un " " no se toma como etiqueta
				int inicio = i;
				bool termina_etiqueta = false;
				int conteo = 1;

				do {
					i++;

					if (str[i] == ' ' || str[i] == '>') {
						tag = str.substr (inicio, conteo); // Tag contiene la etiqueta
						tag += '>';
						termina_etiqueta = true;
					}

					conteo++;
				}
				while (!termina_etiqueta);

				// Inserta la etiqueta en el diccionario
				auto it = m_map.find (tag);

				if (it == m_map.end()) {
					m_map[tag] = 1;
				}
				else {
					m_map[tag]++;
				}
			}
		}

		i++;
	}
}

void LeeXML::printResults() const {
	cout << "Etiquetas con cantidad" << endl;

	for (map<string, int>::const_iterator it = m_map.begin(); it != m_map.end(); it++) {
		cout << it->first << " : " << it->second << endl;
	}
}

void LeeXML::sendResults() {
	char* cstr = new char [MAX_CHAR];
	for (mapTag::const_iterator it = m_map.begin(); it != m_map.end(); ++it) {
		string strTmp = it->first;
		strcpy (cstr, strTmp.c_str());
		mailBox->enviar(cstr,it->second,type);
	}
	//Envía bandera de envío
	strcpy(cstr,"END");
	mailBox->enviar(cstr,501,type);
	delete[] cstr;
}
