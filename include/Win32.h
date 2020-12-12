#pragma once

#include <windows.h>
#include "OpenGL.h"

#define PROC_DEFAULT DefWindowProc(window, msg, wParam, lParam);

static int32 w = 1280;
static int32 h = 720;
static glm::vec3 color;

LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
  static PAINTSTRUCT paint;

  switch(msg) {
    case WM_PAINT: {
      opengl_render(w, h, color);
      BeginPaint(window, &paint);
      EndPaint(window, &paint);
      return PROC_DEFAULT;
    }
    case WM_SIZE: {
      w = LOWORD(lParam);
      h = HIWORD(lParam);
      return PROC_DEFAULT;  
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      return PROC_DEFAULT;
    }
    case WM_CLOSE: {
      DestroyWindow(window);   
      return PROC_DEFAULT;
    }
  }
  return PROC_DEFAULT;
}

HWND create_and_show_window(HINSTANCE hInstance, int nCmdShow) {
  const char* className = "CustomFloatingWindow";

  WNDCLASS windowClass = {};
  windowClass.lpfnWndProc   = window_proc;
  windowClass.hInstance     = hInstance;
  windowClass.lpszClassName = className;

  RegisterClass(&windowClass);

  HWND window = CreateWindowEx(
      0,
      className,
      "AdventOfCode2020",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, w, h, 
      0,
      0,
      hInstance,
      0
      );
  if(!window) return nullptr;

  HDC hdc = GetDC(window);

  PIXELFORMATDESCRIPTOR pfd;
  memset(&pfd, 0, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;

  int pf = ChoosePixelFormat(hdc, &pfd);
  if(!pf) return nullptr;
  if(SetPixelFormat(hdc, pf, &pfd) == FALSE) return nullptr;
  DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  
  HGLRC hrc = wglCreateContext(hdc);
  wglMakeCurrent(hdc, hrc);

  ShowWindow(window, nCmdShow);
  return window;
}

typedef BOOL fptr_wglSwapIntervalEXT(int interval);

int start_application(HINSTANCE hInstance, int nCmdShow, void (*fptr_AOCLogic)(), const glm::vec3& bgCol) {
  HWND window = create_and_show_window(hInstance, nCmdShow);
  MSG message;

  color = bgCol;

  fptr_wglSwapIntervalEXT* wglSwapInterval = (fptr_wglSwapIntervalEXT*)wglGetProcAddress("wglSwapIntervalEXT");
  if(!wglSwapInterval) return 1; //todo: add an assert

  opengl_init();
  fptr_AOCLogic();
  
  while(true) {
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
      if(message.message == WM_QUIT) goto quit;
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    PostMessage(window, WM_PAINT, 0, 0);
    wglSwapInterval(1);
  }
quit:
  opengl_end();
  return 0;
}
