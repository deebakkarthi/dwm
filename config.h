#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */

static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */

static const int swallowfloating    = 1;       /* 1 means swallow floating windows by default*/

static const int showbar            = 1;        /* 0 means no bar */

static const int showsystray        = 1;       /*0 means no tray*/
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/

static const int topbar             = 1;        /* 0 means bottom bar */

static const char *fonts[]          = {"Monaco:size=13:style=Regular", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "Monaco:size=13";

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char sel_border[]      = "#007aff";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  sel_border},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class          instance    title           tags mask     isfloating   isterminal  noswallow  monitor */
    { "Gimp",         NULL,       NULL,           0,            1,           0,           0,        -1 },
    { "Firefox",      NULL,       NULL,           0,            0,           0,          -1,        -1 },
    { "kdenlive",     NULL,       NULL,           0,            1,           0,           0,        -1 },
    { "st-256color",           NULL,       NULL,           0,            0,           1,           0,        -1 },
    { NULL,           NULL,       "Event Tester", 0,            0,           0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
    /* modifier                     key        function        argument */
    { 0,XF86XK_HomePage,          spawn,     SHCMD("st -e ranger") },
    { 0,XF86XK_Mail,                spawn,     SHCMD("st -e neomutt") },
    { 0,XF86XK_Search,              spawn,     SHCMD("st -e fzf") },
    { 0,XF86XK_Calculator,          spawn,     SHCMD("st -e bc -l") },
    { 0,XF86XK_Tools,               spawn,     SHCMD("spotify") },
    { 0,XF86XK_AudioPrev,           spawn,     SHCMD("mpc prev") },
    { 0,XF86XK_AudioPlay,           spawn,     SHCMD("mpc toggle") },
    { 0,XF86XK_AudioNext,           spawn,     SHCMD("mpc next") },
    { 0,XF86XK_AudioMute,           spawn,     SHCMD("amixer sset Master toggle ; pkill -RTMIN+1 dwmblocks") },
    { 0,XF86XK_AudioRaiseVolume,    spawn,     SHCMD("amixer sset Master 5%+ ; pkill -RTMIN+1 dwmblocks") },
    { 0,XF86XK_AudioLowerVolume,    spawn,     SHCMD("amixer sset Master 5%- ; pkill -RTMIN+1 dwmblocks") },
    { MODKEY,                       XK_Print,    spawn,       SHCMD("scrot '%Y-%m-%d_%X.png' -e 'mv $f /home/deebakkarthi/pictures/screenshots/' ") },
    { MODKEY,                       XK_grave,   togglescratch, {.v = scratchpadcmd } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    
    { MODKEY,                       XK_minus,    spawn,       SHCMD("amixer sset Master 5%- ; pkill -RTMIN+1 dwmblocks") },
    /*{ MODKEY|ShiftMask,             XK_minus,    spawn,       SHCMD("") },*/

    { MODKEY,                       XK_equal,    spawn,       SHCMD("amixer sset Master 5%+ ; pkill -RTMIN+1 dwmblocks") },
    /*{ MODKEY|ShiftMask,             XK_equal,    spawn,       SHCMD("") },*/

    /*{ MODKEY,                       XK_BackSpace, spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_BackSpace, spawn,      SHCMD("") },*/

    { MODKEY,                       XK_Tab,    view,           {0} },
    /*{ MODKEY|ShiftMask,             XK_Tab,      spawn,          SHCMD("") },*/

    { MODKEY,                       XK_q,      spawn,           SHCMD("syscontrol") },
    { MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

    { MODKEY,                       XK_w,      spawn,          SHCMD("brave") },
    { MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("brave --incognito") },

    { MODKEY,                       XK_e,      quit,           {0} },
    /*{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("") },*/

    /*{ MODKEY,                       XK_r,      spawn,          SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD("") },*/

    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    /*{ MODKEY|ShiftMask,             XK_t,      spawn,           SHCMD("") }, */

    { MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
    /*{ MODKEY|ShiftMask,             XK_y,      spawn,           SHCMD("") }, */

    { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[2]} },
    /*{ MODKEY|ShiftMask,             XK_y,      spawn,           SHCMD("") }, */

    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },

    /*{ MODKEY,                       XK_o,      spawn,           SHCMD("") }, */
    /*{ MODKEY|ShiftMask,             XK_o,      spawn,           SHCMD("") }, */

    /*{ MODKEY,                       XK_p,      spawn,           SHCMD("") }, */ 
    /*{ MODKEY|ShiftMask,             XK_p,      spawn,           SHCMD("") }, */

    { MODKEY,                       XK_bracketleft,     incrgaps,     {.i = +1 } },
    { MODKEY|ShiftMask,             XK_bracketleft,     incrgaps,     {.i = -1 } },

    { MODKEY,                       XK_bracketright,    defaultgaps,  {0} },      
    { MODKEY|ShiftMask,             XK_bracketright,    togglegaps,   {0} },   

    /*{ MODKEY,                       XK_backslash,       spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_backslash,       spawn,      SHCMD("") }, */

    /*{ MODKEY,                       XK_a,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_a,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_s,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_s,      spawn,      SHCMD("") },*/


    { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd} },
    /*{ MODKEY|ShiftMask,             XK_d,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_f,      fullscreen,     {0}},
    /*{ MODKEY|ShiftMask,             XK_f,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_g,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_g,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    /*{ MODKEY|ShiftMask,             XK_h,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    /*{ MODKEY|ShiftMask,             XK_j,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    /*{ MODKEY|ShiftMask,             XK_k,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    /*{ MODKEY|ShiftMask,             XK_l,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_semicolon,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_semicolon,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_apostrophe,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_apostrophe,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },

    /*{ MODKEY,                       XK_z,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_z,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_x,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_x,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_c,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_c,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_v,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_v,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_b,      togglebar,      {0} },
    /*{ MODKEY|ShiftMask,             XK_b,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_n,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_n,      spawn,      SHCMD("") },*/

    /*{ MODKEY,                       XK_m,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_m,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

    /*{ MODKEY,                       XK_slash,      spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,             XK_slash,      spawn,      SHCMD("") },*/

    { MODKEY,                       XK_space,  zoom,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
/*Quitting*/
static const int EMPTY_WINDOW_COUNT = 6;
/*This the number of windows in an empty desktop, this also includes daemons so
 * the number is higher than 0*/

