  _____        __     __  _______ _____            _____ ______ _____  
 |  __ \     /\\ \   / / |__   __|  __ \     /\   / ____|  ____|  __ \ 
 | |__) |   /  \\ \_/ /     | |  | |__) |   /  \ | |    | |__  | |__) |
 |  _  /   / /\ \\   /      | |  |  _  /   / /\ \| |    |  __| |  _  / 
 | | \ \  / ____ \| |       | |  | | \ \  / ____ \ |____| |____| | \ \ 
 |_|  \_\/_/    \_\_|       |_|  |_|  \_\/_/    \_\_____|______|_|  \_\                                                                                                  
                                                                                                  

RAYTRACER is a c program that renders scenes in 3 dimensions and save them as
images in files.

To build the software, just run the command "make" or "make all".
To delete the software, just run "make clean".

You can render any scene stocked in the json format by running the command
"./rt /path/to/your/scene.json output.ppm", output.ppm being your output file.

Only the ppm format is allowed here. The same goes for the json format.
Both of these are mandatory.

NB: "Make clean" only deletes the software but does not delete
the scenes that where rendered before.

Usage :

- ./rt /path/to/your/scene.json output.ppm

ex:

- ./rt examples/scenes/stage-0/5-multiple-mesh.json output.ppm

Author : romain.baguet 
         rayane.amrouche 
