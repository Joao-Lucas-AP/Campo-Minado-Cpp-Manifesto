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

	//Carregar textura
	Texture t;
	t.loadFromFile("D:/CampoMinadoCPP/Campo minado C++/Campo minado C++/Images/tiles.jpg");
	
	Sprite s(t);
	
	//Carregar fonte
	Font font;
	font.loadFromFile("D:/CampoMinadoCPP/Campo minado C++/Campo minado C++/Images/PressStart2P-Regular.ttf");
	
	bool playerWon = false;

	int cellsRevealed = 0;

	//Menu principal iniciado
	GameState gameState = GameState::MainMenu;

	//Elementos do menu
	RectangleShape playButton(Vector2f(200, 80));
	playButton.setFillColor(Color(100, 100, 100));
	playButton.setPosition(app.getSize().x / 2 - playButton.getSize().x / 2, app.getSize().y / 2 - playButton.getSize().y / 2);

	Text playButtonText("Play", font, 32);
	playButtonText.setPosition(playButton.getPosition().x + playButton.getSize().x / 2 - playButtonText.getLocalBounds().width / 2,
	playButton.getPosition().y + playButton.getSize().y / 2 - playButtonText.getLocalBounds().height / 2);

	//Elementos da tela de game over
	RectangleShape playAgainButton(Vector2f(200, 80));
	playAgainButton.setFillColor(Color(100, 100, 100));
	playAgainButton.setPosition(app.getSize().x / 2 - playAgainButton.getSize().x / 2, app.getSize().y / 2 - playAgainButton.getSize().y / 2);

	Text playAgainButtonText("Play", font, 32);
	playAgainButtonText.setPosition(playAgainButton.getPosition().x + playAgainButton.getSize().x / 2 - playAgainButtonText.getLocalBounds().width / 2,
	playAgainButton.getPosition().y + playAgainButton.getSize().y / 2 - playAgainButtonText.getLocalBounds().height / 2);


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
						//Iniciar o game
						gameState = GameState::Playing;

						//Inicializar grid e sgrid
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
							//Game over
							gameState = GameState::GameOver;
						}
						else if (grid[x][y] == 0 && sgrid[x][y] == 10)
						{
							//Revela células vazias e vizinhas
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
						bool allCellsRevealed = true;

						for (int i = 1; i <= 10; i++)
						{
							for (int j = 1; j <= 10; j++)
							{
								if (grid[i][j] != 9 && sgrid[i][j] == 10)
								{
									allCellsRevealed = false;
									break;
								}
							}

							if (!allCellsRevealed)
							{
								break;
							}
						}

						if (allCellsRevealed)
						{
							//Vitória!
							playerWon = true;
							gameState = GameState::GameOver;
						}


						else if (sgrid[x][y] == 10)
						{
							//Revela a célula
							sgrid[x][y] = grid[x][y];
						}
					}
					else if (e.key.code == Mouse::Right)
					{
						if (sgrid[x][y] == 10)
						{
							//Marca uma bandeira
							sgrid[x][y] = 11;
						}
						else if (sgrid[x][y] == 11)
						{
							//Remove a bandeira
							sgrid[x][y] = 10;
						}
					}
				}
			}

			else if (gameState == GameState::GameOver)
			{
				if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left)
				{
					if (playAgainButton.getGlobalBounds().contains(pos.x, pos.y))
					{
						//Iniciar o game
						gameState = GameState::Playing;

						//Inicializar grid e sgrid
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
		}

		//Desenho do game
		app.clear(Color::White);

		for (int i = 1; i <= 10; i++)
		{
			for (int j = 1; j <= 10; j++)
			{
				if (sgrid[i][j] == 11)
				{
					//Desenha o sprite da bandeira
					s.setTextureRect(IntRect(11 * w, 0, w, w));
				}
				else
				{
					//Desenha o sprite da célula baseada no valor
					s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
				}

				s.setPosition(i * w, j * w);
				app.draw(s);
			}
		}
		//Desenho do menu principal
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
			//Desenho do grid de gameplay
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
			//Desenho do game over caso o jogador vença
			if (playerWon)
			{
				RectangleShape gObackgroundRect(Vector2f(app.getSize().x, app.getSize().y));
				gObackgroundRect.setFillColor(Color::Green);
				app.draw(gObackgroundRect);

				RectangleShape playAgainButton(Vector2f(320, 80));
				playAgainButton.setFillColor(Color::Black);
				playAgainButton.setPosition(app.getSize().x / 2 - playAgainButton.getSize().x / 2, app.getSize().y / 2 - playAgainButton.getSize().y / 2);

				Text gameOverTitleText("Victory", font, 32);
				gameOverTitleText.setPosition(app.getSize().x / 2 - gameOverTitleText.getLocalBounds().width / 2, 100);

				app.draw(gameOverTitleText);

				Text playAgainButtonText("Play again", font, 30);
				playAgainButtonText.setPosition(playAgainButton.getPosition().x + playAgainButton.getSize().x / 2 - playAgainButtonText.getLocalBounds().width / 2,
					playAgainButton.getPosition().y + playAgainButton.getSize().y / 2 - playAgainButtonText.getLocalBounds().height / 2);

				app.draw(playAgainButton);
				app.draw(playAgainButtonText);
			}
			//Desenho do game over caso o jogador perca
			else 
			{
				RectangleShape gObackgroundRect(Vector2f(app.getSize().x, app.getSize().y));
			    gObackgroundRect.setFillColor(Color::Red);
			    app.draw(gObackgroundRect);

			    RectangleShape playAgainButton(Vector2f(320, 80));
			    playAgainButton.setFillColor(Color::Black);
			    playAgainButton.setPosition(app.getSize().x / 2 - playAgainButton.getSize().x / 2, app.getSize().y / 2 - playAgainButton.getSize().y / 2);

				Text gameOverTitleText("Game over", font, 32);
				gameOverTitleText.setPosition(app.getSize().x / 2 - gameOverTitleText.getLocalBounds().width / 2, 100);

				app.draw(gameOverTitleText);

				Text playAgainButtonText("Play again", font, 30);
				playAgainButtonText.setPosition(playAgainButton.getPosition().x + playAgainButton.getSize().x / 2 - playAgainButtonText.getLocalBounds().width / 2,
				playAgainButton.getPosition().y + playAgainButton.getSize().y / 2 - playAgainButtonText.getLocalBounds().height / 2);

				app.draw(playAgainButton);
				app.draw(playAgainButtonText);
			}		
		}
		app.display();
	}
}