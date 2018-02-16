#include <Frontier.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc,char *argv[])
{
	fm::CmdArgs args(argc,argv);

	args.addSwitch<fm::String>("m",[&](fm::String arg) -> bool {
		
		cout << "m: " << arg.str() << endl;

		return true;
	});

	fm::Result res = args.processArgs();
	if (!res)
	{
		cout << res << endl;
	}
}

