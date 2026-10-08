#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <algorithm>
using namespace std;

// Stores emergency details
struct Emergency
{
    int id;
    string patientName;
    string location;
    int severity;
    string condition;
    bool assigned;

    // Higher priority for smaller severity number
    bool operator<(const Emergency &other) const
    {
        return severity > other.severity;
    }
};

// Stores ambulance details
struct Ambulance
{
    int id;
    string currentLocation;
    bool available;
};

// Stores hospital details
struct Hospital
{
    string name;
    string location;
};

// Stores road information
struct Edge
{
    int destination;
    int distance;
};

class MediRoute
{
private:
    // Stores all locations
    vector<string> locations;

    // Stores roads between locations
    vector<vector<Edge>> graph;

    // Stores ambulances
    vector<Ambulance> ambulances;

    // Stores hospitals
    vector<Hospital> hospitals;

    // Stores emergencies by priority
    priority_queue<Emergency> emergencyQueue;

    // Stores completed emergency IDs
    queue<int> completedCases;

    int emergencyCounter;

public:
    // Constructor
    MediRoute()
    {
        emergencyCounter = 1;

        // Add locations
        locations = {
            "City Center",
            "Railway Station",
            "Airport",
            "Civil Hospital",
            "University",
            "Market",
            "Industrial Area",
            "Residential Area"
        };

        graph.resize(locations.size());

        createRoads();
        createAmbulances();
        createHospitals();
    }

    // Add a road between two locations
    void addRoad(int a, int b, int distance)
    {
        graph[a].push_back({b, distance});
        graph[b].push_back({a, distance});
    }

    // Create roads
    void createRoads()
    {
        addRoad(0, 1, 4);
        addRoad(0, 2, 8);
        addRoad(0, 3, 3);
        addRoad(0, 4, 5);

        addRoad(1, 2, 5);
        addRoad(1, 5, 3);

        addRoad(2, 6, 6);

        addRoad(3, 4, 2);
        addRoad(3, 7, 4);

        addRoad(4, 5, 4);

        addRoad(5, 6, 5);
        addRoad(5, 7, 6);

        addRoad(6, 7, 3);
    }

    // Add ambulances across different starting locations
    void createAmbulances()
    {
        ambulances.push_back({1, "City Center", true});
        ambulances.push_back({2, "Railway Station", true});
        ambulances.push_back({3, "Civil Hospital", true});
        ambulances.push_back({4, "University", true});
    }

    // Add hospitals
    void createHospitals()
    {
        hospitals.push_back({"Civil Hospital", "Civil Hospital"});
        hospitals.push_back({"City Medical Center", "City Center"});
        hospitals.push_back({"University Hospital", "University"});
    }

    // Find location number
    int getLocationIndex(string location)
    {
        for (int i = 0; i < locations.size(); i++)
        {
            if (locations[i] == location)
                return i;
        }

        return -1;
    }

    // Find the shortest path using Dijkstra
    pair<int, vector<int>> dijkstra(string source, string destination)
    {
        int src = getLocationIndex(source);
        int dest = getLocationIndex(destination);

        int n = locations.size();

        vector<int> distance(n, INT_MAX);
        vector<int> parent(n, -1);

        // Priority queue for shortest distance
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        distance[src] = 0;
        pq.push({0, src});

        while (!pq.empty())
        {
            int currentDistance = pq.top().first;
            int currentNode = pq.top().second;

            pq.pop();

            if (currentDistance > distance[currentNode])
                continue;

            // Check connected locations
            for (Edge edge : graph[currentNode])
            {
                int newDistance = currentDistance + edge.distance;

                if (newDistance < distance[edge.destination])
                {
                    distance[edge.destination] = newDistance;
                    parent[edge.destination] = currentNode;

                    pq.push({newDistance, edge.destination});
                }
            }
        }

        vector<int> path;

        if (distance[dest] == INT_MAX)
            return {INT_MAX, path};

        // Create the shortest path
        int current = dest;

        while (current != -1)
        {
            path.push_back(current);
            current = parent[current];
        }

        reverse(path.begin(), path.end());

        return {distance[dest], path};
    }

