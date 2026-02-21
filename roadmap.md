# ICPC Management System - Project Roadmap

**Goal:** Implement a production-quality ICPC Management System in C++ that passes all OJ test cases.

**Success Criteria:**
- All required commands implemented correctly (ADDTEAM, START, SUBMIT, FLUSH, FREEZE, SCROLL, QUERY_RANKING, QUERY_SUBMISSION, END)
- Complex ranking and freeze/scroll logic working correctly
- Performance requirements met (up to 10^4 teams, 3×10^5 operations)
- Passes local test cases
- Code ready for OJ submission via Git

---

## Current Status: M0 - Project Initialization

**Phase:** PLANNING (Athena)
**Started:** 2026-02-20

---

## Planned Milestones

### M1: Core Data Structures and Basic Commands (TBD cycles)
**Status:** Not Started
**Description:** Implement fundamental data structures (Team, Submission, Problem status) and basic commands (ADDTEAM, START, END). Establish the main program structure with command parsing.

**Key Deliverables:**
- Team management (add, duplicate detection)
- Competition lifecycle (start, end, state validation)
- Command parser that handles all command formats
- Basic error handling for invalid operations
- CMakeLists.txt and Makefile setup

**Acceptance Criteria:**
- Can add teams and detect duplicates
- Can start competition only once
- Can end competition
- Compiles successfully and produces `code` executable

---

### M2: Submission Tracking and Problem Status (TBD cycles)
**Status:** Not Started
**Description:** Implement submission recording and per-team problem status tracking.

**Key Deliverables:**
- Submission data structure with all fields (problem, team, status, time)
- Per-team problem state (solved, attempts, solve time)
- SUBMIT command processing
- QUERY_SUBMISSION with filtering (problem=, status=)

**Acceptance Criteria:**
- Correctly records all submission types
- Handles monotonically non-decreasing timestamps
- Query returns most recent matching submission
- Tracks first AC time and penalty calculations

---

### M3: Scoreboard and Ranking Logic (TBD cycles)
**Status:** Not Started
**Description:** Implement scoreboard with complete ranking algorithm including tie-breaking rules.

**Key Deliverables:**
- Ranking comparator (solved count → penalty → solve times → team name)
- FLUSH command updates rankings
- QUERY_RANKING command
- Scoreboard display format

**Acceptance Criteria:**
- Rankings correctly ordered by all 4 tie-breaking rules
- Rankings only update on FLUSH, not on SUBMIT
- Scoreboard output matches exact format specification
- Handles teams with no solves correctly

---

### M4: Freeze and Scroll Operations (TBD cycles)
**Status:** Not Started
**Description:** Implement freeze/scroll mechanism - the most complex feature.

**Key Deliverables:**
- FREEZE command with state validation
- Frozen problem status tracking (pre-freeze attempts / post-freeze submissions)
- SCROLL command with proper unfreeze order
- Ranking change detection and output during scroll
- Scoreboard display with frozen status notation

**Acceptance Criteria:**
- Can only freeze when not already frozen
- Frozen problems show `-x/y` or `0/y` format correctly
- Scroll processes lowest-ranked team's smallest frozen problem first
- Outputs ranking changes during scroll
- Outputs scoreboard before and after scroll
- Cannot freeze after scroll until scrolled

---

### M5: Integration and Edge Cases (TBD cycles)
**Status:** Not Started
**Description:** Integration testing, edge case handling, and performance validation.

**Key Deliverables:**
- Passes all local test cases
- Edge cases handled (empty teams, no submissions, etc.)
- Performance validation with large datasets
- Memory leak checks
- Code review and cleanup

**Acceptance Criteria:**
- All local test cases pass
- Handles maximum constraints (10^4 teams, 3×10^5 ops)
- Runs within time/memory limits
- No memory leaks or undefined behavior
- Code is clean and well-organized

---

### M6: OJ Submission Preparation (TBD cycles)
**Status:** Not Started
**Description:** Final preparation for OJ submission.

**Key Deliverables:**
- .gitignore configured correctly
- CMakeLists.txt and Makefile verified
- Build process tested in clean environment
- Repository structure matches OJ requirements
- Final code review

**Acceptance Criteria:**
- `git clone` + `cmake .` + `make` produces working `code` executable
- .gitignore excludes CMakeFiles/ and CMakeCache.txt
- Code compiles with gcc-13/g++-13
- Ready for OJ evaluation

---

## Lessons Learned

*This section will be updated as the project progresses*

### What Worked

### What Didn't Work

### Process Improvements

---

## Cycle Budget Tracking

| Milestone | Estimated | Actual | Notes |
|-----------|-----------|--------|-------|
| M1 | TBD | - | |
| M2 | TBD | - | |
| M3 | TBD | - | |
| M4 | TBD | - | |
| M5 | TBD | - | |
| M6 | TBD | - | |

---

*Last updated: 2026-02-20 by Athena*
