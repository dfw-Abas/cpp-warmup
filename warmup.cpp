// =====================================================================
//  C++ WARM-UP  ·  African Olympiad Academy
//
//  The question descriptions are in README.md. Read them there.
//  This file holds the data, one worked example, and your answers.
//
//  BUILD AND RUN:   g++ -std=c++17 -Wall -o run warmup.cpp && ./run
//            or:    ./run.sh
//
//  Work on ONE question at a time. Write it, run it, check the output
//  against the README, commit it, move on.
// =====================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

/**
 * THE RECORD
 * One delivery: who brought it, how much, and what grade it was given.
 */
struct Delivery {
    string farmer;
    double kg;
    char   grade;
};

/**
 * THE DATA
 * Do not change these. Every expected answer in the README comes from exactly this data.
 */

// Q1 — one morning's weights, in the order they were weighed
vector<double> morningWeights = {
    45.5, 80.0, 12.0, -3.0, 66.5, 20.0, 31.0, 95.0, 50.0, 18.5, 0.0, 72.0
};

// Q2, Q3, Q4, Q7, Q8 — the day's logbook
vector<Delivery> logbook = {
    {"Keza",    45.5, 'A'},
    {"Bonu",    80.0, 'B'},
    {"Isanda",  20.0, 'A'},
    {"Keza",    12.0, 'C'},
    {"Onyinye", 66.5, 'B'},
    {"Karimi",  31.0, 'A'},
    {"Karimi",  31.0, 'A'},
    {"Keza",    18.5, 'B'},
    {"Bonu",    50.0, 'C'},
    {"Isanda",  20.0, 'A'},
    {"Onyinye", 72.0, 'A'}
};   // yes, one of these rows is in here twice. Q8 is about that.

// Q5 — records as the volunteers typed them, before any checking. Several of these are wrong. That is the point.
struct RawRecord {
    string id;
    string kg;        // text, because a volunteer can type anything
    char   grade;
};

vector<RawRecord> rawRecords = {
    {"F012", "45.5",  'A'},
    {"F7",   "20",    'A'},
    {"F033", "heavy", 'B'},
    {"F021", "0",     'C'},
    {"F044", "80",    'b'},
    {"F055", "",      'A'},
    {"F066", "31.5",  'D'},
    {"F077", "92",    'B'}
};

// Q6 — an Igisoro board. 16 pits, in sowing order.
//      Position 0 is unused so that board really is pit 1.
vector<int> board = {0, 0, 0, 1, 0, 0, 4, 1, 0, 0, 2, 2, 0, 1, 5, 0, 0};

// ---------------------------------------------------------------------
//  A HELPER YOU MAY FIND USEFUL
// ---------------------------------------------------------------------
void printBoard(const vector<int> &b) {
    for (int i = 1; i <= 16; i++) cout << b[i] << " ";
    cout << "\n";
}

int findFarmer(const vector<Delivery> &log, string name);
double payment(double kg, char grade);

