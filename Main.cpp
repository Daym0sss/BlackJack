#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Game.h"

#include <Windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <string>
#include <iostream>

using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

#define APP_BUTTON_ID 1
#define RECORDS_BUTTON_ID 2
#define EXIT_BUTTON_ID 3

#define EXIT_GAME_BUTTON_ID 4
#define TAKE_CARD_BUTTON_ID 5
#define NOT_TAKE_CARD_BUTTON_ID 6
#define NEW_GAME_BUTTON_ID 7
#define MAKE_BET_BUTTON_ID 8

#define APP_WINDOW_ID 101
#define RECORDS_WINDOW_ID 102
#define MONEY_EDIT_ID 103
#define BET_EDIT_ID 104

int WWidth = 800;
int WHeight = 600;

int cardWidth = 60;
int cardHeight = 90;

int deckX = 150;
int deckY = 180;

int playerCardsY = 265;
int playerCardsX = 250;

int dealerCardsY = 165;
int dealerCardsX = 250;

HWND game_window_hwnd;
HWND app_button_hwnd;
HWND records_button_hwnd;
HWND exit_button_hwnd;

HWND exit_game_button_hwnd;
HWND take_card_button_hwnd;
HWND not_take_card_button_hwnd;
HWND new_game_button_hwnd;
HWND edit_hwnd;
HWND user_bet_hwnd;
HWND make_bet_btn_hwnd;

bool close = false;
bool gameOver = false;
bool bet_made = false;
bool new_game = false;

HINSTANCE hInst;

Game game;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GameProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool isNumber(string str);

int exitButtonX = WWidth - 150;
int exitButtonY = 400;

int takeCardButtonX = WWidth - 110;
int takeCardButtonY = 40;

int notTakeCardButtonX = WWidth - 110;
int notTakeCardButtonY = 110;

int newGameButtonX = WWidth / 2 - 50;
int newGameButtonY = 400;

int moneyTextX = WWidth / 2;
int moneyTextY = 30;

int betTextX = WWidth / 2 - 180;
int betTextY = 30;

