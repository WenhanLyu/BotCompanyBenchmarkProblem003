#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// Submission record
struct Submission {
    string problem;
    string team;
    string status;
    int time;

    Submission(const string& p, const string& t, const string& s, int tm)
        : problem(p), team(t), status(s), time(tm) {}
};

// Per-problem state for a team
struct ProblemState {
    bool solved = false;
    int solve_time = 0;
    int wrong_attempts = 0;  // Wrong attempts before first AC
    int total_submissions = 0;

    ProblemState() : solved(false), solve_time(0), wrong_attempts(0), total_submissions(0) {}
};

// Team ranking info (computed at FLUSH time)
struct TeamRanking {
    string team_name;
    int solved_count;
    int penalty_time;
    vector<int> solve_times;  // Sorted in descending order
    int ranking;

    TeamRanking() : team_name(""), solved_count(0), penalty_time(0), ranking(0) {}
    TeamRanking(const string& name, int solved, int penalty, const vector<int>& times)
        : team_name(name), solved_count(solved), penalty_time(penalty), solve_times(times), ranking(0) {}
};

// Ranking comparator: returns true if team1 ranks HIGHER than team2
// (i.e., team1 should appear before team2 in ranking order)
bool compareTeamRanking(const TeamRanking& t1, const TeamRanking& t2) {
    // 1. More solved problems rank higher
    if (t1.solved_count != t2.solved_count) {
        return t1.solved_count > t2.solved_count;
    }

    // 2. Less penalty time ranks higher
    if (t1.penalty_time != t2.penalty_time) {
        return t1.penalty_time < t2.penalty_time;
    }

    // 3. Compare solve times vector (sorted descending)
    // Smaller maximum solve time ranks higher
    size_t min_size = min(t1.solve_times.size(), t2.solve_times.size());
    for (size_t i = 0; i < min_size; i++) {
        if (t1.solve_times[i] != t2.solve_times[i]) {
            return t1.solve_times[i] < t2.solve_times[i];
        }
    }

    // 4. Lexicographically smaller team name ranks higher
    return t1.team_name < t2.team_name;
}

class ICPCManagementSystem {
private:
    set<string> teams;
    bool competition_started;
    int duration_time;
    int problem_count;

    // Submission tracking
    vector<Submission> submissions;  // All submissions in order
    map<string, map<string, ProblemState>> team_problems;  // team -> problem -> state

    // Ranking tracking (updated on FLUSH)
    map<string, TeamRanking> current_rankings;  // team -> ranking info
    bool has_flushed;  // Whether FLUSH has been called at least once
    bool isFrozen;  // Whether the scoreboard is currently frozen

    bool isValidTeamName(const string& name) {
        if (name.empty() || name.length() > 20) {
            return false;
        }
        for (char c : name) {
            if (!isalnum(c) && c != '_') {
                return false;
            }
        }
        return true;
    }

    // Calculate penalty time for a team's solved problems
    int calculatePenaltyTime(const string& team_name) {
        int total_penalty = 0;
        if (team_problems.find(team_name) == team_problems.end()) {
            return 0;
        }

        for (const auto& [problem, state] : team_problems[team_name]) {
            if (state.solved) {
                // Penalty = 20 * wrong_attempts + solve_time
                total_penalty += 20 * state.wrong_attempts + state.solve_time;
            }
        }
        return total_penalty;
    }

    // Count number of solved problems for a team
    int countSolvedProblems(const string& team_name) {
        int count = 0;
        if (team_problems.find(team_name) == team_problems.end()) {
            return 0;
        }

        for (const auto& [problem, state] : team_problems[team_name]) {
            if (state.solved) {
                count++;
            }
        }
        return count;
    }

    // Get solve times for a team (sorted in descending order)
    vector<int> getSolveTimes(const string& team_name) {
        vector<int> times;
        if (team_problems.find(team_name) == team_problems.end()) {
            return times;
        }

        for (const auto& [problem, state] : team_problems[team_name]) {
            if (state.solved) {
                times.push_back(state.solve_time);
            }
        }

        // Sort in descending order (largest first)
        sort(times.begin(), times.end(), greater<int>());
        return times;
    }

public:
    ICPCManagementSystem() : competition_started(false), duration_time(0), problem_count(0), has_flushed(false), isFrozen(false) {}

    void addTeam(const string& team_name) {
        if (competition_started) {
            cout << "[Error]Add failed: competition has started.\n";
            return;
        }

        if (!isValidTeamName(team_name)) {
            // Invalid team name - should not happen per spec
            return;
        }

        if (teams.find(team_name) != teams.end()) {
            cout << "[Error]Add failed: duplicated team name.\n";
            return;
        }

        teams.insert(team_name);
        cout << "[Info]Add successfully.\n";
    }

    void startCompetition(int duration, int problems) {
        if (competition_started) {
            cout << "[Error]Start failed: competition has started.\n";
            return;
        }

        competition_started = true;
        duration_time = duration;
        problem_count = problems;
        cout << "[Info]Competition starts.\n";
    }

    void endCompetition() {
        cout << "[Info]Competition ends.\n";
    }

