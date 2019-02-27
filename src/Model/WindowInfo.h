#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H

#include <string>
#include <cstdint>

struct WindowInfo
{
	WindowInfo(const std::string& t, int X, int Y, int W, int H, std::uint32_t f) :
		title(t),
		x(X),
		y(Y),
		w(W),
		h(H),
		flags(f)
	{
	}
	
	WindowInfo();
	
	void SetTitle(const std::string&);
	void SetPos(int, int);
	void SetSize(int, int);
	void SetOpenGL(bool);
	void SetVisible(bool);
	void SetResizable(bool);
	
	WindowInfo(const WindowInfo&) = default;
	WindowInfo& operator=(const WindowInfo&) = default;
	
	WindowInfo(WindowInfo&&) = default;
	WindowInfo& operator=(WindowInfo&&) = default;
	
	std::string title;
	int x, y, w, h;
	std::uint32_t flags;
};

#endif //WINDOW_INFO_H