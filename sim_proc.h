#ifndef SIM_PROC_H
#define SIM_PROC_H

//global variables
int global_debug = 0;
int global_trace_depleted = 0;
int global_pipeline_empty = 0;
int global_cycle_count = 0;
int global_line_count = 0;

typedef struct proc_params{
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int width;
}proc_params;

// Put additional data structures here as per your requirement

//--------------------------------------------------------------------------------------------
//Stats
//--------------------------------------------------------------------------------------------
class Tej_Instr_Stats
{
    public:
        int fetch_start;
        int decode_start;
        int rename_start;
        int regread_start;
        int dispatch_start;
        int issue_start;
        int exec_start;
        int wb_start;
        int retire_start;
        int retire_end;
        int seq_no;
        int src1;
        int src2;
        int dest;
        int op_type;
        Tej_Instr_Stats()
        {
            fetch_start = 0; decode_start=0;rename_start=0;regread_start=0;dispatch_start=0;issue_start=0;
            exec_start=0;wb_start=0;retire_start=0;retire_end=0;seq_no=0;src1=0;src2=0;dest=0;
        }

};


//--------------------------------------------------------------------------------------------
//Common functional structures
//--------------------------------------------------------------------------------------------

//Reorder Buffer Entry
class Tej_Reorder_Buffer_Entry
{
    public:
        int value;
        int dst;
        int rdy;
        uint64_t pc;
        int exception;
        int mispredict;
        Tej_Instr_Stats stats;
};

//Reorder Buffer
class Tej_Reorder_Buffer
{
public:
    Tej_Reorder_Buffer_Entry *rob_pointer;
    int rob_size;
    int head_index;
    int tail_index;
    bool full;
    Tej_Reorder_Buffer(int u_rob_size)
    {
        head_index = 0;
        tail_index = 0;
        full = false;
        rob_size = u_rob_size;
        rob_pointer = new Tej_Reorder_Buffer_Entry[rob_size];
    }
};

//Issue Queue Entry
class Tej_Issue_Queue_Entry
{
    public:
    int valid;
    int dest_tag;
    int rs1_ready;
    int rs1_tag_value;
    int rs2_ready;
    int rs2_tag_value;
    Tej_Instr_Stats stats;
};

//Issue Queue
class Tej_Issue_Queue
{
    public:
    Tej_Issue_Queue_Entry* iq_pointer;
    int iq_size;
    bool full;
    Tej_Issue_Queue(int u_iq_size)
    {
        iq_size = u_iq_size;
        iq_pointer = new Tej_Issue_Queue_Entry[iq_size];
    }
};

//Architecture Register File
class Tej_Arch_Register_File
{
    public:
        int* rf_pointer;
        int rf_size;

};

//Rename Map Table
class Tej_Rename_Map_Table_Entry
{
    public:
    int valid;
    int rob_tag;
};

class Tej_Rename_Map_Table
{
    public:
    int rmt_size;
    Tej_Rename_Map_Table_Entry* rmt_pointer;
    Tej_Rename_Map_Table(int u_rmt_size)
    {
        rmt_size = u_rmt_size;
        rmt_pointer = new Tej_Rename_Map_Table_Entry[rmt_size];
    }
};


//Pipeline registers
class Tej_Fetch_Pipeline_Reg
{
    public:
    int valid;
    uint64_t pc;
    int op_type;
    int dest;
    int src1;
    int src2;
    Tej_Instr_Stats stats;
    Tej_Fetch_Pipeline_Reg()
    {
        valid = 0;
        pc = 0;
        op_type = 0;
        dest = 0;
        src1 = 0;
        src2 = 0;
    }

};

class Tej_Fetch_Pipeline_Reg_File
{
    public:
        int fetch_pipeline_rf_size;
        Tej_Fetch_Pipeline_Reg* fetch_pipeline_rf_pointer;
        int head_index;
        int tail_index;
        bool full;
        Tej_Fetch_Pipeline_Reg_File(int u_fetch_pipeline_rf_size)
        {
            head_index = 0;
            tail_index = 0;
            fetch_pipeline_rf_size = u_fetch_pipeline_rf_size;
            fetch_pipeline_rf_pointer = new Tej_Fetch_Pipeline_Reg[fetch_pipeline_rf_size];
            full = false;
        }
};

class Tej_Decode_Pipeline_Reg
{
    public:
    int valid;
    uint64_t pc;
    int op_type;
    int dest;
    int src1;
    int src2;
    Tej_Instr_Stats stats;
    Tej_Decode_Pipeline_Reg()
    {
        valid = 0;
        pc = 0;
        op_type = 0;
        dest = 0;
        src1 = 0;
        src2 = 0;
    }
};

class Tej_Decode_Pipeline_Reg_File
{
    public:
        int decode_pipeline_rf_size;
        Tej_Decode_Pipeline_Reg* decode_pipeline_rf_pointer;
        int head_index;
        int tail_index;
        bool full;
        Tej_Decode_Pipeline_Reg_File(int u_decode_pipeline_rf_size)
        {
            decode_pipeline_rf_size = u_decode_pipeline_rf_size;
            head_index = 0;
            tail_index = 0;
            decode_pipeline_rf_pointer = new Tej_Decode_Pipeline_Reg[decode_pipeline_rf_size];
            full = false;
        }

};

class Tej_Rename_Pipeline_Reg
{
    public:
    uint64_t pc;
    int op_type;
    int dest;
    int src1;
    int is_src1_rob;
    int src1_override;
    int src2;
    int is_src2_rob;
    int src2_override;
    Tej_Instr_Stats stats;
    Tej_Rename_Pipeline_Reg()
    {
        pc=0;op_type=0;dest=0;src1=0;is_src1_rob=0;src2=0;is_src2_rob=0;src1_override=0;src2_override=0;
    }

};

