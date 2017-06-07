#ifndef SEMIHONEST_EVA_H__
#define SEMIHONEST_EVA_H__
#include <emp-tool.h>
#include <emp-ot.h>
#include <iostream>

template<typename IO>
void eval_feed(Backend* be, EmpParty party, block * label, const bool*, int length);
template<typename IO>
void eval_reveal(Backend* be, bool* clear, EmpParty party, const block * label, int length);

template<typename IO>
class SemiHonestEva: public Backend { public:
	IO* io = nullptr;
	SHOTIterated<IO>* ot;
	HalfGateEva<IO> * gc;
	SemiHonestEva(IO *io, HalfGateEva<IO> * gc, const block& seed): Backend(BOB) {
		this->io = io;
		ot = new SHOTIterated<IO>(io, false, seed);
		this->gc = gc;	
		Feed_internal = eval_feed<IO>;
		Reveal_internal = eval_reveal<IO>;
	}
	~SemiHonestEva() {
		delete ot;
	}
};


template<typename IO>
void eval_feed(Backend* be, EmpParty party, block * label, const bool* b, int length) {
	SemiHonestEva<IO> * backend = (SemiHonestEva<IO>*)(be);
	if(party == ALICE) {
		backend->io->recv_block(label, length);

        //std::cout << osuCrypto::IoStream::lock << "recving " << label[0] << std::endl << osuCrypto::IoStream::unlock;

	} else {
		backend->ot->recv_cot(label, b, length);
	}
}

template<typename IO>
void eval_reveal(Backend* be, bool * b, EmpParty party, const block * label, int length) {
	SemiHonestEva<IO> * backend = (SemiHonestEva<IO>*)(be);
	block tmp;
	for (int i = 0; i < length; ++i) {
		if(isOne(&label[i]))
			b[i] = true;
		else if (isZero(&label[i]))
			b[i] = false;
		else {
			if (party == BOB || party == PUBLIC) {
				backend->io->recv_block(&tmp, 1);
				b[i] = !(block_cmp(&tmp, &label[i], 1));
			} else if (party == ALICE) {
				backend->io->send_block(&label[i], 1);
				b[i] = false;
			}
		}
	}
	if(party == PUBLIC)
		backend->io->send_data(b, length);
}
#endif// GARBLE_CIRCUIT_SEMIHONEST_H__