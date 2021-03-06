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

// MOOSE includes
#include "AddOutputAction.h"
#include "FEProblem.h"
#include "Factory.h"
#include "OutputWarehouse.h"
#include "Output.h"
#include "MooseApp.h"
#include "FileMesh.h"
#include "MooseApp.h"

// libMesh includes
#include "libmesh/mesh_function.h"
#include "libmesh/mesh_refinement.h"
#include "libmesh/explicit_system.h"

template<>
InputParameters validParams<AddOutputAction>()
{
   InputParameters params = validParams<MooseObjectAction>();
   return params;
}

AddOutputAction::AddOutputAction(const std::string & name, InputParameters params) :
    MooseObjectAction(name, params)
{
}

void
AddOutputAction::act()
{
  // Create a list of reserved names
  std::set<std::string> reserved;
  reserved.insert("none");                  // allows 'none' to be used as a keyword in the postprocessors 'outputs' parameter
//  reserved.insert("moose_debug_outputter"); // the [Debug] block creates this object

  // Get a reference to the OutputWarehouse
  OutputWarehouse & output_warehouse = _app.getOutputWarehouse();

  // Get the output object name
  std::string object_name = getShortName();

  // Reject the reserved names
  if (reserved.find(object_name) != reserved.end())
    mooseError("The name " << object_name << " is a reserved name for outputters");

  // Check that an object by the same name does not already exist; this must be done before the object
  // is created to avoid getting misleading errors from the Parser
  if (output_warehouse.hasOutput(object_name))
    mooseError("The output object named '" << object_name << "' already exists");

  // Add a pointer to the FEProblem class
  _moose_object_pars.addPrivateParam<FEProblem *>("_fe_problem",  _problem);

  // Apply the common parameters
  _moose_object_pars.applyParameters(output_warehouse.getCommonParameters());

  // Set the correct value for the binary flag for XDA/XDR output
  if (_type.compare("XDR") == 0)
    _moose_object_pars.set<bool>("_binary") = true;
  else if (_type.compare("XDA") == 0)
    _moose_object_pars.set<bool>("_binary") = false;

  // Adjust the checkpoint suffix if auto recovery was enabled
  if (object_name.compare("auto_recovery_checkpoint") == 0)
    _moose_object_pars.set<std::string>("suffix") = "auto_recovery";

  // Create the object and add it to the warehouse
  Output * output = static_cast<Output *>(_factory.create(_type, object_name, _moose_object_pars));
  output_warehouse.addOutput(output);
}