class Tej_Rename_Pipeline_Reg_File
{
    public:
        int rename_pipeline_rf_size;
        Tej_Rename_Pipeline_Reg* rename_pipeline_rf_pointer;
        int head_index;
        int tail_index;
        bool full;
        Tej_Rename_Pipeline_Reg_File(int u_rename_pipeline_rf_size)
        {
            rename_pipeline_rf_size = u_rename_pipeline_rf_size;
            head_index = 0;
            tail_index = 0;
            rename_pipeline_rf_pointer = new Tej_Rename_Pipeline_Reg[rename_pipeline_rf_size];
            full = false;
        }
};

class Tej_RegisterRd_Pipeline_Reg
{
    public:
    //uint64_t pc;
    int op_type;
    int dest;
    int src1_or_value;
    int is_src1_rob;
    int src1_rdy;
    int src2_or_value;
    int is_src2_rob;
    int src2_rdy;
    Tej_Instr_Stats stats;
    Tej_RegisterRd_Pipeline_Reg()
    {
        op_type=0;dest=0;src1_or_value=0;is_src1_rob=0;src1_rdy=0;src2_or_value=0;is_src2_rob=0;src2_rdy=0;
    }
};

class Tej_RegisterRd_Pipeline_Reg_File
{
    public:
        int registerrd_pipeline_rf_size;
        Tej_RegisterRd_Pipeline_Reg* registerrd_pipeline_rf_pointer;
        int head_index;
        int tail_index;
        bool full;
        Tej_RegisterRd_Pipeline_Reg_File(int u_registerrd_pipeline_rf_size)
        {
            registerrd_pipeline_rf_size = u_registerrd_pipeline_rf_size;
            head_index = 0;
            tail_index = 0;
            registerrd_pipeline_rf_pointer = new Tej_RegisterRd_Pipeline_Reg[registerrd_pipeline_rf_size];
            full = false;
        }
};

class Tej_Execution_Pipeline_Reg
{
    public:
    int op_type;
    int dest;
    Tej_Instr_Stats stats;
    int valid;
    int latency;
    Tej_Execution_Pipeline_Reg()
    {
        op_type = 0;
        dest = 0;
        valid = 0;
        latency = 0;
    }
};

class Tej_Execution_List
{
    public:
    Tej_Execution_Pipeline_Reg **execution_list_pointer;
    int execution_list_size;
    int execution_list_depth;
    Tej_Execution_List(int u_execution_list_size, int u_execution_list_depth)
    {
        execution_list_size = u_execution_list_size;
        execution_list_depth = u_execution_list_depth;
        execution_list_pointer = new Tej_Execution_Pipeline_Reg*[execution_list_size];
        for (int i=0;i<execution_list_size;i++)
        {
            execution_list_pointer[i] = new Tej_Execution_Pipeline_Reg[execution_list_depth];
        }
    }
};

class Tej_Writeback_Pipeline_Reg
{
    public:
    Tej_Instr_Stats stats;
    int dest;
    int valid;
    Tej_Writeback_Pipeline_Reg()
    {
        dest = 0;
    }
};

class Tej_Writeback_Pipeline_Reg_File
{
    public:
    int writeback_pipeline_rf_size;
    int width;
    int exec_depth;
    Tej_Writeback_Pipeline_Reg *writeback_pipeline_rf_pointer;
    Tej_Writeback_Pipeline_Reg_File(int u_width, int u_exec_depth)
    {
        width = u_width;
        exec_depth = u_exec_depth;
        writeback_pipeline_rf_size = width*exec_depth;
        writeback_pipeline_rf_pointer = new Tej_Writeback_Pipeline_Reg[writeback_pipeline_rf_size];
    }
};

//--------------------------------------------------------------------------------------------
//Fetch stage functions
//--------------------------------------------------------------------------------------------
void Fetch(FILE* fp, Tej_Fetch_Pipeline_Reg_File* fetch_pip_reg_file)
{
    uint64_t pc;
    int op_type, dest, src1,src2;
    int decode_empty_slots = 0;
    //head index - the point at which the producer inserts items into the buffer.
    //tail index - the point at which the consumer finds the next item in the buffer.

    //checking condition when fetch pipeline regs are full. In this case, we dont fetch new instructions.
    if (fetch_pip_reg_file->full == true)
        return;

    //checking whether there is a free pipeline reg. If there is fetch a new instruction and store it there.
    //start checking from tail index. Tail index is the first empty slot.
    for (int i = 0; i < fetch_pip_reg_file->fetch_pipeline_rf_size; i++)
    {
        //check if fetch pipeline reg is full
        int condition1 = fetch_pip_reg_file->full;
        if (condition1) {break;}
        int new_index = fetch_pip_reg_file->head_index;
        if (fscanf(fp, "%llx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) != EOF)
            {
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.seq_no = global_line_count;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.fetch_start = global_cycle_count-1;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.decode_start = global_cycle_count;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.dest = dest;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.src1 = src1;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.src2 = src2;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].stats.op_type = op_type;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].pc = pc;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].op_type = op_type;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].dest = dest;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].src1 = src1;
                fetch_pip_reg_file->fetch_pipeline_rf_pointer[new_index].src2 = src2;
                fetch_pip_reg_file->head_index = (fetch_pip_reg_file->head_index+1)%fetch_pip_reg_file->fetch_pipeline_rf_size;
                if (fetch_pip_reg_file->head_index == fetch_pip_reg_file->tail_index) {fetch_pip_reg_file->full = true;}
                global_line_count++;
            }
            else
            {
                global_trace_depleted = 1;
                return;
            }
    }

}

