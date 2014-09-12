``post_process_ogs5``

=================
This code is used to get gloabal solutions from the subdomains, which are caculated by using PETSc solver,  collected and stored in binray file.

The binary file of nodal solutiuons can be generated if ``$DAT_TYPE`` for domain output is BINRAY, e.g.
```
#OUTPUT
.
.
. 
 $GEO_TYPE
   DOMAIN 
 $DAT_TYPE
   BINRAY
 $TIM_TYPE
   STEPS 1
   
```
A number of binary files of nodal varariables are generated after a parallel computation. The files names take the form as
```
[input file name]_[process name]_domain_node_value_header.txt
[input file name]_[process name]_domain_variables.bin   
```

The number of the binary files is identical to the number of the PDEs solved in the computation.

The source code can be compiled by a simple command, e.g
```
g++ -o post-process -O3 post_process.cpp
```


The post-process is performed by using the compiled command as

```
./post-process [input file name]_[process name]
```

After that, several VTK files with be generated according to the times that are specified .out file. Then one need to use cat command to concatenate the corresponding vtk mesh file and these files together for visualization. For example, the input file is ``foo``, the mesh is partitioned into 5 partitions, and the process type for output is ``DEFORMATION``. After partitioning, a new VTK mesh file will be generated with name of ``foo_renum_4.vtk``. A complete VTK file can be then obtained by command
```
cat foo_renum_4.vtk  foo_DEFORMATION_[number].vtk > [new file name].vtk
```
