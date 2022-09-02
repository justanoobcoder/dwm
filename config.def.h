/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include <stddef.h>
#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "eww-bar"; /* Alternate bar class name */
static const char *altbarcmd        = "$HOME/bar.sh"; /* Alternate bar launch command */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#db2316";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_red  },
};

static const char *const autostart[] = {
	"eww", "daemon", NULL,
	"eww", "open", "bar", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class                instance                title           tags mask   isfloating  isterminal  noswallow   monitor */
    { NULL,                 "google-chrome",        NULL,           1 << 1,     0,          0,          0,          -1 },
    { NULL,                 "brave-browser",        NULL,           1 << 1,     0,          0,          0,          -1 },
    { NULL,                 "microsoft-edge",       NULL,           1 << 1,     0,          0,          0,          -1 },
    { "Firefox",            NULL,                   NULL,           1 << 1,     0,          0,          0,          -1 },
    { "Opera",              NULL,                   NULL,           1 << 1,     0,          0,          0,          -1 },
    { "Alacritty",          NULL,                   NULL,           0,          0,          1,          0,          -1 },
};

/* layout(s) */
static const float mfact            = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster            = 1;    /* number of clients in master area */
static const int resizehints        = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection    = 2;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen     = 1; /* 1 will force focus on the fullscreen window */

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
	{ 0, MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define MULTIKEY_THRESHOLD_MS_PRESS 200
#define MULTIKEY_THRESHOLD_MS_HOLD 700

#include "movestack.c"
static const Key keys[] = {
    /* npresses,    modifier                        key         function        argument */
    { 0,            MODKEY,                         XK_p,       spawn,          SHCMD("~/.config/rofi/launcher/launcher.sh") },
    { 0,            MODKEY,                         XK_Return,  spawn,          SHCMD("$TERMINAL") },
    { 0,            MODKEY,                         XK_w,       spawn,          SHCMD("$BROWSER") },
    { 0,            MODKEY,                         XK_c,       spawn,          SHCMD("rofi -modi \"clipboard:greenclip print\" -show clipboard -run-command '{cmd}'") },
    { 0,            MODKEY|ShiftMask,               XK_c,       spawn,          SHCMD("pkill greenclip && greenclip clear && greenclip daemon &") },
    { 0,            MODKEY,                         XK_b,       togglebar,      {0} },
    { 0,            MODKEY,                         XK_j,       focusstack,     {.i = +1 } },
    { 0,            MODKEY,                         XK_k,       focusstack,     {.i = -1 } },
    { 0,            MODKEY,                         XK_i,       incnmaster,     {.i = +1 } },
    { 0,            MODKEY,                         XK_d,       incnmaster,     {.i = -1 } },
    { 0,            MODKEY,                         XK_h,       setmfact,       {.f = -0.05} },
    { 0,            MODKEY,                         XK_l,       setmfact,       {.f = +0.05} },
    { 0,            MODKEY,                         XK_s,       togglesticky,   {0} },
    { 0,            MODKEY|ShiftMask,               XK_Return,  zoom,           {0} },
    { 0,            MODKEY,                         XK_Tab,     view,           {0} },
    { 0,            MODKEY,                         XK_q,       killclient,     {0} },
    { 0,            MODKEY,                         XK_x,       killunsel,      {0} },
    { 0,            MODKEY,                         XK_t,       setlayout,      {.v = &layouts[0]} },
    { 0,            MODKEY,                         XK_f,       setlayout,      {.v = &layouts[1]} },
    { 0,            MODKEY,                         XK_m,       setlayout,      {.v = &layouts[2]} },
    { 0,            MODKEY,                         XK_space,   setlayout,      {0} },
    { 0,            MODKEY|ShiftMask,               XK_j,       movestack,      {.i = +1 } },
    { 0,            MODKEY|ShiftMask,               XK_k,       movestack,      {.i = -1 } },
    { 0,            MODKEY|ShiftMask,               XK_space,   togglefloating, {0} },
    { 0,            MODKEY,                         XK_0,       view,           {.ui = ~0 } },
    { 0,            MODKEY|ShiftMask,               XK_0,       tag,            {.ui = ~0 } },
    { 0,            MODKEY,                         XK_comma,   focusmon,       {.i = -1 } },
    { 0,            MODKEY,                         XK_period,  focusmon,       {.i = +1 } },
    { 0,            MODKEY|ShiftMask,               XK_comma,   tagmon,         {.i = -1 } },
    { 0,            MODKEY|ShiftMask,               XK_period,  tagmon,         {.i = +1 } },
    { 0,            MODKEY|WINKEY,                  XK_h,       incrgaps,       {.i = +1 } },
    { 0,            MODKEY|WINKEY,                  XK_l,       incrgaps,       {.i = -1 } },
    { 0,            MODKEY|WINKEY|ShiftMask,        XK_h,       incrogaps,      {.i = +1 } },
    { 0,            MODKEY|WINKEY|ShiftMask,        XK_l,       incrogaps,      {.i = -1 } },
    { 0,            MODKEY|WINKEY|ControlMask,      XK_h,       incrigaps,      {.i = +1 } },
    { 0,            MODKEY|WINKEY|ControlMask,      XK_l,       incrigaps,      {.i = -1 } },
    { 0,            MODKEY|WINKEY,                  XK_0,       togglegaps,     {0} },
    { 0,            MODKEY|WINKEY|ShiftMask,        XK_0,       defaultgaps,    {0} },
    { 0,            MODKEY,                         XK_y,       incrihgaps,     {.i = +1 } },
    { 0,            MODKEY,                         XK_o,       incrihgaps,     {.i = -1 } },
    { 0,            MODKEY|ControlMask,             XK_y,       incrivgaps,     {.i = +1 } },
    { 0,            MODKEY|ControlMask,             XK_o,       incrivgaps,     {.i = -1 } },
    { 0,            MODKEY|WINKEY,                  XK_y,       incrohgaps,     {.i = +1 } },
    { 0,            MODKEY|WINKEY,                  XK_o,       incrohgaps,     {.i = -1 } },
    { 0,            MODKEY|ShiftMask,               XK_y,       incrovgaps,     {.i = +1 } },
    { 0,            MODKEY|ShiftMask,               XK_o,       incrovgaps,     {.i = -1 } },
    { 0,            MODKEY|ShiftMask,               XK_q,       spawn,          SHCMD("powermenu") },
    { 0,            MODKEY|ShiftMask,               XK_r,       quit,           {1} }, 
    { 0,            WINKEY|ShiftMask,               XK_s,       spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") },

    { 0,            0,                              XK_Print,   spawn,          SHCMD("dscrot --full") },
    { 0,            ShiftMask,                      XK_Print,   spawn,          SHCMD("dscrot") },
    { 0,            MODKEY,                         XK_F1,      spawn,          SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
    { 0,            MODKEY,                         XK_F2,      spawn,          SHCMD("killall -q screenkey ; screenkey -p fixed -g $(slop -n -f '%g') -M &") },
    { 0,            MODKEY,                         XK_F3,      spawn,          SHCMD("killall -q screenkey") },

    { 0,            0, XF86XK_AudioMute,                        spawn,          SHCMD("changeVolume toggle") },
    { 0,            0, XF86XK_AudioRaiseVolume,                 spawn,          SHCMD("changeVolume 5%+") },
    { 0,            0, XF86XK_AudioLowerVolume,                 spawn,          SHCMD("changeVolume 5%-") },
    { 0,            0, XF86XK_MonBrightnessUp,                  spawn,          SHCMD("brightnessctl set 10%+") },
    { 0,            0, XF86XK_MonBrightnessDown,                spawn,          SHCMD("brightnessctl set 10%-") },

    TAGKEYS(                                     XK_1,                      0)
    TAGKEYS(                                     XK_2,                      1)
    TAGKEYS(                                     XK_3,                      2)
    TAGKEYS(                                     XK_4,                      3)
    TAGKEYS(                                     XK_5,                      4)
    TAGKEYS(                                     XK_6,                      5)
    TAGKEYS(                                     XK_7,                      6)
    TAGKEYS(                                     XK_8,                      7)
    TAGKEYS(                                     XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