//--------------------------------------------------------------------------------------------
//Decode stage functions
//--------------------------------------------------------------------------------------------
void Decode(Tej_Fetch_Pipeline_Reg_File *fetch_pip_reg_file, Tej_Decode_Pipeline_Reg_File *decode_pip_reg_file)
{
    //start decoding from the tail pointer of fetch-decode pipeline reg if the decode-rename register is not full
    //or the fetch-decode pipeline register is not empty

    for (int i = 0; i < fetch_pip_reg_file->fetch_pipeline_rf_size; i++)
    {
        int condition1 = decode_pip_reg_file->full;
        int condition2 = (fetch_pip_reg_file->head_index == fetch_pip_reg_file->tail_index) && !fetch_pip_reg_file->full;
        if (condition1 || condition2)
            break;
        //updating stats
        Tej_Decode_Pipeline_Reg *reg_dst = &decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->head_index];
        Tej_Fetch_Pipeline_Reg *reg_src = &fetch_pip_reg_file->fetch_pipeline_rf_pointer[fetch_pip_reg_file->tail_index];
        reg_dst->stats.seq_no = reg_src->stats.seq_no;
        reg_dst->stats.src1 = reg_src->stats.src1;
        reg_dst->stats.src2 = reg_src->stats.src2;
        reg_dst->stats.dest = reg_src->stats.dest;
        reg_dst->stats.op_type = reg_src->stats.op_type;
        reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
        reg_dst->stats.decode_start = reg_src->stats.decode_start;
        reg_dst->stats.rename_start = global_cycle_count;

        //updating non stats
        decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->head_index].dest = 
        fetch_pip_reg_file->fetch_pipeline_rf_pointer[fetch_pip_reg_file->tail_index].dest;
        decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->head_index].op_type = 
        fetch_pip_reg_file->fetch_pipeline_rf_pointer[fetch_pip_reg_file->tail_index].op_type;
        decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->head_index].pc = 
        fetch_pip_reg_file->fetch_pipeline_rf_pointer[fetch_pip_reg_file->tail_index].pc;
        decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->head_index].src1 = 
        fetch_pip_reg_file->fetch_pipeline_rf_pointer[fetch_pip_reg_file->tail_index].src1;
        decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->head_index].src2 = 
        fetch_pip_reg_file->fetch_pipeline_rf_pointer[fetch_pip_reg_file->tail_index].src2;

        //update head and tail index of both regs
        fetch_pip_reg_file->tail_index = (fetch_pip_reg_file->tail_index + 1) % fetch_pip_reg_file->fetch_pipeline_rf_size;
        fetch_pip_reg_file->full = false;
        decode_pip_reg_file->head_index = (decode_pip_reg_file->head_index + 1) % decode_pip_reg_file->decode_pipeline_rf_size;
        if (decode_pip_reg_file->head_index == decode_pip_reg_file->tail_index) {decode_pip_reg_file->full = true;}

    }
}

