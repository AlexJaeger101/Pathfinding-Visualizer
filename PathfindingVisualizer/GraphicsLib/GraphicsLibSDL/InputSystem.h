#pragma once

#include <Trackable.h>
#include <Vector2D.h>
#include <SDL.h>

class MouseButtonState : public Trackable
{
public:
	enum MouseButton
	{
		LEFT = SDL_BUTTON_LEFT,
		RIGHT = SDL_BUTTON_RIGHT,
		CENTER = SDL_BUTTON_MIDDLE
	};
	enum PressState
	{
		RELEASED,
		PRESSED
	};

	MouseButtonState(Uint32 state);

	PressState getPressedState(MouseButton theButton);

private:
	Uint32 mState = 0;
};

class InputSystem : public Trackable
{
public:
	enum MouseButton
	{
		LEFT = MouseButtonState::LEFT,
		RIGHT = MouseButtonState::RIGHT,
		CENTER = MouseButtonState::CENTER
	};

	enum Key
	{
		A_KEY = SDLK_a,
		B_KEY = SDLK_b,
		C_KEY = SDLK_c,
		D_KEY = SDLK_d,
		E_KEY = SDLK_e,
		F_KEY = SDLK_f,
		G_KEY = SDLK_g,
		H_KEY = SDLK_h,
		I_KEY = SDLK_i,
		J_KEY = SDLK_j,
		K_KEY = SDLK_k,
		L_KEY = SDLK_l,
		M_KEY = SDLK_m,
		N_KEY = SDLK_n,
		O_KEY = SDLK_o,
		P_KEY = SDLK_p,
		Q_KEY = SDLK_q,
		R_KEY = SDLK_r,
		S_KEY = SDLK_s,
		T_KEY = SDLK_t,
		U_KEY = SDLK_u,
		V_KEY = SDLK_v,
		W_KEY = SDLK_w,
		X_KEY = SDLK_x,
		Y_KEY = SDLK_y,
		Z_KEY = SDLK_z,
		ZERO_KEY = SDLK_0,
		ONE_KEY = SDLK_1,
		TWO_KEY = SDLK_2,
		THREE_KEY = SDLK_3,
		FOUR_KEY = SDLK_4,
		FIVE_KEY = SDLK_5,
		SIX_KEY = SDLK_6,
		SEVEN_KEY = SDLK_7,
		EIGHT_KEY = SDLK_8,
		NINE_KEY = SDLK_9,
		BACKSPACE_KEY = SDLK_BACKSPACE,
		TAB_KEY = SDLK_TAB,
		EXCLARE_KEY = SDLK_EXCLAIM,
		DBL_QUOTE_KEY = SDLK_QUOTEDBL,
		HASH_KEY = SDLK_HASH,
		PERCENT_KEY = SDLK_PERCENT,
		DOLLAR_KEY = SDLK_DOLLAR,
		AMPERSAND_KEY = SDLK_AMPERSAND,
		SGL_QUOTE_KEY = SDLK_QUOTE,
		LEFT_PAREN_KEY = SDLK_LEFTPAREN,
		RIGHT_PAREN_KEY = SDLK_RIGHTPAREN,
		ASTERISK_KEY = SDLK_ASTERISK,
		PLUS_KEY = SDLK_PLUS,
		COMMA_KEY = SDLK_COMMA,
		MINUS_KEY = SDLK_MINUS,
		PERIOD_KEY = SDLK_PERIOD,
		SLASH_KEY = SDLK_SLASH,
		COLON_KEY = SDLK_COLON,
		SEMI_COLON_KEY = SDLK_SEMICOLON,
		LESS_KEY = SDLK_LESS,
		EQUALS_KEY = SDLK_EQUALS,
		GREATER_KEY = SDLK_GREATER,
		QUESTION_KEY = SDLK_QUESTION,
		AT_KEY = SDLK_AT,
		LEFT_BRACKET_KEY = SDLK_LEFTBRACKET,
		BCK_SLASH_KEY = SDLK_BACKSLASH,
		RIGHT_BRACKET_KEY = SDLK_RIGHTBRACKET,
		CARET_KEY = SDLK_CARET,
		UNDERSCORE_KEY = SDLK_UNDERSCORE,
		BACKQUOTE_KEY = SDLK_BACKQUOTE,
		ENTER_KEY = SDLK_RETURN,
		RETURN_KEY = SDLK_RETURN2,
		SPACE_KEY = SDLK_SPACE,
		ESCAPE_KEY = SDLK_ESCAPE,
		UP_KEY = SDLK_UP,
		DOWN_KEY = SDLK_DOWN,
		RIGHT_KEY = SDLK_RIGHT,
		LEFT_KEY = SDLK_LEFT
	};

	enum PressState
	{
		PRESSED = MouseButtonState::PRESSED,
		RELEASED = MouseButtonState::RELEASED
	};

	InputSystem();
	~InputSystem();

	bool init();
	void cleanup();

	void update(double dt);

	//interface polling functions
	Vector2D getCurrentMousePos();
	bool isMouseButtonPressed(InputSystem::MouseButton button);
	bool isKeyPressed(InputSystem::Key theKey);

private:

	void processKeyEvent(const SDL_KeyboardEvent& theEvent);
	void processMouseEvent(const SDL_Event& theEvent);
	void processWindowEvent(const SDL_WindowEvent& theEvent);
};

