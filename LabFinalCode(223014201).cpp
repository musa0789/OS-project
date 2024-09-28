#include <iostream>
#include <climits>
#include <cstring>

using namespace std;
#define MAX 100
struct Process
{
    int process;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int priority;
};
struct GanttChartEntry
{
    int process;
    int start_time;
    int end_time;
};
void enterTaskData(Process processes[], int *taskCount);
void enterTaskPriorities(Process processes[], int taskCount);
void executeFCFS(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount);
void executeSJFNonPreemptive(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount);
void executeSJFPreemptive(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount);
void executePriorityScheduling(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount);
void executeRoundRobin(Process processes[], int taskCount, int timeQuantum, GanttChartEntry ganttChart[], int &ganttCount);
void initializeMetrics(Process processes[], int taskCount);
void displayTaskMetrics(const Process processes[], int taskCount, bool includePriority = false);
void calculateAverageMetrics(const Process processes[], int taskCount);
void evaluateAllSchedulingAlgorithms(Process processes[], int taskCount, int timeQuantum);
void printAverageMetrics(const string &algorithmName, const Process processes[], int taskCount);
void displayGanttChart(const GanttChartEntry ganttChart[], int ganttCount);

void enterTaskData(Process processes[], int *taskCount)
{
    cout << "Please input the number of processes (1 to " << MAX << ") => ";
    cin >> *taskCount;
    if (*taskCount <= 0 || *taskCount > MAX)
    {
        cout << "Invalid input. Please enter valid Processes =>" << endl;
        cout << "Please input the number of processes (1 to " << MAX << ") => ";
        cin >> *taskCount;
    }

    cout << "Process No => ";
    for (int i = 0; i < *taskCount; i++)
    {
        cin >> processes[i].process;
        while (processes[i].process <= 0)
        {
            cout << "Invalid input. Please enter valid Process No=>" << endl;
            cout << "Process No => ";
            cin >> processes[i].process;
        }
    }
    cout << "Arrival Time => ";
    for (int i = 0; i < *taskCount; i++)
    {
        cin >> processes[i].arrival_time;
        while (processes[i].arrival_time < 0)
        {
            cout << "Invalid input. Please enter valid Arrival Time=>" << endl;
            cout << "Arrival Time => ";
            cin >> processes[i].arrival_time;
        }
    }
    cout << "Burst Time => ";
    for (int i = 0; i < *taskCount; i++)
    {
        cin >> processes[i].burst_time;
        while (processes[i].burst_time <= 0)
        {
            cout << "Invalid input. Please enter valid Burst Time=>" << endl;
            cout << "Burst Time => ";
            cin >> processes[i].burst_time;
        }
    }
    for (int i = 0; i < *taskCount; i++)
    {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].priority = processes[i].burst_time;
    }
}
 //Priorities call
    void enterTaskPriorities(Process processes[], int taskCount){
    for (int i = 0; i < taskCount; ++i)
    {
        bool valid_input = false;
        while (!valid_input)
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');

            cout << "Input Priority for Process " << processes[i].process << ":=> ";
            cin >> processes[i].priority;

            if (cin.fail() || processes[i].priority <= 0)
            {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "Invalid input. please try again:" << endl;
            }
            else
            {
                valid_input = true;
            }
        }
    }
}
    void initializeMetrics(Process processes[], int taskCount){
    for (int i = 0; i < taskCount; ++i)
    {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}
   //FCFS function
   void executeFCFS(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount){
    int current_time = 0;
    ganttCount = 0;

    for (int i = 0; i < taskCount; ++i)
    {
        current_time = max(current_time, processes[i].arrival_time);

        ganttChart[ganttCount++] = {processes[i].process, current_time, current_time + processes[i].burst_time};

        processes[i].turnaround_time = current_time + processes[i].burst_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        current_time += processes[i].burst_time;
    }
}
    int findShortestJob(Process processes[], int taskCount, int current_time, int is_completed[]){
    int min_burst = INT_MAX;
    int shortest = -1;

    for (int i = 0; i < taskCount; ++i)
    {
        if (processes[i].arrival_time <= current_time && !is_completed[i] && processes[i].burst_time < min_burst)
        {
            min_burst = processes[i].burst_time;
            shortest = i;
        }
    }

    return shortest;
}
    //SJF NON-Preemptive function
    void executeSJFNonPreemptive(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount){
    int is_completed[MAX] = {0};
    int current_time = 0, completed = 0;
    ganttCount = 0;

    while (completed != taskCount)
    {
        int shortest = findShortestJob(processes, taskCount, current_time, is_completed);

        if (shortest == -1)
        {
            ++current_time;
            continue;
        }

        ganttChart[ganttCount++] = {processes[shortest].process, current_time, current_time + processes[shortest].burst_time};

        processes[shortest].turnaround_time = current_time + processes[shortest].burst_time - processes[shortest].arrival_time;
        processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
        current_time += processes[shortest].burst_time;
        is_completed[shortest] = 1;
        ++completed;
    }
}

    int findShortestJob(Process processes[], int taskCount, int current_time){
    int min_remaining = INT_MAX;
    int shortest = -1;

    for (int i = 0; i < taskCount; ++i)
    {
    if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && processes[i].remaining_time < min_remaining)
        {
            min_remaining = processes[i].remaining_time;
            shortest = i;
        }
    }

    return shortest;
}
    //SJF Preemptive function
    void executeSJFPreemptive(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount){
    int current_time = 0, completed = 0;
    ganttCount = 0;

    while (completed != taskCount)
    {
        int shortest = findShortestJob(processes, taskCount, current_time);

        if (shortest == -1)
        {
            ++current_time;
            continue;
        }

        if (ganttCount == 0 || ganttChart[ganttCount - 1].process != processes[shortest].process)
        {
            ganttChart[ganttCount++] = {processes[shortest].process, current_time, current_time + 1};
        }
        else
        {
            ganttChart[ganttCount - 1].end_time = current_time + 1;
        }
        processes[shortest].remaining_time--;
        ++current_time;

        if (processes[shortest].remaining_time == 0)
        {
            processes[shortest].turnaround_time = current_time - processes[shortest].arrival_time;
            processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
            ++completed;
        }
    }
}

    int findHighestPriority(Process processes[], int taskCount, int current_time, int is_completed[]){
    int highest_priority = INT_MAX;
    int highest = -1;

    for (int i = 0; i < taskCount; ++i)
    {
        if (processes[i].arrival_time <= current_time && !is_completed[i])
        {
            if (processes[i].priority < highest_priority)
            {
                highest_priority = processes[i].priority;
                highest = i;
            }
        }
    }
    return highest;
}
 //Round Robin function
