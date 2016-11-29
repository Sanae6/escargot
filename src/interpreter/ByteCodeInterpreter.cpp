#include "Escargot.h"
#include "ByteCode.h"
#include "ByteCodeInterpreter.h"
#include "runtime/Environment.h"
#include "runtime/EnvironmentRecord.h"

namespace Escargot {

NEVER_INLINE void registerOpcode(Opcode opcode, void* opcodeAddress)
{
    static std::unordered_set<void *> labelAddressChecker;
    if (labelAddressChecker.find(opcodeAddress) != labelAddressChecker.end()) {
        ESCARGOT_LOG_ERROR("%d\n", opcode);
        RELEASE_ASSERT_NOT_REACHED();
    }

    static int cnt = 0;
    g_opcodeTable.m_table[opcode] = opcodeAddress;
    g_opcodeTable.m_reverseTable[labelAddressChecker.size()] = std::make_pair(opcodeAddress, opcode);
    labelAddressChecker.insert(opcodeAddress);

    if (opcode == EndOpcode) {
        labelAddressChecker.clear();
    }
}

template <typename CodeType>
ALWAYS_INLINE void executeNextCode(size_t& programCounter)
{
    programCounter += sizeof(CodeType);
}

void ByteCodeIntrepreter::interpret(ExecutionState& state, CodeBlock* codeBlock)
{
    if (UNLIKELY(codeBlock == nullptr)) {
        goto FillOpcodeTable;
    }

    {
        ASSERT(codeBlock->byteCodeBlock() != nullptr);
        ByteCodeBlock* byteCodeBlock = codeBlock->byteCodeBlock();
        ExecutionContext* ec = state.executionContext();
        LexicalEnvironment* env = ec->lexicalEnvironment();
        Value* registerFile = ALLOCA(byteCodeBlock->m_requiredRegisterFileSizeInValueSize * sizeof(Value), Value, state);
        char* codeBuffer = byteCodeBlock->m_code.data();
        size_t programCounter = (size_t)(&codeBuffer[0]);
        ByteCode* currentCode;

    #define NEXT_INSTRUCTION() \
        goto NextInstruction

        NextInstruction:
        currentCode = (ByteCode *)programCounter;
        ASSERT(((size_t)currentCode % sizeof(size_t)) == 0);

        goto *(currentCode->m_opcodeInAddress);

        LoadLiteralOpcodeLbl:
        {
            LoadLiteral* code = (LoadLiteral*)currentCode;
            registerFile[code->m_registerIndex] = code->m_value;
            executeNextCode<LoadLiteral>(programCounter);
            NEXT_INSTRUCTION();
        }

        LoadByNameOpcodeLbl:
        {
            LoadByName* code = (LoadByName*)currentCode;
            registerFile[code->m_registerIndex] = loadByName(state, env, code->m_name);
            executeNextCode<LoadByName>(programCounter);
            NEXT_INSTRUCTION();
        }

        StoreByNameOpcodeLbl:
        {
            StoreByName* code = (StoreByName*)currentCode;
            env->record()->setMutableBinding(state, code->m_name, registerFile[code->m_registerIndex]);
            executeNextCode<StoreByName>(programCounter);
            NEXT_INSTRUCTION();
        }

        BinaryPlusOpcodeLbl:
        {
            BinaryPlus* code = (BinaryPlus*)currentCode;
            Value v0 = registerFile[code->m_srcIndex0];
            Value v1 = registerFile[code->m_srcIndex1];
            // TODO
            registerFile[code->m_srcIndex0] = Value(v0.asNumber() + v1.asNumber());
            executeNextCode<StoreByName>(programCounter);
            NEXT_INSTRUCTION();
        }

        StoreExecutionResultOpcodeLbl:
        {
            StoreExecutionResult* code = (StoreExecutionResult*)currentCode;
            *state.exeuctionResult() = registerFile[code->m_registerIndex];
            executeNextCode<StoreExecutionResult>(programCounter);
            NEXT_INSTRUCTION();
        }

        EndOpcodeLbl:
        {
            return;
        }
    }

    FillOpcodeTable:
    {
#define REGISTER_TABLE(opcode, pushCount, popCount) \
        registerOpcode(opcode##Opcode, &&opcode##OpcodeLbl);
        FOR_EACH_BYTECODE_OP(REGISTER_TABLE);
    }
    return;
}


Value ByteCodeIntrepreter::loadByName(ExecutionState& state, LexicalEnvironment* env, const AtomicString& name)
{
    while (env) {
        EnvironmentRecord::GetBindingValueResult result = env->record()->getBindingValue(state, name);
        if (result.m_hasBindingValue) {
            return result.m_value;
        }
        env = env->outerEnvironment();
    }
    // TODO throw reference error
    RELEASE_ASSERT_NOT_REACHED();
}



}
