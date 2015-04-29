#ifndef ISAAC_BACKEND_TEMPLATES_MPRODUCT_H
#define ISAAC_BACKEND_TEMPLATES_MPRODUCT_H

#include "isaac/backend/templates/base.h"

namespace isaac
{

class model;

struct mproduct_parameters : public base::parameters_type
{
  mproduct_parameters(unsigned int simd_width
                            , int_t local_size_0, int_t KL, int_t local_size_1, int_t D
                            , int_t ms, int_t ks, int_t ns
                            , fetching_policy_type A_fetching_policy, fetching_policy_type B_fetching_policy
                            , int_t local_fetch_0, int_t local_fetch_1);

  int_t kL;
  int_t depth;

  int_t mS;
  int_t kS;
  int_t nS;

  fetching_policy_type A_fetching_policy;
  fetching_policy_type B_fetching_policy;

  int_t local_fetch_0;
  int_t local_fetch_1;

  int_t mL;
  int_t nL;

  bool prefetch;
  bool unroll_outer;
};

class mproduct : public base_impl<mproduct, mproduct_parameters>
{
private:
  unsigned int lmem_usage(expressions_tuple const & expressions) const;
  unsigned int registers_usage(expressions_tuple const & expressions) const;
  int is_invalid_impl(driver::Device const &, expressions_tuple const &) const;
  std::string generate_impl(const char * suffix, expressions_tuple const & expressions, driver::Device const & device, std::vector<mapping_type> const &) const;
  void enqueue_block(driver::CommandQueue & queue, int_t M, int_t N, int_t K, array const & A, array const & B, array const & C,
                     value_scalar const &alpha, value_scalar const &beta, driver::Program & program, const char * suffix, execution_options_type const & options);
  array create_slice(array & M, int_t s0_0, int_t s0_1, int_t s1_0, int_t s1_1, bool swap);
  std::vector<int_t> infos(expressions_tuple const & expressions,  lhs_rhs_element *&C, lhs_rhs_element *&A, lhs_rhs_element *&B, lhs_rhs_element *&alpha, lhs_rhs_element *&beta);
public:
  mproduct(mproduct::parameters_type const & parameters, bool check_bound, char A_trans, char B_trans);
  std::vector<int_t> input_sizes(expressions_tuple const & expressions);
  void cleanup(values_holder beta, controller<expressions_tuple> const & ctr, model & fallback,
               lhs_rhs_element* eA, lhs_rhs_element* eB, lhs_rhs_element* eC, lhs_rhs_element* ebeta, array const & A, array const & B, array const & C);
  void enqueue(driver::CommandQueue & queue, driver::Program & program, const char * suffix, base & fallback, controller<expressions_tuple> const &ctr);
private:
  const char A_trans_;
  const char B_trans_;
  expression_type type_;
  bool check_bounds_;
};

class mproduct_nn : public mproduct
{
public:
  mproduct_nn(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};

class mproduct_tn : public mproduct
{
public:
  mproduct_tn(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};


class mproduct_nt : public mproduct
{
public:
  mproduct_nt(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};


class mproduct_tt : public mproduct
{
public:
  mproduct_tt(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};


}

#endif