//--------------------------------------------------------------------------------------------
//Rename stage functions
//--------------------------------------------------------------------------------------------
void Rename(Tej_Decode_Pipeline_Reg_File *decode_pip_reg_file, Tej_Rename_Pipeline_Reg_File *rename_pip_reg_file,
Tej_Reorder_Buffer *rob, Tej_Rename_Map_Table *rmt)
{

    //checking if rob has atleast decode-rename bundle size of space
    int bundle_size = 0;
    if (decode_pip_reg_file->full) {bundle_size = decode_pip_reg_file->decode_pipeline_rf_size;}
    else
    {
        bundle_size = (decode_pip_reg_file->head_index > decode_pip_reg_file->tail_index) ?
         (decode_pip_reg_file->head_index - decode_pip_reg_file->tail_index) :
         (decode_pip_reg_file->decode_pipeline_rf_size + decode_pip_reg_file->head_index - decode_pip_reg_file->tail_index);
    }
    int rob_free_spaces = 0;
    if (rob->full) {rob_free_spaces = 0;}
    else
    {
        rob_free_spaces = (rob->head_index >= rob->tail_index) ? 
        (rob->rob_size - rob->head_index + rob->tail_index) :
        (rob->tail_index - rob->head_index);
    }
    //do nothing if bundle size is greater than the total space in ROB.
    if (bundle_size > rob_free_spaces) {return;}
    for (int i=0; i<decode_pip_reg_file->decode_pipeline_rf_size; i++)
    {
        //checking if decode regs are empty, rename registers are full, rob is full
        int condition1 = (decode_pip_reg_file->head_index==decode_pip_reg_file->tail_index) && (!decode_pip_reg_file->full);
        int condition2 = (rename_pip_reg_file->full == true);
        int condition3 = (rob->full == true);
        if (condition1 || condition2 || condition3) {break;}
        //renaming sources before rob update to avoid taking the new value if one source reg and dest reg are same
        //updating non stats
        rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].op_type = 
        decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].op_type;
        //dest reg will always point to rob value
        rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].dest = rob->head_index;
        //src1 renaming
        if (decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].src1 >= 0)
        {
            int src_reg1 = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].src1;
            if (rmt->rmt_pointer[src_reg1].valid == 1)
            {
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].src1 = rmt->rmt_pointer[src_reg1].rob_tag;
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].is_src1_rob = 1;
            }
            else
            {
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].src1 = src_reg1;
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].is_src1_rob = 0;
            }
        }
        else
        {
            int src_reg1 = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].src1;
            rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].src1 = src_reg1;
            rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].is_src1_rob = 0;
        }
        //src2 renaming
        if (decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].src2 >= 0)
        {
            int src_reg2 = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].src2;
            if (rmt->rmt_pointer[src_reg2].valid == 1)
            {
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].src2 = rmt->rmt_pointer[src_reg2].rob_tag;
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].is_src2_rob = 1;
            }
            else
            {
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].src2 = src_reg2;
                rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].is_src2_rob = 0;
            }
        }
        else
        {
            int src_reg2 = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].src2;
            rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].src2 = src_reg2;
            rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index].is_src2_rob = 0;
        }
        //rob update
        rob->rob_pointer[rob->head_index].dst = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].dest;
        rob->rob_pointer[rob->head_index].pc = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].pc;
        rob->rob_pointer[rob->head_index].rdy = 0;
        rob->rob_pointer[rob->head_index].stats.seq_no = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].stats.seq_no;
        //rmt update
        if (decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].dest >= 0)
        {
            int dest_reg = decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index].dest;
            rmt->rmt_pointer[dest_reg].rob_tag = rob->head_index;
            rmt->rmt_pointer[dest_reg].valid = 1;
        }

        //rename pipeline regs update
        //updating stats
        Tej_Decode_Pipeline_Reg *reg_src = &decode_pip_reg_file->decode_pipeline_rf_pointer[decode_pip_reg_file->tail_index];
        Tej_Rename_Pipeline_Reg *reg_dst = &rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->head_index];
        reg_dst->stats.seq_no = reg_src->stats.seq_no;
        reg_dst->stats.dest = reg_src->stats.dest;
        reg_dst->stats.src1 = reg_src->stats.src1;
        reg_dst->stats.src2 = reg_src->stats.src2;
        reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
        reg_dst->stats.decode_start = reg_src->stats.decode_start;
        reg_dst->stats.rename_start = reg_src->stats.rename_start;
        reg_dst->stats.op_type = reg_src->stats.op_type;
        reg_dst->stats.regread_start = global_cycle_count;
        //setting override variables as 0
        reg_dst->src1_override=0;
        reg_dst->src2_override=0;

        //updating head and tail pointers of rob, decode and rename pipe regs
        rob->head_index = (rob->head_index+1)%rob->rob_size;
        if (rob->head_index == rob->tail_index) {rob->full = true;}
        rename_pip_reg_file->head_index = (rename_pip_reg_file->head_index+1)%rename_pip_reg_file->rename_pipeline_rf_size;
        if (rename_pip_reg_file->head_index == rename_pip_reg_file->tail_index) {rename_pip_reg_file->full = true;}
        decode_pip_reg_file->tail_index = (decode_pip_reg_file->tail_index+1)%decode_pip_reg_file->decode_pipeline_rf_size;
        decode_pip_reg_file->full = false;
        
    }
}
//--------------------------------------------------------------------------------------------
//Register read stage functions
//--------------------------------------------------------------------------------------------
void RegRead(Tej_Rename_Pipeline_Reg_File *rename_pip_reg_file, Tej_RegisterRd_Pipeline_Reg_File *registerrd_pip_reg_file, 
Tej_Reorder_Buffer *rob)
{
    for (int i=0; i<rename_pip_reg_file->rename_pipeline_rf_size;i++)
    {
        //check if rename registers are empty or registerrd registers are full
        int condition1 = registerrd_pip_reg_file->full;
        int condition2 = (rename_pip_reg_file->head_index == rename_pip_reg_file->tail_index) && (!rename_pip_reg_file->full);
        if (condition1 || condition2) {break;}

        Tej_Rename_Pipeline_Reg *reg_src = &rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index];
        Tej_RegisterRd_Pipeline_Reg *reg_dst = &registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index];
        //update stats
        reg_dst->stats.seq_no = reg_src->stats.seq_no;
        reg_dst->stats.dest = reg_src->stats.dest;
        reg_dst->stats.src1 = reg_src->stats.src1;
        reg_dst->stats.src2 = reg_src->stats.src2;
        reg_dst->stats.op_type = reg_src->stats.op_type;
        reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
        reg_dst->stats.decode_start = reg_src->stats.decode_start;
        reg_dst->stats.rename_start = reg_src->stats.rename_start;
        reg_dst->stats.regread_start = reg_src->stats.regread_start;
        reg_dst->stats.dispatch_start = global_cycle_count;

        //update non stats
        registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].dest = 
        rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].dest;
        registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].op_type = 
        rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].op_type;
        registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src1_or_value =
        rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].src1;
        registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src2_or_value =
        rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].src2;
        //check rob if the source depends on rob. If it doesnt the value is either imm or from ARF, so it will be ready.
        if (rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].is_src1_rob)
        {
            if (rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].src1_override)
            {
                registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src1_rdy = 1;
            }
            else 
            {
                registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src1_rdy = 
                rob->rob_pointer[rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].src1].rdy;
            }
        }
        else
        {
            registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src1_rdy = 1;
        }
        if (rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].is_src2_rob)
        {
            if (rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].src2_override)
            {
                registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src2_rdy = 1;
            }
            else 
            {
                registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src2_rdy = 
                rob->rob_pointer[rename_pip_reg_file->rename_pipeline_rf_pointer[rename_pip_reg_file->tail_index].src2].rdy;
            }
        }
        else
        {
            registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->head_index].src2_rdy = 1;
        }
    //updating head and tail index of pipeline regs
    rename_pip_reg_file->tail_index = (rename_pip_reg_file->tail_index + 1)%rename_pip_reg_file->rename_pipeline_rf_size;
    rename_pip_reg_file->full = false;
    registerrd_pip_reg_file->head_index = (registerrd_pip_reg_file->head_index + 1)%registerrd_pip_reg_file->registerrd_pipeline_rf_size;
    if (registerrd_pip_reg_file->head_index == registerrd_pip_reg_file->tail_index) {registerrd_pip_reg_file->full = true;}

    }


}
//--------------------------------------------------------------------------------------------
//Dispatch stage functions
//--------------------------------------------------------------------------------------------
void Dispatch(Tej_RegisterRd_Pipeline_Reg_File *registerrd_pip_reg_file, Tej_Issue_Queue *iq)
{
    //checking to see if regread file is empty
    int condition1 = (registerrd_pip_reg_file->head_index==registerrd_pip_reg_file->tail_index) && !registerrd_pip_reg_file->full;
    if (condition1) {return;}
    //no. of instructions in bundle
    int registerrd_bundle_count;
    if (registerrd_pip_reg_file->full){registerrd_bundle_count = registerrd_pip_reg_file->registerrd_pipeline_rf_size;}
    else
    {
        registerrd_bundle_count = (registerrd_pip_reg_file->head_index > registerrd_pip_reg_file->tail_index) ? 
        (registerrd_pip_reg_file->head_index - registerrd_pip_reg_file->tail_index) : 
        (registerrd_pip_reg_file->registerrd_pipeline_rf_size + registerrd_pip_reg_file->head_index - registerrd_pip_reg_file->tail_index);
    }
    //check if there are atleast "no. of instructions in bundle" vaccancies in iq
    int iq_vaccancies = 0;
    for (int i = 0; i < iq->iq_size; i++)
    {
        if (iq->iq_pointer[i].valid == 0) {iq_vaccancies++;}
    }
    //do nothing if less vaccancies than bundle count
    if (iq_vaccancies < registerrd_bundle_count) {return;}
    for (int i = 0; i < registerrd_pip_reg_file->registerrd_pipeline_rf_size; i++)
    {
        int condition1 = (registerrd_pip_reg_file->head_index==registerrd_pip_reg_file->tail_index) && !registerrd_pip_reg_file->full;
        if (condition1) {break;}
        registerrd_pip_reg_file->full = false;
        for (int j = 0; j < iq->iq_size; j++)
        {
            if (iq->iq_pointer[j].valid == 0)
            {
                Tej_RegisterRd_Pipeline_Reg *reg_src = &registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[registerrd_pip_reg_file->tail_index];
                Tej_Issue_Queue_Entry *reg_dst = &iq->iq_pointer[j];
                reg_dst->dest_tag = reg_src->dest;
                reg_dst->rs1_ready = reg_src->src1_rdy;
                reg_dst->rs1_tag_value = reg_src->src1_or_value;
                reg_dst->rs2_ready = reg_src->src2_rdy;
                reg_dst->rs2_tag_value = reg_src->src2_or_value;
                reg_dst->valid = 1;
                reg_dst->stats.seq_no = reg_src->stats.seq_no;
                reg_dst->stats.dest = reg_src->stats.dest;
                reg_dst->stats.src1 = reg_src->stats.src1;
                reg_dst->stats.src2 = reg_src->stats.src2;
                reg_dst->stats.op_type = reg_src->stats.op_type;
                reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
                reg_dst->stats.decode_start = reg_src->stats.decode_start;
                reg_dst->stats.rename_start = reg_src->stats.rename_start;
                reg_dst->stats.regread_start = reg_src->stats.regread_start;
                reg_dst->stats.dispatch_start = reg_src->stats.dispatch_start;
                reg_dst->stats.issue_start = global_cycle_count;
                //updating tail pointer of pipeline register
                registerrd_pip_reg_file->tail_index = (registerrd_pip_reg_file->tail_index + 1)%registerrd_pip_reg_file->registerrd_pipeline_rf_size;
                break;
            }
        }
    }

}
//--------------------------------------------------------------------------------------------
//Issue stage functions
//--------------------------------------------------------------------------------------------
void Issue(Tej_Issue_Queue *iq, Tej_Execution_List *exec_list)
{
    for (int i=0; i<exec_list->execution_list_size; i++)
    {
        //check if start of execution list is empty at every iteration
        int exec_list_full = 1;
        int exec_list_free_index;
        for (int k = 0; k<exec_list->execution_list_size;k++)
        {
            if (exec_list->execution_list_pointer[k][0].valid==0) 
            {
                exec_list_full = 0;
                exec_list_free_index = k;
                break;
            }
        }
        if (exec_list_full){break;}

        int oldest_value=-1;
        int oldest_index=-1;
        int all_invalid=1;
        //this loop finds the oldest instruction in the IQ.
        for (int j=0; j<iq->iq_size; j++)
        {
            int condition1 = (iq->iq_pointer[j].valid == 1);
            int condition2 = (iq->iq_pointer[j].rs1_ready == 1);
            int condition3 = (iq->iq_pointer[j].rs2_ready == 1);
            if (condition1 && condition2 && condition3)
            {
                all_invalid = 0;
                if (oldest_value == -1)
                {
                    oldest_value = iq->iq_pointer[j].stats.fetch_start;
                    oldest_index = j;
                }
                else if (iq->iq_pointer[j].stats.fetch_start < oldest_value)
                {
                    oldest_value = iq->iq_pointer[j].stats.fetch_start;
                    oldest_index = j;
                }
            }
        }
        if (all_invalid) {break;}
        //move oldest value to exec list
        iq->iq_pointer[oldest_index].valid = 0;
        Tej_Execution_Pipeline_Reg *reg_dst = &exec_list->execution_list_pointer[exec_list_free_index][0];
        Tej_Issue_Queue_Entry *reg_src = &iq->iq_pointer[oldest_index];
        //updating stats
        reg_dst->stats.seq_no = reg_src->stats.seq_no;
        reg_dst->stats.dest = reg_src->stats.dest;
        reg_dst->stats.src1 = reg_src->stats.src1;
        reg_dst->stats.src2 = reg_src->stats.src2;
        reg_dst->stats.op_type = reg_src->stats.op_type;
        reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
        reg_dst->stats.decode_start = reg_src->stats.decode_start;
        reg_dst->stats.rename_start = reg_src->stats.rename_start;
        reg_dst->stats.regread_start = reg_src->stats.regread_start;
        reg_dst->stats.dispatch_start = reg_src->stats.dispatch_start;
        reg_dst->stats.issue_start = reg_src->stats.issue_start;
        reg_dst->stats.exec_start = global_cycle_count;
        //updating non stats
        reg_dst->dest = reg_src->dest_tag;
        reg_dst->valid = 1;
        if (reg_dst->stats.op_type == 0) {reg_dst->latency = 1;}
        else if (reg_dst->stats.op_type == 1) {reg_dst->latency = 2;}
        else if (reg_dst->stats.op_type == 2) {reg_dst->latency = 5;}
    }
}
//--------------------------------------------------------------------------------------------
//Execute stage functions
//--------------------------------------------------------------------------------------------
void Execute(Tej_Execution_List *exec_list, Tej_Writeback_Pipeline_Reg_File *writeback_pip_reg_file, 
Tej_Issue_Queue *iq, Tej_RegisterRd_Pipeline_Reg_File *registerrd_pip_reg_file, Tej_Rename_Pipeline_Reg_File *rename_pip_reg_file)
{
    //since writeback clears every cycle, just start writing from 0
    int writeback_pip_rf_insert_index = 0;
    for (int i=0;i<exec_list->execution_list_size;i++)
    {
        for (int j=exec_list->execution_list_depth-1; j>=0;j--)
        {
            if (exec_list->execution_list_pointer[i][j].valid == 1)
            {

                //if valid instruction and execution is finished(latency is 1 ie it will finish at the end of the cycle), 
                //write it to wb pipeline register. We also have to do early wakeup in the issue stage, dispatch and register read
                if (exec_list->execution_list_pointer[i][j].latency == 1)
                {
                    exec_list->execution_list_pointer[i][j].valid = 0;
                    Tej_Writeback_Pipeline_Reg *reg_dst = &writeback_pip_reg_file->writeback_pipeline_rf_pointer[writeback_pip_rf_insert_index];
                    Tej_Execution_Pipeline_Reg *reg_src = &exec_list->execution_list_pointer[i][j];
                    //updating stats
                    reg_dst->stats.seq_no = reg_src->stats.seq_no;
                    reg_dst->stats.dest = reg_src->stats.dest;
                    reg_dst->stats.src1 = reg_src->stats.src1;
                    reg_dst->stats.src2 = reg_src->stats.src2;
                    reg_dst->stats.op_type = reg_src->stats.op_type;
                    reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
                    reg_dst->stats.decode_start = reg_src->stats.decode_start;
                    reg_dst->stats.rename_start = reg_src->stats.rename_start;
                    reg_dst->stats.regread_start = reg_src->stats.regread_start;
                    reg_dst->stats.dispatch_start = reg_src->stats.dispatch_start;
                    reg_dst->stats.issue_start = reg_src->stats.issue_start;
                    reg_dst->stats.exec_start = reg_src->stats.exec_start;
                    reg_dst->stats.wb_start = global_cycle_count;
                    reg_dst->valid = 1;
                    //updating non stats
                    reg_dst->dest = reg_src->dest;
                    writeback_pip_rf_insert_index++;
                    //wake up instruction in issue stage.
                    for (int k=0; k<iq->iq_size;k++)
                    {
                        if ((iq->iq_pointer[k].rs1_ready == 0) && (iq->iq_pointer[k].rs1_tag_value == reg_src->dest))
                        {
                            iq->iq_pointer[k].rs1_ready = 1;
                        }
                        if ((iq->iq_pointer[k].rs2_ready == 0) && (iq->iq_pointer[k].rs2_tag_value == reg_src->dest))
                        {
                            iq->iq_pointer[k].rs2_ready = 1;
                        }
                    }

                    //update dispatch pipeline register
                    for (int k=0;k<registerrd_pip_reg_file->registerrd_pipeline_rf_size;k++)
                    {
                        if (!registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[k].src1_rdy)
                        {
                            if (registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[k].src1_or_value == reg_src->dest)
                            {
                                registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[k].src1_rdy = 1;
                            }
                        }
                        if (!registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[k].src2_rdy)
                        {
                            if (registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[k].src2_or_value == reg_src->dest)
                            {
                                registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[k].src2_rdy = 1;
                            }
                        }
                    }

                    //update register read pipeline register. It has to override the register read function that will happen later,
                    //so introducing a override variable in the register read pipeline reg
                    for (int k=0;k<rename_pip_reg_file->rename_pipeline_rf_size;k++)
                    {
                        if (rename_pip_reg_file->rename_pipeline_rf_pointer[k].is_src1_rob)
                        {
                            if (rename_pip_reg_file->rename_pipeline_rf_pointer[k].src1 == reg_src->dest)
                            {
                                rename_pip_reg_file->rename_pipeline_rf_pointer[k].src1_override = 1;
                            }
                        }
                        if (rename_pip_reg_file->rename_pipeline_rf_pointer[k].is_src2_rob)
                        {
                            if (rename_pip_reg_file->rename_pipeline_rf_pointer[k].src2 == reg_src->dest)
                            {
                                rename_pip_reg_file->rename_pipeline_rf_pointer[k].src2_override = 1;
                            }
                        }
                        
                    }

                }
                //if valid instruction is not finished, move it to next pipeline stage(index)
                else
                {
                    exec_list->execution_list_pointer[i][j].valid = 0;
                    Tej_Execution_Pipeline_Reg *reg_dst = &exec_list->execution_list_pointer[i][j+1];
                    Tej_Execution_Pipeline_Reg *reg_src = &exec_list->execution_list_pointer[i][j];
                    //updating stats
                    reg_dst->stats.seq_no = reg_src->stats.seq_no;
                    reg_dst->stats.dest = reg_src->stats.dest;
                    reg_dst->stats.src1 = reg_src->stats.src1;
                    reg_dst->stats.src2 = reg_src->stats.src2;
                    reg_dst->stats.op_type = reg_src->stats.op_type;
                    reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
                    reg_dst->stats.decode_start = reg_src->stats.decode_start;
                    reg_dst->stats.rename_start = reg_src->stats.rename_start;
                    reg_dst->stats.regread_start = reg_src->stats.regread_start;
                    reg_dst->stats.dispatch_start = reg_src->stats.dispatch_start;
                    reg_dst->stats.issue_start = reg_src->stats.issue_start;
                    reg_dst->stats.exec_start = reg_src->stats.exec_start;
                    //updating non stats
                    reg_dst->dest = reg_src->dest;
                    reg_dst->valid = 1;
                    reg_dst->latency = reg_src->latency - 1;
                } 
            }
        }
    }
}
//--------------------------------------------------------------------------------------------
//Writeback stage functions
//--------------------------------------------------------------------------------------------
void Writeback(Tej_Writeback_Pipeline_Reg_File *writeback_pip_reg_file, Tej_Reorder_Buffer *rob, 
Tej_RegisterRd_Pipeline_Reg_File *registerrd_pip_reg_file, Tej_Issue_Queue *iq)
{
    for (int i=0; i<writeback_pip_reg_file->writeback_pipeline_rf_size;i++)
    {
        if (writeback_pip_reg_file->writeback_pipeline_rf_pointer[i].valid == 1)
        {
            writeback_pip_reg_file->writeback_pipeline_rf_pointer[i].valid = 0;
            //update dst ready in ROB.
            rob->rob_pointer[writeback_pip_reg_file->writeback_pipeline_rf_pointer[i].dest].rdy = 1;
            //updating stats in ROB
            Tej_Reorder_Buffer_Entry *reg_dst = &rob->rob_pointer[writeback_pip_reg_file->writeback_pipeline_rf_pointer[i].dest];
            Tej_Writeback_Pipeline_Reg *reg_src = &writeback_pip_reg_file->writeback_pipeline_rf_pointer[i];
            reg_dst->stats.seq_no = reg_src->stats.seq_no;
            reg_dst->stats.dest = reg_src->stats.dest;
            reg_dst->stats.src1 = reg_src->stats.src1;
            reg_dst->stats.src2 = reg_src->stats.src2;
            reg_dst->stats.op_type = reg_src->stats.op_type;
            reg_dst->stats.fetch_start = reg_src->stats.fetch_start;
            reg_dst->stats.decode_start = reg_src->stats.decode_start;
            reg_dst->stats.rename_start = reg_src->stats.rename_start;
            reg_dst->stats.regread_start = reg_src->stats.regread_start;
            reg_dst->stats.dispatch_start = reg_src->stats.dispatch_start;
            reg_dst->stats.issue_start = reg_src->stats.issue_start;
            reg_dst->stats.exec_start = reg_src->stats.exec_start;
            reg_dst->stats.wb_start = reg_src->stats.wb_start;
            reg_dst->stats.retire_start = global_cycle_count;

        }
    }
}
//--------------------------------------------------------------------------------------------
//Retire stage functions
//--------------------------------------------------------------------------------------------
void Retire(Tej_Reorder_Buffer *rob, int max_instr_retire_count, Tej_Rename_Map_Table *rmt, Tej_Execution_List *exec_list)
{
    for (int i=0; i<max_instr_retire_count; i++)
    {
        //check if rob is empty
        if (!rob->full && (rob->head_index==rob->tail_index)) {break;}
        if (rob->rob_pointer[rob->tail_index].rdy==1)
        {
            if (rob->rob_pointer[rob->tail_index].dst >= 0)
            {
            //updating rmt if it matches
            if (rmt->rmt_pointer[rob->rob_pointer[rob->tail_index].dst].rob_tag == rob->tail_index)
            {
                rmt->rmt_pointer[rob->rob_pointer[rob->tail_index].dst].valid = 0;
            }
            }
            rob->full = 0;
            rob->rob_pointer[rob->tail_index].stats.retire_end = global_cycle_count;


            //printing the instruction
            printf("%d fu{%d} src{%d,%d} dst{%d} FE{%d,%d} DE{%d,%d} RN{%d,%d} RR{%d,%d} DI{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d} RT{%d,%d}\n",
            rob->rob_pointer[rob->tail_index].stats.seq_no,
            rob->rob_pointer[rob->tail_index].stats.op_type,
            rob->rob_pointer[rob->tail_index].stats.src1,
            rob->rob_pointer[rob->tail_index].stats.src2,
            rob->rob_pointer[rob->tail_index].stats.dest,
            rob->rob_pointer[rob->tail_index].stats.fetch_start,
            (rob->rob_pointer[rob->tail_index].stats.decode_start - rob->rob_pointer[rob->tail_index].stats.fetch_start),
            rob->rob_pointer[rob->tail_index].stats.decode_start,
            (rob->rob_pointer[rob->tail_index].stats.rename_start - rob->rob_pointer[rob->tail_index].stats.decode_start),
            rob->rob_pointer[rob->tail_index].stats.rename_start,
            (rob->rob_pointer[rob->tail_index].stats.regread_start - rob->rob_pointer[rob->tail_index].stats.rename_start),
            rob->rob_pointer[rob->tail_index].stats.regread_start,
            (rob->rob_pointer[rob->tail_index].stats.dispatch_start - rob->rob_pointer[rob->tail_index].stats.regread_start),
            rob->rob_pointer[rob->tail_index].stats.dispatch_start,
            (rob->rob_pointer[rob->tail_index].stats.issue_start - rob->rob_pointer[rob->tail_index].stats.dispatch_start),
            rob->rob_pointer[rob->tail_index].stats.issue_start,
            (rob->rob_pointer[rob->tail_index].stats.exec_start - rob->rob_pointer[rob->tail_index].stats.issue_start),
            rob->rob_pointer[rob->tail_index].stats.exec_start,
            (rob->rob_pointer[rob->tail_index].stats.wb_start - rob->rob_pointer[rob->tail_index].stats.exec_start),
            rob->rob_pointer[rob->tail_index].stats.wb_start,
            (rob->rob_pointer[rob->tail_index].stats.retire_start - rob->rob_pointer[rob->tail_index].stats.wb_start),
            rob->rob_pointer[rob->tail_index].stats.retire_start,
            (rob->rob_pointer[rob->tail_index].stats.retire_end - rob->rob_pointer[rob->tail_index].stats.retire_start)
            );
            rob->tail_index = (rob->tail_index + 1)%rob->rob_size;
            //delete
            //printf("DEBUG: Retiring instruction\n");
            //global_pipeline_empty=1;
            //global_trace_depleted=1;
        }
        else {break;}
    }
}
//--------------------------------------------------------------------------------------------
//Advance Cycle Function
//--------------------------------------------------------------------------------------------
int Advance_Cycle(Tej_Fetch_Pipeline_Reg_File *fetch_pip_reg_file, Tej_Decode_Pipeline_Reg_File *decode_pip_reg_file, 
Tej_Reorder_Buffer *rob)
{
    //to check if pipeline is empty, we check if all the pipeline registers are empty
    int check_fetch_pip_reg_empty = (fetch_pip_reg_file->head_index==fetch_pip_reg_file->tail_index) && !fetch_pip_reg_file->full;
    int check_decode_pip_reg_empty = (decode_pip_reg_file->head_index==decode_pip_reg_file->tail_index) && !decode_pip_reg_file->full;
    int check_rob_empty = (rob->head_index==rob->tail_index) && !rob->full;
    if (global_debug)
    {
        printf("DEBUG: Checking conditions for empty pipeline:\n");
        printf("Fetch-Decode Pipeline Registers empty: %d\n", check_fetch_pip_reg_empty);
        printf("Decode-Rename Pipeline Registers empty: %d\n", check_decode_pip_reg_empty);
        printf("Reorder Buffer empty: %d\n", check_rob_empty);

    }
 
    if (check_fetch_pip_reg_empty && check_decode_pip_reg_empty && check_rob_empty)
    {
        //Debug to conditions met
        if (global_debug)
        {
            printf("DEBUG:All conditions met for an empty pipeline!!\n\n");
        }
        global_pipeline_empty = 1;
    }

    /*
    //Delete
    if (global_cycle_count==300)
    {
        global_pipeline_empty=1;
        global_trace_depleted =1;
    }
    */

    if (global_trace_depleted && global_pipeline_empty)
    {
        return 0;
    }
    else return 1;
}

