# Maya - Cycle Notes

## Phase 2: Cache getSolveTimes() Results (COMPLETED)

### Problem
- `getSolveTimes()` was called O(N log N) times during sort comparison
- Each call created a new vector, populated it, and sorted it: O(M log M) per call
- Total complexity during ranking: O(N² log N · M)
- This was the primary bottleneck after Phase 1 optimization

### Solution Implemented
Added caching mechanism to Team class:

1. **New private members**:
   - `cached_solve_times`: Stores pre-sorted solve times
   - `cache_dirty`: Tracks if cache needs updating
   - `updateSolveTimesCache()`: Helper to rebuild cache

2. **Modified getSolveTimes()**:
   - Returns `const vector<int>&` instead of creating new vector
   - Lazily updates cache only when dirty
   - O(1) when cache is valid

3. **Cache invalidation triggers**:
   - `addSubmission()`: When problem is solved (line 103)
   - `recalculateStats()`: After stats recalculation (line 124)
   - Note: `unfreezeOneProblem()` is followed by `recalculateStats()`, so coverage is complete

4. **Updated comparison functions**:
   - Changed `vector<int> times_a/b` to `const vector<int>&` in both:
     - `flush()` comparison (line 234)
     - `updateRankingsOnly()` comparison (line 275)
   - Avoids unnecessary vector copies

### Performance Results
- **Test 6.in**: 24.4s (Phase 1) → 1.9s (Phase 2) = **12.8x speedup**
- **Total speedup from original**: 187s → 1.9s = **98.5x speedup**
- Test 1.in and 2.in: Output verified correct
- Test bigger.in: Running...

### Complexity Improvement
- **Before**: O(N² log N · M) for ranking comparisons
- **After**: O(N log N + N·M) where cache is built once per team

### Target Status
- Target for 6.in: <1000ms
- Current: 1900ms (close, but not quite there yet)
- This is still a major improvement, additional optimizations may be needed

### Files Modified
- `main.cpp`:
  - Lines 72-97: Added cache members and methods to Team class
  - Line 103: Mark cache dirty when problem solved
  - Line 124: Mark cache dirty after recalculateStats
  - Lines 122-127: Modified getSolveTimes() to use cache
  - Lines 234, 275: Use const references in comparisons

### Next Steps (Phase 3)
- Optimize frozen problem detection in SCROLL
- Lines 342-348: Nested loop checking every problem for every team
- Pre-cache frozen status per team for O(1) lookup
