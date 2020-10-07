#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Consolas:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#008bc2";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "Q", "D", "A", "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                  instance    title       tags mask     isfloating   monitor */
	{ "Gimp",                 NULL,       NULL,       0,            1,           -1 },
	{ "firefox",              NULL,       NULL,       1 << 1,       0,           -1 },
	{ "htop",                 NULL,       NULL,       0,            1,           -1 },
	{ "Spotify",              NULL,       NULL,       1 << 5,       0,           -1 },
	{ "jetbrains-rider",      NULL,       NULL,       1 << 2,       1,           -1 },
	{ "code-oss",             NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Google-chrome",        NULL,       NULL,       1 << 3,       0,           -1 },
	{ "copyq",                NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[]        = { "rofi", "-show", "drun", "-drun-display-format", "{name}", NULL };
static const char *termcmd[]        = { "alacritty", NULL };
static const char *rangercmd[]      = { "alacritty", "--class", "ranger,ranger", "-e", "ranger", NULL };
static const char *htopcmd[]        = { "alacritty", "-d", "150", "50", "--class", "htop,htop", "-e", "htop", NULL };
static const char *monitorcmd[]     = { "/home/bogdan/.config/scripts/monitors.sh", NULL };
static const char *suspendcmd[]     = { "systemctl", "suspend", NULL };
static const char *lockcmd[]        = { "/home/bogdan/.config/scripts/lockscreen.sh", NULL };
static const char *mpvcmd[]         = { "/home/bogdan/.config/scripts/mpv.sh", NULL };
static const char *printcmd[]       = { "flameshot", "gui", NULL };
static const char *mutecmd0[]       = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *mutecmd1[]       = { "pactl", "set-sink-mute", "1", "toggle", NULL };
static const char *volupcmd[]       = { "amixer", "-q", "set", "Master", "5%+", "unmute", NULL };
static const char *voldowncmd[]     = { "amixer", "-q", "set", "Master", "5%-", "unmute", NULL };
static const char *brupcmd[]        = { "light", "-A", "20", NULL };
static const char *brdowncmd[]      = { "light", "-U", "20", NULL };

static Key keys[] = {
  /* modifier                     key                               function        argument */
  { MODKEY,                       XK_r,                             spawn,          {.v = roficmd } },
  { MODKEY|ShiftMask,             XK_Return,                        spawn,          {.v = termcmd } },
  { MODKEY|ControlMask,           XK_m,                             spawn,          {.v = monitorcmd } },
  { MODKEY,                       XK_e,                             spawn,          {.v = rangercmd } },
  { ControlMask|ShiftMask,        XK_Escape,                        spawn,          {.v = htopcmd } },
  { ALTKEY|ControlMask,           XK_Delete,                        spawn,          {.v = lockcmd } },
  { ALTKEY,                       XK_F4,                            spawn,          {.v = suspendcmd } },
  { ALTKEY|ControlMask,           XK_p,                             spawn,          {.v = mpvcmd } },
  { 0,                            XK_Print,                         spawn,          {.v = printcmd } },
  { 0,                            XF86XK_AudioMute,                 spawn,          {.v = mutecmd0 } },
  { 0,                            XF86XK_AudioMute,                 spawn,          {.v = mutecmd1 } },
  { 0,                            XF86XK_AudioLowerVolume,          spawn,          {.v = voldowncmd } },
  { 0,                            XF86XK_AudioRaiseVolume,          spawn,          {.v = volupcmd } },
  { 0,                            XF86XK_MonBrightnessUp,           spawn,          {.v = brupcmd} },
  { 0,                            XF86XK_MonBrightnessDown,         spawn,          {.v = brdowncmd} },
  { MODKEY,                       XK_b,                             togglebar,      {0} },
  { ALTKEY,                       XK_Tab,                           focusstack,     {.i = +1 } },
  { ALTKEY|ShiftMask,             XK_Tab,                           focusstack,     {.i = -1 } },
  { MODKEY,                       XK_j,                             focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,                             focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,                             incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_u,                             incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,                             setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,                             setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return,                        zoom,           {0} },
  { MODKEY,                       XK_grave,                         view,           {0} },
  { MODKEY,                       XK_w,                             killclient,     {0} },
  { MODKEY,                       XK_t,                             setlayout,      {0} },
  { MODKEY,                       XK_f,                             setlayout,      {0} },
  { MODKEY,                       XK_m,                             setlayout,      {0} },
  { MODKEY,                       XK_space,                         setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,                         togglefloating, {0} },
  { MODKEY,                       XK_0,                             view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,                             tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,                         focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period,                        focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,                         tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period,                        tagmon,         {.i = +1 } },
  { MODKEY,                       XK_s,                             togglesticky,   {0} },
  TAGKEYS(                        XK_q,                                             0)
  TAGKEYS(                        XK_d,                                             1)
  TAGKEYS(                        XK_a,                                             2)
  TAGKEYS(                        XK_1,                                             3)
  TAGKEYS(                        XK_2,                                             4)
  TAGKEYS(                        XK_3,                                             5)
  TAGKEYS(                        XK_4,                                             6)
  { ALTKEY|ShiftMask,             XK_q,                             quit,           {0} },
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

