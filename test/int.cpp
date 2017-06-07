#include <typeinfo>
#include "emp-sh2pc/semihonest.h"
#include <emp-tool.h>

template<typename Op, typename Op2>
void test_int(EmpParty party, int range1 = 1<<25, int range2 = 1<<25, int runs = 100) {
	PRG prg(fix_key);
	for(int i = 0; i < runs; ++i) {
		long long ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		ia %= range1;
		ib %= range2;
		while( Op()(int(ia), int(ib)) != Op()(int(ia), int(ib)) ) {
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			ia %= range1;
			ib %= range2;
		}
	
		Integer a(32, ia, ALICE); 
		Integer b(32, ib, BOB);

		Integer res = Op2()(a,b);

		if (res.reveal<int>(PUBLIC) != Op()(int(ia),int(ib))) {
			cout << ia <<"\t"<<ib<<"\t"<<Op()(int(ia), int(ib))<<"\t"<<res.reveal<int>(PUBLIC)<<endl<<flush;
		}
		assert(res.reveal<int>(PUBLIC) == Op()(int(ia),int(ib)));
	}
	cout << typeid(Op2).name()<<"\t\t\tDONE"<<endl;
}

void scratch_pad() {
	Integer a(32, 9, ALICE);
	cout << "HW "<<a.hamming_weight().reveal<string>(PUBLIC)<<endl;
	cout << "LZ "<<a.leading_zeros().reveal<string>(PUBLIC)<<endl;
	cout << local_gc->gid<<endl;
}
int int_main(int port, EmpParty party) {
	NetIO * io = new NetIO(party==ALICE ? nullptr : SERVER_IP, port);

	setup_semi_honest(io, party, toBlock((char)party));

//	scratch_pad();return 0;
	test_int<std::plus<int>, std::plus<Integer>>(party);
	test_int<std::minus<int>, std::minus<Integer>>(party);
	test_int<std::multiplies<int>, std::multiplies<Integer>>(party);
	test_int<std::divides<int>, std::divides<Integer>>(party);
	test_int<std::modulus<int>, std::modulus<Integer>>(party);

	test_int<std::bit_and<int>, std::bit_and<Integer>>(party);
	test_int<std::bit_or<int>, std::bit_or<Integer>>(party);
	test_int<std::bit_xor<int>, std::bit_xor<Integer>>(party);

	delete io;

    std::cout << "int passed" << std::endl;
    return 0;
}



#ifndef _MSC_VER // not visual studio 
int main(int argc, char** argv) {
    int port;
    int party;
    if (argc > 1)
    {
        parse_party_and_port(argv, argc, &party, &port);
        int_main(port, (EmpParty)party);

    }
    else
    {
#ifdef THREADING
        auto thrd = std::thread([&]() {
            int_main(1212, ALICE);
        });

        int_main(1212, BOB);

        thrd.join();
#else
        std::cout << "expecting 2 command line args, party and port" << std::endl;
#endif
    }
}
#endif