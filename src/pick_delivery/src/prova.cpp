#include <stdlib.h>
#include <string>
#include <iostream>
#include <list>
#include "users.h"
#include "aula.h"
#include "robot.h"

using namespace std;
list<user>					userlist;
list<aula>					aulelist;
robot					    rob[2];

int	main(int argc, char **argv)
{
	user us = user("alessio", 0);
	userlist.push_back(us);

	user us1 = user("giorgia", 1);
	userlist.push_back(us1);

	aula a = aula("a1", 5, 5);
	aulelist.push_back(a);

	rob[0] = robot();
	rob[1] = robot();

	cout << "Prova stampe" << endl;
	for (auto& u : userlist)
	{
		if (u.hash=="alessio0")
		{
			rob[0].sender = &u;
			cout << "sender: " << rob[0].sender->hash << endl;
		}
	}

	return (0);
}