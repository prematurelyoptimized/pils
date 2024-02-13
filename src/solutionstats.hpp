#ifndef PILS_SOLUTIONSTATS
#define PILS_SOLUTIONSTATS

enum SolutionStatus { CONVERGED, INFEASIBLE };

struct SolutionStats {
	size_t pivots;
	SolutionStatus status;
};

#endif
