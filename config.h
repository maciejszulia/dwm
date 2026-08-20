/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int barheightpx        = 5;        /* pixels added above+below bar text */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=11" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_dim[]         = "#6f6f6f";  /* labels, behind their values */
static const char col_caut[]        = "#e5c07b";  /* limit running low */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeWarn] = { "#ff4444", col_gray1, col_gray2 },
	[SchemeDim]  = { col_dim,   col_gray1, col_gray2 },
	[SchemeCaut] = { col_caut,  col_gray1, col_gray2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
                              "10", "11", "12", "13", "14", "15", "16", "17", "18" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	/* st -c <class> ustawia WM_CLASS, wiec terminale agro trafiaja na stale tagi */
	{ "agro-watch", NULL,     NULL,       1 << 0,       0,           -1 },
	{ "agro-omx",   NULL,     NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 0 keeps tiled splits exact */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(MOD,KEY,TAG) \
	{ MOD,                          KEY,      view,           {.ui = 1 << TAG} }, \
	{ MOD|ControlMask,              KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MOD|ShiftMask,                KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MOD|ControlMask|ShiftMask,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *clipcmd[] = { "clipmenu", NULL };
static const char *volu[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *vold[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *volt[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *briu[] = { "/home/ms/.local/bin/brightness-up", NULL };
static const char *brid[] = { "/home/ms/.local/bin/brightness-down", NULL };
static const char *xfvolu[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *xfvold[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *xfvolm[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *xfmicm[] = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };
static const char *xfbriu[] = { "/home/ms/.local/bin/brightness-up", NULL };
static const char *xfbrid[] = { "/home/ms/.local/bin/brightness-down", NULL };
static const char *scrfull[] = { "/home/ms/.local/bin/screenshot-full", NULL };
static const char *scrarea[] = { "/home/ms/.local/bin/screenshot-area", NULL };
static const char *cheatcmd[] = { "st", "-e", "/home/ms/.local/bin/sciaga", NULL };
static const char *omxviewcmd[] = { "st", "-e", "/home/ms/.local/bin/omx-podglad", NULL };
static const char *speechcmd[] = { "/home/ms/bin/speech-to-codex-record", "toggle", NULL };
static const char *termcmd[]  = { "st", NULL };
/* left click anywhere on the status clears a blinking weather alert */
static const char *weatherack[] = { "/home/ms/.local/bin/weather", "--ack", NULL };
/* scroll on the status steps the same rotation, up = previous fact */
static const char *weatherprev[] = { "/home/ms/.local/bin/weather", "--turn", "-1", NULL };
static const char *weathernext[] = { "/home/ms/.local/bin/weather", "--turn", "1", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = clipcmd } },
	{ Mod3Mask,                      XK_slash,  spawn,          {.v = speechcmd } },
	{ MODKEY,                       XK_equal,  spawn,          {.v = volu } },
	{ MODKEY,                       XK_minus,  spawn,          {.v = vold } },
	{ MODKEY,                       XK_F1,     spawn,          {.v = volt } },
	{ MODKEY|ShiftMask,             XK_equal,  spawn,          {.v = briu } },
	{ MODKEY|ShiftMask,             XK_minus,  spawn,          {.v = brid } },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn, {.v = xfvolu } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, {.v = xfvold } },
	{ 0,                            XF86XK_AudioMute,         spawn, {.v = xfvolm } },
	{ 0,                            XF86XK_AudioMicMute,      spawn, {.v = xfmicm } },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn, {.v = xfbriu } },
	{ 0,                            XF86XK_MonBrightnessDown, spawn, {.v = xfbrid } },
	{ 0,                            XK_Print,  spawn,          {.v = scrfull } },
	{ ShiftMask,                    XK_Print,  spawn,          {.v = scrarea } },
	{ MODKEY,                       XK_F12,    spawn,          {.v = cheatcmd } },
	{ MODKEY,                       XK_F9,     spawn,          {.v = omxviewcmd } },
	{ MODKEY,                       XK_F11,    settile50,      {0} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(MODKEY,                 XK_1,                      0)
	TAGKEYS(MODKEY,                 XK_2,                      1)
	TAGKEYS(MODKEY,                 XK_3,                      2)
	TAGKEYS(MODKEY,                 XK_4,                      3)
	TAGKEYS(MODKEY,                 XK_5,                      4)
	TAGKEYS(MODKEY,                 XK_6,                      5)
	TAGKEYS(MODKEY,                 XK_7,                      6)
	TAGKEYS(MODKEY,                 XK_8,                      7)
	TAGKEYS(MODKEY,                 XK_9,                      8)
	TAGKEYS(MODKEY|Mod1Mask,        XK_1,                      9)
	TAGKEYS(MODKEY|Mod1Mask,        XK_2,                      10)
	TAGKEYS(MODKEY|Mod1Mask,        XK_3,                      11)
	TAGKEYS(MODKEY|Mod1Mask,        XK_4,                      12)
	TAGKEYS(MODKEY|Mod1Mask,        XK_5,                      13)
	TAGKEYS(MODKEY|Mod1Mask,        XK_6,                      14)
	TAGKEYS(MODKEY|Mod1Mask,        XK_7,                      15)
	TAGKEYS(MODKEY|Mod1Mask,        XK_8,                      16)
	TAGKEYS(MODKEY|Mod1Mask,        XK_9,                      17)
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = weatherack } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button4,        spawn,          {.v = weatherprev } },
	{ ClkStatusText,        0,              Button5,        spawn,          {.v = weathernext } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
	{ ClkTagBar,            0,              Button5,        shiftview,      {.i = +1} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
