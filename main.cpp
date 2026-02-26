#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

/**
 * ICPC Management System
 *
 * This program implements a management system for ICPC competitions,
 * handling team registration, submissions, scoreboard management,
 * freeze/scroll operations, and various queries.
 */

// Enum for submission status
enum SubmitStatus {
    ACCEPTED,
    WRONG_ANSWER,
    RUNTIME_ERROR,
    TIME_LIMIT_EXCEED
};

// Convert string to SubmitStatus
SubmitStatus stringToStatus(const string& s) {
    if (s == "Accepted") return ACCEPTED;
    if (s == "Wrong_Answer") return WRONG_ANSWER;
    if (s == "Runtime_Error") return RUNTIME_ERROR;
    if (s == "Time_Limit_Exceed") return TIME_LIMIT_EXCEED;
    return WRONG_ANSWER; // default
}

// Convert SubmitStatus to string
string statusToString(SubmitStatus status) {
    switch (status) {
        case ACCEPTED: return "Accepted";
        case WRONG_ANSWER: return "Wrong_Answer";
        case RUNTIME_ERROR: return "Runtime_Error";
        case TIME_LIMIT_EXCEED: return "Time_Limit_Exceed";
        default: return "Wrong_Answer";
    }
}

// Submission structure
struct Submission {
    string problem;
    SubmitStatus status;
    int time;

    Submission(string p, SubmitStatus s, int t) : problem(p), status(s), time(t) {}
};

// Problem status for a team
struct ProblemStatus {
    bool solved;
    int solve_time;
    int wrong_attempts;
    vector<Submission> submissions;

    // For freeze/scroll
    int frozen_wrong_attempts;
    int frozen_submissions_count;
    bool is_frozen;

    ProblemStatus() : solved(false), solve_time(0), wrong_attempts(0),
                      frozen_wrong_attempts(0), frozen_submissions_count(0), is_frozen(false) {}
};

// Team class
class Team {
public:
    string name;
    map<string, ProblemStatus> problems; // problem_name -> status
    int solved_count;
    int penalty_time;
    vector<Submission> all_submissions; // All submissions for queries

    Team() : name(""), solved_count(0), penalty_time(0) {}
    Team(string n) : name(n), solved_count(0), penalty_time(0) {}

    void addSubmission(const string& problem, SubmitStatus status, int time, bool is_frozen) {
        all_submissions.push_back(Submission(problem, status, time));

        if (problems.find(problem) == problems.end()) {
            problems[problem] = ProblemStatus();
        }

        ProblemStatus& ps = problems[problem];
        ps.submissions.push_back(Submission(problem, status, time));

        if (is_frozen && !ps.solved) {
            ps.frozen_submissions_count++;
            if (ps.frozen_submissions_count == 1) {
                ps.is_frozen = true;
                ps.frozen_wrong_attempts = ps.wrong_attempts;
            }
        } else if (!ps.solved) {
            if (status == ACCEPTED) {
                ps.solved = true;
                ps.solve_time = time;
            } else {
                ps.wrong_attempts++;
            }
        }
    }

    void recalculateStats() {
        solved_count = 0;
        penalty_time = 0;

        for (auto& pair : problems) {
            ProblemStatus& ps = pair.second;
            if (ps.solved) {
                solved_count++;
                penalty_time += ps.solve_time + 20 * ps.wrong_attempts;
            }
        }
    }

    vector<int> getSolveTimes() const {
        vector<int> times;
        for (const auto& pair : problems) {
            if (pair.second.solved) {
                times.push_back(pair.second.solve_time);
            }
        }
        sort(times.rbegin(), times.rend()); // Sort in descending order
        return times;
    }
};

// Scoreboard class
class Scoreboard {
public:
    unordered_map<string, Team> teams;
    bool competition_started;
    bool is_frozen;
    int duration_time;
    int problem_count;
    vector<string> problem_names;
    vector<pair<string, int>> rankings; // team_name, rank

    Scoreboard() : competition_started(false), is_frozen(false), duration_time(0), problem_count(0) {}

