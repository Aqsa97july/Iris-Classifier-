#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <map>
#include <random>

using namespace std;

struct Flower {
    double features[4];
    string species;
};

// Load dataset from CSV
vector<Flower> loadData(const string& filename) {
    vector<Flower> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string val;
        Flower f;

        for (int i = 0; i < 4; i++) {
            getline(ss, val, ',');
            f.features[i] = stod(val);
        }

        getline(ss, f.species, ',');
        data.push_back(f);
    }

    return data;
}

// KNN Prediction function
string predict(vector<Flower>& trainData, Flower testPoint, int k) {
    vector<pair<double, string>> distances;

    for (auto &f : trainData) {
        double dist = 0;

        for (int i = 0; i < 4; i++) {
            dist += pow(testPoint.features[i] - f.features[i], 2);
        }

        distances.push_back({sqrt(dist), f.species});
    }

    sort(distances.begin(), distances.end());

    map<string, int> votes;

    for (int i = 0; i < k; i++) {
        votes[distances[i].second]++;
    }

    string best;
    int maxVotes = 0;

    for (auto &v : votes) {
        if (v.second > maxVotes) {
            maxVotes = v.second;
            best = v.first;
        }
    }

    return best;
}

int main() {
    // Load data
    vector<Flower> data = loadData("iris.csv");

    if (data.empty()) {
        cout << "Dataset not loaded." << endl;
        return 1;
    }

    // Shuffle data
    random_device rd;
    mt19937 g(rd());
    shuffle(data.begin(), data.end(), g);

    // Train-test split
    int trainSize = data.size() * 0.8;
    vector<Flower> trainData(data.begin(), data.begin() + trainSize);

    int k = 3;

    // User input
    Flower userInput;

    cout << "Enter Iris flower features:\n";

    cout << "Sepal Length: ";
    cin >> userInput.features[0];

    cout << "Sepal Width: ";
    cin >> userInput.features[1];

    cout << "Petal Length: ";
    cin >> userInput.features[2];

    cout << "Petal Width: ";
    cin >> userInput.features[3];

    // Prediction
    string result = predict(trainData, userInput, k);

    cout << "\nPredicted Species: " << result << endl;

    return 0;
}