//   Copyright (c) 2003-2012 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
//   Some rights reserved: http://opensource.org/licenses/mit

#ifndef		_mach_override_
#define		_mach_override_

#include <sys/types.h>
#include <mach/error.h>

#define	err_cannot_override	(err_local|1)

__BEGIN_DECLS


    mach_error_t
mach_override_ptr(
	void *originalFunctionAddress,
    const void *overrideFunctionAddress,
    void **originalFunctionReentryIsland );

__END_DECLS

 
#ifdef	__cplusplus
#define MACH_OVERRIDE( ORIGINAL_FUNCTION_RETURN_TYPE, ORIGINAL_FUNCTION_NAME, ORIGINAL_FUNCTION_ARGS, ERR )		\
{																												\
	static ORIGINAL_FUNCTION_RETURN_TYPE (*ORIGINAL_FUNCTION_NAME##_reenter)ORIGINAL_FUNCTION_ARGS;				\
	static bool ORIGINAL_FUNCTION_NAME##_overriden = false;														\
	class mach_override_class__##ORIGINAL_FUNCTION_NAME {														\
	public:																										\
		static kern_return_t override(void *originalFunctionPtr) {												\
			kern_return_t result = err_none;																	\
			if (!ORIGINAL_FUNCTION_NAME##_overriden) {															\
				ORIGINAL_FUNCTION_NAME##_overriden = true;														\
				result = mach_override_ptr( (void*)originalFunctionPtr,											\
											(void*)mach_override_class__##ORIGINAL_FUNCTION_NAME::replacement,	\
											(void**)&ORIGINAL_FUNCTION_NAME##_reenter );						\
			}																									\
			return result;																						\
		}																										\
		static ORIGINAL_FUNCTION_RETURN_TYPE replacement ORIGINAL_FUNCTION_ARGS {

#define END_MACH_OVERRIDE( ORIGINAL_FUNCTION_NAME )																\
		}																										\
	};																											\
																												\
	err = mach_override_class__##ORIGINAL_FUNCTION_NAME::override((void*)ORIGINAL_FUNCTION_NAME);				\
}
#endif

#endif	
