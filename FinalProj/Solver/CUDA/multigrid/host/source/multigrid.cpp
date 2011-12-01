
#include"../include/multigrid.h"

int main(int argc,char** argv)
{
	init();
	vcycle();
	release();

	return 0;
}
	