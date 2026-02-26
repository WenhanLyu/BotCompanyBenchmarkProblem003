# Project Roadmap: ICPC Management System

**Project Goal**: Implement a complete ICPC Management System in C++ that passes all ACMOJ test cases for problem 1986.

**Submission Budget**: 5 attempts
**Constraints**: Must produce executable named `code` via CMake/Make build process

---

## Current Status

**Phase**: Planning
**Current Milestone**: None (gathering requirements)
**Cycles Used**: 1

---

## Planned Milestones

### M1: Core System Foundation
**Status**: Not Started
**Estimated Cycles**: 6-8
**Description**: Implement basic team management, competition lifecycle, and submission tracking without freeze/scroll features.

**Acceptance Criteria**:
- ADDTEAM, START, SUBMIT, END commands work correctly
- FLUSH updates scoreboard with correct ranking logic
- All ranking tie-breakers implemented correctly (solved count → penalty time → solve time comparison → lexicographic)
- QUERY_RANKING and QUERY_SUBMISSION work correctly
- Passes simple test cases (1.in, 2.in, test.in)

**Key Components**:
- Team data structure with submission history
- Problem tracking for each team
- Ranking algorithm with all tie-breakers
- Command parser
- CMakeLists.txt and build system

---

### M2: Freeze and Scroll Mechanism
**Status**: Not Started
**Estimated Cycles**: 6-8
**Description**: Add the complex freeze/scroll scoreboard functionality.

**Acceptance Criteria**:
- FREEZE command works correctly (prevents ranking updates)
- SCROLL command implements correct unfreezing algorithm
- Frozen problem display format correct (-x/y notation)
- Ranking changes during scroll displayed correctly
- Passes medium complexity tests (3.in, 4.in, 5.in)

**Key Components**:
- Freeze state management
- Scroll algorithm (unfreeze lowest-ranked team's smallest frozen problem)
- Scoreboard snapshot and restoration
- Warning messages for frozen state

---

### M3: Performance Optimization and Edge Cases
**Status**: Not Started
**Estimated Cycles**: 4-6
**Description**: Ensure system handles large inputs efficiently and all edge cases.

**Acceptance Criteria**:
- Handles N=10^4 teams, opt=3×10^5 operations
- All operations within time limits (2000ms per test)
- Passes all local test cases including big.in and bigger.in
- Memory usage within 512 MiB limit
- Edge cases handled: duplicate times, multiple freezes, empty queries

**Key Components**:
- Performance profiling
- Algorithm optimization if needed
- Edge case handling
- Thorough testing

---

### M4: Final Validation and Submission Preparation
**Status**: Not Started
**Estimated Cycles**: 2-3
**Description**: Final validation, code review, and prepare for ACMOJ submission.

**Acceptance Criteria**:
- All local tests pass
- Code review complete (clean, well-structured)
- .gitignore configured correctly
- Build system tested from clean checkout
- Ready for ACMOJ evaluation

**Key Components**:
- Final testing
- Code cleanup
- Documentation
- Submission preparation

---

## Lessons Learned

*(Will be updated after each milestone)*

---

## Notes

- The freeze/scroll mechanism is the most algorithmically complex part
- Ranking with multiple tie-breakers needs careful implementation
- Performance matters - must be efficient for large inputs
- Build system must work correctly on OJ environment
