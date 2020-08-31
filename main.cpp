#include "SettlementReader.h"
#include <iostream>
#include <regex>
using namespace std;


int main(int var_num, const char** var_val)
{
	//SettlementReader reader(var_num, var_val);
	//reader.read();


	string pattern{ "(-[0-9]+(.[0-9]+)?|[0-9]+(.[0-9]+)?)" };
	pattern = "[[:alpha:]]+" + pattern;
	regex r(pattern);
	smatch results;
	string test = "MA005\t\r";
	if (regex_search(test, results, r))
		cout << results.str() << endl;;



	return 0;
}



