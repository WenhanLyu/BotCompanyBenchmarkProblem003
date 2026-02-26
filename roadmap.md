# Project Roadmap: ICPC Management System

**Project Goal**: Implement a complete ICPC Management System in C++ that passes all ACMOJ test cases for problem 1986.

**Submission Budget**: 5 attempts
**Constraints**: Must produce executable named `code` via CMake/Make build process
**Time Limit**: 2000ms per test case
**Memory Limit**: 512 MiB

---

## Current Status

**Phase**: Planning (Athena)
**Current Milestone**: M1 - Performance Optimization
**Cycles Used This Milestone**: 0
**Total Project Cycles**: ~10

---

## Evaluation Summary (2026-02-26)

**Functional Status**: ✅ COMPLETE
- All 9 commands implemented correctly
- Ranking algorithm with 4-level tie-breaking working
- Freeze/scroll mechanism functionally correct
- Error handling complete
- 11/12 test cases pass functionally

**Performance Status**: ❌ CRITICAL ISSUES
- **6.in**: 187 seconds (FAIL - exceeds 2s limit by 93x)
- **bigger.in**: >20 minutes (FAIL - exceeds 2s limit by >600x)
- **Root causes identified**:
  - SCROLL calls flush() inside loop → O(N²·M·log N) complexity
  - getSolveTimes() sorts on every call during comparisons
  - Inefficient data structure usage in hot paths

**Conclusion**: Implementation is correct but will fail OJ submission due to performance timeouts.

---

## Completed Milestones

### M0: Initial Implementation (COMPLETED - Ares, ~8 cycles)
**Description**: Build complete functional implementation

**What Was Achieved**:
- ✅ All 9 commands working
- ✅ Team management, submission tracking
- ✅ Ranking algorithm with all tie-breakers
- ✅ Freeze/scroll mechanism
- ✅ Query operations
- ✅ Build system (CMakeLists.txt)
- ✅ Passes 1.in, 2.in, small.in, 3.in, 4.in, 5.in, 7.in, big.in functionally

**Issues Found**:
- Level 3 ranking bug (fixed in d67e15b)
- SCROLL error message formatting (fixed in ca80a2e)
- Critical performance bottlenecks (documented)

**Lessons Learned**:
- Functional correctness achieved quickly with good architecture
- Performance analysis should happen earlier in development
- Test suite progression (small → medium → large) worked well
- Multiple independent evaluation workers (Elena, Marcus, Sophia, Kai, Zara) provided comprehensive coverage

---

## Active Milestone

### M1: Performance Optimization for OJ Submission
**Status**: ACTIVE
**Allocated Cycles**: 6
**Started**: 2026-02-26

**Description**:
Optimize the implementation to meet OJ time limits (2000ms per test case) and memory limits (512 MiB). Address critical performance bottlenecks identified in evaluation phase.

**Acceptance Criteria** (ALL must pass):
1. **bigger.in** completes in <2000ms (currently >20 min)
2. **6.in** completes in <1000ms (currently 187s)
3. **All other tests** complete in <500ms (currently pass)
4. No regression in functional correctness (all tests produce correct output)
5. Memory usage stays under 512 MiB for all tests
6. Code remains clean and maintainable (no hacky optimizations)

**Critical Bottlenecks to Fix** (from Kai's analysis):
1. **PRIORITY 1**: Remove flush() call inside SCROLL loop (line 339)
   - Current: O(N²·M·log N)
   - Target: O(N·M·log N)
   - Expected speedup: 100x for larger tests

2. **PRIORITY 2**: Cache getSolveTimes() results during ranking
   - Current: Sorts on every comparison call
   - Target: Sort once per team per flush
   - Expected speedup: 10x for flush operations

3. **PRIORITY 3**: Optimize frozen problem detection
   - Current: Linear scans in hot paths
   - Target: Use sets or bit vectors

4. **PRIORITY 4**: Reserve vector capacities
   - Reduces reallocations during growth

**Implementation Strategy**:
- Phase 1: Fix SCROLL flush() bottleneck → test on 6.in
- Phase 2: Cache solve times → test on bigger.in
- Phase 3: Additional optimizations if needed
- Validate no functional regressions after each phase

**Risk Assessment**:
- Low risk: Well-understood algorithmic issues
- Clear optimization path identified
- Can test incrementally on progressively larger inputs

---

## Planned Milestones

### M2: Final Validation and ACMOJ Submission
**Status**: Not Started
**Estimated Cycles**: 2-3

**Description**:
Final validation, code review, and prepare for ACMOJ submission.

**Acceptance Criteria**:
- All local tests pass (functional + performance)
- Code review complete (clean, well-structured)
- .gitignore configured correctly
- Build system tested from clean checkout
- Git repository ready for submission
- Ready for ACMOJ evaluation

**Activities**:
- Run full test suite 3 times to ensure stability
- Code cleanup and documentation review
- Verify build from clean clone
- Final git housekeeping
- Submit to ACMOJ

---

## Performance Targets

| Test File | Current Time | Target Time | Status |
|-----------|-------------|-------------|--------|
| 1.in | <0.01s | <0.5s | ✅ PASS |
| 2.in | <0.01s | <0.5s | ✅ PASS |
| small.in | <0.02s | <0.5s | ✅ PASS |
| 3.in | 2.89s | <0.5s | ⚠️ SLOW |
| 4.in | 4.06s | <0.5s | ⚠️ SLOW |
| 5.in | 4.60s | <0.5s | ⚠️ SLOW |
| 6.in | 187s | <1.0s | ❌ FAIL |
| 7.in | 3.10s | <1.0s | ⚠️ SLOW |
| big.in | 3.82s | <1.0s | ⚠️ SLOW |
| bigger.in | >1200s | <2.0s | ❌ FAIL |

---

## Lessons Learned

### What Worked Well:
- ✅ Athena's evaluation phase with multiple independent workers
- ✅ Structured analysis (requirements, architecture, testing, performance)
- ✅ Test-driven development using provided test cases
- ✅ Early focus on functional correctness before optimization
- ✅ Comprehensive bug identification before optimization phase

### What Could Be Improved:
- ⚠️ Performance testing should happen earlier in M0
- ⚠️ Should profile performance during initial implementation
- ⚠️ Could have caught O(N²) complexity issues during code review

### Adjustments for M1:
- Optimize aggressively but test after each change
- Use profiling data to guide optimization priorities
- Maintain functional correctness throughout

---

## Risk Management

### Current Risks:
1. **Performance optimization breaks functionality**: MEDIUM
   - Mitigation: Test after each optimization phase

2. **Optimizations insufficient**: LOW
   - Mitigation: Clear algorithmic improvements identified with >100x expected speedup

3. **ACMOJ submission limit (5 attempts)**: MEDIUM
   - Mitigation: Only submit when ALL local tests pass with margin (target <50% of time limit)

### Contingency Plans:
- If M1 optimizations insufficient: Profile and identify remaining bottlenecks
- If functional regressions appear: Revert and optimize more carefully
- If submission fails: Analyze OJ feedback and create focused fix milestone

---

## Notes

- The freeze/scroll mechanism is algorithmically complex but now works correctly
- Performance is the ONLY remaining barrier to success
- Kai's analysis provides clear optimization path
- Should achieve 100x+ speedup with Priority 1 fix alone
- Conservative estimate: M1 completes in 4-6 cycles

