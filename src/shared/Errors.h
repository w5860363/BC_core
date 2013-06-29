/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKECORE_ERRORS_H
#define BLIZZLIKECORE_ERRORS_H

#include "Common.h"

#ifdef HAVE_ACE_STACK_TRACE_H                               // old versions ACE not have Stack_Trace.h but used at some oS for better compatibility
#define WPAssert( assertion ) { if (!(assertion)) { ACE_Stack_Trace st; fprintf( stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s\n%s\n", __FILE__, __LINE__,__FUNCTION__,  #assertion, st.c_str()); assert( #assertion &&0 ); } }
#else
#define WPAssert( assertion ) { if (!(assertion)) { fprintf( stderr, "\n%s:%i in %s ASSERTION FAILED2:\n  %s\n", __FILE__, __LINE__,__FUNCTION__,  #assertion); assert( #assertion &&0 ); } }
#endif
#define WPError( assertion, errmsg ) if ( ! (assertion) ) { sLog.outError( "%\n%s:%i in %s ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); assert( false ); }
#define WPWarning( assertion, errmsg ) if ( ! (assertion) ) { sLog.outError( "\n%s:%i in %s WARNING:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); }

#define WPFatal( assertion, errmsg ) if ( ! (assertion) ) { sLog.outError( "\n%s:%i in %s FATAL ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); assert( #assertion &&0 ); abort(); }

#define ASSERT WPAssert
#endif

