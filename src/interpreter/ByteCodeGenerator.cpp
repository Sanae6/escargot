#include "Escargot.h"
#include "ByteCodeGenerator.h"
#include "interpreter/ByteCode.h"
#include "parser/ast/AST.h"

namespace Escargot {

void ByteCodeGenerator::generateByteCode(Context* c, CodeBlock* codeBlock, Node* ast)
{
    ByteCodeBlock* block = new ByteCodeBlock();

    ASSERT(ast->type() == Program);
    ParserContextInformation info(false, true);
    ByteCodeGenerateContext ctx(codeBlock, block, info);
    ast->generateStatementByteCode(block, &ctx);

#ifndef NDEBUG
    if (getenv("DUMP_BYTECODE") && strlen(getenv("DUMP_BYTECODE"))) {
        printf("dumpBytecode...>>>>>>>>>>>>>>>>>>>>>>\n");
        size_t idx = 0;
        size_t bytecodeCounter = 0;
        char* code = block->m_code.data();
        char* end = &block->m_code.data()[block->m_code.size()];
        while (&code[idx] < end) {
            ByteCode* currentCode = (ByteCode *)(&code[idx]);

            Opcode opcode = EndOpcode;
            for (size_t i = 0; i < OpcodeKindEnd; i ++) {
                if (g_opcodeTable.m_reverseTable[i].first == currentCode->m_opcodeInAddress) {
                    opcode = g_opcodeTable.m_reverseTable[i].second;
                    break;
                }
            }

            if (opcode == EndOpcode) {
                break;
            }

            switch (opcode) {
#define DUMP_BYTE_CODE(code, pushCount, popCount) \
            case code##Opcode:\
                currentCode->dumpCode(); \
                idx += sizeof(code); \
                continue;

            FOR_EACH_BYTECODE_OP(DUMP_BYTE_CODE)

#undef  DUMP_BYTE_CODE
            default:
                RELEASE_ASSERT_NOT_REACHED();
                break;
            };
        }
        printf("dumpBytecode...<<<<<<<<<<<<<<<<<<<<<<\n");
    }
#endif

    codeBlock->m_byteCodeBlock = block;
}

}
