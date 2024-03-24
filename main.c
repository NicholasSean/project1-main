#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct {
    int arrival_time;
    char process_name[9];
    int service_time;
    int mem_required;
} Process;

Process* read_file(const char* file, int* processCount);
void print_output (int curr_time, const char* status, const char*  process_name, int time_rem, int proc_remain, int finished_bool);

int main() {
    //for round robin infinite mem.
    const char* filename = "spec.txt";
    int process_count;
    // hard code quantum value for the time being.
    int quantum = 3;
    Process* process_queue = read_file(filename, &process_count);
    for (int i = 0; i < process_count; i++) {
            printf("Process Name: %s\n", process_queue[i].process_name);
            printf("Arrival Time: %d\n", process_queue[i].arrival_time);
            printf("Service Time: %d\n", process_queue[i].service_time);
            printf("Memory Required: %d\n", process_queue[i].mem_required);
            printf("\n");
        }
    
    // start the round robin using a bool.
    // make this its own function.
    const char* RUNNING = "RUNNING";
    const char* FINISHED = "FINISHED";
    const char* READY = "READY";
    int start_bool = 1;
    int proc_left = 0;
    int curr_time = 0;
    int initial_total = 0;
    while (start_bool) {
        int printed_bool = 0;
        // Loop to print whenever a new process is queued until all services has been queued appropriately.
        for (int i = initial_total; i < process_count; i++) {
            if (process_queue[i].arrival_time >= curr_time && printed_bool == 0 && process_queue[i].service_time > 0) {
                print_output(curr_time, RUNNING, process_queue[i].process_name, process_queue[i].service_time, proc_left, 0);
                printed_bool = 1;
                initial_total++;
                curr_time++;
            }
        }
        curr_time += quantum;
    }


    free(process_queue);


}

// fucntion to print the output.
void print_output (int curr_time, const char* status, const char* process_name, int time_rem, int proc_remain, int finished_bool) {
    if (finished_bool == 1) {
        printf("%d,%s,process-name=%s,proc-remaining=%d\n", curr_time, status, process_name, proc_remain);
    }
    else {
        printf("%d,%s,process-name=%s,time-remaining=%d\n", curr_time, status, process_name, time_rem);
    }
}

// Count the number of lines in the file to create the array.
// Need to dynamically create the queue array based on number of total process in processes.txt.
// Populate the queue with the values of the file.
Process* read_file(const char* filename, int* process_count) {
    FILE *openFile = fopen(filename, "r");

    int i;
    *process_count = 0;
    while ((i = getc(openFile)) != EOF) {
        if (i == '\n') {
            (*process_count)++;
        }
    }
    // Create the queue and reset the pointer to top of file
    Process *process_queue = (Process*)malloc(*process_count * sizeof(Process));
    rewind(openFile);
    printf("Process Count: %d\n", *process_count);
    printf("\n");
    for (int j = 0; j < *process_count; j++) {
        fscanf(openFile, "%d %s %d %d", 
               &process_queue[j].arrival_time, 
               process_queue[j].process_name,
               &process_queue[j].service_time, 
               &process_queue[j].mem_required);
    }
    fclose(openFile);
    return process_queue;
} 