    bool addTeam(const string& team_name) {
        if (competition_started) {
            cout << "[Error]Add failed: competition has started." << endl;
            return false;
        }
        if (teams.find(team_name) != teams.end()) {
            cout << "[Error]Add failed: duplicated team name." << endl;
            return false;
        }
        teams[team_name] = Team(team_name);
        cout << "[Info]Add successfully." << endl;
        return true;
    }

    bool startCompetition(int duration, int prob_count) {
        if (competition_started) {
            cout << "[Error]Start failed: competition has started." << endl;
            return false;
        }
        competition_started = true;
        duration_time = duration;
        problem_count = prob_count;

        // Generate problem names A, B, C, ...
        problem_names.clear();
        for (int i = 0; i < prob_count; i++) {
            problem_names.push_back(string(1, 'A' + i));
        }

        cout << "[Info]Competition starts." << endl;
        return true;
    }

    void submitProblem(const string& problem, const string& team_name, SubmitStatus status, int time) {
        if (teams.find(team_name) != teams.end()) {
            teams[team_name].addSubmission(problem, status, time, is_frozen);
        }
    }

    void flush(bool silent = false) {
        if (!silent) {
            cout << "[Info]Flush scoreboard." << endl;
        }

        // Recalculate all team stats
        for (auto& pair : teams) {
            pair.second.recalculateStats();
        }

        // Create ranking
        vector<Team*> team_ptrs;
        for (auto& pair : teams) {
            team_ptrs.push_back(&pair.second);
        }

        // Sort teams
        sort(team_ptrs.begin(), team_ptrs.end(), [](Team* a, Team* b) {
            // More solved problems rank higher
            if (a->solved_count != b->solved_count) {
                return a->solved_count > b->solved_count;
            }
            // Less penalty time rank higher
            if (a->penalty_time != b->penalty_time) {
                return a->penalty_time < b->penalty_time;
            }
            // Compare solve times
            vector<int> times_a = a->getSolveTimes();
            vector<int> times_b = b->getSolveTimes();

            size_t min_len = min(times_a.size(), times_b.size());
            for (size_t i = 0; i < min_len; i++) {
                if (times_a[i] != times_b[i]) {
                    return times_a[i] > times_b[i];
                }
            }

            // Lexicographic comparison
            return a->name < b->name;
        });

        // Update rankings
        rankings.clear();
        for (size_t i = 0; i < team_ptrs.size(); i++) {
            rankings.push_back({team_ptrs[i]->name, i + 1});
        }
    }

    bool freeze() {
        if (is_frozen) {
            cout << "[Error]Freeze failed: scoreboard has been frozen." << endl;
            return false;
        }
        is_frozen = true;
        cout << "[Info]Freeze scoreboard." << endl;
        return true;
    }

    void unfreezeOneProblem(Team& team, const string& problem_name) {
        ProblemStatus& ps = team.problems[problem_name];

        if (!ps.is_frozen) return;

        // Process the frozen submissions
        int frozen_start_idx = ps.submissions.size() - ps.frozen_submissions_count;
        for (size_t i = frozen_start_idx; i < ps.submissions.size(); i++) {
            const Submission& sub = ps.submissions[i];
            if (!ps.solved) {
                if (sub.status == ACCEPTED) {
                    ps.solved = true;
                    ps.solve_time = sub.time;
                } else {
                    ps.wrong_attempts++;
                }
            }
        }

        ps.is_frozen = false;
        ps.frozen_submissions_count = 0;
    }

