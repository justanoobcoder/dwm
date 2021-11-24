/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include <stddef.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;        /* vert inner gap between windows */
static const unsigned int gappoh    = 10;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;        /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/bar.sh"; /* Alternate bar launch command */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]     = "#db2316";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_red  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class                instance                title           tags mask   isfloating  isterminal  noswallow   monitor */
    { "Blueman-manager",    NULL,                   NULL,           0,          1,          0,          1,          -1 },
    { "obs",                NULL,                   NULL,           1 << 8,     0,          0,          0,          -1 },
    { NULL,                 "nm-connection-editor", NULL,           0,          1,          0,          1,          -1 },
    { NULL,                 "google-chrome",        NULL,           1 << 1,     0,          0,          0,          -1 },
    { NULL,                 "brave-browser",        NULL,           1 << 1,     0,          0,          0,          -1 },
    { NULL,                 "microsoft-edge-dev",   NULL,           1 << 1,     0,          0,          0,          -1 },
    { "Firefox",            NULL,                   NULL,           1 << 1,     0,          0,          0,          -1 },
    { "Opera",              NULL,                   NULL,           1 << 1,     0,          0,          0,          -1 },
    { "st-256color",        NULL,                   NULL,           0,          0,          1,          0,          -1 },
    { NULL,                 NULL,                   "Event Tester", 0,          0,          0,          1,          -1 }, /* xev */
};

/* layout(s) */
static const float mfact            = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster            = 1;    /* number of clients in master area */
static const int resizehints        = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection    = 2;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen     = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };

#include "movestack.c"
static Key keys[] = {
    /* modifier                     key         function        argument */
    { MODKEY,                       XK_d,       spawn,          SHCMD("~/.config/rofi/launcher/launcher.sh") },
    { MODKEY,                       XK_Return,  spawn,          SHCMD("$TERMINAL") },
    { MODKEY,                       XK_w,       spawn,          SHCMD("$BROWSER") },
    { MODKEY,                       XK_b,       togglebar,      {0} },
    { MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_p,       incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_j,       movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,       movestack,      {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_h,       incrgaps,       {.i = +1 } },
    { MODKEY|Mod4Mask,              XK_l,       incrgaps,       {.i = -1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_h,       incrogaps,      {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_l,       incrogaps,      {.i = -1 } },
    { MODKEY|Mod4Mask|ControlMask,  XK_h,       incrigaps,      {.i = +1 } },
    { MODKEY|Mod4Mask|ControlMask,  XK_l,       incrigaps,      {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_0,       togglegaps,     {0} },
    { MODKEY|Mod4Mask|ShiftMask,    XK_0,       defaultgaps,    {0} },
    { MODKEY,                       XK_y,       incrihgaps,     {.i = +1 } },
    { MODKEY,                       XK_o,       incrihgaps,     {.i = -1 } },
    { MODKEY|ControlMask,           XK_y,       incrivgaps,     {.i = +1 } },
    { MODKEY|ControlMask,           XK_o,       incrivgaps,     {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_y,       incrohgaps,     {.i = +1 } },
    { MODKEY|Mod4Mask,              XK_o,       incrohgaps,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_y,       incrovgaps,     {.i = +1 } },
    { MODKEY|ShiftMask,             XK_o,       incrovgaps,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Return,  zoom,           {0} },
    { MODKEY,                       XK_Tab,     view,           {0} },
    { MODKEY,                       XK_q,       killclient,     {0} },
    { MODKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_space,   setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
    { MODKEY,                       XK_Down,    moveresize,     {.v = "0x 25y 0w 0h" } },
    { MODKEY,                       XK_Up,      moveresize,     {.v = "0x -25y 0w 0h" } },
    { MODKEY,                       XK_Right,   moveresize,     {.v = "25x 0y 0w 0h" } },
    { MODKEY,                       XK_Left,    moveresize,     {.v = "-25x 0y 0w 0h" } },
    { MODKEY|ShiftMask,             XK_Down,    moveresize,     {.v = "0x 0y 0w 25h" } },
    { MODKEY|ShiftMask,             XK_Up,      moveresize,     {.v = "0x 0y 0w -25h" } },
    { MODKEY|ShiftMask,             XK_Right,   moveresize,     {.v = "0x 0y 25w 0h" } },
    { MODKEY|ShiftMask,             XK_Left,    moveresize,     {.v = "0x 0y -25w 0h" } },
    { MODKEY|ControlMask,           XK_Up,      moveresizeedge, {.v = "t"} },
    { MODKEY|ControlMask,           XK_Down,    moveresizeedge, {.v = "b"} },
    { MODKEY|ControlMask,           XK_Left,    moveresizeedge, {.v = "l"} },
    { MODKEY|ControlMask,           XK_Right,   moveresizeedge, {.v = "r"} },
    { MODKEY|ControlMask|ShiftMask, XK_Up,      moveresizeedge, {.v = "T"} },
    { MODKEY|ControlMask|ShiftMask, XK_Down,    moveresizeedge, {.v = "B"} },
    { MODKEY|ControlMask|ShiftMask, XK_Left,    moveresizeedge, {.v = "L"} },
    { MODKEY|ControlMask|ShiftMask, XK_Right,   moveresizeedge, {.v = "R"} },
    { MODKEY,                       XK_0,       view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
    TAGKEYS(                        XK_1,                       0)
    TAGKEYS(                        XK_2,                       1)
    TAGKEYS(                        XK_3,                       2)
    TAGKEYS(                        XK_4,                       3)
    TAGKEYS(                        XK_5,                       4)
    TAGKEYS(                        XK_6,                       5)
    TAGKEYS(                        XK_7,                       6)
    TAGKEYS(                        XK_8,                       7)
    TAGKEYS(                        XK_9,                       8)
    { MODKEY|ShiftMask,             XK_q,       spawn,          SHCMD("dsysact") },
    { WINKEY|ShiftMask,             XK_s,       spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") },
    { MODKEY|ShiftMask,             XK_r,       quit,           {1} }, 
    { MODKEY,                       XK_Print,   spawn,          SHCMD("drecord") },
    { MODKEY,                       XK_Delete,  spawn,          SHCMD("drecord --kill") },
    { 0,                            XK_Print,   spawn,          SHCMD("dscrot --full") },
    { ShiftMask,                    XK_Print,   spawn,          SHCMD("dscrot") },
    { MODKEY,                       XK_F1,      spawn,          SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
    { MODKEY,                       XK_F2,      spawn,          SHCMD("killall -q screenkey ; screenkey -p fixed -g $(slop -n -f '%g') -M &") },
    { MODKEY,                       XK_F3,      spawn,          SHCMD("killall -q screenkey") },
    { 0, XF86XK_AudioMute,                      spawn,          SHCMD("changeVolume toggle; polybar-msg hook volume 1") },
    { 0, XF86XK_AudioRaiseVolume,               spawn,          SHCMD("changeVolume 5%+; polybar-msg hook volume 1") },
    { 0, XF86XK_AudioLowerVolume,               spawn,          SHCMD("changeVolume 5%-; polybar-msg hook volume 1") },
    { 0, XF86XK_MonBrightnessUp,                spawn,          SHCMD("xbacklight -inc 10%") },
    { 0, XF86XK_MonBrightnessDown,              spawn,          SHCMD("xbacklight -dec 10%") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          SHCMD("$TERMINAL") },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

