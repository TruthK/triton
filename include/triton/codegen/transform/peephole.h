#ifndef TDL_INCLUDE_CODEGEN_OPTIMIZE_TRANS_H
#define TDL_INCLUDE_CODEGEN_OPTIMIZE_TRANS_H


namespace triton {

namespace ir {
  class module;
  class value;
  class instruction;
  class trans_inst;
  class builder;
  class constant_int;
  class dot_inst;
}

namespace codegen{
namespace transform{

class peephole {
private:
  bool rewrite_trans_phi(ir::instruction* value, ir::builder &builder);
  bool rewrite_dot_fp32(ir::dot_inst *dot, ir::builder& builder, bool trans_a, bool trans_b, ir::value *A, ir::value *B, ir::value *D);
  bool rewrite_dot_hmma(ir::dot_inst *dot, ir::builder& builder, bool trans_a, bool trans_b, ir::value *A, ir::value *B, ir::value *D);
  bool rewrite_dot(ir::instruction *value, ir::builder& builder);
  bool rewrite_unit_red(ir::instruction *value, ir::builder& builder);
  bool rewrite_gep_ptr_min_off_plus_off(ir::instruction *value, ir::builder& builder);

private:

public:
  peephole() {}
  void run(ir::module &mod);
};


}
}
}

#endif
