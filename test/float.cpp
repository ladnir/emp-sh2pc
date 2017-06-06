#include <typeinfo>
#include "emp-sh2pc/semihonest/semihonest.h"
#include <emp-tool/emp-tool.h>

void test_float(EmpParty party) {
	Float a(24, 9, 0.11);
	Float b(24, 9, 0.41);
	cout << a.reveal<double>(PUBLIC)<<endl;
	cout << b.reveal<double>(PUBLIC)<<endl;
	cout << (a+b).reveal<double>(PUBLIC)<<endl;
	cout << (a-b).reveal<double>(PUBLIC)<<endl;
	cout << (a*b).reveal<double>(PUBLIC)<<endl;
	double res = (a/b).reveal<double>(BOB);
	cout << res <<endl;
}

int float_main(int port, EmpParty party) {
	NetIO * io = new NetIO(party==ALICE ? nullptr : SERVER_IP, port);
    

	setup_semi_honest(io, party, toBlock((char)party));

	test_float(party);

	delete io;
	return 0;
}


#ifndef _MSC_VER // not visual studio 
int main(int argc, char** argv) {
    int port;
    int party;
    if (argc > 1)
    {
        parse_party_and_port(argv, argc, &party, &port);
        float_main(port, (EmpParty)party);

    }
    else
    {
#ifdef THREADING
        auto thrd = std::thread([&]() {
            float_main(1212, ALICE);
        });

        float_main(1212, BOB);

        thrd.join();
#else
        std::cout << "expecting 2 command line args, party and port" << std::endl;
#endif
    }
}
#endif