int makeBetBtnX = WWidth / 2 - 165;
int makeBetBtnY = 80;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int ncmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"BlackJack";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);

	hwnd = CreateWindow(wcex.lpszClassName, L"BlackJack", WS_OVERLAPPEDWINDOW, 100, 100, WWidth, WHeight, nullptr, nullptr, wcex.hInstance, nullptr);

	ShowWindow(hwnd, ncmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HBRUSH hBrush;
	static HPEN hPen;

	ULONG_PTR gdiToken;
	GdiplusStartupInput gdiInput;
	GdiplusStartup(&gdiToken, &gdiInput, NULL);

	switch (message)
	{
		case WM_CREATE:
		{
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			hPen = CreatePen(PS_SOLID, 7, RGB(255, 0, 0));
		
			app_button_hwnd = CreateWindow(
				L"BUTTON",
				L"Start game",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				WWidth / 2 - 75,
				150,
				200,
				50,
				hWnd,
				(HMENU)APP_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
				NULL
			);

			records_button_hwnd = CreateWindow(
				L"BUTTON",
				L"The biggest wins table",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				WWidth / 2 - 75,
				250,
				200,
				50,
				hWnd,
				(HMENU)RECORDS_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
				NULL
			);

			exit_button_hwnd = CreateWindow(
				L"BUTTON",
				L"Exit",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				WWidth / 2 - 75,
				350,
				200,
				50,
				hWnd,
				(HMENU)EXIT_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
				NULL
			);

			break;
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			GdiplusShutdown(gdiToken);
			break;
		}

		case WM_SIZE:
		{
			WWidth = LOWORD(lParam);
			WHeight = HIWORD(lParam);

			InvalidateRect(hWnd, NULL, TRUE);

			break;
		}

		case WM_ERASEBKGND:
		{
			return 1;
		}

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			HDC hdcBackBuff = CreateCompatibleDC(hdc);
			HBITMAP bitmap = CreateCompatibleBitmap(hdc, WWidth, WHeight);
			SelectObject(hdcBackBuff, bitmap);

			Graphics graphics(hdcBackBuff);
			Bitmap bmp(L"D://BlackJack/images/mainWindow.png");
			graphics.DrawImage(&bmp, 0, 0, WWidth, WHeight);

			BitBlt(hdc, 0, 0, WWidth, WHeight, hdcBackBuff, 0, 0, SRCCOPY);
			DeleteObject(bitmap);
			DeleteDC(hdcBackBuff);

			EndPaint(hWnd, &ps);

			break;
		}
		case WM_CLOSE:
		{
			if (MessageBox(hWnd, L"Are you sure you want to quit?", L"BlackJack", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
			}
			return 0;
			break;
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == APP_BUTTON_ID)
			{
				WNDCLASS w;
				memset(&w, 0, sizeof(WNDCLASS));
				w.lpfnWndProc = GameProc;
				w.hInstance = hInst;
				w.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
				w.lpszClassName = L"Game";
				w.hCursor = LoadCursor(NULL, IDC_ARROW);
				RegisterClass(&w);
				game_window_hwnd = CreateWindow(w.lpszClassName, L"Game", WS_OVERLAPPEDWINDOW, 150, 150, WWidth, WHeight, hWnd, NULL, w.hInstance, NULL);

				ShowWindow(game_window_hwnd, SW_NORMAL);
				UpdateWindow(game_window_hwnd);
				break;
			}
			else if (LOWORD(wParam) == RECORDS_BUTTON_ID)
			{

			}
			else if (LOWORD(wParam) == EXIT_BUTTON_ID)
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK GameProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HBRUSH hBrush;
	static HPEN hPen;

	ULONG_PTR gdiToken;
	GdiplusStartupInput gdiInput;
	GdiplusStartup(&gdiToken, &gdiInput, NULL);

	switch (message)
	{
		case WM_DESTROY:
		{
			GdiplusShutdown(gdiToken);
			break;
		}

		case WM_SIZE:
		{
			WWidth = LOWORD(lParam);
			WHeight = HIWORD(lParam);

			if (wParam == SIZE_MAXIMIZED)
			{
				deckX = 320;
				deckY = 320;

				playerCardsX = 465;
				playerCardsY = 450;

				dealerCardsX = 465;
				dealerCardsY = 250;

				exitButtonX = WWidth - 250;
				exitButtonY = 650;

				takeCardButtonX = WWidth - 130;
				takeCardButtonY = 40;

				notTakeCardButtonX = WWidth - 130;
				notTakeCardButtonY = 110;

				moneyTextX = WWidth / 2;
				moneyTextY = 30;

				newGameButtonX = WWidth / 2 - 50;
				newGameButtonY = 650;

				betTextX = WWidth / 2 - 180;
				betTextY = 30;

				makeBetBtnX = WWidth / 2 - 155;
				makeBetBtnY = 80;
			}
			else
			{
				deckX = 150;
				deckY = 180;

				playerCardsY = 265;
				playerCardsX = 250;

				dealerCardsY = 165;
				dealerCardsX = 250;

				exitButtonX = WWidth - 150;
				exitButtonY = 400;

				takeCardButtonX = WWidth - 110;
				takeCardButtonY = 40;

				notTakeCardButtonX = WWidth - 110;
				notTakeCardButtonY = 110;

				moneyTextX = WWidth / 2;
				moneyTextY = 30;

				newGameButtonX = WWidth / 2 - 30;
				newGameButtonY = 400;

				betTextX = WWidth / 2 - 180;
				betTextY = 30;

				makeBetBtnX = WWidth / 2 - 155;
				makeBetBtnY = 80;
			}
			InvalidateRect(hwnd, NULL, TRUE);

			break;
		}

		case WM_ERASEBKGND:
		{
			return 1;
		}

		case WM_PAINT:
		{
			DestroyWindow(exit_game_button_hwnd);
			DestroyWindow(take_card_button_hwnd);
			DestroyWindow(not_take_card_button_hwnd);
			DestroyWindow(edit_hwnd);
			DestroyWindow(new_game_button_hwnd);
			DestroyWindow(user_bet_hwnd);
			DestroyWindow(make_bet_btn_hwnd);

			exit_game_button_hwnd = CreateWindow(
				L"BUTTON",
				L"End game",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				exitButtonX,
				exitButtonY,
				100,
				50,
				hwnd,
				(HMENU)EXIT_GAME_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL
			);

			
			take_card_button_hwnd = CreateWindow(
				L"BUTTON",
				L"Take card",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				takeCardButtonX,
				takeCardButtonY,
				100,
				50,
				hwnd,
				(HMENU)TAKE_CARD_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL
			);

			not_take_card_button_hwnd = CreateWindow(
				L"BUTTON",
				L"Not take card",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				notTakeCardButtonX,
				notTakeCardButtonY,
				100,
				50,
				hwnd,
				(HMENU)NOT_TAKE_CARD_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL
			);

			edit_hwnd = CreateWindowEx(
				WS_EX_CLIENTEDGE, 
				L"edit", 
				NULL, 
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 
				moneyTextX, moneyTextY, 
				150, 30, 
				hwnd, 
				(HMENU)MONEY_EDIT_ID,
				NULL, 
				NULL);

			new_game_button_hwnd = CreateWindow(
				L"BUTTON",
				L"New game",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				newGameButtonX,
				newGameButtonY,
				100,
				50,
				hwnd,
				(HMENU)NEW_GAME_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL
			);

			user_bet_hwnd = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				L"edit",
				NULL,
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
				betTextX, betTextY,
				150, 30,
				hwnd,
				(HMENU)BET_EDIT_ID,
				NULL,
				NULL);

			make_bet_btn_hwnd = CreateWindow(
				L"BUTTON",
				L"Make bet",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				makeBetBtnX,
				makeBetBtnY,
				100,
				30,
				hwnd,
				(HMENU)MAKE_BET_BUTTON_ID,
				(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL
			);

			if (!new_game && game.deck.cards.size() == 0)
			{
				SetWindowText(edit_hwnd, L"500");
				
			}
			else
			{
				string playerMoney = "Your money : " + to_string(game.player.playerMoney);
				SetWindowText(edit_hwnd, wstring(playerMoney.begin(), playerMoney.end()).c_str());
			}
			EnableWindow(edit_hwnd, false);

			if (!bet_made)
			{
				EnableWindow(user_bet_hwnd, true);
				EnableWindow(make_bet_btn_hwnd, true);
				EnableWindow(take_card_button_hwnd, false);
				EnableWindow(not_take_card_button_hwnd, false);
			}
			else
			{
				EnableWindow(user_bet_hwnd, false);
				EnableWindow(make_bet_btn_hwnd, false);
				if (game.player.getCardsValues() >= 20)
				{
					EnableWindow(take_card_button_hwnd, false);
				}
				else
				{
					EnableWindow(take_card_button_hwnd, true);
				}
				EnableWindow(not_take_card_button_hwnd, true);
			}

			hdc = BeginPaint(hwnd, &ps);
			HDC hdcBackBuff = CreateCompatibleDC(hdc);
			HBITMAP bitmap = CreateCompatibleBitmap(hdc, WWidth, WHeight);
			SelectObject(hdcBackBuff, bitmap);

			Graphics graphics(hdcBackBuff);
			Bitmap bmp(L"D://BlackJack/images/table.png");
			graphics.DrawImage(&bmp, 0, 0, WWidth, WHeight);

			Bitmap tmp(L"D://BlackJack/images/reversedCard.png");
			graphics.DrawImage(&tmp, deckX, deckY, cardWidth, cardHeight);

			for (int i = 0;i < game.player.playerCards.size();i++)
			{
				string path = "D://BlackJack/images/" + game.player.playerCards.at(i).image + game.player.playerCards.at(i).suit + ".png";
				Bitmap img(wstring(path.begin(), path.end()).c_str());
				graphics.DrawImage(&img, playerCardsX + i * 70, playerCardsY, cardWidth, cardHeight);
			}

			if (!gameOver)
			{
				EnableWindow(new_game_button_hwnd, false);
				for (int i = 0;i < game.dealer.dealerCards.size();i++)
				{
					if (i == 0)
					{
						string path = "D://BlackJack/images/" + game.dealer.dealerCards.at(i).image + game.dealer.dealerCards.at(i).suit + ".png";
						Bitmap img(wstring(path.begin(), path.end()).c_str());
						graphics.DrawImage(&img, dealerCardsX, dealerCardsY, cardWidth, cardHeight);
					}
					else
					{
						Bitmap img(L"D://BlackJack/images/reversedCard.png");
						graphics.DrawImage(&img, dealerCardsX + i * 70, dealerCardsY, cardWidth, cardHeight);
					}
				}
			}
			else
			{
				EnableWindow(not_take_card_button_hwnd, false);
				EnableWindow(take_card_button_hwnd, false);
				if (game.player.playerMoney == 0)
				{
					EnableWindow(new_game_button_hwnd, false);
				}
				else
				{
					EnableWindow(new_game_button_hwnd, true);
				}
				SetWindowText(user_bet_hwnd, L"");
				for (int i = 0;i < game.dealer.dealerCards.size();i++)
				{
					string path = "D://BlackJack/images/" + game.dealer.dealerCards.at(i).image + game.dealer.dealerCards.at(i).suit + ".png";
					Bitmap img(wstring(path.begin(), path.end()).c_str());
					graphics.DrawImage(&img, dealerCardsX + i * 70, dealerCardsY, cardWidth, cardHeight);
				}
			}

			BitBlt(hdc, 0, 0, WWidth, WHeight, hdcBackBuff, 0, 0, SRCCOPY);
			DeleteObject(bitmap);
			DeleteDC(hdcBackBuff);

			EndPaint(hwnd, &ps);

			break;
		}

		case WM_CREATE:
		{
			gameOver = false;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == EXIT_GAME_BUTTON_ID)
			{
				close = true;
			}
			else if (LOWORD(wParam) == TAKE_CARD_BUTTON_ID)
			{
				game.PlayerMove('y');
				game.DealerMove();
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == NOT_TAKE_CARD_BUTTON_ID)
			{
				string winner = game.endGame();
				gameOver = true;
				if (winner == "player")
				{
					MessageBox(hwnd, L"You won, you can look at the cards and take your money back", L"WIN!", MB_OK | MB_ICONEXCLAMATION);

				}
				else if (winner == "dealer")
				{
					MessageBox(hwnd, L"You lose, you can look at the cards, good luck next time =)", L"Lose...", MB_OK | MB_ICONERROR);
				}
				else
				{
					MessageBox(hwnd, L"Game has ended in a draw, get your bet back and look at the cards", L"Draw", MB_OK | MB_ICONWARNING);
				}
				
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == NEW_GAME_BUTTON_ID)
			{
				gameOver = false;
				bet_made = false;
				new_game = true;
				game.player.playerCards.clear();
				game.dealer.dealerCards.clear();

				InvalidateRect(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == MAKE_BET_BUTTON_ID)
			{
				TCHAR tmp[15];
				GetWindowText(user_bet_hwnd, tmp, GetWindowTextLength(user_bet_hwnd) + 1);
				wstring wstr (tmp);
				string num (wstr.begin(), wstr.end());

				if (num.length() > 0 && isNumber(num))
				{
					if (stoi(num) >= 10)
					{
						if (new_game)
						{
							if (stoi(num) <= game.player.playerMoney)
							{
								EnableWindow(user_bet_hwnd, false);
								EnableWindow(make_bet_btn_hwnd, false);
								bet_made = true;

								game.newGame();
								new_game = false;
								game.player.MakeBet(stoi(num));
							}
							else
							{
								MessageBox(hwnd, L"Bet can not be more than you have", L"Error", MB_OK);
							}
						}
						else
						{
							if (stoi(num) <= 500)
							{
								EnableWindow(user_bet_hwnd, false);
								EnableWindow(make_bet_btn_hwnd, false);
								bet_made = true;

								game.startGame();
								game.player.MakeBet(stoi(num));
							}
							else
							{
								MessageBox(hwnd, L"Bet can not be more than you have", L"Error", MB_OK);
							}
							
						}
					}
					else
					{
						MessageBox(hwnd, L"Bet can not be less than 10", L"Error", MB_OK);
					}
					
				}
				else
				{
					MessageBox(hwnd, L"Check what you entered in bet field", L"Error", MB_OK);
				}
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}

		case WM_CLOSE:
		{
			if (close)
			{
				if (MessageBox(hwnd, L"Are you sure you want to quit?", L"New Game", MB_OKCANCEL) == IDOK)
				{
					close = false;
					
					DestroyWindow(hwnd);
				}
				return 0;
			}
			break;
		}

		default: 
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

bool isNumber(string str)
{
	for (char ch : str)
	{
		if (isdigit(ch) == 0)
		{
			return false;
		}
	}
	return true;
}