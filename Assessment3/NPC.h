#ifndef NPC_H
#define NPC_H

#include <string>

class NPC {

public:
	NPC();
	~NPC();

	void loadDialogue();
	void printDialogue();

private:

	std::string name;

};

#endif