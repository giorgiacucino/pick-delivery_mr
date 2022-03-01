#include <stdlib.h>
#include <string>
using namespace std;

class robot
{
	public:
		int		status;
		float	distance;
        float	prevdist;
		int		free;
		user	*sender;
		user	*receiver;
		aula	*auladest;
		int		broadcast;
	robot()
	{
		status = 0; //0 nothing assigned, 1 picking, 2 delivering
		free = 1;
		distance = 0;
		prevdist = 0;
		sender = NULL;
		receiver = NULL;
		auladest = NULL;
		broadcast = 0;
	}
};
