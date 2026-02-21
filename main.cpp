#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>

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

class ICPCManagementSystem {
private:
    set<string> teams;
    bool competition_started;
    int duration_time;
    int problem_count;

    // Submission tracking
    vector<Submission> submissions;  // All submissions in order
    map<string, map<string, ProblemState>> team_problems;  // team -> problem -> state

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

public:
    ICPCManagementSystem() : competition_started(false), duration_time(0), problem_count(0) {}

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
        // TODO: Implement for later milestones
        cout << "[Info]Flush scoreboard.\n";
    }

    void freeze() {
        // TODO: Implement for later milestones
        cout << "[Info]Freeze scoreboard.\n";
    }

    void scroll() {
        // TODO: Implement for later milestones
        cout << "[Info]Scroll scoreboard.\n";
    }

    void queryRanking(const string& team_name) {
        // TODO: Implement for later milestones
        if (teams.find(team_name) == teams.end()) {
            cout << "[Error]Query ranking failed: cannot find the team.\n";
        } else {
            cout << "[Info]Complete query ranking.\n";
        }
    }

    void querySubmission(const string& team_name, const string& problem, const string& status) {
        // TODO: Implement for later milestones
        if (teams.find(team_name) == teams.end()) {
            cout << "[Error]Query submission failed: cannot find the team.\n";
        } else {
            cout << "[Info]Complete query submission.\n";
            cout << "Cannot find any submission.\n";
        }
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
