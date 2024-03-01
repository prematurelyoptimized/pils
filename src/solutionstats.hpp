#ifndef PILS_SOLUTIONSTATS
#define PILS_SOLUTIONSTATS

enum SolutionStatus { CONVERGED, INFEASIBLE };

struct SolutionStats {
	size_t pivots;
	size_t divisibility_bound_improvements;
	SolutionStatus status;

	SolutionStats(void) {
		pivots = 0;
		divisibility_bound_improvements = 0;
	}
};

#endif
