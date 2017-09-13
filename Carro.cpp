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
std::atomic_flag Carro::lock = ATOMIC_FLAG_INIT;

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

	while (Carro::numPassageiros > 0 && Parque::numPessoas.load(std::memory_order_relaxed) > 0) {
		//cerr << "Espera esvaziar" << endl;
		//usleep(1000); 
	}
}

int Carro::getNVoltas() {

	//cerr << "Pegando a volta" << endl;
	return Carro::voltas;
}


void Carro::run() {

	while (Parque::numPessoas.load(std::memory_order_relaxed) > 0) {
		//cerr << "opa" <<endl;
		esperaEncher();
		while (Carro::lock.test_and_set()) {/*SKIP*/}
		Carro::lock.clear();
		daUmaVolta();
		while (Carro::lock.test_and_set()) {}
		Carro::lock.clear();
		esperaEsvaziar();
		Carro::voltaAcabou = false;
		//cerr << "Run do carro" << endl;
		Carro::voltas++;
	}
		cerr << getNVoltas() << endl;
}
