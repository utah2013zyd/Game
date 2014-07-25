#ifndef __Orz_ControllerQt_QtKeyMap_h__
#define __Orz_ControllerQt_QtKeyMap_h__
#include <QWidget>
#include <orz/Framework_Base/System/IInputManager.h>
 
static Orz::KeyCode QtKC2OrzKC(unsigned int keycode)
{
    switch (keycode) {
     
        case Qt::Key_Backspace:
			return Orz::KC_BACK; // (08) BACKSPACE key
        case Qt::Key_Backtab:
        case Qt::Key_Tab:
			return Orz::KC_TAB; // (09) TAB key
        case Qt::Key_Clear:
			return Orz::KC_UNASSIGNED; // (0C) CLEAR key
        case Qt::Key_Enter:
        case Qt::Key_Return:
			return Orz::KC_RETURN; //(0D) Return key
        case Qt::Key_Shift:
			return Orz::KC_LSHIFT; // (10) SHIFT key
        case Qt::Key_Control:
			return Orz::KC_LCONTROL; // (11) CTRL key
        case Qt::Key_Menu:
        case Qt::Key_Alt:
			return Orz::KC_LMENU; // (12) ALT key

        case Qt::Key_Pause:
			return Orz::KC_PAUSE; // (13) PAUSE key
        case Qt::Key_CapsLock:
			return Orz::KC_CAPITAL; // (14) CAPS LOCK key
        case Qt::Key_Kana_Lock:
        case Qt::Key_Kana_Shift:
			return Orz::KC_UNASSIGNED;//VK_KANA; // (15) Input Method Editor (IME) Kana mode
        case Qt::Key_Hangul:
            return Orz::KC_UNASSIGNED;//VK_HANGUL; // VK_HANGUL (15) IME Hangul mode
            // VK_JUNJA (17) IME Junja mode
            // VK_FINAL (18) IME final mode
        case Qt::Key_Hangul_Hanja:
            return Orz::KC_UNASSIGNED;//VK_HANJA; // (19) IME Hanja mode
        case Qt::Key_Kanji:
            return Orz::KC_UNASSIGNED;//VK_KANJI; // (19) IME Kanji mode
        case Qt::Key_Escape:
			return Orz::KC_ESCAPE;//VK_ESCAPE; // (1B) ESC key
            // VK_CONVERT (1C) IME convert
            // VK_NONCONVERT (1D) IME nonconvert
            // VK_ACCEPT (1E) IME accept
            // VK_MODECHANGE (1F) IME mode change request
        case Qt::Key_Space:
			return Orz::KC_SPACE;//VK_SPACE; // (20) SPACEBAR
        case Qt::Key_PageUp:
			return Orz::KC_PGUP;//VK_PRIOR; // (21) PAGE UP key
        case Qt::Key_PageDown:
			return Orz::KC_PGDOWN;//VK_NEXT; // (22) PAGE DOWN key
        case Qt::Key_End:
			return Orz::KC_END;//VK_END; // (23) END key
        case Qt::Key_Home:
			return Orz::KC_HOME;//VK_HOME; // (24) HOME key
        case Qt::Key_Left:
			return Orz::KC_LEFT;//VK_LEFT; // (25) LEFT ARROW key
        case Qt::Key_Up:
			return Orz::KC_UP;//VK_UP; // (26) UP ARROW key
        case Qt::Key_Right:
			return Orz::KC_RIGHT;//VK_RIGHT; // (27) RIGHT ARROW key
        case Qt::Key_Down:
			return Orz::KC_DOWN;//VK_DOWN; // (28) DOWN ARROW key
        case Qt::Key_Select:
			return Orz::KC_UNASSIGNED;//VK_SELECT; // (29) SELECT key
        case Qt::Key_Print:
			return Orz::KC_UNASSIGNED;//VK_PRINT; // (2A) PRINT key
#if QT_VERSION >= 0x040200
        case Qt::Key_Execute:
			return Orz::KC_UNASSIGNED;//VK_EXECUTE;// (2B) EXECUTE key
#endif
            //dunno on this
            //case Qt::Key_PrintScreen:
            //      return VK_SNAPSHOT; // (2C) PRINT SCREEN key
        case Qt::Key_Insert:
			return Orz::KC_INSERT;//VK_INSERT; // (2D) INS key
        case Qt::Key_Delete:
			return Orz::KC_DELETE;//VK_DELETE; // (2E) DEL key
        case Qt::Key_Help:
			return Orz::KC_F1;//VK_HELP; // (2F) HELP key
        case Qt::Key_0:
        case Qt::Key_ParenLeft:
			return Orz::KC_0;//VK_0;    //  (30) 0) key
        case Qt::Key_1:
			return Orz::KC_1;//VK_1; //  (31) 1 ! key
        case Qt::Key_2:
        case Qt::Key_At:
			return Orz::KC_2;//VK_2; //  (32) 2 & key
        case Qt::Key_3:
        case Qt::Key_NumberSign:
			return Orz::KC_3;//VK_3; //case '3': case '#';
        case Qt::Key_4:
        case Qt::Key_Dollar: //  (34) 4 key '$';
			return Orz::KC_4;//VK_4;
        case Qt::Key_5:
        case Qt::Key_Percent:
			return Orz::KC_5;//VK_5; //  (35) 5 key  '%'
        case Qt::Key_6:
        case Qt::Key_AsciiCircum:
			return Orz::KC_6;//VK_6; //  (36) 6 key  '^'
        case Qt::Key_7:
        case Qt::Key_Ampersand:
			return Orz::KC_7;//VK_7; //  (37) 7 key  case '&'
        case Qt::Key_8:
        case Qt::Key_Asterisk:
			return Orz::KC_8; //  (38) 8 key  '*'
        case Qt::Key_9:
        case Qt::Key_ParenRight:
			return Orz::KC_9; //  (39) 9 key '('
        case Qt::Key_A:
            return Orz::KC_A; //  (41) A key case 'a': case 'A': return 0x41;
        case Qt::Key_B:
            return Orz::KC_B; //  (42) B key case 'b': case 'B': return 0x42;
        case Qt::Key_C:
            return Orz::KC_C; //  (43) C key case 'c': case 'C': return 0x43;
        case Qt::Key_D:
            return Orz::KC_D; //  (44) D key case 'd': case 'D': return 0x44;
        case Qt::Key_E:
            return Orz::KC_E; //  (45) E key case 'e': case 'E': return 0x45;
        case Qt::Key_F:
            return Orz::KC_F; //  (46) F key case 'f': case 'F': return 0x46;
        case Qt::Key_G:
            return Orz::KC_G; //  (47) G key case 'g': case 'G': return 0x47;
        case Qt::Key_H:
            return Orz::KC_H; //  (48) H key case 'h': case 'H': return 0x48;
        case Qt::Key_I:
            return Orz::KC_I; //  (49) I key case 'i': case 'I': return 0x49;
        case Qt::Key_J:
            return Orz::KC_J; //  (4A) J key case 'j': case 'J': return 0x4A;
        case Qt::Key_K:
            return Orz::KC_K; //  (4B) K key case 'k': case 'K': return 0x4B;
        case Qt::Key_L:
            return Orz::KC_L; //  (4C) L key case 'l': case 'L': return 0x4C;
        case Qt::Key_M:
            return Orz::KC_M; //  (4D) M key case 'm': case 'M': return 0x4D;
        case Qt::Key_N:
            return Orz::KC_N; //  (4E) N key case 'n': case 'N': return 0x4E;
        case Qt::Key_O:
            return Orz::KC_O; //  (4F) O key case 'o': case 'O': return 0x4F;
        case Qt::Key_P:
            return Orz::KC_P; //  (50) P key case 'p': case 'P': return 0x50;
        case Qt::Key_Q:
            return Orz::KC_Q; //  (51) Q key case 'q': case 'Q': return 0x51;
        case Qt::Key_R:
            return Orz::KC_R; //  (52) R key case 'r': case 'R': return 0x52;
        case Qt::Key_S:
            return Orz::KC_S; //  (53) S key case 's': case 'S': return 0x53;
        case Qt::Key_T:
            return Orz::KC_T; //  (54) T key case 't': case 'T': return 0x54;
        case Qt::Key_U:
            return Orz::KC_U; //  (55) U key case 'u': case 'U': return 0x55;
        case Qt::Key_V:
            return Orz::KC_V; //  (56) V key case 'v': case 'V': return 0x56;
        case Qt::Key_W:
            return Orz::KC_W; //  (57) W key case 'w': case 'W': return 0x57;
        case Qt::Key_X:
            return Orz::KC_X; //  (58) X key case 'x': case 'X': return 0x58;
        case Qt::Key_Y:
            return Orz::KC_Y; //  (59) Y key case 'y': case 'Y': return 0x59;
        case Qt::Key_Z:
            return Orz::KC_Z; //  (5A) Z key case 'z': case 'Z': return 0x5A;
        case Qt::Key_Meta:
            return Orz::KC_LWIN; // (5B) Left Windows key (Microsoft Natural keyboard)
        //case Qt::Key_Meta_R: FIXME: What to do here?
        //    return VK_RWIN; // (5C) Right Windows key (Natural keyboard)
            // VK_APPS (5D) Applications key (Natural keyboard)
            // VK_SLEEP (5F) Computer Sleep key
            // VK_SEPARATOR (6C) Separator key
            // VK_SUBTRACT (6D) Subtract key
            // VK_DECIMAL (6E) Decimal key
            // VK_DIVIDE (6F) Divide key
            // handled by key code above

        case Qt::Key_NumLock:
			return Orz::KC_NUMLOCK;//VK_NUMLOCK; // (90) NUM LOCK key

        case Qt::Key_ScrollLock:
			return Orz::KC_SCROLL;//VK_SCROLL; // (91) SCROLL LOCK key

            // VK_LSHIFT (A0) Left SHIFT key
            // VK_RSHIFT (A1) Right SHIFT key
            // VK_LCONTROL (A2) Left CONTROL key
            // VK_RCONTROL (A3) Right CONTROL key
            // VK_LMENU (A4) Left MENU key
            // VK_RMENU (A5) Right MENU key
            // VK_BROWSER_BACK (A6) Windows 2000/XP: Browser Back key
            // VK_BROWSER_FORWARD (A7) Windows 2000/XP: Browser Forward key
            // VK_BROWSER_REFRESH (A8) Windows 2000/XP: Browser Refresh key
            // VK_BROWSER_STOP (A9) Windows 2000/XP: Browser Stop key
            // VK_BROWSER_SEARCH (AA) Windows 2000/XP: Browser Search key
            // VK_BROWSER_FAVORITES (AB) Windows 2000/XP: Browser Favorites key
            // VK_BROWSER_HOME (AC) Windows 2000/XP: Browser Start and Home key
            // VK_VOLUME_MUTE (AD) Windows 2000/XP: Volume Mute key
            // VK_VOLUME_DOWN (AE) Windows 2000/XP: Volume Down key
            // VK_VOLUME_UP (AF) Windows 2000/XP: Volume Up key
            // VK_MEDIA_NEXT_TRACK (B0) Windows 2000/XP: Next Track key
            // VK_MEDIA_PREV_TRACK (B1) Windows 2000/XP: Previous Track key
            // VK_MEDIA_STOP (B2) Windows 2000/XP: Stop Media key
            // VK_MEDIA_PLAY_PAUSE (B3) Windows 2000/XP: Play/Pause Media key
            // VK_LAUNCH_MAIL (B4) Windows 2000/XP: Start Mail key
            // VK_LAUNCH_MEDIA_SELECT (B5) Windows 2000/XP: Select Media key
            // VK_LAUNCH_APP1 (B6) Windows 2000/XP: Start Application 1 key
            // VK_LAUNCH_APP2 (B7) Windows 2000/XP: Start Application 2 key

            // VK_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
        case Qt::Key_Semicolon:
        case Qt::Key_Colon:
			return Orz::KC_COLON;//VK_OEM_1; //case ';': case ':': return 0xBA;
            // VK_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
        case Qt::Key_Plus:
        case Qt::Key_Equal:
			return Orz::KC_EQUALS;// VK_OEM_PLUS; //case '=': case '+': return 0xBB;
            // VK_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
        case Qt::Key_Comma:
        case Qt::Key_Less:
			return Orz::KC_COMMA; //case ',': case '<': return 0xBC;
            // VK_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
        case Qt::Key_Minus:
        case Qt::Key_Underscore:
			return Orz::KC_MINUS;//VK_OEM_MINUS; //case '-': case '_': return 0xBD;
            // VK_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
        case Qt::Key_Period:
        case Qt::Key_Greater:
			return Orz::KC_PERIOD;//VK_OEM_PERIOD; //case '.': case '>': return 0xBE;
            // VK_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
        case Qt::Key_Slash:
        case Qt::Key_Question:
			return Orz::KC_SLASH;//VK_OEM_2; //case '/': case '?': return 0xBF;
            // VK_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
        case Qt::Key_AsciiTilde:
        case Qt::Key_QuoteLeft:
			return Orz::KC_GRAVE;//VK_OEM_3; //case '`': case '~': return 0xC0;
            // VK_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
        case Qt::Key_BracketLeft:
        case Qt::Key_BraceLeft:
			return Orz::KC_LBRACKET;//VK_OEM_4; //case '[': case '{': return 0xDB;
            // VK_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
        case Qt::Key_Backslash:
        case Qt::Key_Bar:
			return Orz::KC_BACKSLASH;//VK_OEM_5; //case '\\': case '|': return 0xDC;
            // VK_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
        case Qt::Key_BracketRight:
        case Qt::Key_BraceRight:
			return Orz::KC_RBRACKET;//VK_OEM_6; // case ']': case '}': return 0xDD;
            // VK_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
        case Qt::Key_QuoteDbl:
			return Orz::KC_APOSTROPHE;//VK_OEM_7; // case '\'': case '"': return 0xDE;
            // VK_OEM_8 (DF) Used for miscellaneous characters; it can vary by keyboard.
            // VK_OEM_102 (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
            // VK_PROCESSKEY (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
            // VK_PACKET (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
            // VK_ATTN (F6) Attn key
            // VK_CRSEL (F7) CrSel key
            // VK_EXSEL (F8) ExSel key
            // VK_EREOF (F9) Erase EOF key
            // VK_PLAY (FA) Play key
            // VK_ZOOM (FB) Zoom key
            // VK_NONAME (FC) Reserved for future use
            // VK_PA1 (FD) PA1 key
            // VK_OEM_CLEAR (FE) Clear key

		case Qt::Key_F1:
			return Orz::KC_F1;
		case Qt::Key_F2:
			return Orz::KC_F2;
		case Qt::Key_F3:
			return Orz::KC_F3;
		case Qt::Key_F4:
			return Orz::KC_F4;
		case Qt::Key_F5:
			return Orz::KC_F5;
		case Qt::Key_F6:
			return Orz::KC_F6;
		case Qt::Key_F7:
			return Orz::KC_F7;
		case Qt::Key_F8:
			return Orz::KC_F8;
		case Qt::Key_F9:
			return Orz::KC_F9;
		case Qt::Key_F10:
			return Orz::KC_F10;
		case Qt::Key_F11:
			return Orz::KC_F11;
		case Qt::Key_F12:
			return Orz::KC_F12;
		case Qt::Key_F13:
			return Orz::KC_F13;
		case Qt::Key_F14:
			return Orz::KC_F14;
		case Qt::Key_F15:
			return Orz::KC_F15;

        default:
			return Orz::KC_UNASSIGNED;
    }



}
 
static Orz::MouseButtonID QtMB2OrzMB(Qt::MouseButton qtmb)
{
	switch(qtmb)
	{
	case Qt::LeftButton:
		return Orz::MB_Left;
	case Qt::MidButton:
		return Orz::MB_Middle;
	case Qt::RightButton:
		return Orz::MB_Right;
	case Qt::XButton1:
		return Orz::MB_Button3;
	case Qt::XButton2:
		return Orz::MB_Button4;
	
	}
	return Orz::MB_Left;
}

#endif