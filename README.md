# Terrain-Generation

<h3>Introduction</h3>
	<p>Terrain generation in this demo is achieved by converting noise into a height map. There are many ways of creating noise which could be generated from a bitmap or an algorithm. In this demo noise is created with the use of a common technique used to create a basic terrain which is the 1/f noise (“pink noise”). To create the noise Midpoint Displacement is used, which is also known as a diamond square algorithm.Since the 1/f noise is too isotropic, we can incorporate the Voronoi diagrams to give the terrain a more realistic look.</p> 

<h3>Diamond Square algorithm</h3>
	<p>This algorithm is essentially a way of generating 1/f noise by a midpoint displacement method. This algorithm recursively goes through two steps, diamond step and a square step, hence the name. For example, to start with a two dimensional array, the four corners of the array must be initially set and all the other values are to be calculated. In the diamond step take the average of the four corners plus a random offset value and place it in the center of the diamond. Then in the square step take the average of all corners plus a random offset and place it in the center of the square.</p>