void executeRoundRobin(Process processes[], int taskCount, int timeQuantum, GanttChartEntry ganttChart[], int &ganttCount){

    int current_time = 0, completed = 0;
    ganttCount = 0;
    bool done;

    while (completed != taskCount)
    {
        done = true;
        for (int i = 0; i < taskCount; ++i)
        {
            if (processes[i].remaining_time > 0)
            {
                done = false;
                int timeSlice = min(timeQuantum, processes[i].remaining_time);

                if (ganttCount == 0 || ganttChart[ganttCount - 1].process != processes[i].process)
                {
                    ganttChart[ganttCount++] = {processes[i].process, current_time, current_time + timeSlice};
                }
                else
                {
                    ganttChart[ganttCount - 1].end_time = current_time + timeSlice;
                }

                current_time += timeSlice;
                processes[i].remaining_time -= timeSlice;

                if (processes[i].remaining_time == 0)
                {
                    processes[i].turnaround_time = current_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    ++completed;
                }
            }
        }
    }
}
  //Priority Scheduling function
void executePriorityScheduling(Process processes[], int taskCount, GanttChartEntry ganttChart[], int &ganttCount){
    int is_completed[MAX] = {0};
    int current_time = 0, completed = 0;
    ganttCount = 0;

    while (completed != taskCount)
    {
        int highest = findHighestPriority(processes, taskCount, current_time, is_completed);

        if (highest == -1)
        {
            ++current_time;
            continue;
        }
        ganttChart[ganttCount++] = {processes[highest].process, current_time, current_time + processes[highest].burst_time};

        processes[highest].turnaround_time = current_time + processes[highest].burst_time - processes[highest].arrival_time;
        processes[highest].waiting_time = processes[highest].turnaround_time - processes[highest].burst_time;
        current_time += processes[highest].burst_time;
        is_completed[highest] = 1;
        ++completed;
    }
}

    void calculateAverageMetrics(const Process processes[], int taskCount){

    double total_waiting_time = 0.0, total_turnaround_time = 0.0;

    for (int i = 0; i < taskCount; ++i)
    {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }
    cout << "Average Table:"<<endl;
    cout << "================================================"<<endl;
    cout << "|Average Waiting Time:" << total_waiting_time / taskCount <<"            \t"<<"|"<< endl;
    cout << "|Average Turnaround Time:" << total_turnaround_time / taskCount <<"        \t"<<"|"<< endl;
    cout << "================================================"<<endl<<endl;
    }
