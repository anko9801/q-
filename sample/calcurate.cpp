#include "../q++.hpp"
#include <iostream>

using namespace std;

void CCNOT(int control1, int control2, int target) {
	printf("ccx q[%d], q[%d], q[%d];\n", control1, control2, target);
}

void CNOT(int control, int target) {
	printf("cx q[%d], q[%d];\n", control, target);
}
void HalfAdder(Qubits *qubits, int a, int b, int c){
	qubits->CCNOT(a, b, c);
	qubits->CNOT (b, a);
}

void FullAdder(Qubits *qbits, int a, int b, int c0, int c1) {
	qbits->CCNOT(a, b, c1);
	qbits->CNOT (a, b);
	qbits->CCNOT(b, c0, c1);
	qbits->CNOT (b, c0);
	/*CCNOT(a, b, c1);
	CNOT (a, b);
	CCNOT(b, c0, c1);
	CNOT (b, c0); */
}

int Add(int bit, int a, int b) {
	Qubits qbits (b << bit | a, 3*bit+1);
	for (int i = 0;i < bit;i++) {
		FullAdder(&qbits, i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	res >>= 2 * bit;
	return res;
}

void HalfSuber(Qubits *qubits, int a, int b, int c){
	qubits->CNOT (b, a);
	qubits->CCNOT(a, b, c);
}

void FullSubtractor(Qubits *qbits, int a, int b, int c0, int c1) {
	qbits->CCNOT(a, b, c1);
	qbits->CNOT (a, b);
	qbits->CCNOT(b, c0, c1);
	qbits->CNOT (b, c0);
	/*CNOT (b, c0);
	CCNOT(b, c0, c1);
	CNOT (a, b);
	CCNOT(a, b, c1);*/
}

int Sub_a(int bit, int a, int b) {
	int rev_a = 0;
	for (int i = 0;i < bit;i++) {
		rev_a |= (a & (1 << i)) >> i;
	}
	Qubits qbits = Qubits(b << bit | rev_a, 3*bit+1);

	for (int i = 0;i < bit;i++) {
		FullSubtractor(&qbits, i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	res >>= 2 * bit;
	res = a+1-res;
	return res;
}

void CarryGateR(Qubits *q, int a, int b, int c, int d) {
	q->CCNOT(c, b, a);
	q->CNOT(c, b);
	q->CCNOT(d, b, a);
}

void CarryGateL(Qubits *q, int a, int b, int c, int d) {
	q->CCNOT(d, b, a);
	q->CNOT(c, b);
	q->CCNOT(c, b, a);
}

void SumGate(Qubits *q, int a, int b, int c) {
	q->CNOT(b, a);
	q->CNOT(c, a);
}

void add_3bit(Qubits *q) {
	// a 0:2 b 3:5 c 6:9
	for (int i = 0;i < 3;i++) {
		CarryGateR(q, 6+i, i, 3+i, 7+i);
	}
	q->CNOT(2, 5);
	SumGate(q, 8, 2, 5);
	for (int i = 1;i >= 0;i--) {
		CarryGateL(q, 6+i, i, 3+i, 7+i);
		SumGate(q, 6+i, i, 3+i);
	}
}

void sub_3bit(Qubits *q) {
	for (int i = 0;i < 3;i++) {
		CarryGateL(q, 6+i, i, 3+i, 7+i);
	}
}

int add(int a, int b) {
	//printf("        %s\n", btoS(10, a | b << 3).c_str());
	Qubits q = Qubits(a | b << 3, 10);
	add_3bit(&q);
	int ans = q.M_all();
	//ans >>= 6;
	return ans;
}

void FullSuber(Qubits *q, int a, int b, int c0, int c1) {
	
	/*qbits->CNOT (b, c0);
	qbits->CCNOT(b, c0, c1);
	qbits->CNOT (a, b);
	qbits->CCNOT(a, b, c1);*/
}

int Sub(int bit, int a, int b) {
	Qubits qbits (a << 2*bit | b, bit*3+1);
	//Qubits qbits (a, 4);
	for (int i = 0;i < bit;i++) {
		FullSuber(&qbits, i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	return res;
}

int main() {
	/*
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 3);
		HalfAdder(&x, 1, 0, 2);
		int add = x.M_all();
		HalfSuber(&x, 1, 0, 2);
		int sub = x.M_all();
		printf("%s => %s => %s\n", btoS(x.bits, i).c_str(), btoS(x.bits, add).c_str(), btoS(x.bits, sub).c_str());
	}
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 4);
		FullAdder(&x, 0, 1, 2, 3);
		int after_add = x.M_all();
		FullSuber(&x, 0, 1, 2, 3);
		int after_sub = x.M_all();
		printf("%s => %s => %s\n", btoS(x.bits, i).c_str(), btoS(x.bits, after_add).c_str(), btoS(x.bits, after_sub).c_str());
	}*/

	int a = 1;
	int b = 1;
	int c = 0;
	printf("1\n1\n");
	for (int i = 0;i < 10;i++) {
		c = a;
		a = Add(5, a, b);
		b = c;
		printf("%d\n", a);
	}

	/*
	printf("Add\n");
	for (int a = 0; a < 16; a++) {
		for (int b = 0; b < 16; b++) {
			printf("%d + %d = %d\n", a, b, Add(4, a, b));
		}
	}
	printf("Sub\n");
	for (int a = 0; a < 16; a++) {
		for (int b = 0; b < 16; b++) {
			printf("%d - %d = %d\n", a, b, Sub_a(4, a, b));
		}
	}*/
	/*
	for (int a = 0;a < 16;a++) {
		printf("%x - %x = %s\n", a, 0, btoS(4, Sub(1, a, 0)).c_str());
	}*/
	return 0;
}
