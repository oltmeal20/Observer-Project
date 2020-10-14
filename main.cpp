#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
//#include <math.h>
#include <cmath>

using namespace std;

// base class used by classes that do OBSERVING!
class Observer {
public:
    virtual void update(int runs, int hits,
        float win_average, int innings) = 0;
};

class Subject {
public:
    virtual void registerObserver(Observer* o) = 0;
    virtual void unregisterObserver(Observer* o) = 0;
    virtual void notifyObservers() = 0;
};

class SportsData :public Subject {
private:
    int runs;
    int hits;
    float win_average;
    int innings;
    vector<Observer*> observerList;

public:
    SportsData() {
        srand(time(0));  // Initialize random number generator.
    }

    void registerObserver(Observer* o) {
        observerList.push_back(o);
    }

    void unregisterObserver(Observer* o) {
        for (int i = 0; i < observerList.size(); i++) {
            if (observerList.at(i) == o) {
                observerList.erase(observerList.begin() + i);
            }
        }
    }

    void notifyObservers() {
        for (int i = 0; i < observerList.size(); i++) {
            if (observerList.at(i) != nullptr) {
                observerList.at(i)->update(runs, hits, win_average, innings);
            }
        }
    }

    // This method is used update displays
    // when data changes
    void dataChanged() {
        //get latest data
        hits = getLatestHits();
        runs = getLatestRuns();
        win_average = getLatestWinAverage();
        innings = getInnings();

        notifyObservers();
    }

private:
    int getLatestRuns() {
        int value = rand() % 10;
        return (value > hits) ? hits : value;
    }


    int getLatestHits() {
        return (rand() % 10);
    }

    float getLatestWinAverage() {
        return floorf(((float)rand() / (RAND_MAX)) * 1000) / 1000;
    }

    int getInnings() {
        int value = rand() % 10;
        value += 1;
        return value;
    }
};

class CalcScoreDisplay : public Observer {
private:
    float runRate;

public:
    void update(int runs, int hits,
        float win_average, int innings) {

        runRate = (hits > 0) ? (ceilf(((float)runs / (float)hits) * 1000) / 1000) : 0;
        display();
    }

    void display() {
        cout << "\nRuns Per Hit: \n" << runRate << "\n";
    }
};

class CurrentScoreDisplay : public Observer {
private:
    int runs, hits;
    float win_average;

public:
    void update(int new_runs, int new_hits,
        float new_win_average, int innings) {
        runs = new_runs;
        hits = new_hits;
        win_average = new_win_average;
        display();
    }

    void display() {
        cout << "\nCurrent Score Display:\n"
            << "Runs: " << runs <<
            "\nHits: " << hits <<
            "\nWin Average: " << win_average << "\n";
    }
};

class EraDisplay : public Observer {
private:
    float innings, runs, era;

public:
    void update(int new_runs, int new_hits,
        float new_win_average, int new_innings) {
        runs = new_runs;
        innings = new_innings;

        era = (runs / innings) * 9;
        display();
    }

    void display() {
        cout << "\nERA Display:\n"
            << "Inning: " << innings << 
            "\nERA: " << era << "\n";
    }
};

int main() {
    // create objects for testing
    CalcScoreDisplay calcScoreDisplay = CalcScoreDisplay();
    CurrentScoreDisplay currentScoreDisplay = CurrentScoreDisplay();

    /*** My added object ***/ 
    EraDisplay eraDisplay = EraDisplay();


    // pass the displays to BASEBALL!!! data
    SportsData sportsData = SportsData();

    // register display elements
    sportsData.registerObserver(&calcScoreDisplay);
    sportsData.registerObserver(&currentScoreDisplay);
    sportsData.registerObserver(&eraDisplay);

    // in real app you would have some logic to
    // call this function when data changes
    sportsData.dataChanged();

    //remove an observer
    sportsData.unregisterObserver(&calcScoreDisplay);

    // now only currentScoreDisplay gets the
    // notification
    sportsData.dataChanged();
}