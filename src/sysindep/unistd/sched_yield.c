int _sched_yield(void);

int sched_yield(void) {
	_sched_yield();
	return 0;
}
