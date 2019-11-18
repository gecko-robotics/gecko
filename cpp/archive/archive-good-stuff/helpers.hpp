/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once


inline constexpr int  G_EXIT_FAILURE = 1;
inline constexpr int  G_EXIT_SUCCESS = 0;

#define GECKO_DEPRECATED(msg) [[deprecated(msg)]]

char *getUserName(void);
