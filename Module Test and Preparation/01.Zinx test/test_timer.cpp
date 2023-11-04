#include<sys/timerfd.h>
#include<stdio.h>
#include<unistd.h>
int main()
{
	int iTimerfd = timerfd_create(CLOCK_MONOTONIC, 0);
	struct itimerspec period
	{{5, 0},{5, 0}
	};
	timerfd_settime(iTimerfd,0, &period,NULL);
	__uint64_t count = 0;
	while(1) {
		read(iTimerfd, &count, sizeof(count));
		puts("time our");
	}
}