    void submit(const string& problem, const string& team, const string& status, int time) {
        // Record the submission
        submissions.emplace_back(problem, team, status, time);

        // Update team-problem state
        ProblemState& state = team_problems[team][problem];

        if (!state.solved) {
            if (status == "Accepted") {
                // First AC for this problem
                state.solved = true;
                state.solve_time = time;
                // wrong_attempts is already tracked (incremented for each non-AC before this)
            } else {
                // Non-AC submission on unsolved problem
                state.wrong_attempts++;
            }
        }
        // If already solved, we don't update wrong_attempts or solve_time

        state.total_submissions++;
    }

    void flush() {
        cout << "[Info]Flush scoreboard.\n";

        // Compute rankings for all teams
        vector<TeamRanking> rankings;
        for (const string& team_name : teams) {
            int solved = countSolvedProblems(team_name);
            int penalty = calculatePenaltyTime(team_name);
            vector<int> times = getSolveTimes(team_name);

            rankings.emplace_back(team_name, solved, penalty, times);
        }

        // Sort teams by ranking comparator
        sort(rankings.begin(), rankings.end(), compareTeamRanking);

        // Assign ranking numbers and store
        current_rankings.clear();
        for (size_t i = 0; i < rankings.size(); i++) {
            rankings[i].ranking = i + 1;
            current_rankings[rankings[i].team_name] = rankings[i];
        }

        has_flushed = true;
    }

    void freeze() {
        if (isFrozen) {
            cout << "[Error]Freeze failed: scoreboard has been frozen.\n";
            return;
        }

        isFrozen = true;
        cout << "[Info]Freeze scoreboard.\n";
    }

    void scroll() {
        if (!isFrozen) {
            cout << "[Error]Scroll failed: scoreboard has not been frozen.\n";
            return;
        }

        cout << "[Info]Scroll scoreboard.\n";

        // TODO: Phase 2 - Output scoreboard before/after scrolling

        // After scrolling, unfreeze the scoreboard
        isFrozen = false;
    }

    void queryRanking(const string& team_name) {
        if (teams.find(team_name) == teams.end()) {
            cout << "[Error]Query ranking failed: cannot find the team.\n";
            return;
        }

        cout << "[Info]Complete query ranking.\n";

        // Output warning if frozen
        if (isFrozen) {
            cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.\n";
        }

        int ranking;
        if (!has_flushed) {
            // Before first FLUSH: compute lexicographic ranking
            vector<string> sorted_teams(teams.begin(), teams.end());
            sort(sorted_teams.begin(), sorted_teams.end());

            ranking = 1;
            for (const string& t : sorted_teams) {
                if (t == team_name) {
                    break;
                }
                ranking++;
            }
        } else {
            // After FLUSH: use stored ranking
            ranking = current_rankings[team_name].ranking;
        }

        cout << team_name << " NOW AT RANKING " << ranking << "\n";
    }

    void querySubmission(const string& team_name, const string& problem, const string& status) {
        if (teams.find(team_name) == teams.end()) {
            cout << "[Error]Query submission failed: cannot find the team.\n";
            return;
        }

        cout << "[Info]Complete query submission.\n";

        // Search backwards through submissions to find the last matching one
        for (int i = submissions.size() - 1; i >= 0; i--) {
            const Submission& sub = submissions[i];

            // Check if this submission matches the criteria
            if (sub.team != team_name) {
                continue;
            }

            // Check problem filter (ALL means any problem)
            if (problem != "ALL" && sub.problem != problem) {
                continue;
            }

            // Check status filter (ALL means any status)
            if (status != "ALL" && sub.status != status) {
                continue;
            }

            // Found a match!
            cout << sub.team << " " << sub.problem << " " << sub.status << " " << sub.time << "\n";
            return;
        }

        // No matching submission found
        cout << "Cannot find any submission.\n";
    }
};

int main() {
    ICPCManagementSystem system;
    string line;

    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "ADDTEAM") {
            string team_name;
            iss >> team_name;
            system.addTeam(team_name);
        } else if (command == "START") {
            string duration_str, problem_str;
            int duration, problems;
            iss >> duration_str >> duration >> problem_str >> problems;
            system.startCompetition(duration, problems);
        } else if (command == "SUBMIT") {
            string problem, by, team, with, status, at;
            int time;
            iss >> problem >> by >> team >> with >> status >> at >> time;
            system.submit(problem, team, status, time);
        } else if (command == "FLUSH") {
            system.flush();
        } else if (command == "FREEZE") {
            system.freeze();
        } else if (command == "SCROLL") {
            system.scroll();
        } else if (command == "QUERY_RANKING") {
            string team_name;
            iss >> team_name;
            system.queryRanking(team_name);
        } else if (command == "QUERY_SUBMISSION") {
            // Parse: QUERY_SUBMISSION [team_name] WHERE PROBLEM=[problem_name] AND STATUS=[status]
            string team_name, where, problem_part, and_part, status_part;
            iss >> team_name >> where >> problem_part >> and_part >> status_part;

            // Extract problem and status from PROBLEM=X and STATUS=Y
            string problem = problem_part.substr(8); // Skip "PROBLEM="
            string status = status_part.substr(7); // Skip "STATUS="

            system.querySubmission(team_name, problem, status);
        } else if (command == "END") {
            system.endCompetition();
            break;
        }
    }

    return 0;
}
