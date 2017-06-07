#include "semihonest_gen.h"
#include "semihonest_eva.h"

template<typename IO>
static void setup_semi_honest(IO* io, EmpParty party, const block& seed) {
	if(party == ALICE) {
		local_gc = new HalfGateGen<IO>(io, seed ^ OneBlock);
		local_backend = new SemiHonestGen<IO>(io, (HalfGateGen<IO, RTCktOpt::on>*)local_gc, seed);
	} else {
		local_gc = new HalfGateEva<IO>(io);
		local_backend = new SemiHonestEva<IO>(io, (HalfGateEva<IO, RTCktOpt::on>*)local_gc, seed);
	}
}

