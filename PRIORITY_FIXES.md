# Priority Fixes for Test Cases - Quick Reference

**Date**: 2026-02-20
**Test Status**: FAILING
**Test Files**: error.in, 1.in (identical)

## Critical Blockers (Must Fix)

### 1. SCROLL Not Displaying Scoreboard ⚠️ CRITICAL
**Lines**: 21-22, 29
**Current**: Only outputs `[Info]Scroll scoreboard.`
**Required**: Must output:
- Scoreboard BEFORE scroll
- Ranking changes (if any)
- Scoreboard AFTER scroll

**Example Expected Output**:
```
[Info]Scroll scoreboard.
team0 1 3 16 + + + -1 . . . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
team0 1 3 16 + + + -1 . . . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
```

### 2. FREEZE Allows Duplicates ⚠️ CRITICAL
**Lines**: 14-15, 28
**Current**: Second FREEZE succeeds
**Required**: Second FREEZE must fail with:
```
[Error]Freeze failed: scoreboard has been frozen.
```

**Fix**: Add `isFrozen` boolean flag, check before allowing FREEZE

### 3. SCROLL Missing Error Check ⚠️ CRITICAL
**Lines**: 5, 22
**Current**: SCROLL succeeds when not frozen
**Required**: Must fail with error when `isFrozen == false`:
```
[Error]Scroll failed: scoreboard has not been frozen.
```

### 4. QUERY_SUBMISSION Not Finding Data ⚠️ CRITICAL
**Line**: 35-36
**Current**: Returns "Cannot find any submission"
**Required**: Should return:
```
team0 A Accepted 1
```

**Issue**: Submissions from SUBMIT commands not being stored or retrieved

### 5. QUERY_RANKING Missing Freeze Warning ⚡ HIGH
**Lines**: 16-17, 19-20, 24-25, 26-27
**Current**: Missing warning line when frozen
**Required**: When `isFrozen == true`, must output:
```
[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.
```

---

## What's Working ✅

1. ADDTEAM - Accepts/rejects correctly
2. START - Starts competition, rejects duplicates
3. SUBMIT - Silent (no output) ✓
4. FLUSH - Basic functionality working
5. FREEZE - Basic functionality (but allows duplicates)
6. QUERY_RANKING - Outputs ranking number correctly
7. QUERY_SUBMISSION - Correct behavior for non-existent teams
8. END - Works correctly

---

## Implementation Checklist

### Phase 1: State Management (2-3 cycles)
- [ ] Add `bool isFrozen` flag to system state
- [ ] FREEZE: Check `isFrozen`, set error if true, else set `isFrozen = true`
- [ ] SCROLL: Check `isFrozen`, set error if false, else execute scroll
- [ ] SCROLL: Set `isFrozen = false` at end of scroll

### Phase 2: SCROLL Output (4-5 cycles)
- [ ] Implement scoreboard data structure (rankings, problems per team)
- [ ] Implement scoreboard formatter (convert to output format)
- [ ] SCROLL: Output scoreboard before unfreezing
- [ ] SCROLL: Track frozen problems per team
- [ ] SCROLL: Unfreeze problems (lowest rank team, smallest problem first)
- [ ] SCROLL: Detect and output ranking changes
- [ ] SCROLL: Output scoreboard after unfreezing

### Phase 3: Submission Storage (2-3 cycles)
- [ ] Fix submission storage in SUBMIT handler
- [ ] Verify submissions can be queried by team/problem/status
- [ ] Test QUERY_SUBMISSION retrieval logic

### Phase 4: Polish (1 cycle)
- [ ] Add freeze warning to QUERY_RANKING when `isFrozen == true`

**Total Estimated Cycles**: 9-12 cycles

---

## Test Verification Commands

```bash
# Build
cmake . && make

# Run error.in test
./code < data/003/data_test/error.in > error.actual.out 2>&1

# Run 1.in test
./code < data/003/data_test/1.in > 1.actual.out 2>&1

# Compare (should be identical since inputs are identical)
diff error.actual.out 1.actual.out

# Check specific failures
grep -n "Scroll scoreboard" error.actual.out
grep -n "Freeze scoreboard" error.actual.out
grep -n "Warning.*frozen" error.actual.out
```

---

## Key Specification References

### SCROLL Specification (README.md lines 155-176)
- Must flush first
- Output "[Info]Scroll scoreboard.\n"
- Output scoreboard before scrolling (after flush)
- Output ranking changes that occur
- Output scoreboard after scrolling
- Unfreeze from lowest-ranked team, smallest problem number first

### FREEZE Specification (README.md lines 149-154)
- If already frozen and not yet scrolled, output error

### QUERY_RANKING Specification (README.md lines 189-198)
- If frozen, output additional warning line

### Scoreboard Format (README.md lines 170-188)
```
team_name ranking solved_count total_penalty [problem_statuses...]
```

Problem statuses:
- `+` or `+X` = solved (X wrong before solve)
- `.` = no submissions
- `-X` = not solved, X wrong submissions
- `X/Y` or `-X/Y` = frozen (X wrong before freeze, Y during freeze)

---

## Expected Final Output (First 25 Lines)

```
[Info]Add successfully.
[Info]Add successfully.
[Info]Add successfully.
[Info]Competition starts.
[Error]Scroll failed: scoreboard has not been frozen.
[Info]Complete query submission.
Cannot find any submission.
[Error]Add failed: competition has started.
[Error]Add failed: competition has started.
[Error]Add failed: competition has started.
[Error]Add failed: competition has started.
[Error]Start failed: competition has started.
[Info]Flush scoreboard.
[Info]Freeze scoreboard.
[Error]Freeze failed: scoreboard has been frozen.
[Info]Complete query ranking.
[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.
team1 NOW AT RANKING 2
[Info]Flush scoreboard.
[Info]Complete query ranking.
[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.
team0 NOW AT RANKING 1
[Info]Scroll scoreboard.
[Complete scoreboard output here...]
```
