#ifndef ATIDLAS_BACKEND_TEMPLATES_MREDUCTION_H
#define ATIDLAS_BACKEND_TEMPLATES_MREDUCTION_H

#include <vector>

#include "atidlas/symbolic/expression.h"
#include "atidlas/backend/templates/base.h"

namespace atidlas
{

struct mreduction_parameters : public base::parameters_type
{
  mreduction_parameters(unsigned int _simd_width,
                                unsigned int _local_size_0, unsigned int _local_size_1,
                                unsigned int _num_groups_0, unsigned int _num_groups_1, fetching_policy_type _fetch_policy);
  unsigned int num_groups_0;
  unsigned int num_groups_1;
  fetching_policy_type fetch_policy;
};


class mreduction : public base_impl<mreduction, mreduction_parameters>
{
protected:
  enum reduction_type
  {
    REDUCE_ROWS,
    REDUCE_COLUMNS
  };
  mreduction(mreduction::parameters_type const & , reduction_type, binding_policy_t);
private:
  virtual int check_invalid_impl(cl::Device const &, expressions_tuple const &) const;
  unsigned int lmem_usage() const;
  std::string generate_impl(unsigned int, expressions_tuple const &, std::vector<mapping_type> const &, unsigned int, std::vector<mapped_mreduction*> const &) const;
  std::vector<std::string> generate_impl(unsigned int, expressions_tuple const &, std::vector<mapping_type> const &) const;
public:
  virtual std::vector<int_t> input_sizes(expressions_tuple const & expressions);
  void enqueue(cl::CommandQueue & queue,std::vector<cl_ext::lazy_compiler> & programs,unsigned int label, controller<expressions_tuple> const &);
private:
  reduction_type reduction_type_;
};

class mreduction_rows : public mreduction
{
public:
  mreduction_rows(mreduction::parameters_type  const &, binding_policy_t binding_policy = BIND_ALL_UNIQUE);
  mreduction_rows(unsigned int simd, unsigned int ls1, unsigned int ls2, unsigned int ng1, unsigned int ng2, fetching_policy_type fetch, binding_policy_t bind = BIND_ALL_UNIQUE);
};

class mreduction_cols : public mreduction
{
public:
  mreduction_cols(mreduction::parameters_type  const &, binding_policy_t binding_policy = BIND_ALL_UNIQUE);
  mreduction_cols(unsigned int simd, unsigned int ls1, unsigned int ls2, unsigned int ng1, unsigned int ng2, fetching_policy_type fetch, binding_policy_t bind = BIND_ALL_UNIQUE);
};

}

#endif