    // Display all locations
    void displayLocations()
    {
        cout << "\n========== AVAILABLE LOCATIONS ==========\n";

        for (int i = 0; i < locations.size(); i++)
        {
            cout << i + 1 << ". " << locations[i] << endl;
        }
    }

    // Register a new emergency
    void registerEmergency()
    {
        Emergency e;

        e.id = emergencyCounter++;

        cout << "\n========== REGISTER EMERGENCY ==========\n";

        cout << "Patient Name: ";
        cin.ignore();
        getline(cin, e.patientName);

        displayLocations();

        int choice;

        cout << "Enter patient location: ";
        cin >> choice;

        if (choice < 1 || choice > locations.size())
        {
            cout << "Invalid location.\n";
            return;
        }

        e.location = locations[choice - 1];

        cout << "\nEnter patient condition: ";
        cin.ignore();
        getline(cin, e.condition);

        cout << "\nSeverity Level:\n";
        cout << "1. Critical\n";
        cout << "2. Serious\n";
        cout << "3. Moderate\n";

        cout << "Enter severity: ";
        cin >> e.severity;

        if (e.severity < 1 || e.severity > 3)
        {
            cout << "Invalid severity.\n";
            return;
        }

        e.assigned = false;

        // Add emergency to priority queue
        emergencyQueue.push(e);

        cout << "\nEmergency registered successfully!";
        cout << "\nEmergency ID: " << e.id;

        if (e.severity == 1)
            cout << "\nPriority: CRITICAL";
        else if (e.severity == 2)
            cout << "\nPriority: SERIOUS";
        else
            cout << "\nPriority: MODERATE";

        cout << "\n";
    }

    // Find the nearest available ambulance
    int findNearestAmbulance(string emergencyLocation)
    {
        int selectedAmbulance = -1;
        int shortestDistance = INT_MAX;

        for (int i = 0; i < ambulances.size(); i++)
        {
            if (!ambulances[i].available)
                continue;

            // Find distance from ambulance to patient
            auto result = dijkstra(ambulances[i].currentLocation, emergencyLocation);

            int distance = result.first;

            if (distance < shortestDistance)
            {
                shortestDistance = distance;
                selectedAmbulance = i;
            }
        }

        return selectedAmbulance;
    }

    // Find the nearest hospital
    string findNearestHospital(string emergencyLocation)
    {
        string nearestHospital = "";
        int shortestDistance = INT_MAX;

        for (Hospital hospital : hospitals)
        {
            auto result = dijkstra(emergencyLocation, hospital.location);

            if (result.first < shortestDistance)
            {
                shortestDistance = result.first;
                nearestHospital = hospital.name;
            }
        }

        return nearestHospital;
    }

    // Dispatch an ambulance
    void dispatchAmbulance()
    {
        if (emergencyQueue.empty())
        {
            cout << "\nNo pending emergencies.\n";
            return;
        }

        // Get the highest priority emergency
        Emergency current = emergencyQueue.top();
        emergencyQueue.pop();

        cout << "\n========== DISPATCH SYSTEM ==========\n";
        cout << "Emergency ID: " << current.id << endl;
        cout << "Patient: " << current.patientName << endl;
        cout << "Location: " << current.location << endl;

        if (current.severity == 1)
            cout << "Priority: CRITICAL\n";
        else if (current.severity == 2)
            cout << "Priority: SERIOUS\n";
        else
            cout << "Priority: MODERATE\n";

        // Find nearest ambulance
        int ambulanceIndex = findNearestAmbulance(current.location);

        if (ambulanceIndex == -1)
        {
            cout << "\nNo ambulance is available.\n";
            emergencyQueue.push(current);
            return;
        }

        Ambulance &ambulance = ambulances[ambulanceIndex];

        // Find shortest route
        auto route = dijkstra(ambulance.currentLocation, current.location);

        cout << "\nAmbulance Assigned: " << ambulance.id << endl;
        cout << "Starting Location: " << ambulance.currentLocation << endl;
        cout << "Distance: " << route.first << " km\n";

        cout << "\nShortest Route:\n";

        for (int i = 0; i < route.second.size(); i++)
        {
            cout << locations[route.second[i]];
            if (i != route.second.size() - 1)
                cout << " -> ";
        }

        cout << "\n";

        int eta = route.first * 2;
        cout << "Estimated ETA: " << eta << " minutes\n";

        string hospital = findNearestHospital(current.location);
        cout << "\nNearest Hospital: " << hospital << endl;

        ambulance.available = false;

        cout << "\nAmbulance " << ambulance.id << " dispatched successfully.\n";

        completedCases.push(current.id);
    }

