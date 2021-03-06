#include "StmtCompiler.h"
#include "CodeGen.h"
#include "CodeGen_X86.h"
#include "CodeGen_GPU_Host.h"
#include "CodeGen_ARM.h"
#include "CodeGen_PNaCl.h"

namespace Halide {
namespace Internal {

using std::string;
using std::vector;

StmtCompiler::StmtCompiler(Target target) {
    if (target.os == Target::OSUnknown) {
        target = get_host_target();
    }

    // The awkward mapping from targets to code generators
    if ((target.features & Target::CUDA) || (target.features & Target::OpenCL)) {
        if (target.arch == Target::X86) {
            contents = new CodeGen_GPU_Host<CodeGen_X86>(target);
        }
        else if (target.arch == Target::ARM) {
            contents = new CodeGen_GPU_Host<CodeGen_ARM>(target);
        }
        else {
            user_error << "Invalid target architecture for GPU backend: "
                       << target.to_string() << "\n";
        }
    } else if (target.arch == Target::X86) {
        contents = new CodeGen_X86(target);
    } else if (target.arch == Target::ARM) {
        contents = new CodeGen_ARM(target);
    } else if (target.arch == Target::PNaCl) {
        contents = new CodeGen_PNaCl(target);
    }
}

void StmtCompiler::compile(Stmt stmt, string name,
                           const vector<Argument> &args,
                           const vector<Buffer> &images_to_embed) {
    contents.ptr->compile(stmt, name, args, images_to_embed);
}

void StmtCompiler::compile_to_bitcode(const string &filename) {
    contents.ptr->compile_to_bitcode(filename);
}

void StmtCompiler::compile_to_native(const string &filename, bool assembly) {
    contents.ptr->compile_to_native(filename, assembly);
}

JITCompiledModule StmtCompiler::compile_to_function_pointers() {
    return contents.ptr->compile_to_function_pointers();
}

}
}
