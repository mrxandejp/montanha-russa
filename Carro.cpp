/*
 * Carro.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */
#include "include/Carro.h"
#include "include/Parque.h"
#include <iostream>
#include <unistd.h>
using namespace std;


const int Carro::CAPACIDADE = 5;
atomic<int> Carro::numPassageiros = ATOMIC_VAR_INIT(0);
bool Carro::voltaAcabou = false;


Carro::Carro(Parque *p) {
	this->voltas = 0;
	this->parque = p;
}

Carro::~Carro() {

}

void Carro::esperaEncher() {

	while (Carro::numPassageiros < Carro::CAPACIDADE && Carro::voltas < 5) {
		//cerr << "Esperando encher" << endl;
	}
}

void Carro::daUmaVolta() {
	Carro::voltaAcabou = false;
	usleep(1000);
	cerr << "Dando uma volta" << endl;
	Carro::voltaAcabou = true;
}

void Carro::esperaEsvaziar() {

	while (Carro::numPassageiros > 0) {
		cerr << "Espera esvaziar" << endl;
		usleep(1000); 
	}
}

int Carro::getNVoltas() {

	//cerr << "Pegando a volta" << endl;
	return voltas;
}


void Carro::run() {

	while (Parque::numPessoas > 0) {
		//cerr << "opa" <<endl;
		esperaEncher();

		daUmaVolta();

		esperaEsvaziar();

		//cerr << "Run do carro" << endl;
		voltas++;
	}
}
