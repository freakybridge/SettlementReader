#include "SettlementReader.h"
#include <iostream>
#include <regex>
using namespace std;


int main(int var_num, const char** var_val)
{
	SettlementReader reader(var_num, var_val);
	reader.read();
	reader.prepare();
	


	return 0;
}



