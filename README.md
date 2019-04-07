# rodm

This repository contains the different C files to run the RODM project.

To compile any code, use the command `gcc yourfile.c -O9 -o yourfile`, and execute it using `./yourfile [input1] [input2] [input3]`

NB: Since some graphs need text preprocessing (renumbering the nodes mainly), you may need to input 3 arguments depending on the function and the graph.

The data needs to be stored in a `data` folder at the root directory.

##TP1:
`degrees.c`
`bfs.c`
`count_triangles.c`

##TP2:
`pagerank.c`
`kcore.c`

##TP3:
`community.c`

##Utilitaries:
`file.c`
`utils.c`
