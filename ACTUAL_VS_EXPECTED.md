# Actual vs Expected Output Comparison

## Example 1: SCROLL When Not Frozen (Line 5)

### Input
```
SCROLL
```

### Actual Output ❌
```
[Info]Scroll scoreboard.
```

### Expected Output ✅
```
[Error]Scroll failed: scoreboard has not been frozen.
```

### Issue
SCROLL should fail when scoreboard is not frozen. Missing prerequisite check.

---

## Example 2: Duplicate FREEZE (Lines 14-15)

### Input
```
FREEZE
FREEZE
```

### Actual Output ❌
```
[Info]Freeze scoreboard.
[Info]Freeze scoreboard.
```

### Expected Output ✅
```
[Info]Freeze scoreboard.
[Error]Freeze failed: scoreboard has been frozen.
```

### Issue
Second FREEZE should fail - scoreboard already frozen. Missing duplicate detection.

---

## Example 3: QUERY_RANKING While Frozen (Lines 16-17)

### Input
```
QUERY_RANKING team1
```

### Actual Output ❌
```
[Info]Complete query ranking.
team1 NOW AT RANKING 2
```

### Expected Output ✅
```
[Info]Complete query ranking.
[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.
team1 NOW AT RANKING 2
```

### Issue
Missing freeze warning line when scoreboard is frozen.

---

## Example 4: SCROLL Display (Line 21)

### Input
```
SCROLL
```

### Actual Output ❌
```
[Info]Scroll scoreboard.
```

### Expected Output ✅
```
[Info]Scroll scoreboard.
team0 1 3 16 + + + -1 . . . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
team0 1 3 16 + + + -1 . . . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
```

### Issue
Missing complete scoreboard output (before and after scroll). No frozen problems in this case, so scoreboard stays same.

---

## Example 5: SCROLL with Frozen Problems (Line 29)

### Input
```
SCROLL
```

### Context
- team0 has frozen problems E and F (submitted during freeze)
- E: Accepted at time 20
- F: Accepted at time 30

### Actual Output ❌
```
[Info]Scroll scoreboard.
```

### Expected Output ✅
```
[Info]Scroll scoreboard.
team0 1 3 16 + + + -1 -0/1 -0/1 . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
team0 team1 4 46
team0 team2 5 66
team0 1 5 66 + + + -1 + + . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
```

### Explanation of Expected Output
1. **Line 1**: Info message
2. **Lines 2-4**: Scoreboard BEFORE scroll
   - team0: rank 1, 3 solved, penalty 16
   - Problems: `+ + + -1` = A solved, B solved, C solved, D failed
   - Problems: `-0/1 -0/1` = E frozen (0 wrong before, 1 during), F frozen (0 wrong before, 1 during)
3. **Lines 5-6**: Ranking changes
   - `team0 team1 4 46` = team0 surpasses team1 with 4 solved, penalty 46 (after unfreezing E)
   - `team0 team2 5 66` = team0 surpasses team2 with 5 solved, penalty 66 (after unfreezing F)
   - Note: Actually team0 already ranked #1, so ranking changes might not apply here. Let me reconsider...
   - Actually, since team0 was already #1 and team1/team2 have 0 solved, unfreezing won't cause ranking changes
4. **Lines 7-9** (or 5-7 if no changes): Scoreboard AFTER scroll
   - team0: rank 1, 5 solved, penalty 66
   - Problems: `+ + + -1 + +` = A,B,C,E,F solved, D failed

### Corrected Expected Output
```
[Info]Scroll scoreboard.
team0 1 3 16 + + + -1 -0/1 -0/1 . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
team0 1 5 66 + + + -1 + + . . . .
team1 2 0 0 . . . . . . . . . .
team2 3 0 0 . . . . . . . . . .
```

### Issue
Missing complete scoreboard output and unfreezing logic.

---

## Example 6: QUERY_SUBMISSION for Existing Submission (Lines 35-36)

### Input
```
QUERY_SUBMISSION team0 WHERE PROBLEM=A AND STATUS=ALL
```

### Context
team0 submitted problem A with Accepted status at time 1 (line 11 of input)

### Actual Output ❌
```
[Info]Complete query submission.
Cannot find any submission.
```

### Expected Output ✅
```
[Info]Complete query submission.
team0 A Accepted 1
```

### Issue
Submission exists but is not found. Data storage or retrieval broken.

---

## Problem Status Format Reference

From specification (README.md):

### Solved Problem
- `+` if no wrong attempts
- `+X` if X wrong attempts before solving

### Unsolved Problem (Not Frozen)
- `.` if no submissions
- `-X` if X wrong submissions

### Frozen Problem
- `X/Y` or `-X/Y` where:
  - X = wrong submissions before freeze (use `0` instead of `-0`)
  - Y = total submissions during freeze

### Examples
- `+` = solved on first try
- `+2` = solved after 2 wrong attempts
- `.` = no submissions
- `-3` = 3 wrong submissions, not solved
- `0/1` = frozen, 0 wrong before freeze, 1 submission during freeze
- `-2/3` = frozen, 2 wrong before freeze, 3 submissions during freeze

---

## Penalty Calculation Reference

For a solved problem:
```
penalty = 20 * (wrong_submissions) + solve_time
```

Total penalty = sum of penalties for all **solved** problems

### Example: team0 after scroll
- A: 20*0 + 1 = 1
- B: 20*0 + 5 = 5
- C: 20*0 + 10 = 10
- D: not solved (doesn't count)
- E: 20*0 + 20 = 20
- F: 20*0 + 30 = 30
- **Total**: 1 + 5 + 10 + 20 + 30 = 66

---

## Ranking Tiebreakers Reference

Teams are ranked by:
1. **More solved problems** (higher is better)
2. **Less penalty time** (lower is better)
3. **Smaller max solve time** among solved problems (compare largest, then 2nd largest, etc.)
4. **Lexicographic team name** (alphabetically earlier is better)

In this test:
- team0: 5 solved → rank 1
- team1: 0 solved → rank 2 (lexicographically before team2)
- team2: 0 solved → rank 3
