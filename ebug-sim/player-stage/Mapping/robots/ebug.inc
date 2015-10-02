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
	mass 0.240		# (kg)
	size [0.120 0.120 0.085]
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

  
  
  
	# Body shape: Octagon of Radius 1
	block( 
		points 16
		point[0] [0 0.06]
		point[1] [0.023 0.0554]
		point[2] [0.0424 0.0424]
		point[3] [0.0554 0.023]
		point[4] [0.06 0]
		point[5] [0.0554 -0.023]
		point[6] [0.0424 -0.0424]
		point[7] [0.023 -0.0554]
		point[8] [0 -0.06]
		point[9] [-0.023 -0.0554]
		point[10] [-0.0424 -0.0424]
		point[11] [-0.0554 -0.023]
		point[12] [-0.06 0]
		point[13] [-0.0554 0.023]
		point[14] [-0.0424 0.0424]
		point[15] [-0.023 0.0554]
		z [0.067 0.085] # Relative height
	)
  

	# chasis
	block
	(
		points 4
		point[0] [ 0.049 0.032 ]
		point[1] [ 0.049 -0.032 ]
		point[2] [ -0.049 -0.032 ]
		point[3] [ -0.049 0.032 ]
		z [0.026 0.067]
		color "white"
	)


	# left wheel
	block
	(
		points 4
		point[0] [  0.034  -0.032 ]
		point[1] [  0.034  -0.039 ]
		point[2] [ -0.034  -0.039 ]
		point[3] [ -0.034  -0.032 ]
		z [ 0 0.067 ]
		color "gray15"
	)

	# right wheel
	block
	(
		points 4
		point[0] [  0.034  0.032 ]
		point[1] [  0.034  0.039 ]
		point[2] [ -0.034  0.039 ]
		point[3] [ -0.034  0.032 ]
		z [ 0 0.067 ]
		color "gray15"
	)

	# screen
	block( 
		points 4
		point[0] [ 0 -0.040 ]
		point[1] [ 0.036 -0.040 ]
		point[2] [ 0.036 0.040 ]
		point[3] [ 0 0.040 ]
		z [0.085 0.100]
		color "black"
	)

	# Sensor
	ebug_ranger() 
  
  
)


##### SENSOR DEFINITION #######

define ebug_ranger ranger
(
	sensor(
		pose [ 0 0 0 0 ]
	
		#size [0.036 0.080 0.015 ]
		range [0 3] # [min max]
		fov 360 # Field of view in degrees
		samples 16
	)
	

)




