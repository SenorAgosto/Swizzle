MAKE_EXECUTABLE(swizzle
	DEPENDENCIES	
		swzl	
		${Boost_LIBRARIES}		# boost::intrusive_ptr
		${PluginFactory_LIBRARIES}
		${TBB_LIBRARIES} 		# only defined on Unix
		${Wield_LIBRARIES}
)
