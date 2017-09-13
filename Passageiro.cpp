/*
 * Passageiro.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno opaaaaaaaaaa 
 */

#include "include/Passageiro.h"
#include "include/Carro.h"
#include "include/Parque.h"
#include <unistd.h>
#include <algorithm> 
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
using namespace std;


#define MAX_NUM_VOLTAS 1

int Passageiro::highEle = 0;

vector<int> Passageiro::turn(Carro::CAPACIDADE);	

Passageiro::Passageiro(int id, Carro *c) {
	this->id = id;
	this->carro = c;
}

Passageiro::~Passageiro() {
}

void Passageiro::entraNoCarro() {
	// Protocolo de entrada do Algoritmo da Padaria
	// Incrementa o numero de passageiros no carro (use a funcao fetch_add) 
	// passo 1

	int /*n = Carro::CAPACIDADE,*/i=0, j;
	int lock[Parque::numPessoas];

	while (Carro::numPassageiros < Carro::CAPACIDADE) {

		lock[id] = 1;
		//cerr << "lock!! TRUE " << Carro::numPassageiros << endl;
		//int highEle = 0;
		int max;
		for (max = 0 ; max < Carro::CAPACIDADE ; max++);
	    {
	        if (turn[max] > highEle)
	            highEle = turn[max];
	    }
	    cerr << "highEle: " << highEle << endl;
		turn[id] = highEle + 1;
		cerr << "turn[" << id << "] = " << turn[id] << endl;
		lock[id] = 0;
		
		for (int j = 0; j < Carro::CAPACIDADE ; j++)
		{
			//if(j != i) {

			while(lock[j]){}
			//cerr << "lock!! PASSEI" << Carro::numPassageiros << endl;
			while (turn[j] != 0 && (turn[j] < turn[id] || turn[id] == turn[j] && j < id)){}
		}


		Carro::numPassageiros.fetch_add(1, std::memory_order_seq_cst);
		cerr << id << " entrando no carro" << endl;	

	}
	cerr << "pulei para ca!! " << Carro::numPassageiros << endl;

}

void Passageiro::esperaVoltaAcabar() {
	 while (!Carro::voltaAcabou) {
		cerr << "Esperando volta acabar" << endl;
		usleep(1000);
	 }
}

void Passageiro::saiDoCarro() {
	// Protocolo de saida do Algoritmo da Padaria
	turn[id] = 0;
	// Decrementa o numero de passageiros no carro (use a funcao fetch_add)
	cerr << id << " saindo do carro" << endl;
	Carro::numPassageiros.fetch_sub(1, std::memory_order_seq_cst);
}

void Passageiro::passeiaPeloParque() {
	usleep(1000);
	cerr << id << " passeando pelo parque" << endl;
}

bool Passageiro::parqueFechado() {
	if (carro->getNVoltas() < MAX_NUM_VOLTAS)
		return false;

	return true;
}

void Passageiro::run() {
	while (!parqueFechado()) {
		entraNoCarro(); // protocolo de entrada
		cerr << "eh nois" << endl;
		esperaVoltaAcabar();

		saiDoCarro(); // protocolo de saida

		passeiaPeloParque(); // secao nao critica
	}
	Parque::numPessoas--;
	// decrementa o numero de pessoas no parque
}

