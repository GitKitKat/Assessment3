#ifndef NPC_H
#define NPC_H

/* Includes: 
Standard header files */
#include <string>

class NPC {

public:
	NPC();
	~NPC();

	void LoadDialogue();

	void SetName();
	void PrintDialogue();

private:

	std::string name;

};

#endif