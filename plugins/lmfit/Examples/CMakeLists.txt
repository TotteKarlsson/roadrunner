CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
#rrLevenbergMarquardtDemo.py
)

install(    
FILES 
${examples} 
DESTINATION plugins/Examples
COMPONENT Python
)

