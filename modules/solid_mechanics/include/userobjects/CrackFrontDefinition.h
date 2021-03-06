#ifndef CRACKFRONTDEFINITION_H
#define CRACKFRONTDEFINITION_H

#include "GeneralUserObject.h"
#include "BoundaryRestrictable.h"
#include <set>

class CrackFrontDefinition;
class AuxiliarySystem;

template<>
InputParameters validParams<CrackFrontDefinition>();
void addCrackFrontDefinitionParams(InputParameters& params);

/**
 * Works on top of NodalNormalsPreprocessor
 */
class CrackFrontDefinition :
  public GeneralUserObject,
  public BoundaryRestrictable
{
public:
  CrackFrontDefinition(const std::string & name, InputParameters parameters);
  virtual ~CrackFrontDefinition();

  virtual void initialSetup();
  virtual void initialize();
  virtual void finalize();
  virtual void execute();
  virtual void threadJoin(const UserObject & uo);

  const Node & getCrackFrontNode(const unsigned int node_index) const;
  const RealVectorValue & getCrackFrontTangent(const unsigned int node_index) const;
  Real getCrackFrontForwardSegmentLength(const unsigned int node_index) const;
  Real getCrackFrontBackwardSegmentLength(const unsigned int node_index) const;
  const RealVectorValue & getCrackDirection(const unsigned int node_index) const;
  bool treatAs2D() const {return _treat_as_2d;}

protected:

  enum DIRECTION_METHOD
  {
    CRACK_DIRECTION_VECTOR,
    CRACK_MOUTH,
    CURVED_CRACK_FRONT
  };

  enum END_DIRECTION_METHOD
  {
    NO_SPECIAL_TREATMENT,
    END_CRACK_DIRECTION_VECTOR
  };

  enum CRACK_NODE_TYPE
  {
    MIDDLE_NODE,
    END_1_NODE,
    END_2_NODE
  };

  AuxiliarySystem & _aux;
  MooseMesh & _mesh;

  std::vector<unsigned int> _ordered_crack_front_nodes;
  std::vector<RealVectorValue> _tangent_directions;
  std::vector<RealVectorValue> _crack_directions;
  std::vector<std::pair<Real,Real> > _segment_lengths;
  Real _overall_length;
  DIRECTION_METHOD _direction_method;
  END_DIRECTION_METHOD _end_direction_method;
  RealVectorValue _crack_direction_vector;
  RealVectorValue _crack_direction_vector_end_1;
  RealVectorValue _crack_direction_vector_end_2;
  std::vector<BoundaryName> _crack_mouth_boundary_names;
  std::vector<BoundaryID> _crack_mouth_boundary_ids;
  RealVectorValue _crack_mouth_coordinates;
  RealVectorValue _crack_plane_normal_from_curved_front;
  bool _treat_as_2d;
  unsigned int _axis_2d;

  void getCrackFrontNodes(std::set<unsigned int>& nodes);
  void orderCrackFrontNodes(std::set<unsigned int> nodes);
  void orderEndNodes(std::vector<unsigned int> &end_nodes);
  void updateCrackFrontGeometry();
  void updateDataForCrackDirection();
  RealVectorValue calculateCrackFrontDirection(const Node* crack_front_node,
                                               const RealVectorValue& tangent_direction,
                                               const CRACK_NODE_TYPE ntype) const;

};


#endif /* CRACKFRONTDEFINITION_H */
