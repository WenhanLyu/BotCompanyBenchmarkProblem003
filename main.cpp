#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <cctype>

using namespace std;

class ICPCManagementSystem {
private:
    set<string> teams;
    bool competition_started;
    int duration_time;
    int problem_count;

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
        // TODO: Implement for later milestones
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
