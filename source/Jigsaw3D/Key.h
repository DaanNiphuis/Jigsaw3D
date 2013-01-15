#ifndef LD_KEY_H
#define LD_KEY_H

#if defined(_WIN32)

struct Key
{
	enum Enum
	{
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,

		One = 30,
		Two = 31,
		Three = 32,
		Four = 33,
		Five = 34,
		Six = 35,
		Seven = 36,
		Eight = 37,
		Nine = 38,
		Zero = 39,

		Return = 40,
		Escape = 41,
		BackSpace = 42,
		Tab = 43,
		Space = 44,

		Minus = 45,
		Equals = 46,
		LeftBracket = 47,
		RightBracket = 48,
		Backslash = 49, /**< Located at the lower left of the return 
						*   key on ISO keyboards and at the right end 
						*   of the QWERTY row on ANSI keyboards. 
						*   Produces REVERSE SOLIDUS (backslash) and 
						*   VERTICAL LINE in a US layout, REVERSE 
						*   SOLIDUS and VERTICAL LINE in a UK Mac 
						*   layout, NUMBER SIGN and TILDE in a UK 
						*   Windows layout, DOLLAR SIGN and POUND SIGN
						*   in a Swiss German layout, NUMBER SIGN and 
						*   APOSTROPHE in a German layout, GRAVE 
						*   ACCENT and POUND SIGN in a French Mac 
						*   layout, and ASTERISK and MICRO SIGN in a 
						*   French Windows layout.
						*/
		NonUSHash = 50, /**< ISO USB keyboards actually use this code 
						*   instead of 49 for the same key, but all 
						*   OSes I've seen treat the two codes 
						*   identically. So, as an implementor, unless
						*   your keyboard generates both of those 
						*   codes and your OS treats them differently,
						*   you should generate BACKSLASH
						*   instead of this code. As a user, you 
						*   should not rely on this code because SDL 
						*   will never generate it with most (all?) 
						*   keyboards. 
						*/
		Semicolon = 51,
		Apostrophe = 52,
		Grave = 53, /**< Located in the top left corner (on both ANSI 
					*   and ISO keyboards). Produces GRAVE ACCENT and 
					*   TILDE in a US Windows layout and in US and UK 
					*   Mac layouts on ANSI keyboards, GRAVE ACCENT 
					*   and NOT SIGN in a UK Windows layout, SECTION 
					*   SIGN and PLUS-MINUS SIGN in US and UK Mac 
					*   layouts on ISO keyboards, SECTION SIGN and 
					*   DEGREE SIGN in a Swiss German layout (Mac: 
					*   only on ISO keyboards), CIRCUMFLEX ACCENT and 
					*   DEGREE SIGN in a German layout (Mac: only on 
					*   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
					*   French Windows layout, COMMERCIAL AT and 
					*   NUMBER SIGN in a French Mac layout on ISO 
					*   keyboards, and LESS-THAN SIGN and GREATER-THAN
					*   SIGN in a Swiss German, German, or French Mac 
					*   layout on ANSI keyboards.
					*/
		Comma = 54,
		Period = 55,
		Slash = 56,

		CAPSLOCK = 57,

		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,

		PrintScreen = 70,
		ScrollLock= 71,
		Pause = 72,
		Insert = 73, /**< insert on PC, help on some Mac keyboards (but
						does send code 73, not 117) */
		Home = 74,
		PageUp = 75,
		Delete = 76,
		End = 77,
		PageDown = 78,
		Right = 79,
		Left = 80,
		Down = 81,
		Up = 82,

		NumlockClear = 83, /**< num lock on PC, clear on Mac keyboards */
		KP_Divide = 84,
		KP_Multiply = 85,
		KP_Minus = 86,
		KP_Plus = 87,
		KP_Enter = 88,
		KP_One = 89,
		KP_Two = 90,
		KP_Three = 91,
		KP_Four = 92,
		KP_Five = 93,
		KP_Six = 94,
		KP_Seven = 95,
		KP_Eight = 96,
		KP_Nine = 97,
		KP_Zero = 98,
		KP_Period = 99,

		NonUSBackslash = 100, /**< This is the additional key that ISO 
								*   keyboards have over ANSI ones, 
								*   located between left shift and Y. 
								*   Produces GRAVE ACCENT and TILDE in a
								*   US or UK Mac layout, REVERSE SOLIDUS
								*   (backslash) and VERTICAL LINE in a 
								*   US or UK Windows layout, and 
								*   LESS-THAN SIGN and GREATER-THAN SIGN
								*   in a Swiss German, German, or French
								*   layout. */
		Application = 101, /**< windows contextual menu, compose */
		Power = 102, /**< The USB document says this is a status flag, 
						*   not a physical key - but some Mac keyboards 
						*   do have a power key. */
		KP_Equals = 103,
		F13 = 104,
		F14 = 105,
		F15 = 106,
		F16 = 107,
		F17 = 108,
		F18 = 109,
		F19 = 110,
		F20 = 111,
		F21 = 112,
		F22 = 113,
		F23 = 114,
		F24 = 115,
		Execute = 116,
		Help = 117,
		Menu = 118,
		Select = 119,
		Stop = 120,
		Again = 121,   /**< redo */
		Undo = 122,
		Cut = 123,
		Copy = 124,
		Paste = 125,
		Find = 126,
		Mute = 127,
		VolumeUp = 128,
		VolumeDown = 129,
		/* not sure whether there's a reason to enable these */
		/*     LOCKINGCAPSLOCK = 130,  */
		/*     LOCKINGNUMLOCK = 131, */
		/*     LOCKINGSCROLLLOCK = 132, */
		KP_Comma = 133,
		KP_EqualsAS400 = 134,

