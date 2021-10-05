
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	vector<Command*> v_commands;
	DataCommands* data;
	void start();
	virtual ~CLI();
};
#endif /* CLI_H_ */

