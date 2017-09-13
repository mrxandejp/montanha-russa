/*
 * Passageiro.h
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#ifndef PASSAGEIRO_H_
#define PASSAGEIRO_H_
#include <vector>
#include <atomic>
using namespace std;

class Carro;
class Parque;

class Passageiro {
public:
	Passageiro(int id, Carro *c, Parque *p);
	virtual ~Passageiro();
	void entraNoCarro();
	void esperaVoltaAcabar();
	void saiDoCarro();
	void passeiaPeloParque();
	bool parqueFechado();

	static int highEle;
	void run();
	static vector<int> turn;
private:
	int id;
	Carro *carro;
	Parque *parque;
};

#endif /* PASSAGEIRO_H_ */