//--------------------------------------------------------------------------------------------
//Global debug functions to print interface registers
//--------------------------------------------------------------------------------------------
void debug_fetch_pipeline_registers(Tej_Fetch_Pipeline_Reg_File *fetch_pip_reg_file)
{
    printf("DEBUG: Fetch-Decode Registers:\n");
    printf("Tail index: %d\nHead index: %d\nFull: %d\n\n", fetch_pip_reg_file->tail_index, fetch_pip_reg_file->head_index, fetch_pip_reg_file->full);
    for (int i=0; i<fetch_pip_reg_file->fetch_pipeline_rf_size; i++)
    {
        Tej_Fetch_Pipeline_Reg reg = fetch_pip_reg_file->fetch_pipeline_rf_pointer[i];
        printf("Register %d:\n", i);
        printf("PC: %llx, Op_type: %d, Dest: %d, Src1: %d, Src2: %d\n\n", reg.pc, reg.op_type, reg.dest, reg.src1, reg.src2);
    }
}

void debug_decode_pipeline_registers(Tej_Decode_Pipeline_Reg_File *decode_pip_reg_file)
{
    printf("DEBUG: Decode-Rename Registers:\n");
    printf("Tail index: %d\nHead index: %d\nFull: %d\n\n", decode_pip_reg_file->tail_index, decode_pip_reg_file->head_index, decode_pip_reg_file->full);
    for (int i=0; i<decode_pip_reg_file->decode_pipeline_rf_size; i++)
    {
        Tej_Decode_Pipeline_Reg reg = decode_pip_reg_file->decode_pipeline_rf_pointer[i];
        printf("Register %d:\n", i);
        printf("PC: %llx, Op_type: %d, Dest: %d, Src1: %d, Src2: %d\n\n", reg.pc, reg.op_type, reg.dest, reg.src1, reg.src2);
    }
}