    // Make ambulance available again
    void completeTrip()
    {
        int ambulanceID;

        cout << "\nEnter ambulance ID: ";
        cin >> ambulanceID;

        for (Ambulance &ambulance : ambulances)
        {
            if (ambulance.id == ambulanceID)
            {
                ambulance.available = true;
                cout << "\nAmbulance " << ambulance.id << " is now AVAILABLE.\n";
                return;
            }
        }

        cout << "\nAmbulance not found.\n";
    }

    // Display ambulance status
    void displayAmbulances()
    {
        cout << "\n========== AMBULANCE STATUS ==========\n";

        for (Ambulance ambulance : ambulances)
        {
            cout << "Ambulance " << ambulance.id
                 << " | Location: " << ambulance.currentLocation
                 << " | Status: ";

            if (ambulance.available)
                cout << "AVAILABLE";
            else
                cout << "BUSY";

            cout << endl;
        }
    }

    // Display pending emergencies
    void displayPendingEmergencies()
    {
        if (emergencyQueue.empty())
        {
            cout << "\nNo pending emergencies.\n";
            return;
        }

        cout << "\n========== PENDING EMERGENCIES ==========\n";

        priority_queue<Emergency> temp = emergencyQueue;

        while (!temp.empty())
        {
            Emergency e = temp.top();
            temp.pop();

            cout << "\nEmergency ID: " << e.id;
            cout << "\nPatient: " << e.patientName;
            cout << "\nLocation: " << e.location;
            cout << "\nSeverity: " << e.severity;
            cout << "\nCondition: " << e.condition;
            cout << "\n-------------------------\n";
        }
    }

    // Display all roads
    void displayRoadNetwork()
    {
        cout << "\n========== ROAD NETWORK ==========\n";

        for (int i = 0; i < locations.size(); i++)
        {
            cout << "\n" << locations[i] << " : ";

            for (Edge edge : graph[i])
            {
                cout << locations[edge.destination]
                     << " (" << edge.distance << " km)  ";
            }
        }

        cout << endl;
    }

    // Main menu
    void run()
    {
        int choice;

        do
        {
            cout << "\n\n";
            cout << "=============================================\n";
            cout << "              MEDI-ROUTE\n";
            cout << "  Emergency Ambulance Dispatcher System\n";
            cout << "=============================================\n";

            cout << "\n1. Register Emergency";
            cout << "\n2. Dispatch Ambulance";
            cout << "\n3. View Pending Emergencies";
            cout << "\n4. View Ambulance Status";
            cout << "\n5. Complete Ambulance Trip";
            cout << "\n6. View Locations";
            cout << "\n7. View Road Network";
            cout << "\n8. Exit";

            cout << "\n\nEnter your choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    registerEmergency();
                    break;
                case 2:
                    dispatchAmbulance();
                    break;
                case 3:
                    displayPendingEmergencies();
                    break;
                case 4:
                    displayAmbulances();
                    break;
                case 5:
                    completeTrip();
                    break;
                case 6:
                    displayLocations();
                    break;
                case 7:
                    displayRoadNetwork();
                    break;
                case 8:
                    cout << "\nThank you for using Medi-Route!\n";
                    break;
                default:
                    cout << "\nInvalid choice.\n";
            }

        } while (choice != 8);
    }
};

int main()
{
    MediRoute system;
    system.run();
    return 0;
}