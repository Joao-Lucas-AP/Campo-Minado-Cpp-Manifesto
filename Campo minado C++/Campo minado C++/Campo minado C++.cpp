#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <time.h>
#include <filesystem>

using namespace sf;
using namespace std;

enum class GameState 
{
	MainMenu,
	Playing,
	GameOver
};

int main()
{
	srand(time(0));

	RenderWindow app(VideoMode(400, 400), "Start the minesweeper");

	int w = 32;
	int grid[12][12];
	int sgrid[12][12];

	Texture t;
	t.loadFromFile("D:/CampoMinadoCPP/Campo minado C++/Campo minado C++/Images/tiles.jpg");
	Sprite s(t);
	Font font;
	font.loadFromFile("D:/CampoMinadoCPP/Campo minado C++/Campo minado C++/Images/PressStart2P-Regular.ttf");
	
	//Elementos do menu
	GameState gameState = GameState::MainMenu;

	RectangleShape playButton(Vector2f(200, 80));
	playButton.setFillColor(Color(100, 100, 100));
	playButton.setPosition(app.getSize().x / 2 - playButton.getSize().x / 2, app.getSize().y / 2 - playButton.getSize().y / 2);

	Text playButtonText("Play", font, 32);
	playButtonText.setPosition(playButton.getPosition().x + playButton.getSize().x / 2 - playButtonText.getLocalBounds().width / 2,
	playButton.getPosition().y + playButton.getSize().y / 2 - playButtonText.getLocalBounds().height / 2);

	while (app.isOpen())
	{
		Vector2i pos = Mouse::getPosition(app);
		int x = pos.x / w;
		int y = pos.y / w;

		Event e;
		while (app.pollEvent(e)) 
		{
			if (e.type == Event::Closed) 
			{
				app.close();
			}
			if (gameState == GameState::MainMenu)
			{
				if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left)
				{
					if (playButton.getGlobalBounds().contains(pos.x, pos.y))
					{
						// Start game
						gameState = GameState::Playing;

						// Initialize game grid and sgrid
						for (int i = 1; i <= 10; i++)
						{
							for (int j = 1; j <= 10; j++)
							{
								sgrid[i][j] = 10;

								if (rand() % 5 == 0)
								{
									grid[i][j] = 9;
								}
								else
								{
									grid[i][j] = 0;
								}
							}
						}

						for (int i = 1; i <= 10; i++) 
						{
							for (int j = 1; j <= 10; j++) 
							{
								int n = 0;

								if (grid[i][j] == 9) 
								{
									continue;
								}

								for (int dx = -1; dx <= 1; dx++) 
								{
									for (int dy = -1; dy <= 1; dy++) 
									{
										if (grid[i + dx][j + dy] == 9) 
										{
											n++;
										}
									}
								}

								grid[i][j] = n;
							}
						}

						// Calculate numbers
						for (int i = 1; i <= 10; i++)
						{
							for (int j = 1; j <= 10; j++)
							{
								int n = 0;

								if (grid[i][j] == 9)
								{
									continue;
								}

								if (grid[i + 1][j] == 9)
								{
									n++;
								}

								if (grid[i][j + 1] == 9)
								{
									n++;
								}

								if (grid[i - 1][j] == 9)
								{
									n++;
								}

								if (grid[i][j - 1] == 9)
								{
									n++;
								}

								if (grid[i + 1][j + 1] == 9)
								{
									n++;
								}

								if (grid[i - 1][j - 1] == 9)
								{
									n++;
								}

								if (grid[i - 1][j + 1] == 9)
								{
									n++;
								}

								if (grid[i + 1][j - 1] == 9)
								{
									n++;
								}

								grid[i][j] = n;
							}
						}
					}
				}				
			}
			else if (gameState == GameState::Playing)
			{
				if (e.type == Event::MouseButtonPressed)
				{
					if (e.key.code == Mouse::Left)
					{
						if (grid[x][y] == 9)
						{
							// Game over logic
							gameState = GameState::GameOver;
						}
						else if (grid[x][y] == 0 && sgrid[x][y] == 10)
						{
							// Reveal empty cells and neighboring cells
							sgrid[x][y] = grid[x][y];

							for (int i = x - 1; i <= x + 1; i++)
							{
								for (int j = y - 1; j <= y + 1; j++)
								{
									if (grid[i][j] != 9 && sgrid[i][j] == 10)
									{
										sgrid[i][j] = grid[i][j];
									}
								}
							}
						}
						else if (sgrid[x][y] == 10)
						{
							// Reveal the cell
							sgrid[x][y] = grid[x][y];
						}
					}
					else if (e.key.code == Mouse::Right)
					{
						if (sgrid[x][y] == 10)
						{
							// Mark the cell as flagged
							sgrid[x][y] = 11;
						}
						else if (sgrid[x][y] == 11)
						{
							// Remove the flag from the cell
							sgrid[x][y] = 10;
						}
					}
				}
			}
		}

		app.clear(Color::White);
		for (int i = 1; i <= 10; i++)
		{
			for (int j = 1; j <= 10; j++)
			{
				if (sgrid[i][j] == 11)
				{
					// Draw the flag sprite
					s.setTextureRect(IntRect(11 * w, 0, w, w));
				}
				else
				{
					// Draw the cell sprite based on its value
					s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
				}

				s.setPosition(i * w, j * w);
				app.draw(s);
			}
		}
		if (gameState == GameState::MainMenu)
		{
			RectangleShape backgroundRect(Vector2f(app.getSize().x, app.getSize().y));
			backgroundRect.setFillColor(Color::Black);
			app.draw(backgroundRect);

			Text titleText("Minesweeper", font, 32);
			titleText.setPosition(app.getSize().x / 2 - titleText.getLocalBounds().width / 2, 100);
			app.draw(titleText);

			app.draw(playButton);
			app.draw(playButtonText);
		}
		else if (gameState == GameState::Playing)
		{
			// Draw the game grid
			for (int i = 1; i <= 10; i++)
			{
				for (int j = 1; j <= 10; j++)
				{
					s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
					s.setPosition(i * w, j * w);
					app.draw(s);
				}
			}
		}
		else if (gameState == GameState::GameOver)
		{
			// Draw the game over screen
			RectangleShape gameOverBackground(Vector2f(400, 400));
			gameOverBackground.setFillColor(Color(220, 220, 220));

			app.draw(gameOverBackground);
		}

		app.display();
	}
}