		INTERNATIONAL1 = 135, /**< used on Asian keyboards, see footnotes in USB doc */
		INTERNATIONAL2 = 136,
		INTERNATIONAL3 = 137, /**< Yen */
		INTERNATIONAL4 = 138,
		INTERNATIONAL5 = 139,
		INTERNATIONAL6 = 140,
		INTERNATIONAL7 = 141,
		INTERNATIONAL8 = 142,
		INTERNATIONAL9 = 143,
		LANG1 = 144, /**< Hangul/English toggle */
		LANG2 = 145, /**< Hanja conversion */
		LANG3 = 146, /**< Katakana */
		LANG4 = 147, /**< Hiragana */
		LANG5 = 148, /**< Zenkaku/Hankaku */
		LANG6 = 149, /**< reserved */
		LANG7 = 150, /**< reserved */
		LANG8 = 151, /**< reserved */
		LANG9 = 152, /**< reserved */

		ALTERASE = 153, /**< Erase-Eaze */
		SYSREQ = 154,
		CANCEL = 155,
		CLEAR = 156,
		PRIOR = 157,
		RETURN2 = 158,
		SEPARATOR = 159,
		Out = 160,
		Oper = 161,
		ClearAgain = 162,
		CRSEL = 163,
		EXSEL = 164,

		KP_00 = 176,
		KP_000 = 177,
		THOUSANDSSEPARATOR = 178,
		DECIMALSEPARATOR = 179,
		CURRENCYUNIT = 180,
		CURRENCYSUBUNIT = 181,
		KP_LEFTPAREN = 182,
		KP_RIGHTPAREN = 183,
		KP_LEFTBRACE = 184,
		KP_RIGHTBRACE = 185,
		KP_TAB = 186,
		KP_BACKSPACE = 187,
		KP_A = 188,
		KP_B = 189,
		KP_C = 190,
		KP_D = 191,
		KP_E = 192,
		KP_F = 193,
		KP_XOR = 194,
		KP_POWER = 195,
		KP_PERCENT = 196,
		KP_LESS = 197,
		KP_GREATER = 198,
		KP_AMPERSAND = 199,
		KP_DBLAMPERSAND = 200,
		KP_VERTICALBAR = 201,
		KP_DBLVERTICALBAR = 202,
		KP_COLON = 203,
		KP_HASH = 204,
		KP_SPACE = 205,
		KP_AT = 206,
		KP_EXCLAM = 207,
		KP_MEMSTORE = 208,
		KP_MEMRECALL = 209,
		KP_MEMCLEAR = 210,
		KP_MEMADD = 211,
		KP_MEMSUBTRACT = 212,
		KP_MEMMULTIPLY = 213,
		KP_MEMDIVIDE = 214,
		KP_PLUSMINUS = 215,
		KP_CLEAR = 216,
		KP_CLEARENTRY = 217,
		KP_BINARY = 218,
		KP_OCTAL = 219,
		KP_DECIMAL = 220,
		KP_HEXADECIMAL = 221,

		LCtrl = 224,
		LShift = 225,
		LAlt = 226, /**< alt, option */
		LGui = 227, /**< windows, command (apple), meta */
		RCtrl = 228,
		RShift = 229,
		RAlt = 230, /**< alt gr, option */
		RGui = 231, /**< windows, command (apple), meta */

		Mode = 257,    /**< I'm not sure if this is really not covered 
						*   by any of the above, but since there's a 
						*   special KMOD_MODE for it I'm adding it here
						*/

		/*@}*//*Usage page 0x07*/

		/**
		*  \name Usage page 0x0C
		*  
		*  These values are mapped from usage page 0x0C (USB consumer page).
		*/
		/*@{*/

		AudioNext = 258,
		AudioPrev = 259,
		AudioStop = 260,
		AudioPlay = 261,
		AudioMute = 262,
		MediaSelect = 263,
		WWW = 264,
		Mail = 265,
		Calculator = 266,
		Computer = 267,
		AC_Search = 268,
		AC_Home = 269,
		AC_Back = 270,
		AC_Forward = 271,
		AC_Stop = 272,
		AC_Refresh = 273,
		AC_Bookmarks = 274,

		BrightnessDown = 275,
		BrightnessUp = 276,
		DisplaySwitch = 277, /**< display mirroring/dual display switch, video mode switch */
		KBDILLUMTOGGLE = 278,
		KBDILLUMDOWN = 279,
		KBDILLUMUP = 280,
		Eject = 281,
		Sleep = 282,

		NumKeyboardKeys = 512, /**< not a key, just marks the number of scancodes for array bounds */

		/* Add any other keys here. */
		PointerLeft,
		PointerMiddle,
		PointerRight,

		NumKeysTotal
	};
};

#endif

#endif