void debug_rename_pipeline_registers(Tej_Rename_Pipeline_Reg_File *rename_pip_reg_file)
{
    printf("DEBUG: Rename-RegRd Registers:\n");
    printf("Tail index: %d\nHead index: %d\nFull: %d\n\n", rename_pip_reg_file->tail_index, rename_pip_reg_file->head_index, rename_pip_reg_file->full);
    for (int i=0; i<rename_pip_reg_file->rename_pipeline_rf_size; i++)
    {
        Tej_Rename_Pipeline_Reg reg = rename_pip_reg_file->rename_pipeline_rf_pointer[i];
        printf("Register %d:\n", i);
        printf("PC: %llx, Op_type: %d, Dest: %d, Src1: %d, Src1_Rob: %d, Src2: %d, Src2_Rob: %d\n\n",
        reg.pc, reg.op_type, reg.dest, reg.src1, reg.is_src1_rob, reg.src2, reg.is_src2_rob);
    }
}

void debug_regread_pipeline_registers(Tej_RegisterRd_Pipeline_Reg_File *registerrd_pip_reg_file)
{
    printf("DEBUG: RegRd-Dispatch Registers:\n");
    printf("Tail index: %d\nHead index: %d\nFull: %d\n\n", registerrd_pip_reg_file->tail_index, registerrd_pip_reg_file->head_index, registerrd_pip_reg_file->full);
    for (int i=0; i<registerrd_pip_reg_file->registerrd_pipeline_rf_size; i++)
    {
        Tej_RegisterRd_Pipeline_Reg reg = registerrd_pip_reg_file->registerrd_pipeline_rf_pointer[i];
        printf("Register %d:\n", i);
        printf("Op_type: %d, Dest: %d, Src1: %d, Src1_Rdy: %d, Src2: %d, Src2_Rdy: %d\n\n",
        reg.op_type, reg.dest, reg.src1_or_value, reg.src1_rdy, reg.src2_or_value, reg.src2_rdy);
    }
}

