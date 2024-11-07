
import numpy as np 
import argparse



if __name__ == "__main__":

    parser = argparse.ArgumentParser(
                    prog='config_gen',
                    description='Generate a basic uniform random lammps configuration',
                    epilog='')
    

    parser.add_argument('-n',
                            dest="nparticles",
                            type=int,
                            default=400,
                            help="Number of particles in the initial configuration")
    
    args = parser.parse_args()
    
    nparticles=args.nparticles
    r0= 400./60**3
    lbox=(nparticles/r0)**(1./3)

    positions=np.random.rand(nparticles,3)*lbox*0.99 - lbox*0.99/2

    header=f'''LAMMPS data file

{nparticles} atoms

1 atom types

-{lbox/2:.3f} {lbox/2:.3f} xlo xhi
-{lbox/2:.3f} {lbox/2:.3f} ylo yhi
-{lbox/2:.3f} {lbox/2:.3f} zlo zhi

Masses

1 1
'''


    atoms='''Atoms

'''

    for iparticle in range(nparticles):
        atoms+=f"{iparticle+1} 1 {positions[iparticle,0]:.3f} {positions[iparticle,1]:.3f} {positions[iparticle,2]:.3f} 0 0 0  \n"
    
    velocities='''Velocities

'''

    for iparticle in range(nparticles):
        velocities+=f"{iparticle + 1} 0 0 0  \n"
    



    print(header)
    print(atoms)
    print(velocities)
