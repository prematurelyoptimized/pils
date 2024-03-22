#ifndef PILS_SOLUTIONSTATS
#define PILS_SOLUTIONSTATS

enum SolutionStatus { CONVERGED, INFEASIBLE };

struct SolutionStats {
	size_t pivots;
	size_t divisibility_cuts;
	size_t divisibility_bound_improvements;
	size_t gomory_cuts;
	SolutionStatus status;

	SolutionStats(void) {
		pivots = 0;
		divisibility_bound_improvements = 0;
		divisibility_cuts = 0;
		gomory_cuts = 0;
	}
};

#endif
