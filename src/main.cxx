#include <stdio.h>
#include <string.h>

int run_alsa(int argc,char* argv[]);
int run_jack(int argc,char* argv[]);

int main(int argc,char* argv[])
{
	if (argc>1)
	{
		if (!strcmp(argv[1],"--alsa")) return run_alsa(argc-1,argv+1);
		if (!strcmp(argv[1],"--jack")) return run_jack(argc-1,argv+1);
	}
	printf("Usage: polarbear [ --alsa | --jack ] [-h | --help ]\n");
	return -1;
}