//since rmt is big, just show the entries which are valid, ie if they map to a rob entry.
void debug_rename_map_table(Tej_Rename_Map_Table *rmt)
{
    printf("DEBUG: Rename Map Table\n");
    int flag_valid_entry = 0;
    for (int i=0; i<rmt->rmt_size;i++)
    {
        if (rmt->rmt_pointer[i].valid)
        {
        printf("Index: %d, Valid: %d, RoB tag: %d\n", i, rmt->rmt_pointer[i].valid, rmt->rmt_pointer[i].rob_tag);
        flag_valid_entry = 1;
        }
    }
    if (!flag_valid_entry)
    {
        printf("No mapping found!!\n");
    }
    printf("\n");
}

//since rob is huge, just show the entries which are valid
void debug_reorder_buffer(Tej_Reorder_Buffer *rob)
{
    printf("DEBUG: Reorder Buffer\n");
    printf("Tail index: %d\nHead index: %d\nFull: %d\n\n", rob->tail_index, rob->head_index, rob->full);
    if ((rob->head_index == rob->tail_index) && !rob->full)
    {
        printf("ROB is empty!!\n");
        return;
    }
    for (int i=0; i<rob->rob_size; i++)
    {
        int new_index = (i + rob->tail_index)%rob->rob_size;
        if (!rob->full && (new_index == rob->head_index)) {break;}
        printf("Index: %d, Dest: %d, Rdy: %d, PC: %llx\n", i, rob->rob_pointer[i].dst, rob->rob_pointer[i].rdy, rob->rob_pointer[i].pc);
    }
}



#endif
