#ifndef INPUT_H
#define INPUT_H

#include "Key.h"
#include "Vector2.h"
#include "Vector3.h"

class Input
{
public:
	static void createInstance();
	static Input* getInstance();
	static void destroyInstance();

	void update();

	inline bool userQuit()				const {return m_userQuit;}
	inline bool isPressed	(Key::Enum key)	const {return m_keyState[key] && !m_previousKeyState[key];}
	inline bool isDown		(Key::Enum key)	const {return m_keyState[key];}
	inline bool isReleased	(Key::Enum key)	const {return !m_keyState[key] && m_previousKeyState[key];}

	inline Vector2	getPointerPosition()	const {return m_pointerPosition;}
	inline Vector2	getPointerVelocity()	const {return m_pointerVelocity;}

	void setPointerPosition(const Vector2& p_position);
	void resetPointerPosition();

	void hideMouseCursor(bool p_hide);

private:
	Input();
	~Input();
	
	static Input* ms_instance;
	
	bool m_appHasFocus;
	bool m_userQuit;

	Vector2 m_pointerPosition;
	Vector2 m_pointerVelocity;
	
	Vector3 m_accelerometerValue;
    
    bool m_previousKeyState[Key::NumKeysTotal];
	bool m_keyState[Key::NumKeysTotal];

#if defined(_WIN32)

#define PREV_VELOCITY_COUNT 5

#if PREV_VELOCITY_COUNT > 0
	// Mouse input is smoothend by averaging the mouse speed over a number of frames.
	Vector2 m_prevVelocities[PREV_VELOCITY_COUNT];
#endif

#endif

};

#endif
