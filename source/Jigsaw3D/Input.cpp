#include "Input.h"

#include "Renderer.h"

#include "SDL/SDL.h"

Input* Input::ms_instance = 0;

void Input::createInstance()
{
	if (ms_instance == 0)
	{
		ms_instance = new Input();
	}
}

Input* Input::getInstance()
{
	createInstance();
	return ms_instance;
}

void Input::destroyInstance()
{
	delete ms_instance;
	ms_instance = 0;
}

void Input::update()
{
	memcpy(m_previousKeyState, m_keyState, Key::NumKeyboardKeys);
	m_pointerVelocity.reset();

	// Update mouse position/velocity
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	const Vector2 mousePos = Vector2(float(mouseX), float(mouseY));

	const Vector2 vel = mousePos - m_pointerPosition;
	m_pointerPosition = mousePos;

	m_pointerVelocity = vel;
	for (unsigned int i = 0; i < PREV_VELOCITY_COUNT; ++i)
	{
		m_pointerVelocity += m_prevVelocities[i];
	}
	m_pointerVelocity /= PREV_VELOCITY_COUNT + 1;

#if PREV_VELOCITY_COUNT > 0
	for (unsigned int i = PREV_VELOCITY_COUNT - 1; i > 0; --i)
	{
		m_prevVelocities[i] = m_prevVelocities[i - 1];
	}
	m_prevVelocities[0] = vel;
#endif

	Uint8 *state = SDL_GetKeyboardState(NULL);
	memcpy(m_keyState, state, Key::NumKeyboardKeys);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				m_appHasFocus = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				m_appHasFocus = false;
				break;
			}
		}
		if (m_appHasFocus == false)
			return;

		switch (event.type)
		{
		case SDL_QUIT:
			m_userQuit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT: m_keyState[Key::PointerLeft] = true; break;
			case SDL_BUTTON_RIGHT: m_keyState[Key::PointerRight] = true; break;
			case SDL_BUTTON_MIDDLE: m_keyState[Key::PointerMiddle] = true; break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT: m_keyState[Key::PointerLeft] = false; break;
			case SDL_BUTTON_RIGHT: m_keyState[Key::PointerRight] = false; break;
			case SDL_BUTTON_MIDDLE: m_keyState[Key::PointerMiddle] = false; break;
			}
			break;
		default:
			break;
		}
	}
}

void Input::setPointerPosition(const Vector2& p_position) 
{
	if (m_appHasFocus == false)
	{
		return;
	}

	m_pointerPosition = p_position;
	SDL_WarpMouseInWindow(static_cast<SDL_Window*>(Renderer::getInstance()->getRenderWindow().getHandle()), 
						  static_cast<int>(p_position.x + 0.5f),
						  static_cast<int>(p_position.y + 0.5f));
}

void Input::resetPointerPosition()
{
	Renderer* renderer = Renderer::getInstance();
	Vector2 newPosition(static_cast<float>(renderer->getScreenWidth()) * 0.5f,
						static_cast<float>(renderer->getScreenHeight()) * 0.5f);
	setPointerPosition(newPosition);
}

void Input::hideMouseCursor(bool p_hide)
{
	if (m_appHasFocus == false)
	{
		return;
	}

	if (p_hide)
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
}

// ***** Private functions *****

Input::Input():
	m_appHasFocus(true),
	m_userQuit(false),
	m_pointerPosition(),
	m_pointerVelocity()
{
	memset(m_keyState, 0, Key::NumKeysTotal);
	memset(m_previousKeyState, 0, Key::NumKeysTotal);

	for (unsigned int i = 0; i < PREV_VELOCITY_COUNT; ++i)
	{
		m_prevVelocities[i].reset();
	}

	// Prevent high start velocity.
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	m_pointerPosition = Vector2(float(mouseX), float(mouseY));
}

Input::~Input()
{
}

