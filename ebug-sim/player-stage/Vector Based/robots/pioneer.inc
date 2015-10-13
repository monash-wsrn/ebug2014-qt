include "sensors/p2dx_sonar.inc"

define ebug position 
(
  gui_nose 1                  	# Draw a nose on the robot so we can see which way it points
  obstacle_return 1           	# Can hit things.
  ranger_return 0.5            	# reflects sonar beams
  blob_return 1               	# Seen by blobfinders  
  fiducial_return 1           	# Seen as "1" fiducial finders

  localization "gps" # odm, gps         	  
  localization_origin [0 0 0 0] 	# Start odometry at (0, 0, 0).

	# Physical parameters
	mass 0.5		# (kg)
	size [0.5 0.5 0.1]
  color "red"			# Default color.
  drive "diff"		 	# Differential steering model.

  # Center of rotation offset from c.o.mass
  origin [0 0 0 0]

  # four DOF kinematics limits
  # [ xmin xmax ymin ymax zmin zmax amin amax ]				
  velocity_bounds [-0.5 0.5 0 0 0 0 -90.0 90.0 ]					
  acceleration_bounds [-0.5 0.5 0 0 0 0 -90 90.0 ]



  # alternative odometric localization with simple error model
  # localization "odom"         		# Change to "gps" to have impossibly perfect, global odometry
  #  odom_error [ 0.05 0.05 0.1 ]   	# Odometry error or slip in X, Y and Theta
                                   	# (Uniform random distribution)   





	
	# use the sonar array defined above with a small vertical offset to
  # drop the sensors into the robot body
  p2dx_sonar( pose [0 0 0 0] ) 
  
  
  
  
  
  # Body shape: Octagon of Radius 1
  block( 
		points 16
		point[0] [0 1]
		point[1] [0.3827 0.9239]
		point[2] [0.7071 0.7071]
		point[3] [0.9239 0.3827]
		point[4] [1 0]
		point[5] [0.9239 -0.3827]
		point[6] [0.7071 -0.7071]
		point[7] [0.3827 -0.9239]
		point[8] [0 -1]
		point[9] [-0.3827 -0.9239]
		point[10] [-0.7071 -0.7071]
		point[11] [-0.9239 -0.3827]
		point[12] [-1 0]
		point[13] [-0.9239 0.3827]
		point[14] [-0.7071 0.7071]
		point[15] [-0.3827 0.9239]

    z [2 2.1] # Relative height
  )

)



define pioneer2dx_base_no_sonar pioneer_base
(
  
 )
















define pioneer2dx_base pioneer2dx_base_no_sonar
(
  
)


define pioneer2dx pioneer2dx_base
(  

)




#  prototype for p2dx sonar sensors
define p2dxsonar sensor
(
  # define the size of each transducer [xsize ysize zsize] in meters
  size [0.01 0.05 0.01 ] 
  # define the range bounds [min max]
  range [0 5.0]
	# define the angular field of view in degrees
	fov 15
	# define the number of samples spread over the fov
  samples 1
	# define the color that ranges are drawn in the gui
	color_rgba [ 0 1 0 0.2 ] 
)


define p2dx_sonar ranger
(
 # 16 transducers spread about the robot as follows
 p2dxsonar( pose [ 0.075 0.130 0 90 ] )
 p2dxsonar( pose [ 0.115 0.115 0 50 ] )
 p2dxsonar( pose [ 0.150 0.080 0 30 ] )
 p2dxsonar( pose [ 0.170 0.025 0 10 ] )
 p2dxsonar( pose [ 0.170 -0.025 0 -10 ] )
 p2dxsonar( pose [ 0.150 -0.080 0 -30 ] )
 p2dxsonar( pose [ 0.115 -0.115 0 -50 ] )
 p2dxsonar( pose [ 0.075 -0.130 0 -90 ] )
 p2dxsonar( pose [ -0.155 -0.130 0 -90 ] )
 p2dxsonar( pose [ -0.195 -0.115 0 -130 ] )
 p2dxsonar( pose [ -0.230 -0.080 0 -150 ] )
 p2dxsonar( pose [ -0.250 -0.025 0 -170 ] )
 p2dxsonar( pose [ -0.250 0.025 0 170 ] )
 p2dxsonar( pose [ -0.230 0.080 0 150 ] )
 p2dxsonar( pose [ -0.195 0.115 0 130 ] )
 p2dxsonar( pose [ -0.155 0.130 0 90 ]	 )
)