    bool scroll() {
        if (!is_frozen) {
            cout << "[Error]Scroll failed: scoreboard has not been frozen." << endl;
            return false;
        }

        cout << "[Info]Scroll scoreboard." << endl;

        // First flush
        flush(true);

        // Output scoreboard before scrolling
        outputScoreboard();

        // Perform scroll operation
        while (true) {
            // Save current rankings to detect changes
            map<string, int> old_rankings_map;
            map<int, string> old_rank_to_team;
            for (const auto& rank_pair : rankings) {
                old_rankings_map[rank_pair.first] = rank_pair.second;
                old_rank_to_team[rank_pair.second] = rank_pair.first;
            }

            // Find lowest-ranked team with frozen problems
            string target_team = "";
            int lowest_rank = -1;

            for (const auto& rank_pair : rankings) {
                const string& team_name = rank_pair.first;
                int rank = rank_pair.second;
                Team& team = teams[team_name];

                // Check if this team has frozen problems
                bool has_frozen = false;
                for (const auto& prob_pair : team.problems) {
                    if (prob_pair.second.is_frozen) {
                        has_frozen = true;
                        break;
                    }
                }

                if (has_frozen && (lowest_rank == -1 || rank > lowest_rank)) {
                    lowest_rank = rank;
                    target_team = team_name;
                }
            }

            // If no team has frozen problems, we're done
            if (target_team == "") break;

            // Find the smallest problem name among frozen problems
            Team& team = teams[target_team];
            string smallest_problem = "";
            for (const string& problem_name : problem_names) {
                if (team.problems.find(problem_name) != team.problems.end() &&
                    team.problems[problem_name].is_frozen) {
                    smallest_problem = problem_name;
                    break;
                }
            }

            // Store old ranking
            int old_rank = lowest_rank;

            // Unfreeze the problem
            unfreezeOneProblem(team, smallest_problem);

            // Recalculate team stats
            team.recalculateStats();

            // Flush to update rankings (silently)
            flush(true);

            // Check if ranking changed
            int new_rank = -1;
            for (const auto& rank_pair : rankings) {
                if (rank_pair.first == target_team) {
                    new_rank = rank_pair.second;
                    break;
                }
            }

            // If ranking changed (improved), output the change
            if (new_rank < old_rank) {
                // Find the team that was at new_rank position before
                string replaced_team = "";
                if (old_rank_to_team.find(new_rank) != old_rank_to_team.end()) {
                    replaced_team = old_rank_to_team[new_rank];
                }

                if (replaced_team != "") {
                    cout << target_team << " " << replaced_team << " "
                         << team.solved_count << " " << team.penalty_time << endl;
                }
            }
        }

        // Output scoreboard after scrolling
        outputScoreboard();

        is_frozen = false;
        return true;
    }

    void outputScoreboard() {
        // Get teams in ranking order
        vector<pair<string, int>> ranked_teams; // (team_name, rank)

        if (rankings.empty()) {
            // No flush yet, use lexicographic order
            vector<string> names;
            for (const auto& pair : teams) {
                names.push_back(pair.first);
            }
            sort(names.begin(), names.end());
            for (size_t i = 0; i < names.size(); i++) {
                ranked_teams.push_back({names[i], i + 1});
            }
        } else {
            // Use current rankings
            ranked_teams = rankings;
        }

        // Output each team's status
        for (const auto& rank_pair : ranked_teams) {
            const string& team_name = rank_pair.first;
            int rank = rank_pair.second;
            const Team& team = teams[team_name];

            // Output: team_name ranking solved_count total_penalty
            cout << team_name << " " << rank << " "
                 << team.solved_count << " " << team.penalty_time;

            // Output problem statuses (A, B, C, ...)
            for (const string& problem_name : problem_names) {
                cout << " ";

                if (team.problems.find(problem_name) == team.problems.end()) {
                    // No submission to this problem
                    cout << ".";
                } else {
                    const ProblemStatus& ps = team.problems.at(problem_name);

                    if (ps.is_frozen) {
                        // Problem is frozen: -x/y or 0/y
                        if (ps.frozen_wrong_attempts == 0) {
                            cout << "0/" << ps.frozen_submissions_count;
                        } else {
                            cout << "-" << ps.frozen_wrong_attempts << "/" << ps.frozen_submissions_count;
                        }
                    } else if (ps.solved) {
                        // Problem is solved: +x or +
                        if (ps.wrong_attempts == 0) {
                            cout << "+";
                        } else {
                            cout << "+" << ps.wrong_attempts;
                        }
                    } else {
                        // Problem not solved and not frozen: -x or .
                        if (ps.wrong_attempts == 0) {
                            cout << ".";
                        } else {
                            cout << "-" << ps.wrong_attempts;
                        }
                    }
                }
            }
            cout << endl;
        }
    }

