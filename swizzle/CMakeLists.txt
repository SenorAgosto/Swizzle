MAKE_LIBRARY(swizzle
	DEPENDENCIES	
		${Boost_LIBRARIES}		# boost::intrusive_ptr
		${TBB_LIBRARIES} 		# only defined on Unix
		${Wield_LIBRARIES}
		${ObjectGraph_LIBRARIES}
)
