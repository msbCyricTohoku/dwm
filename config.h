/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "st"
#define BROWSER "librewolf"

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 10;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
/* Make sure to install a Nerd Font for the icons to render correctly */
static char *fonts[]          = { "JetBrainsMono Nerd Font:size=11", "NotoColorEmoji:pixelsize=11:antialias=true:autohint=true"  };

/* Catppuccin color scheme */
static char normbgcolor[]           = "#24273a";
static char normbordercolor[]       = "#363a4f";
static char normfgcolor[]           = "#cad3f5";
static char selbgcolor[]            = "#89b4fa";
static char selbordercolor[]        = "#89b4fa";
static char selfgcolor[]            = "#24273a";

static char *colors[][3] = {
       /* fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
/* Alacritty uses different flags for naming and geometry */
const char *spcmd1[] = {TERMINAL, "--class", "spterm,spterm", "-o", "window.dimensions.columns=120", "-o", "window.dimensions.lines=34", NULL };
const char *spcmd2[] = {TERMINAL, "--class", "spcalc,spcalc", "-o", "font.size=16", "-o", "window.dimensions.columns=50", "-o", "window.dimensions.lines=20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "", "", "﬏", "", "", "", "", "", "" };


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 * Tip: Use `xprop` in a terminal and click on a window to find its WM_CLASS.
	*/
	/* class,            instance,  title,          tags mask,  isfloating, isterminal, noswallow, monitor */
	{ TERMCLASS,          NULL,      NULL,           0,          0,          1,          0,         -1 },
	{ "vscodium",         NULL,      NULL,           1 << 2,     0,          0,          0,         -1 },
	{ "Caja",             NULL,      NULL,           1 << 3,     1,          0,          0,         -1 },
	{ "Gimp",             NULL,      NULL,           0,          1,          0,          0,         -1 },
	{ "VirtualBox Manager", NULL,    NULL,           0,          1,          0,          0,         -1 },
	{ "Filezilla",        NULL,      NULL,           0,          1,          0,          0,         -1 },
	{ "Transmission-gtk", NULL,      NULL,           0,          1,          0,          0,         -1 },
	{ "Screengrab",       NULL,      NULL,           0,          1,          0,          0,         -1 },
	{ "Thunderbird",      NULL,      NULL,           1 << 8,     0,          0,          0,         -1 },
	{ "libreoffice", NULL, NULL,       0,          1,          0,          0,         -1 },
	{ NULL,               "spterm",  NULL,           SPTAG(0),   1,          1,          0,         -1 },
	{ NULL,               "spcalc",  NULL,           SPTAG(1),   1,          1,          0,         -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },					/* 0 tile */
	{ "TTT",	bstack },				/* 1 bstack */
	{ "[@]",	spiral },				/* 2 spiral */
	{ "[\\]",	dwindle },				/* 3 dwindle */
	{ "[D]",	deck },					/* 4 deck */
	{ "[M]",	monocle },				/* 5 monocle */
	{ "|M|",	centeredmaster },		/* 6 centeredmaster */
	{ ">M>",	centeredfloatingmaster },/* 7 centeredfloatingmaster */
	{ "><>",	NULL },					/* 8 float */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } },

/* helper for spawning shell commands */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",		    INTEGER, &borderpx },
		{ "snap",		        INTEGER, &snap },
		{ "showbar",		    INTEGER, &showbar },
		{ "topbar",		        INTEGER, &topbar },
		{ "nmaster",		    INTEGER, &nmaster },
		{ "resizehints",	    INTEGER, &resizehints },
		{ "mfact",		        FLOAT,	 &mfact },
		{ "gappih",		        INTEGER, &gappih },
		{ "gappiv",		        INTEGER, &gappiv },
		{ "gappoh",		        INTEGER, &gappoh },
		{ "gappov",		        INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		    INTEGER, &smartgaps },
};


