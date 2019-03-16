Group Name: Quoth The Server 404
Members: 
	Shaun Prince : 47721782
	Gavin Dent : 57079804
	Alexis Padilla : 36931764

Controls:
	W - forward (will leave a trail of yarn (That grows!)
			 to damage enemies, like Tron/Snake)
	A/D - Turn

Goal:
	Collect the most mice points (including the rare black mice which
				are worth 10x the points)
	Use the yarn you leave behind (Does 3 dmg) to gate off enemy players, less
	they risk dying and giving you free reign over the remaining mice.
	

Network:
	We added a .2s fixed timestep to the server
	(large time gap so we could see the affect of interpolation).
	Then, We had the client Interpolate each frame toward
	the next server authorative position it received is.
	The client also tracks the delta time between update receive
	so that increased network delays do not trip up the interpolation
	, at least as much as possible. The interpolation is choppy
	for the first few seconds after starting a match, but smooths
	out after allowing for smoother movement even at higher pings.