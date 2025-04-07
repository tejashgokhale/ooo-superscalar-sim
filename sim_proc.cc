#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
#include "sim_proc.h"

using namespace std;

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params
    int op_type, dest, src1, src2;  // Variables are read from trace file
    uint64_t pc; // Variable holds the pc read from input file


    
    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);
    trace_file          = argv[4];
    /*printf("rob_size:%lu "
            "iq_size:%lu "
            "width:%lu "
            "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);*/

//***************************************************************************************************************************
//---------------------------------------------------------------------------------------------------------
//Instantiating structures
//---------------------------------------------------------------------------------------------------------

    //Instantiating interface registers
    Tej_Fetch_Pipeline_Reg_File fetch_pip_reg_file(params.width);
    Tej_Decode_Pipeline_Reg_File decode_pip_reg_file(params.width);
    Tej_Rename_Pipeline_Reg_File rename_pip_reg_file(params.width);
    Tej_RegisterRd_Pipeline_Reg_File registerrd_pip_reg_file(params.width);
    Tej_Execution_List exec_list(params.width, 5);
    Tej_Writeback_Pipeline_Reg_File writeback_pip_reg_file(params.width, 5);

    //Instantiating Other Structures
    Tej_Rename_Map_Table rmt(67);
    Tej_Reorder_Buffer rob(params.rob_size);
    Tej_Issue_Queue iq(params.iq_size);

//***************************************************************************************************************************




    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // The following loop just tests reading the trace and echoing it back to the screen.
    //
    // Replace this loop with the "do { } while (Advance_Cycle());" loop indicated in the Project 3 spec.
    // Note: fscanf() calls -- to obtain a fetch bundle worth of instructions from the trace -- should be
    // inside the Fetch() function.
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    do {
        global_cycle_count++;
        if (global_debug)
        {
            printf("--------------------------------------------------\n");
            printf("DEBUG: Cycle Count: %d\n", global_cycle_count);
            printf("--------------------------------------------------\n");
        }
//---------------------------------------------------------------------------------------------------------
        Retire(&rob, params.width, &rmt, &exec_list);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Retire\n");
            printf("***************************************************\n");
        }

//---------------------------------------------------------------------------------------------------------
        Writeback(&writeback_pip_reg_file, &rob, &registerrd_pip_reg_file, &iq);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Writeback\n");
            printf("***************************************************\n");
        }


//---------------------------------------------------------------------------------------------------------
        Execute(&exec_list, &writeback_pip_reg_file, &iq, &registerrd_pip_reg_file, &rename_pip_reg_file);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Execute\n");
            printf("***************************************************\n");
        }


//---------------------------------------------------------------------------------------------------------
        Issue(&iq, &exec_list);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Issue\n");
            printf("***************************************************\n");
        }


//---------------------------------------------------------------------------------------------------------
        Dispatch(&registerrd_pip_reg_file, &iq);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Dispatch\n");
            printf("***************************************************\n");
        }
        


//---------------------------------------------------------------------------------------------------------
        RegRead(&rename_pip_reg_file, &registerrd_pip_reg_file, &rob);
           if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post RegRead\n");
            printf("***************************************************\n");
            debug_rename_pipeline_registers(&rename_pip_reg_file);
            debug_regread_pipeline_registers(&registerrd_pip_reg_file);
        }

//---------------------------------------------------------------------------------------------------------
        Rename(&decode_pip_reg_file, &rename_pip_reg_file, &rob, &rmt);
           if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Rename\n");
            printf("***************************************************\n");
            debug_decode_pipeline_registers(&decode_pip_reg_file);
            debug_rename_pipeline_registers(&rename_pip_reg_file);
            debug_rename_map_table(&rmt);
            debug_reorder_buffer(&rob);
        }

//---------------------------------------------------------------------------------------------------------

        Decode(&fetch_pip_reg_file, &decode_pip_reg_file);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Decode\n");
            printf("***************************************************\n");
            debug_fetch_pipeline_registers(&fetch_pip_reg_file);
            debug_decode_pipeline_registers(&decode_pip_reg_file);
        }
//---------------------------------------------------------------------------------------------------------

        Fetch(FP, &fetch_pip_reg_file);
        if (global_debug)
        {
            printf("***************************************************\n");
            printf("DEBUG: Post Fetch\n");
            printf("***************************************************\n");
            debug_fetch_pipeline_registers(&fetch_pip_reg_file);
        }
//---------------------------------------------------------------------------------------------------------

        if (global_debug)
        {
            if (global_trace_depleted)
            {
                printf("--------------------------------------------------\n");
                printf("CYCLE COMPLETE: TRACE DEPLETED\n");
                printf("--------------------------------------------------\n");
            }

            cin.get();
        }

    } while (Advance_Cycle(&fetch_pip_reg_file, &decode_pip_reg_file, &rob));
    printf("# === Simulator Command =========\n");
    printf("# %s %d %d %d ", argv[0], params.rob_size, params.iq_size, params.width);
    cout << trace_file <<endl;
    printf("# === Processor Configuration ===\n");
    printf("# ROB_SIZE = %d\n", params.rob_size);
    printf("# IQ_SIZE  = %d\n", params.iq_size);
    printf("# WIDTH    = %d\n", params.width);
    printf("# === Simulation Results ========\n");
    printf("# Dynamic Instruction Count    = %d\n", global_line_count);
    printf("# Cycles                       = %d\n", global_cycle_count);
    printf("# Instructions Per Cycle (IPC) = %.2f", (float(global_line_count-1)/float(global_cycle_count)));


    return 0;
}