    void queryRanking(const string& team_name) {
        if (teams.find(team_name) == teams.end()) {
            cout << "[Error]Query ranking failed: cannot find the team." << endl;
            return;
        }

        cout << "[Info]Complete query ranking." << endl;
        if (is_frozen) {
            cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled." << endl;
        }

        // Find ranking
        int rank = -1;
        for (const auto& pair : rankings) {
            if (pair.first == team_name) {
                rank = pair.second;
                break;
            }
        }

        if (rank == -1) {
            // No flush yet, use lexicographic order
            vector<string> names;
            for (const auto& pair : teams) {
                names.push_back(pair.first);
            }
            sort(names.begin(), names.end());
            for (size_t i = 0; i < names.size(); i++) {
                if (names[i] == team_name) {
                    rank = i + 1;
                    break;
                }
            }
        }

        cout << team_name << " NOW AT RANKING " << rank << endl;
    }

    void querySubmission(const string& team_name, const string& problem_filter, const string& status_filter) {
        if (teams.find(team_name) == teams.end()) {
            cout << "[Error]Query submission failed: cannot find the team." << endl;
            return;
        }

        cout << "[Info]Complete query submission." << endl;

        Team& team = teams[team_name];
        Submission* result = nullptr;

        // Search from the end (last submission)
        for (int i = team.all_submissions.size() - 1; i >= 0; i--) {
            const Submission& sub = team.all_submissions[i];

            bool problem_match = (problem_filter == "ALL" || sub.problem == problem_filter);
            bool status_match = (status_filter == "ALL" || statusToString(sub.status) == status_filter);

            if (problem_match && status_match) {
                result = &team.all_submissions[i];
                break;
            }
        }

        if (result == nullptr) {
            cout << "Cannot find any submission." << endl;
        } else {
            cout << team_name << " " << result->problem << " "
                 << statusToString(result->status) << " " << result->time << endl;
        }
    }
};

int main() {
    // Fast I/O for competitive programming
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Scoreboard scoreboard;
    string command;

    // Main command loop
    while (cin >> command) {
        if (command == "ADDTEAM") {
            string team_name;
            cin >> team_name;
            scoreboard.addTeam(team_name);

        } else if (command == "START") {
            string duration_str, problem_str;
            int duration, prob_count;
            cin >> duration_str >> duration >> problem_str >> prob_count;
            scoreboard.startCompetition(duration, prob_count);

        } else if (command == "SUBMIT") {
            string problem, by_str, team_name, with_str, status_str, at_str;
            int time;
            cin >> problem >> by_str >> team_name >> with_str >> status_str >> at_str >> time;
            scoreboard.submitProblem(problem, team_name, stringToStatus(status_str), time);

        } else if (command == "FLUSH") {
            scoreboard.flush();

        } else if (command == "FREEZE") {
            scoreboard.freeze();

        } else if (command == "SCROLL") {
            scoreboard.scroll();

        } else if (command == "QUERY_RANKING") {
            string team_name;
            cin >> team_name;
            scoreboard.queryRanking(team_name);

        } else if (command == "QUERY_SUBMISSION") {
            string team_name, where_str, problem_part, and_str, status_part;
            cin >> team_name >> where_str >> problem_part >> and_str >> status_part;

            // Parse PROBLEM=X and STATUS=Y
            size_t eq_pos = problem_part.find('=');
            string problem_filter = problem_part.substr(eq_pos + 1);

            eq_pos = status_part.find('=');
            string status_filter = status_part.substr(eq_pos + 1);

            scoreboard.querySubmission(team_name, problem_filter, status_filter);

        } else if (command == "END") {
            // End competition
            cout << "[Info]Competition ends." << endl;
            break;
        }
    }

    return 0;
}
