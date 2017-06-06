#include <emp-tool/emp-tool.h>
#include "emp-sh2pc/semihonest/semihonest.h"

#ifdef _MSC_VER
const string circuit_file_location = EMP_CIRCUIT_PATH;

#else
const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);
#endif

//#define MEM
#define NETWORK
int circuit_file_main(int port, EmpParty party) {
	string file = circuit_file_location+"/AES-non-expanded.txt";//adder_32bit.txt";

    if (std::fstream(file).good() == false)
    {
        std::cout << "failed up open file: " << file << std::endl;
        return 0;
    }


	CircuitFile cf(file.c_str());

	NetIO* io = new NetIO(party==ALICE?nullptr:SERVER_IP, port);

	setup_semi_honest(io, party, toBlock((char)party));

#ifdef MEM
	Integer a(128, 2, ALICE);
	Integer b(128, 3, ALICE);
	Integer c(128, 1, PUBLIC);

	MemIO * memio = new MemIO(cf.table_size());
	if(party == ALICE)
		local_gc = new HalfGateGen<MemIO>(memio);
	else 
		local_gc = new HalfGateEva<MemIO>(memio);
	if(party == ALICE) {		
		double t1 = wallClock();
		for(int i = 0; i < 10000; ++i) {
			memio->clear();
			cf.compute((block*)c.bits, (block*)a.bits, (block*)b.bits);
		}
		cout << wallClock() - t1<< endl;
	}
#endif

#ifdef NETWORK
	io->set_nodelay();
	io->sync();
	double t1 = wallClock();
	Integer a(128, 2, ALICE);
	Integer b(128, 3, BOB);
	Integer c(128, 1, PUBLIC);
	for(int i = 0; i < 10000; ++i) {
        static_assert(sizeof(Bit) == sizeof(block), "vvvvv    assumes Bit is the same as block");
        cf.compute((block*)c.bits, (block*)a.bits, (block*)b.bits);
	}
	cout << wallClock() - t1<<" "<<party<<" "<<c.reveal<string>(BOB)<<endl;
#endif
    std::cout << "circuit_file passed " << std::endl;
    return 0;
}



#ifndef _MSC_VER // not visual studio 
int main(int argc, char** argv) {
    int port;
    int party;
    if (argc > 1)
    {
        parse_party_and_port(argv, argc, &party, &port);
        circuit_file_main(port, (EmpParty)party);

    }
    else
    {
#ifdef THREADING
        auto thrd = std::thread([&]() {
            circuit_file_main(1212, ALICE);
        });

        circuit_file_main(1212, BOB);

        thrd.join();
#else
        std::cout << "expecting 2 command line args, party and port" << std::endl;
#endif
    }
}
#endif