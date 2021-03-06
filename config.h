/* See LICENSE file for copyright and license details. */
/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int sidepad            = 0;       /* vertical padding of bar */
static const int vertpad            = 0;       /* horizontal padding of bar */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int gappx              = 10;       
static const char dmenufont[]       = "Inconsolata Nerd Font:style=Regular:size=14:antialias=true:autohint=true";
static const char *fonts[]          = { dmenufont };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_purple[]      = "#4c2462";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_purple,  col_purple  },
};

/* tagging */
static const char *tags[] = { "١", "٢", "٣", "٤", "٥", "٦", "٧", "٨", "٩" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     iscentered     isfloating   monitor */
    { "Tor Browser",  NULL,       NULL,       0,            1,              1,          -1 },
    { "st-center",    NULL,       NULL,       0,            1,              1,          -1 },
    { "Gcr-prompter", NULL,       NULL,       0,            1,              1,          -1 },
    { "trayer",       NULL,       NULL,       0x100,        0,              1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
    { "[\\]",     dwindle }, /* first entry is default */
    { "[]=",      tile },    
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ ALTKEY,                       KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_purple, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *privbrowsercmd[] = { "firefox", "--private-window", NULL };
static const char *volupcmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *voldowncmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *volmutecmd[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *brupcmd[] = { "light", "-A", "10", NULL };
static const char *brdowncmd[] = { "light", "-U", "10", NULL };
static const char *dpsetcmd[] = { "dpset", NULL };
static const char *screenshotcmd[] = { "screenshot", NULL };
static const char *powermancmd[] = { "powerman", NULL };
static const char *ncmpcppcmd[] = { "music", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *loficmd[] = { "lofi", NULL };

/* macros for some keys */
#define XK_Dp XF86XK_Display
#define XK_VolU XF86XK_AudioRaiseVolume
#define XK_VolD XF86XK_AudioLowerVolume
#define XK_VolM XF86XK_AudioMute
#define XK_BklU XF86XK_MonBrightnessUp
#define XK_BklD XF86XK_MonBrightnessDown
#define XK_Off XF86XK_PowerOff


#include <X11/XF86keysym.h>
#include "movestack.c"
#include "selfrestart.c"
static Key keys[] = {
	/* modifier                     key        function        argument */

	/* app launchers */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = privbrowsercmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = ncmpcppcmd } },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = loficmd } },

	/* display management */
	{ 0,                            XK_Dp,     spawn,          {.v = dpsetcmd } },
	{ MODKEY,                       XK_m,      spawn,          {.v = dpsetcmd } },

	/* volume management */
	{ 0,                            XK_VolU,   spawn,          { .v = volupcmd } },
	{ 0,                            XK_VolD,   spawn,          { .v = voldowncmd } },
	{ 0,                            XK_VolM,   spawn,          { .v = volmutecmd } },

	/* brightness management */
	{ 0,                            XK_BklU,   spawn,          { .v = brupcmd } },
	{ 0,                            XK_BklD,   spawn,          { .v = brdowncmd } },

	/* power management */
	{ 0,                            XK_Off,    spawn,          { .v = powermancmd } },
	{ MODKEY,                       XK_x,      spawn,          { .v = powermancmd } },

	/* screenshot */
	{ 0,                            XK_Print,  spawn,          { .v = screenshotcmd } },
	{ MODKEY,                       XK_p,      spawn,          { .v = screenshotcmd } },

	/* layout management */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,			XK_f,	   setlayout,	   {.v = &layouts[3]  } },
	{ MODKEY|ShiftMask,		XK_f,	   setlayout,	   {.v = &layouts[2]  } },
	
	/* client management */
	{ MODKEY,                       XK_z,      zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	/* stack management */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

	/* monitor management */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* tag management */
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Left,   viewtoleft,     {0} },
	{ MODKEY,                       XK_Right,  viewtoright,    {0} },
	{ MODKEY|ShiftMask,             XK_Left,   tagtoleft,      {0} },
	{ MODKEY|ShiftMask,             XK_Right,  tagtoright,     {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	/* restart/quit */
	{ MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

