#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <list>

using namespace std;

class CityInfo {
    
private:
    string cityName;
    int plateNum;
    vector<int> distances;
    map<string, int> adjacentCities;
    
public:
    CityInfo() {
        cityName=" ";
        // plateNum=-1;
        //distances.push_back(-1);
        //adjacentCities.insert(std::make_pair("empty", -1));
        
        
    }
    
    ~CityInfo() {}
    
    string getCityName() const { return cityName; }
    void setCityName(string _cityName) { cityName = _cityName; }
    
    int getPlateNum() const { return plateNum; }
    void setPlateNum(int _plateNum) { plateNum = _plateNum; }
    
    vector<int> getDistances() const { return distances; }
    void setDistances(int _distances) { distances.push_back(_distances); }
    
    map<string, int> getAdjacents() const { return adjacentCities; }
    void setAdjacents(string city, int plateNum) { adjacentCities.insert({city, plateNum}); }
    
    void printInfo() const{
        cout << "City Name: " << cityName << endl;
        cout << "Plate Number: " << plateNum << endl;
        
        /*
         cout << "Distances: ";
         for (int distance : distances) {
         cout << distance << " ";
         }
         cout << endl;
         */
        cout << "Adjacent Cities: ";
        for (const auto& entry : adjacentCities) {
            cout << entry.first << " (Plate: " << entry.second << ", Distance: ";
            int distanceToAdjacent = distances[entry.second - 1];
            cout << distanceToAdjacent << "), ";
        }
        cout << endl;
    }
    
    void listClosestCities(int k, const CityInfo _City[]) const {
        vector<pair<int, int>> cityDistances;

        for (int i = 1; i <= 81; ++i) {
            if (i != plateNum) {                    //do not check selected city
                int distanceToCity = distances[i - 1];
                cityDistances.push_back(make_pair(i, distanceToCity));
            }
        }

        sort(cityDistances.begin(), cityDistances.end(), [](const pair<int, int>& a, const pair<int, int>& b) { //sort(begin_iterator, end_iterator, comparison_function) cpp's function. mix of                                                                                                           merge, quick, heap
            return a.second < b.second;
        });

        cout << "Closest Cities to " << cityName << " (" << plateNum << "):" << endl;
        for (int i = 0; i < k && i < cityDistances.size(); ++i) {
            cout << "City " << i + 1 << ": " << _City[cityDistances[i].first].getCityName() //plaka kodundan ismi çağırıyor
                 <<"("<< cityDistances[i].first << ") Distance: " << cityDistances[i].second << "." << endl;
        }
    }
    
    void BFS_search(const CityInfo _City[], string _destination) {
            int destinationPlateNumber = -1;

            for (int i = 1; i <= 81; i++) {
                if (_City[i].getCityName() == _destination) {
                    destinationPlateNumber = _City[i].getPlateNum();
                    break;
                }
            }

            if (destinationPlateNumber == -1) {
                cout << "Destination city not found." << endl;
                return;
            }

            queue<int> bfsQueue;
            vector<bool> visited(82, false);
            vector<int> parent(82, -1);

            bfsQueue.push(plateNum);
            visited[plateNum] = true;

            while (!bfsQueue.empty()) {
                int currentCityPlate = bfsQueue.front();
                bfsQueue.pop();

                if (currentCityPlate == destinationPlateNumber) {
                    cout << "Shortest path from " << cityName << " to " << _destination << ": ";
                    printPathDetails(_City, parent, destinationPlateNumber);
                    cout << endl;
                    return;
                }

                for (const auto& entry : _City[currentCityPlate].getAdjacents()) {
                    int neighborPlate = entry.second;

                    if (!visited[neighborPlate]) {
                        bfsQueue.push(neighborPlate);
                        visited[neighborPlate] = true;
                        parent[neighborPlate] = currentCityPlate;
                    }
                }
            }

            cout << "There is no path from " << cityName << " to " << _destination << "." << endl;
        }

        void printPathDetails(const CityInfo _City[], const vector<int>& parent, int destinationPlate) const {
            vector<int> path;
            int sumDistance= 0;

            int currentPlate = destinationPlate;
            while (currentPlate != -1) {
                path.push_back(currentPlate);
                currentPlate = parent[currentPlate];
            }

         //reverse the order. starting from main city
            for (int i = path.size() - 1; i >= 0; --i) {
                cout << _City[path[i]].getCityName();
                if (i > 0) {
                    int distanceToNextCity = _City[path[i]].getDistances()[path[i - 1] - 1];
                    cout << " ->(" << distanceToNextCity << " km)-> ";
                    sumDistance= sumDistance+distanceToNextCity;
                }
            }
            cout<<endl;
            cout<<"Total Distance: "<< sumDistance<<endl;
        }
    };



             
          
  
    
    
    void setCityNamesAndPlateNumsFromFile(CityInfo _City[], istringstream& Line) {
        string CityName;
        //int PlateNum;
        
        for (int i = 1; i <= 81; i++) {
            getline(Line, CityName, ';');
            _City[i].setCityName(CityName);
            _City[i].setPlateNum(i);
        }
    }
    
    void setNeighbourCityDataFromFile(CityInfo _City[], istringstream& Line) {
        string MainCityName, NeighbourCityName;
        
        getline(Line, MainCityName, ',');
        
        for (int i = 1; i <= 81; i++) {
            for (auto& x : MainCityName) {
                x = tolower(x);
            }
            
            if (_City[i].getCityName() == MainCityName) {
                while (getline(Line, NeighbourCityName, ',')) {
                    for (auto& x : NeighbourCityName) {
                        x = tolower(x);
                    }
                    
                    // Remove leading and trailing whitespaces
                    NeighbourCityName.erase(0, NeighbourCityName.find_first_not_of(" \t\n\r\f\v"));
                    NeighbourCityName.erase(NeighbourCityName.find_last_not_of(" \t\n\r\f\v") + 1);
                    
                    for (int j = 1; j <= 81; j++) {
                        if (_City[j].getCityName() == NeighbourCityName) {
                            _City[i].setAdjacents(NeighbourCityName, j);
                        }
                    }
                }
            }
        }
    }