void printAverageMetrics(const string &algorithmName, const Process processes[], int taskCount){

    double total_waiting_time = 0.0, total_turnaround_time = 0.0;

    for (int i = 0; i < taskCount; ++i)
    {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

     cout  <<"|" << algorithmName<< endl;
     cout << "| Average Waiting Time: " << total_waiting_time / taskCount <<"      \t"<< endl;
     cout << "| Average Turnaround Time: " << total_turnaround_time / taskCount <<"\t"<< endl;
     }

 // All Scheduling Algorithms compare
void evaluateAllSchedulingAlgorithms(Process processes[], int taskCount, int timeQuantum)
    {
    Process tempProcesses[MAX];
    GanttChartEntry ganttChart[MAX];
    int ganttCount;
    cout <<endl<<"Comparing All Scheduling Algorithms=>"<<endl;
    cout<< "========================================"<<endl;

    cout<< "========================================"<<endl;
    memcpy(tempProcesses, processes, sizeof(Process) * taskCount);
    initializeMetrics(tempProcesses, taskCount);
    executeFCFS(tempProcesses, taskCount, ganttChart, ganttCount);
    printAverageMetrics("FCFS Algorithms                       |", tempProcesses, taskCount);
    cout<< "========================================"<<endl;
    memcpy(tempProcesses, processes, sizeof(Process) * taskCount);
    initializeMetrics(tempProcesses, taskCount);
    executeSJFNonPreemptive(tempProcesses, taskCount, ganttChart, ganttCount);
    printAverageMetrics("SJF Non-Preemptive Algorithms         |", tempProcesses, taskCount);
    cout<< "========================================"<<endl;

    memcpy(tempProcesses, processes, sizeof(Process) * taskCount);
    initializeMetrics(tempProcesses, taskCount);
    executeSJFPreemptive(tempProcesses, taskCount, ganttChart, ganttCount);
    printAverageMetrics("SJF Preemptive Algorithms             |", tempProcesses, taskCount);
    cout<< "========================================"<<endl;
    memcpy(tempProcesses, processes, sizeof(Process) * taskCount);
    initializeMetrics(tempProcesses, taskCount);
    executeRoundRobin(tempProcesses, taskCount, timeQuantum, ganttChart, ganttCount);
    printAverageMetrics("Round Robin Algorithms                |", tempProcesses, taskCount);
    cout<< "========================================"<<endl;
    memcpy(tempProcesses, processes, sizeof(Process) * taskCount);
    initializeMetrics(tempProcesses, taskCount);
    executePriorityScheduling(tempProcesses, taskCount, ganttChart, ganttCount);
    printAverageMetrics("Priority Algorithms                   |", tempProcesses, taskCount);
    cout<< "========================================"<<endl<<endl;

    }
 //display function
void displayTaskMetrics(const Process processes[], int taskCount, bool includePriority){
    cout<<"Metrics:"<<endl;
    cout<<"==================================================================================";
    cout <<endl <<"|Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time |";
    if (includePriority)
    {
        cout << "\tPriority";
    }
    cout << endl;

    for (int i = 0; i < taskCount; ++i)
    {
       cout <<"|"<<processes[i]. process << "\t\t"
             << processes[i]. arrival_time << "\t\t"
             << processes[i]. burst_time << "\t\t"
             << processes[i]. waiting_time << "\t\t"
              <<"\t"<<processes[i].turnaround_time<<"\t"<<"|";

        if (includePriority)
        {
            cout << '\t' << processes[i].priority;
        }
        cout << endl;
    }
     cout<<"=================================================================================="<<endl<<endl;
     }

   //display Gantt Chart function
void displayGanttChart(const GanttChartEntry ganttChart[], int ganttCount){

    cout << "Gantt Chart:" << endl;
    cout<<"============="<<endl;
    for (int i = 0; i < ganttCount; ++i)
    {
        cout << "| P" << ganttChart[i].process << " [" << ganttChart[i].start_time << " - " << ganttChart[i].end_time << "] ";
    }
    cout << "|" << endl<<endl;
}
//Main function
int main()
  {
    Process processes[MAX];
    int taskCount = 0;
    int choice;
    int quantum;

    while (true)
    {
        cout << "========================================" << endl;
        cout << "|   >>CPU Scheduling Algorithms<<      |" << endl;
        cout << "========================================" << endl;
        cout << "|   1. Input Data                      |" << endl;
        cout << "|   2. FCFS Algorithms                 |" << endl;
        cout << "|   3. SJF Non-Preemptive Algorithms   |" << endl;
        cout << "|   5. Round Robin Algorithms          |" << endl;
        cout << "|   6. Priority Scheduling Algorithms  |" << endl;
        cout << "|   7. Compare All Algorithms          |" << endl;
        cout << "|   8. Exit                            |" << endl;
        cout << "========================================" << endl;
        cout << endl
             << "Please select options => ";

        int n = 0;
        while (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            if (n != 0)
            {
                cout << "Invalid input. Please enter a number between 1 and 8." << endl;
            }
            n++;
            continue;
        }

        switch (choice)
        {
        case 1:
            enterTaskData(processes, &taskCount);
            break;

        case 2:
            if (taskCount == 0)
            {
                break;
            }
            initializeMetrics(processes, taskCount);
            {
                GanttChartEntry ganttChart[MAX];
                int ganttCount = 0;
                executeFCFS(processes, taskCount, ganttChart, ganttCount);
                displayTaskMetrics(processes, taskCount);
                displayGanttChart(ganttChart, ganttCount);
                calculateAverageMetrics(processes, taskCount);
            }
            break;
       case 3:
            if (taskCount == 0)
            {
                break;
            }

            initializeMetrics(processes, taskCount);
            {
                GanttChartEntry ganttChart[MAX];
                int ganttCount = 0;
                executeSJFNonPreemptive(processes, taskCount, ganttChart, ganttCount);
                displayTaskMetrics(processes, taskCount);
                displayGanttChart(ganttChart, ganttCount);
                calculateAverageMetrics(processes, taskCount);
            }
            break;
        case 4:
            if (taskCount == 0)
            {
                break;
            }
            initializeMetrics(processes, taskCount);
            {
                GanttChartEntry ganttChart[MAX];
                int ganttCount = 0;
                executeSJFPreemptive(processes, taskCount, ganttChart, ganttCount);
                displayTaskMetrics(processes, taskCount);
                displayGanttChart(ganttChart, ganttCount);
                calculateAverageMetrics(processes, taskCount);
            }
            break;


        case 5:
            if (taskCount == 0)
            {
                break;
            }
            cout << "Enter Quantum Time => ";
            if (!(cin >> quantum) || quantum <= 0)
            {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "Invalid input. Please enter a positive number for quantum." << endl;
                break;
            }

            initializeMetrics(processes, taskCount);
            {
                GanttChartEntry ganttChart[MAX];
                int ganttCount = 0;
                executeRoundRobin(processes, taskCount, quantum, ganttChart, ganttCount);
                displayTaskMetrics(processes, taskCount);
                displayGanttChart(ganttChart, ganttCount);
                calculateAverageMetrics(processes, taskCount);
            }
            break;

         case 6:
            if (taskCount == 0)
            {
                break;
            }
            enterTaskPriorities(processes, taskCount);
            initializeMetrics(processes, taskCount);
            {
                GanttChartEntry ganttChart[MAX];
                int ganttCount = 0;
                executePriorityScheduling(processes, taskCount, ganttChart, ganttCount);
                displayTaskMetrics(processes, taskCount, true);
                displayGanttChart(ganttChart, ganttCount);
                calculateAverageMetrics(processes, taskCount);
            }
            break;

        case 7:
            if (taskCount == 0)
            {
                cout << "Please select => 1. Input Data " << endl;
                break;
            }
            cout << "Enter Quantum Time for Round Robin: ";
            if (!(cin >> quantum) || quantum <= 0)
            {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "Invalid input. Please enter a positive number for quantum." << endl;
                break;
            }
            enterTaskPriorities(processes, taskCount);
            evaluateAllSchedulingAlgorithms(processes, taskCount, quantum);
            break;

        case 8:
            cout << "Exit program. Goodbye!!!!" << endl;
            return 0;

        default:
            cout << "Please select => 1. Input Data" << endl;
        }
    }
}
