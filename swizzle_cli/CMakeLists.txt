MAKE_EXECUTABLE(swizzle
	DEPENDENCIES	
		swizzle
		${Boost_LIBRARIES}		# boost::intrusive_ptr
		${TBB_LIBRARIES} 		# only defined on Unix
		${Wield_LIBRARIES}
		${ObjectGraph_LIBRARIES}
)
