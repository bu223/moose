/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef SOLUTIONHISTORY_H
#define SOLUTIONHISTORY_H

// MOOSE includes
#include "FileOutput.h"

// Forward declerations
class SolutionHistory;

template<>
InputParameters validParams<SolutionHistory>();

/**
 * Based class for adding basic filename support to output base class
 *
 * @see Exodus
 */
class SolutionHistory :
  public FileOutput
{
public:

  /**
   * Class constructor
   *
   * The constructor performs all of the necessary initialization of the various
   * output lists required for the various output types.
   *
   * @see initAvailable init seperate
   */
  SolutionHistory(const std::string & name, InputParameters & parameters);

  /**
   * Class destructor
   */
  virtual ~SolutionHistory();

  /**
   * Output the data to *.slh file
   */
  virtual void output();

  /**
   * The filename for the output file
   * @return A string of output file including the extension
   */
  virtual std::string filename();

protected:
  //@{
  /**
   * Individual component output is not supported for solution history output
   */
  virtual void outputNodalVariables();
  virtual void outputElementalVariables();
  virtual void outputPostprocessors();
  virtual void outputScalarVariables();
  //@}

};

#endif /* SOLUTIONHISTORY_H */
