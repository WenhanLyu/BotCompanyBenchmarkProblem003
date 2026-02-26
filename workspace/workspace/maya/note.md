# Maya - Cycle Notes

## Phase 4: SCROLL Optimization - Old Rankings Map & Ranking Updates (IN PROGRESS)

### Phase 4.1: Replace old_rankings_map rebuild with vector copy (COMPLETED)

**Problem:**
- Lines 346-351 rebuilt TWO full maps every SCROLL iteration:
  - old_rankings_map: team -> rank mapping (NEVER USED)
  - old_rank_to_team: rank -> team mapping (used once for lookup)
- Building maps required O(N log N) insertions for N teams
- Map lookup was O(log N) vs O(1) for vector access

**Solution:**
- Replaced both maps with simple vector copy: `old_rankings = rankings`
- Vector copy is O(N) with better constants
- Direct array access for lookup: `old_rankings[new_rank - 1].first` is O(1)
- Eliminated unused old_rankings_map entirely

**Performance Results:**
- **Test 6.in**: 1.6s (Phase 3) → 0.65s (Phase 4.1 only) = **2.46x speedup!**
- **Total speedup**: 187s → 0.65s = **288x speedup from original**
- Output verified identical to Phase 3 (correctness maintained)

**Files Modified:**
- main.cpp:
  - Lines 346-351: Replaced map building with `vector<pair<string, int>> old_rankings = rankings;`
  - Lines 410-418: Direct vector access instead of map lookup

### Phase 4.2: Skip updateRankingsOnly() when no problem solved (COMPLETED)

**Problem:**
- updateRankingsOnly() called every SCROLL iteration even when unnecessary
- Full sort of N teams (O(N log N)) even when unfreezing reveals only wrong attempts
- Wrong attempts for unsolved problems don't affect ranking (unsolved problems don't contribute to penalty)

**Solution:**
- Modified unfreezeOneProblem() to return bool indicating if problem became newly solved
- Skip updateRankingsOnly() if problem didn't become solved
- Rationale: If solved_count doesn't change, ranking cannot improve
- Rankings data structure can be "stale" but still correct since actual rankings unchanged

**Performance Results:**
- **Test 6.in**: 0.89s (combined Phase 4.1 + 4.2) < 1.0s target ✓
- Slight overhead vs Phase 4.1 alone (0.65s → 0.89s) but still meets target
- Output verified identical to Phase 3 (correctness maintained)

**Files Modified:**
- main.cpp:
  - Lines 305-327: Modified unfreezeOneProblem() to return bool
  - Lines 390-401: Check if problem_solved before calling updateRankingsOnly()

### Current Status

**Targets:**
- 6.in: <1000ms → **ACHIEVED** (0.89s = 890ms)
- bigger.in: <2000ms → **NOT YET ACHIEVED** (still >15s)

**Analysis:**
- Phase 4.1 provides massive speedup (2.46x) by eliminating map rebuild overhead
- Phase 4.2 provides conditional updates but with small overhead on 6.in
- bigger.in may have different characteristics (more problems become solved during unfreeze)
- Or bigger.in may need additional optimizations not covered in Phase 4 assignment

### Next Steps
- Consider additional optimizations for bigger.in:
  - True incremental ranking updates (O(N) instead of O(N log N))
  - Batch updates (update rankings every K iterations instead of every iteration)
  - Profile bigger.in to identify remaining bottlenecks
- Alternative: Verify if <2000ms target is achievable on current hardware with current approach

## Previous Phases Summary

### Phase 3: Optimize Frozen Problem Detection in SCROLL (COMPLETED)
- Problem: O(N·M) nested loop checking frozen problems
- Solution: Added frozen_problems_count counter for O(1) lookup
- Result: 1.9s → 1.6s (1.19x speedup)

### Phase 2: Cache getSolveTimes() Results (COMPLETED)
- Problem: O(N² log N · M) complexity during ranking
- Solution: Added caching mechanism to avoid repeated sorting
- Result: 24.4s → 1.9s (12.8x speedup)

### Phase 1: SCROLL Optimization (COMPLETED)
- Problem: flush() called in every SCROLL iteration
- Solution: Added updateRankingsOnly() to avoid recalculation
- Result: 187s → 24.4s (7.7x speedup)
