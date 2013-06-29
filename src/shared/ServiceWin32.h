/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifdef WIN32
#ifndef _WIN32_SERVICE_
#define _WIN32_SERVICE_

bool WinServiceInstall();
bool WinServiceUninstall();
bool WinServiceRun();

#endif                                                      // _WIN32_SERVICE_
#endif                                                      // WIN32

