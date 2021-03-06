/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#include "RichardsHalfGaussianSink.h"
#include "Material.h"

#include <iostream>


template<>
InputParameters validParams<RichardsHalfGaussianSink>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<Real>("max", "Maximum of the flux (measured in kg.m^-2.s^-1).  Flux out = max*exp((-0.5*(p - centre)/sd)^2) for p<centre, and Flux out = max for p>centre.  Note, to make this a source rather than a sink, let max<0");
  params.addRequiredParam<Real>("sd", "Standard deviation of the Gaussian (measured in Pa).  Flux out = max*exp((-0.5*(p - centre)/sd)^2) for p<centre, and Flux out = max for p>centre.");
  params.addRequiredParam<Real>("centre", "Centre of the Gaussian (measured in Pa).  Flux out = max*exp((-0.5*(p - centre)/sd)^2) for p<centre, and Flux out = max for p>centre.");
  params.addParam<FunctionName>("multiplying_fcn", 1.0, "If this function is provided, the flux will be multiplied by this function.  This is useful for spatially or temporally varying sinks");
  return params;
}

RichardsHalfGaussianSink::RichardsHalfGaussianSink(const std::string & name,
                                             InputParameters parameters) :
    IntegratedBC(name,parameters),
    _maximum(getParam<Real>("max")),
    _sd(getParam<Real>("sd")),
    _centre(getParam<Real>("centre")),
    _m_func(getFunction("multiplying_fcn"))
{}

Real
RichardsHalfGaussianSink::computeQpResidual()
{
  Real test_fcn_f = _test[_i][_qp]*_m_func.value(_t, _q_point[_qp]);

  if (_u[_qp] >= _centre) {
    return test_fcn_f*_maximum;
  }
  else {
    return test_fcn_f*_maximum*exp(-0.5*std::pow((_u[_qp] - _centre)/_sd, 2));
  }
}

Real
RichardsHalfGaussianSink::computeQpJacobian()
{
  Real test_fcn_f = _test[_i][_qp]*_m_func.value(_t, _q_point[_qp]);

  if (_u[_qp] >= _centre) {
    return 0.0;
  }
  else {
    return -test_fcn_f*_maximum*(_u[_qp] - _centre)/std::pow(_sd, 2)*exp(-0.5*std::pow((_u[_qp] - _centre)/_sd, 2))*_phi[_j][_qp];
  }
}
