[Mesh]
  type = MooseMesh
  file = nonmatching.e
  dim = 2
[]

[Variables]
  [./u]
  [../]
[]

[AuxVariables]
  [./nearest_node_value]
    block = left
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./bottom]
    type = DirichletBC
    variable = u
    boundary = 'leftbottom rightbottom'
    value = 0
  [../]
  [./top]
    type = DirichletBC
    variable = u
    boundary = 'lefttop righttop'
    value = 1
  [../]
[]

[AuxBCs]
  [./nearest_node_value]
    type = NearestNodeValueAux
    variable = nearest_node_value
    boundary = leftright
    paired_variable = u
    paired_boundary = rightleft
  [../]
[]

[Executioner]
  type = Steady
  petsc_options = -snes_mf_operator
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Output]
  output_initial = true
  exodus = true
  perf_log = true
[]