int main() {

    // =================================================================
    //  WORKED EXAMPLE  ·  not a question
    //
    //  This is the house style: walk the data once, keep what you need
    //  as you go, print at the end. Read it before you start Q1.
    //
    //  It answers: how many deliveries were grade A, and what was the
    //  heaviest single delivery in the whole logbook?
    // =================================================================

    int gradeACount = 0;
    Delivery heaviest = logbook[0];        // start from a REAL entry,
                                           // never from a made-up 0
    for (Delivery d : logbook) {
        if (d.grade == 'A') {
            gradeACount++;
        }
        if (d.kg > heaviest.kg) {
            heaviest = d;                  // keep the WHOLE record,
        }                                  // not just the number
    }

    cout << "=== Worked example ===\n";
    cout << "Grade A deliveries: " << gradeACount << "\n";
    cout << "Heaviest: " << heaviest.farmer << " with "
         << heaviest.kg << " kg\n\n";


    // =================================================================
    //  Q1  ·  THE WEIGHING SESSION          (see README)
    //  Uses: morningWeights
    // =================================================================

    cout << "=== Q1 ===\n";
    {
        double total = 0;
        int valid = 0;
        double heaviestWeight = morningWeights[0];
        int heaviestPos = 0;
        int maxRun = 0;
        int currentRun = 0;

        for (size_t i = 0; i < morningWeights.size(); i++) {
            double w = morningWeights[i];
            total += w;
            if (w > heaviestWeight) {
                heaviestWeight = w;
                heaviestPos = i;
            }
            if (w > 0 && w <= 80) {
                valid++;
                currentRun++;
                if (currentRun > maxRun) {
                    maxRun = currentRun;
                }
            } else {
                currentRun = 0;
            }
        }
        cout << "Total: " << total << " kg\n";
        cout << "Valid: " << valid << " of 12\n";
        cout << "Heaviest: " << heaviestWeight << " kg at position " << heaviestPos << "\n";
        cout << "Longest valid run: " << maxRun << "\n";
    }
    cout << "\n";


    // =================================================================
    //  Q2  ·  GRADE SUMMARY                 (see README)
    //  Uses: logbook
    // =================================================================

    cout << "=== Q2 ===\n";
    {
        int countA = 0, countB = 0, countC = 0;
        double totalA = 0, totalB = 0, totalC = 0;

        for (const auto &d : logbook) {
            if (d.grade == 'A') { countA++; totalA += d.kg; }
            else if (d.grade == 'B') { countB++; totalB += d.kg; }
            else if (d.grade == 'C') { countC++; totalC += d.kg; }
        }

        double avgA = (countA > 0) ? (totalA / countA) : 0;
        double avgB = (countB > 0) ? (totalB / countB) : 0;
        double avgC = (countC > 0) ? (totalC / countC) : 0;

        cout << "Grade A: " << countA << " deliveries, " << totalA << " kg total, " << avgA << " kg average\n";
        cout << "Grade B: " << countB << " deliveries, " << totalB << " kg total, " << avgB << " kg average\n";
        cout << "Grade C: " << countC << " deliveries, " << totalC << " kg total, " << avgC << " kg average\n";

        char bestGrade = 'A';
        double bestAvg = avgA;
        if (avgB > bestAvg) { bestAvg = avgB; bestGrade = 'B'; }
        if (avgC > bestAvg) { bestAvg = avgC; bestGrade = 'C'; }

        cout << "Best average: grade " << bestGrade << "\n";
    }
    cout << "\n";


    // =================================================================
    //  Q3  ·  WHO IS WHO                    (see README)
    //  Uses: logbook, and the findFarmer function you write below main
    // =================================================================

    cout << "=== Q3 ===\n";
    {
        cout << "findFarmer(Isanda) = " << findFarmer(logbook, "Isanda") << "\n";
        cout << "findFarmer(Mukamana) = " << findFarmer(logbook, "Mukamana") << "\n";

        vector<string> farmers;
        vector<int> counts;
        vector<double> totals;

        for (const auto &d : logbook) {
            auto it = find(farmers.begin(), farmers.end(), d.farmer);
            if (it == farmers.end()) {
                farmers.push_back(d.farmer);
                counts.push_back(1);
                totals.push_back(d.kg);
            } else {
                size_t idx = distance(farmers.begin(), it);
                counts[idx]++;
                totals[idx] += d.kg;
            }
        }

        string mostDelivName = "";
        int maxDeliv = -1;
        string mostKgName = "";
        double maxKg = -1;

        for (size_t i = 0; i < farmers.size(); i++) {
            if (counts[i] > maxDeliv) {
                maxDeliv = counts[i];
                mostDelivName = farmers[i];
            }
            if (totals[i] > maxKg) {
                maxKg = totals[i];
                mostKgName = farmers[i];
            }
        }

        cout << "Most deliveries: " << mostDelivName << " (" << maxDeliv << ")\n";
        cout << "Most kg: " << mostKgName << " (" << maxKg << " kg)\n";
    }
    cout << "\n";


    // =================================================================
    //  Q4  ·  PAYMENT RUN WITH A DAILY CAP  (see README)
    //  Uses: logbook, and the payment function you write below main
    // =================================================================

    cout << "=== Q4 ===\n";
    {
        double totalPaid = 0;
        for (const auto &d : logbook) {
            double p = payment(d.kg, d.grade);
            bool halved = false;
            if (totalPaid > 100000) {
                p /= 2.0;
                halved = true;
            }
            totalPaid += p;
            cout << d.farmer << " " << d.kg << " kg grade " << d.grade << " -> " << p;
            if (halved) cout << "  (halved)";
            cout << "\n";
        }
        cout << "Total paid: " << totalPaid << "\n";
    }
    cout << "\n";


    // =================================================================
    //  Q5  ·  THE VALIDATION REPORT         (see README)
    //  Uses: rawRecords
    // =================================================================

    cout << "=== Q5 ===\n";
    {
        int accepted = 0;
        int rejected = 0;

        for (const auto &r : rawRecords) {
            bool valid = true;
            string reason = "";

            if (r.id.length() != 4 || r.id[0] != 'F' || !isdigit(r.id[1]) || !isdigit(r.id[2]) || !isdigit(r.id[3])) {
                valid = false;
                reason = "bad id";
            }

            if (valid) {
                if (r.kg.empty()) {
                    valid = false;
                    reason = "weight missing";
                } else {
                    bool isNum = true;
                    size_t dots = 0;
                    for (char c : r.kg) {
                        if (c == '.') dots++;
                        else if (!isdigit(c)) isNum = false;
                    }
                                    if (!isNum || dots > 1) {
                    valid = false;
                    reason = "weight not a number";
                } else {
                    double weight = stod(r.kg);
                    if (weight <= 0 || weight > 80) {
                        valid = false;
                        reason = "weight out of range";
                    }
                }
            }
        }

        if (valid) {
            char g = toupper(r.grade);
            if (g != 'A' && g != 'B' && g != 'C') {
                valid = false;
                reason = "unknown grade";
            }
        }

        if (valid) {
            cout << r.id << " ACCEPT\n";
            accepted++;
        } else {
            cout << r.id << " REJECT - " << reason << "\n";
            rejected++;
        }
    }
    cout << accepted << " accepted, " << rejected << " rejected\n";
    
    cout << "\n";

    // =================================================================
    //  Q6  ·  IGISORO: A FULL TURN          (see README)
    //  Uses: board
    // =================================================================
    cout << "=== Q6 ===\n";
    
        int current = 6;
        int hand = board[current];
        board[current] = 0;
        int totalPits = 0;
        while (hand > 0) {
            current = (current % 16) + 1;
            totalPits++;
            board[current]++;
            hand--;
            if (hand == 0 && board[current] > 1) {
                hand = board[current];
                board[current] = 0;
            }
        }
        int laps = (totalPits + 15) / 16;
        cout << "Final board: ";
        printBoard(board);
        cout << "Landed in pit " << current << " after " << laps << " laps\n";
    

    // =================================================================
    //  STRETCH  ·  optional
    // =================================================================

    // =================================================================
    //  Q7  ·  SORT THE LOGBOOK              (see README)
    //  Uses: logbook
    // =================================================================
    // cout << "=== Q7 ===\n";

    // =================================================================
    //  Q8  ·  SPOTTING DOUBLE-ENTRIES       (see README)
    //  Uses: logbook
    // =================================================================
    // cout << "=== Q8 ===\n";

    return 0;
}
}
// =====================================================================
//  FUNCTIONS YOU WRITE
//
//  C++ reads a file top to bottom, so a function must be DECLARED above
//  the place that calls it. These two are declared here at the bottom,
//  which main() cannot see — so move them ABOVE main(), or leave a
//  one-line declaration up there. Working that out is part of Q3.
// =====================================================================

// Q3:  return the position of the first delivery by this farmer,
//      or -1 if the farmer is not in the log at all.
//
int findFarmer(const vector<Delivery> &log, string name) {
    for (size_t i = 0; i < log.size(); i++) {
        if (log[i].farmer == name) return i;
    }
    return -1;
}


// Q4:  what one delivery earns, before any cap is applied.
//      Grade A pays 10% above the rate, B pays the rate,
//      anything else pays 20% below it. The rate is 450.
//
double payment(double kg, char grade) {
    double rate = 450.0;
    if (grade == 'A') return kg * rate * 1.10;
    if (grade == 'B') return kg * rate;
    return kg * rate * 0.80;
}

