//
//  Jonathan Salwan - Copyright (C) 2013-08
// 
//  http://shell-storm.org
//  http://twitter.com/JonathanSalwan
//
//  Note: Example 1 - http://shell-storm.org/blog/Taint-analysis-with-Pin/
//        Simple taint memory area from read syscall. 
//

#include "pin.H"
#include <fstream>
#include <iostream>
#include <list>
#include <time.h>
#include <AnalysisProcessor.h>
#include <IRBuilder.h>
#include <IRBuilderFactory.h>
#include <Inst.h>
#include <PINContextHandler.h>

//####################### Global Variable
std::ofstream TraceFile;
//####################### Global Variable




/* bytes range tainted */
AnalysisProcessor   ap;
std::list<UINT32> bytesTainted; //Taint시킬 데이터
std::list<REG> regsTainted;

BOOL bit = FALSE;
UINT32 add_start = 0x415260; // 추적을 시작 할 오프셋 
UINT32 add_end = 0x4153b0; // 추적을 그만둘 오프셋
CONTEXT *context;


INT32 Usage()
{

	return 0;
}

VOID init(UINT32 start ,UINT32 end)
{
	return;
}

bool dll_detect(UINT32 Addr)
{
	if (Addr == 0x41529D)
	{
		uint64 reg_value = ap.getRegisterValue(ID_RAX);
		printf("%#x\n", reg_value);
		ap.taintMem(reg_value);
		printf("isMemTainted = %d\n", ap.isMemTainted(reg_value));
		//system("pause");
	}

	if (Addr > add_start && Addr < add_end)
		return TRUE;
	return FALSE;
}

static void trace_callback(IRBuilder *irb, CONTEXT *ctx, BOOL hasEA, ADDRINT ea, THREADID threadId, INS ins, string *insDis, UINT32 insAddr)
{
	if (!dll_detect(insAddr))
		return;

	//printf("%#x %#x %s\n", PIN_GetContextReg(ctx, REG_EAX), *((char *)(0x00400000)),insDis->c_str());
	/* Mutex */
	ap.lock();
	if (hasEA)
		irb->setup(ea);

	/* Update the current context handler */
	ap.updateCurrentCtxH(new PINContextHandler(ctx, threadId));

	/* Setup Information into Irb */
	irb->setThreadID(ap.getThreadID());
	//printf("%s\n" , irb->getDisassembly().c_str());

	Inst *inst = irb->process(ap);
	printf("%d, %s\n", inst->numberOfElements(),inst->getDisassembly().c_str());
	SymbolicEngine *Engine = &ap.getSymbolicEngine();
	list<SymbolicElement *>Ele = inst->getSymbolicElements();
	list<SymbolicElement *>::iterator i;
	if (inst->numberOfElements() != 0)
	{
		for (i = Ele.begin(); i != Ele.end(); i++)
		{
			SymbolicElement *x = *i;
			//printf("x tainted check [%d] \n", x->isTainted);
			if (x->isTainted)
			{
				TraceFile << "[" << inst->getAddress() << "]" << inst->getDisassembly().c_str() << "\t" << x->getDestination() << std::endl;
				break;
			}
				
		}
	}

	
	ap.addInstructionToTrace(inst);

	/* Export some information from Irb to Inst */
	inst->setOpcode(irb->getOpcode());
	inst->setOpcodeCategory(irb->getOpcodeCategory());
	inst->setOperands(irb->getOperands());

	/* Mutex */
	ap.unlock();
}
VOID Instruction(INS ins, VOID *v)
{
	IRBuilder *irb = createIRBuilder(ins);
	/* Callback before */
	if (INS_MemoryOperandCount(ins) > 0)
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_callback,
		IARG_PTR, irb,
		IARG_CONTEXT,
		IARG_BOOL, true,
		IARG_MEMORYOP_EA, 0,
		IARG_THREAD_ID,
		IARG_PTR, ins,
		IARG_PTR, new string(INS_Disassemble(ins)),
		IARG_ADDRINT, INS_Address(ins),
		IARG_END);
	else
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_callback,
		IARG_PTR, irb,
		IARG_CONTEXT,
		IARG_BOOL, false,
		IARG_ADDRINT, 0,
		IARG_THREAD_ID,
		IARG_PTR, ins,
		IARG_PTR, new string(INS_Disassemble(ins)),
		IARG_ADDRINT, INS_Address(ins),
		IARG_END);

	if (INS_Address(ins) == add_end)
	{
		bit = FALSE;
		PIN_RemoveInstrumentation();
	}
}


VOID Fini(INT32 code, VOID *v)
{

	TraceFile.close();

}

int main(int argc, char *argv[])
{
	if (PIN_Init(argc, argv)){
		return Usage();
	}


	string trace_header = string("#\n"
		"# Taint Analysis Start\n"
		"#\n");

	TraceFile.open("MyPinOut.log");
	TraceFile.write(trace_header.c_str(), trace_header.size());
	TraceFile.setf(ios::showbase);



	printf("Taint Analysis trial\n");
	printf("1.stop Address:");
	//scanf("%x", &add_start);
	fflush(stdin);
	printf("\nadd_start : %08x\n", add_start);

	init(0,0);
	PIN_SetSyntaxIntel();
	INS_AddInstrumentFunction(Instruction, 0);
	PIN_AddFiniFunction(Fini, 0);
	PIN_StartProgram();

	return 0;
}

