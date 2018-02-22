MAKE_EXECUTABLE(swizzle_cli
	DEPENDENCIES	
		swizzle	
		${Boost_LIBRARIES}		# boost::intrusive_ptr
		${PluginFactory_LIBRARIES}
		${TBB_LIBRARIES} 		# only defined on Unix
		${Wield_LIBRARIES}
)
