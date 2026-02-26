#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/**
 * ICPC Management System
 *
 * This program implements a management system for ICPC competitions,
 * handling team registration, submissions, scoreboard management,
 * freeze/scroll operations, and various queries.
 */

int main() {
    // Fast I/O for competitive programming
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string command;

    // Main command loop
    while (cin >> command) {
        if (command == "ADDTEAM") {
            // TODO: Implement ADDTEAM command

        } else if (command == "START") {
            // TODO: Implement START command

        } else if (command == "SUBMIT") {
            // TODO: Implement SUBMIT command

        } else if (command == "FLUSH") {
            // TODO: Implement FLUSH command

        } else if (command == "FREEZE") {
            // TODO: Implement FREEZE command

        } else if (command == "SCROLL") {
            // TODO: Implement SCROLL command

        } else if (command == "QUERY_RANKING") {
            // TODO: Implement QUERY_RANKING command

        } else if (command == "QUERY_SUBMISSION") {
            // TODO: Implement QUERY_SUBMISSION command

        } else if (command == "END") {
            // End competition
            cout << "[Info]Competition ends." << endl;
            break;
        }
    }

    return 0;
}
