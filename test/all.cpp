
#include <emp-tool/emp-tool.h>
#include <cryptoTools/Common/CLP.h>

int bit_main         (int port, EmpParty party);
int circuit_file_main(int port, EmpParty party);
int float_main       (int port, EmpParty party);
int int_main         (int port, EmpParty party);

std::vector<std::string> tests{ "bit", "circuit_file", "float", "int" };

void doTests(osuCrypto::CLP& cmd, int port, EmpParty party)
{
    if (cmd.isSet("bit") || !cmd.isSet(tests))  bit_main(port, party);
    if (cmd.isSet("circuit_file") || !cmd.isSet(tests))  circuit_file_main(port, party);
    //if (cmd.isSet("float") || !cmd.isSet(tests))  float_main(port, party);
    if (cmd.isSet("int") || !cmd.isSet(tests))  int_main(port, party);
}

int main(int argc, char** argv) {

    osuCrypto::CLP cmd(argc, argv);
    cmd.setDefault("port", "1212");
    auto port = cmd.get<int>("port");

    if (cmd.isSet("party"))
    {
        auto party = (EmpParty)cmd.get<int>("party");
        doTests(cmd, port, party);
    }
    else
    {
        auto thrd = std::thread([&]() { doTests(cmd, port, BOB); });
        doTests(cmd, port, ALICE);
        thrd.join();
    }
}