void setDistances(CityInfo _City[], istringstream & Line) {
    string Name, Plate_s;
    int Plate_i;

    getline(Line, Plate_s, ';');
    Plate_i = stoi(Plate_s);
    getline(Line, Name, ';');

    for (int i = 1; i <= 81; i++) {
        if (_City[i].getCityName() == Name) {
            for (int j = 1; j <= 81; j++) {
                string Distance_s;
                int Distance_i;

                getline(Line, Distance_s, ';');
                Distance_i = stoi(Distance_s);

                _City[i].setDistances(Distance_i);
            }
        }
    }
}

void printCities(const CityInfo _City[], int plateNum) {
    for (int i = 1; i <= 81; i++) {
        if (i == plateNum) {
            _City[i].printInfo();
        }
    }
}






int main() {
    ifstream File_In_CityDistances("/Users/sarpererbar/Programs/C++/CENG383_2/CENG383_2/Resources/CityDistances.txt");
    ifstream File_In_AdjacentCities("/Users/sarpererbar/Programs/C++/CENG383_2/CENG383_2/Resources/adjacent_cities.txt");
           
    string NextLineOfAdjacentCities;
    string NextLineOfCityDistances;

    CityInfo CityOfTurkey[82];

    if (File_In_AdjacentCities.is_open() && File_In_CityDistances.is_open()) {
        getline(File_In_CityDistances, NextLineOfCityDistances, '\n');
        istringstream NextLineOfCityDistances_stream(NextLineOfCityDistances);

        setCityNamesAndPlateNumsFromFile(CityOfTurkey, NextLineOfCityDistances_stream);
        for (int i = 1; i <= 81; i++) {
            getline(File_In_CityDistances, NextLineOfCityDistances, '\n');
            istringstream NextLineOfCityDistances_stream(NextLineOfCityDistances);

            setDistances(CityOfTurkey, NextLineOfCityDistances_stream);
        }

        for (int i = 1; i <= 81; i++) {
            getline(File_In_AdjacentCities, NextLineOfAdjacentCities, '\n');
            istringstream NextLineOfAdjacentCities_stream(NextLineOfAdjacentCities);

            setNeighbourCityDataFromFile(CityOfTurkey, NextLineOfAdjacentCities_stream);
        }
    }
    string userCityName = " ";
    int userPlateNumber=-1;
    while (true) {
            cout << "Menu:" << endl;
            cout << "a. Enter city" << endl;
            cout << "b. Print selected city" << endl;
            cout << "c. List k closest cities" << endl;
            cout << "d. Find shortest path to" << endl;
            cout << "x. Exit" << endl;

            cout << "Enter your choice: ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
       

            switch (choice) {
                case 'a': {
                    cout << "Enter city (plate id or name): ";
                
                    getline(cin, userCityName);
                    
                    for (int i = 0; i <= 81; i++) {
                        if (CityOfTurkey[i].getCityName() == userCityName) {
                            userPlateNumber = CityOfTurkey[i].getPlateNum();
                            break;
                            
                        }
                       }
                    cout<<"Selected City is "<<CityOfTurkey[userPlateNumber].getCityName()<<endl;
                    break;
                }
                case 'b':
                    CityOfTurkey[userPlateNumber].printInfo();
                    
                    
                    break;
                case 'c': {
                    cout << "Enter k: ";
                    int k;
                    cin >> k;
                    CityOfTurkey[userPlateNumber].listClosestCities(k,CityOfTurkey);
                    break;
                }
                case 'd': {
                    cout << "Enter destination city (plate id or name): ";
                    string destinationCity;
                    getline(cin, destinationCity);
                    CityOfTurkey[userPlateNumber].BFS_search(CityOfTurkey, destinationCity);
                    
                    
                    
                    
                    break;
                }
                case 'x':
                    File_In_CityDistances.close();
                    File_In_AdjacentCities.close();
                    return 0;
                default:
                    cout << "Invalid choice. Try again." << endl;
            }
        }
    
    
    
    


    return 0;
}
