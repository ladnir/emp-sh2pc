#ifndef SEMIHONEST_GEN_H__
#define SEMIHONEST_GEN_H__
#include <emp-tool/emp-tool.h>
#include <emp-ot/emp-ot.h>
#include <iostream>

template<typename IO>
void gen_feed(Backend* be, EmpParty party, block * label, const bool*, int length);
template<typename IO>
void gen_reveal(Backend* be, bool* clear, EmpParty party, const block * label, int length);

template<typename IO>
class SemiHonestGen: public Backend { public:
	IO* io;
	SHOTIterated<IO> * ot;
	PRG prg;
	HalfGateGen<IO> * gc;
	SemiHonestGen(IO* io, HalfGateGen<IO>* gc, const block& seed)
        : Backend(ALICE)
        , prg(seed)
    {
		this->io = io;
		ot = new SHOTIterated<IO>(io, true, prg.random_block());
		this->gc = gc;	
		Feed_internal = gen_feed<IO>;
		Reveal_internal = gen_reveal<IO>;
	}
	~SemiHonestGen() {
		delete ot;
	}
};

template<typename IO>
void gen_feed(Backend* be, EmpParty party, block * label, const bool* b, int length) {
	SemiHonestGen<IO> * backend = (SemiHonestGen<IO>*)(be);
	if(party == ALICE) {
		backend->prg.random_block(label, length);
        std::vector<block> sendLabels(label, label + length);
		for (int i = 0; i < length; ++i) {
			if(b[i]) sendLabels[i] = xorBlocks(sendLabels[i], backend->gc->delta);

            //std::cout << osuCrypto::IoStream::lock << "sending " << sendLabels[i] << std::endl << osuCrypto::IoStream::unlock;
			//backend->io->send_block(&tosend, 1);
        }
        backend->io->send_block(sendLabels.data(), length);
	} else {
		backend->ot->send_cot(label, backend->gc->delta, length);
	}
}

template<typename IO>
void gen_reveal(Backend* be, bool* b, EmpParty party, const block * label, int length) {
	SemiHonestGen<IO> * backend = (SemiHonestGen<IO>*)(be);
	for (int i = 0; i < length; ++i) {
		if(isOne(&label[i]))
			b[i] = true;
		else if (isZero(&label[i]))
			b[i] = false;
		else {
            //std::vector<block> sendLabels ()
			if (party == BOB || party == PUBLIC) {
				backend->io->send_block(&label[i], 1);
				b[i] = false;
			} else if(party == ALICE) {
				block tmp;
				backend->io->recv_block(&tmp, 1);
				b[i] = !(block_cmp(&tmp, &label[i], 1));
			}
		}
	}
	if(party == PUBLIC)
		backend->io->recv_data(b, length);
}
#endif //SEMIHONEST_GEN_H__