#include <X11/XF86keysym.h>
#include "shiftview.c"
//{ MODKEY,                       XK_w,           spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
static const Key keys[] = {
	/* modifier              key            function                argument */
	/* Application Bindings */
	{ MODKEY,			     XK_Return,     spawn,                  {.v = termcmd } },
	{ MODKEY,			     XK_w,          spawn,                  SHCMD("librewolf") },
	{ MODKEY|ShiftMask,		     XK_w,          spawn,                  SHCMD("chromium") },
	{ MODKEY,			     XK_d,          spawn,                  SHCMD("dmenu_run") },
	{ MODKEY,			     XK_e,          spawn,                  SHCMD("emacs") },
	{ MODKEY|ShiftMask,		 XK_e,          spawn,                  SHCMD("emacs") },
	{ MODKEY,			     XK_r,          spawn,                  SHCMD("thunar") },
	{ MODKEY|ShiftMask,      XK_m,          spawn,                  SHCMD("thunderbird") },
	{ MODKEY|ShiftMask,      XK_o,          spawn,                  SHCMD("libreoffice") },
	{ MODKEY|ControlMask,    XK_t,          spawn,                  SHCMD("texstudio") },
	{ MODKEY|ControlMask,    XK_f,          spawn,                  SHCMD("filezilla") },
	{ MODKEY|ControlMask,    XK_b,          spawn,                  SHCMD("transmission-gtk") },
	{ MODKEY|ShiftMask,		 XK_v,          spawn,                  SHCMD("virtualbox") },
    { 0,                     XK_Print,      spawn,                  SHCMD("maim -s | xclip -selection clipboard -t image/png -i") },
	{ ShiftMask,			 XK_Print,      spawn,                  SHCMD("screengrab -r") },
	/* Volume Bindings */
	//{ MODKEY,			XK_F1,		spawn,		SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
	//{ MODKEY,			XK_F2,		spawn,		SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	//{ MODKEY,			XK_F3,		spawn,		SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
//	{ 0, XF86XK_AudioMute,        spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
 //   { 0, XF86XK_AudioLowerVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
  //  { 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },


	{ 0, XF86XK_AudioMute,        spawn, SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle") },
    { 0, XF86XK_AudioLowerVolume, spawn, SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-") },
    { 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("wpctl set-volume -l 1.0 @DEFAULT_AUDIO_SINK@ 5%+") },

	/* Lockscreen Binding */
	{ MODKEY|ControlMask,		XK_l,		spawn,		SHCMD("slock") },



	/* DWM Bindings */
	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)
	TAGKEYS(			XK_1,          0)
	TAGKEYS(			XK_2,          1)
	TAGKEYS(			XK_3,          2)
	TAGKEYS(			XK_4,          3)
	TAGKEYS(			XK_5,          4)
	TAGKEYS(			XK_6,          5)
	TAGKEYS(			XK_7,          6)
	TAGKEYS(			XK_8,          7)
	TAGKEYS(			XK_9,          8)
	{ MODKEY,			XK_0,	       view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,	       tag,                    {.ui = ~0 } },

	{ MODKEY,			XK_Tab,        view,                   {0} },
	{ MODKEY,			XK_q,          killclient,             {0} },
	{ MODKEY|ShiftMask,		XK_q,          quit,                   {0} },
	{ MODKEY,			XK_t,          setlayout,              {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,          setlayout,              {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_y,          setlayout,              {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_y,          setlayout,              {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			XK_u,          setlayout,              {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		XK_u,          setlayout,              {.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			XK_i,          setlayout,              {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_i,          setlayout,              {.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,			XK_b,          togglebar,              {0} },
	{ MODKEY,			XK_f,          togglefullscr,          {0} },
	{ MODKEY,			XK_g,          shiftview,              { .i = -1 } },
	{ MODKEY,			XK_h,          setmfact,               {.f = -0.05} },
	{ MODKEY,			XK_l,          setmfact,               {.f = +0.05} },
	{ MODKEY,			XK_semicolon,  shiftview,              { .i = 1 } },
	{ MODKEY,			XK_space,      zoom,                   {0} },
	{ MODKEY|ShiftMask,		XK_space,      togglefloating,         {0} },
	{ MODKEY,			XK_F5,         xrdb,                   {.v = NULL } },

	/* Gaps Bindings */
	{ MODKEY,			XK_a,          togglegaps,             {0} },
	{ MODKEY|ShiftMask,		XK_a,          defaultgaps,            {0} },
	{ MODKEY,			XK_z,          incrgaps,               {.i = +3 } },
	{ MODKEY,			XK_x,          incrgaps,               {.i = -3 } },

	/* Monitor Bindings */
	{ MODKEY,			XK_Left,       focusmon,               {.i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,       tagmon,                 {.i = -1 } },
	{ MODKEY,			XK_Right,      focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_Right,      tagmon,                 {.i = +1 } },

	/* Scratchpad Bindings */
	{ MODKEY|ShiftMask,		XK_Return,     togglescratch,          {.ui = 0} },
	{ MODKEY,			    XK_apostrophe, togglescratch,          {.ui = 1} },

};

/* button definitions */
static const Button buttons[] = {
	/* click                event mask    button          function        argument */
	{ ClkLtSymbol,          0,            Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,            Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,            Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,       Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,       Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,       Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,            Button1,        view,           {0} },
	{ ClkTagBar,            0,            Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,       Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,       Button3,        toggletag,      {0} },
    { ClkTagBar,		    0,		      Button4,	      shiftview,      {.i = -1} },
	{ ClkTagBar,		    0,		      Button5,	      shiftview,      {.i = 1} },
};
