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


#define MAX_NUM_VOLTAS 5

int Passageiro::highEle = 0;

//vector<int> Passageiro::turn(Carro::CAPACIDADE);	

Passageiro::Passageiro(int id, Carro *c, Parque *p) {
	this->id = id;
	this->carro = c;
	this->parque = p;
}

Passageiro::~Passageiro() {
}

void Passageiro::entraNoCarro() {
	// Protocolo de entrada do Algoritmo da Padaria
	// Incrementa o numero de passageiros no carro (use a funcao fetch_add) 
	// passo 1

	int lock[Parque::numPessoas];

	int max=0;
	for (auto &tred : parque->getPassageiros())
    {
        if (tred->turn > max)
            max += tred->turn;
    }
    
	turn = max + 1;
	//cerr << "highEle: " << turn << endl;
	for (auto &tred : parque->getPassageiros())
	{
		if(tred->id != id) {
			while((tred->turn!=0 && ( turn > tred->turn || (turn == tred->turn && id > tred->id) ) ) || (carro->Carro::numPassageiros) >= (Carro::CAPACIDADE) || (carro->voltaAcabou)){}
		}
	}


	Carro::numPassageiros.fetch_add(1, std::memory_order_seq_cst);
	cerr << id << " entrando no carro" << endl;	

	//cerr << "pulei para ca!! " << Carro::numPassageiros << endl;
	turn = 0;

}

void Passageiro::esperaVoltaAcabar() {
	 while (!Carro::voltaAcabou) {
		//cerr << "Esperando volta acabar" << endl;
		usleep(1000);
	 }
}

void Passageiro::saiDoCarro() {
	// Protocolo de saida do Algoritmo da Padaria

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

