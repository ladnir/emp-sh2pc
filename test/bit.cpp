#include "emp-sh2pc/semihonest.h"
#include <emp-tool.h>

void test_bit(EmpParty party) {
	bool b[] = {true, false};
    EmpParty p[] = {PUBLIC, ALICE, BOB};

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 3; ++j)
			for(int k = 0; k < 2; ++k)
				for (int l= 0; l < 3; ++l)  {
					{
                        //cout << osuCrypto::IoStream::lock << party << " AND i=" << i << " j=" << j << " k=" << k << " l=" << l << endl << osuCrypto::IoStream::unlock;

						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1&b2).reveal(BOB);
						if(party == BOB) {
							if(res != (b[i] && b[k]))
                                cout << "AND i=" << i << " j=" << j << " k=" << k << " l=" << l << " res = " << res << endl;
							assert(res == (b[i] && b[k]));
						}
					}
					{
                        //cout << osuCrypto::IoStream::lock << party <<" XOR i=" << i << " j=" << j << " k=" << k << " l=" << l << endl << osuCrypto::IoStream::unlock;

						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1^b2).reveal(BOB);
						if(party == BOB) {
							if(res != (b[i] ^ b[k]))
								cout <<"XOR i="<<i<<" j="<<j<<" k="<<k<<" l="<<l<< " res = " <<res<<endl;
							assert(res == (b[i] ^ b[k]));
						}
					}
				}
	cout <<"bit passed"<<endl;
}

int bit_main(int port,EmpParty party)
{
    
    NetIO * io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

    char dummy = 0;
    io->send_data(&dummy, 1);
    io->recv_data(&dummy, 1);

    setup_semi_honest(io, party, toBlock((char)party));
    test_bit(party);
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
        bit_main(port, (EmpParty)party);

    }
    else
    {
#ifdef THREADING
        auto thrd = std::thread([&](){
            bit_main(1212, ALICE);
        });

        bit_main(1212, BOB);

        thrd.join();
#else
        std::cout << "expecting 2 command line args, party and port" << std::endl;
#endif
    }
}
#endif