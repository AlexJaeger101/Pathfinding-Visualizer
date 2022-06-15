#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <vector>

#include <sstream>

#include "Game.h"
#include <System.h>
#include <GraphicsSystem.h>
#include <GraphicsBuffer.h>
#include <Font.h>
#include <FontManager.h>
#include <GraphicsBufferManager.h>
#include <InputSystem.h>
#include "InputTranslator.h"
#include "GameMessageManager.h"
#include <Sprite.h>
#include "SpriteManager.h"
#include <Timer.h>
#include <DataRepository.h>
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "DataLoader.h"
#include "ExtraColors.h"
#include "GameEvent.h"
#include <EventSystem.h>

#include "Location.h"
#include "LocationManager.h"
#include "Recipe.h"
#include "Grid.h"
#include "GridVisualizer.h"
#include "GridGraph.h"
#include "GridPathfinder.h"
#include "DepthFirstPathfinder.h"
#include "BFPathfinding.h"
#include "DijkstrasPathfinding.h"
#include "Connection.h"
#include "AStarPathfinding.h"


Game* gpGame = NULL;
const int GRID_SIZE_X = 32;
const int GRID_SIZE_Y = 32;
const std::string GRID_FILENAME("pathgrid.txt");

Game::Game()
	//:Trackable("game class")
	:mpSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpFontManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpRepository(NULL)
	,mpDataLoader(NULL)
	,mTimeLastFrame(0.0f)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	mpRepository = new DataRepository;
	mpDataLoader = new DataLoader("data.txt", mpRepository);

	//create and init GraphicsSystem
	mpSystem = new System();
	bool goodGraphics = mpSystem->init( mpRepository->getEntry(DataKeyEnum::SCREEN_WIDTH).getUIntVal(), mpRepository->getEntry(DataKeyEnum::SCREEN_HEIGHT).getUIntVal());
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpSystem->getGraphicsSystem());
	mpSpriteManager = new SpriteManager;
	mpFontManager = new FontManager;

	mpMessageManager = new GameMessageManager;

	UINT maxUnits = mpRepository->getEntry(DataKeyEnum::MAX_UNITS).getUIntVal();
	mpComponentManager = new ComponentManager(maxUnits);
	mpUnitManager = new UnitManager(maxUnits);
	mpLocationManager = new LocationManager;
	mpInputTranslator = new InputTranslator;

	//create and load the Grid and GridVisualizer
	mpGrid = new Grid(GraphicsSystem::getDisplayWidth(), GraphicsSystem::getDisplayHeight(), GRID_SIZE_X, GRID_SIZE_Y);
	mpGridVisualizer = new GridVisualizer(mpGrid);
	std::ifstream theStream(GRID_FILENAME);
	if (theStream.good())//file found
	{
		mpGrid->load(theStream);
	}
	else//file missing
	{
		int numGridCells = mpGrid->getGridWidth() * mpGrid->getGridHeight();
		for (int i = 0; i < numGridCells; i++)
		{
			mpGrid->setValueAtIndex(i, CLEAR_VALUE);
		}
	}
	theStream.close();

	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	mpPathfinder = new DepthFirstPathfinder(mpGridGraph);

	//load buffers
	loadBuffers();

	//load Fonts
	loadFonts();

	//setup sprites
	createSprites();

	mTargetFPS = mpRepository->getEntry(DataKeyEnum::TARGET_FPS).getUIntVal();
	mTargetElapsedTime = 1000.0f / mTargetFPS;

	EventSystem* pEventSystem = EventSystem::getInstance();
	assert(pEventSystem);

	pEventSystem->addListener((Event::EventType)MOVE_TO, this);
	pEventSystem->addListener((Event::EventType)CREATE_ENEMY, this);
	pEventSystem->addListener((Event::EventType)EXIT_GAME, this);
	pEventSystem->addListener((Event::EventType)CHANGE_GAME_MODE, this);
	pEventSystem->addListener((Event::EventType)SCORE_POINTS, this);
	pEventSystem->addListener((Event::EventType)EDIT_MOD_TERRAIN, this);
	pEventSystem->addListener((Event::EventType)LOAD_SAVE_MAP, this);
	pEventSystem->addListener((Event::EventType)PATH_TO, this);
	pEventSystem->addListener((Event::EventType)CHANGE_PATHFINDING, this);

	restartGame();
	return true;
}

void Game::cleanup()
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	assert(pEventSystem);

	pEventSystem->removeListenerFromAllEvents(this);

	delete mpPathfinder;
	mpPathfinder = nullptr;
	delete mpGridGraph;
	mpGridGraph = nullptr;
	delete mpGridVisualizer;
	mpGridVisualizer = nullptr;
	delete mpGrid;
	mpGrid = nullptr;

	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFontManager;
	mpFontManager = NULL;

	delete mpDataLoader;
	mpDataLoader = NULL;

	delete mpInputTranslator;
	mpInputTranslator = NULL;

	delete mpLocationManager;
	mpLocationManager = NULL;

	delete mpRepository;
	mpRepository = NULL;

	//delete the graphics system
	delete mpSystem;
	mpSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
}

void Game::doLoop()
{
	//game loop
	while (!mShouldExit)
	{
	
		gpPerformanceTracker->clearTracker("loop");
		gpPerformanceTracker->startTracking("loop");

		gpGame->beginLoop();

		gpPerformanceTracker->clearTracker("process");
		gpPerformanceTracker->startTracking("process");

		gpGame->processLoop();

		gpPerformanceTracker->stopTracking("process");
 
		gpGame->endLoop();

		gpPerformanceTracker->stopTracking("loop");
		//std::cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms    ";
		//std::cout << "processing took:" << gpPerformanceTracker->getElapsedTime("process") << "ms\n";
		mTimeLastFrame = (float)gpPerformanceTracker->getElapsedTime("loop");
	}
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

void Game::processLoop()
{
	InputSystem* pInputSystem = mpSystem->getInputSystem();

	float dt = (mTargetElapsedTime * mTimeMult) / 1000.0f;
	update(dt);
	
	render();

	pInputSystem->update(dt);

	Vector2D pos = pInputSystem->getCurrentMousePos();

	mpMessageManager->processMessagesForThisframe();

	//do handling of debug keypresses here only - bypasses event system
	if (pInputSystem->isKeyPressed(InputSystem::UP_KEY))
	{
		Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
	}
	else if (pInputSystem->isKeyPressed(InputSystem::DOWN_KEY))
	{
		mpUnitManager->deleteRandomUnit();
	}
	if (pInputSystem->isKeyPressed(InputSystem::LEFT_KEY))
	{
		mTimeMult -= 0.1f;
		if (mTimeMult < 0.0f)
			mTimeMult = 0.0f;
	}
	else if (pInputSystem->isKeyPressed(InputSystem::RIGHT_KEY))
	{
		mTimeMult += 0.1f;
	}
	else if (pInputSystem->isKeyPressed(InputSystem::T_KEY))
	{
		mTimeMult = 1.0f;
	}
	if (pInputSystem->isKeyPressed(InputSystem::D_KEY))
	{
		mDrawDebugData = true;
	}
	else
	{
		mDrawDebugData = false;
	}
}

void Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( mTargetElapsedTime );
}

void Game::render() const
{
	//draw background
	GraphicsSystem::renderFilledRect(*GraphicsSystem::getBackBuffer(), ZERO_VECTOR2D, GraphicsSystem::getDisplayWidth(), GraphicsSystem::getDisplayHeight(), BACKGROUND_BLUE_COLOR);

	//Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	//GraphicsSystem::draw(Vector2D(0, 0), *pBackgroundSprite);


	if (mCurrentMode == PLAY_GAME_MODE)
	{
		mpGridVisualizer->draw(*GraphicsSystem::getBackBuffer());
#ifdef VISUALIZE_PATH
		mpPathfinder->drawVisualization(mpGrid, GraphicsSystem::getBackBuffer());
#endif
		mpUnitManager->drawAll();

		if (mDrawDebugData)
		{
			drawDebugData();
		}
		displayElapsedTime();
	}
	if (mCurrentMode == EDIT_GAME_MODE)
	{
		mpGridVisualizer->draw(*GraphicsSystem::getBackBuffer());

		if (mDrawDebugData)
		{
			drawDebugData();
		}
		displayElapsedTime();
	}
	else if (mCurrentMode == START_GAME_MODE)
	{
		displayStartGame();
	}
	else if (mCurrentMode == PAUSE_GAME_MODE)
	{
		//mpLocationManager->drawAll();
		mpUnitManager->drawAll();

		displayElapsedTime();

		displayPauseGame();
	}
	else if (mCurrentMode == END_GAME_MODE)
	{
		//mpLocationManager->drawAll();
		mpUnitManager->drawAll();

		displayElapsedTime();

		displayEndGame();
	}


	mpSystem->getGraphicsSystem()->flip();

}

void Game::update(double dt)
{
	if (mCurrentMode == PLAY_GAME_MODE)
	{
		mTotalGameTimeElapsed += dt;

		if (mTotalGameTimeElapsed >= mGameTimeAllotment)
		{
			setNewGameMode(END_GAME_MODE);
		}
		else
		{
			mpUnitManager->updateAll(dt);
			mpComponentManager->update(dt);
			//mpLocationManager->updateAll(dt);
		}
	}

	mpSystem->getInputSystem()->update(dt);

}

void Game::loadFonts()
{
	std::string path = mpRepository->getEntry(DataKeyEnum::ASSET_PATH).getStringVal();
	Font* pFont = mpFontManager->createAndManageFont(COUR_6_FONT_ID, path + "cour.ttf", 6);
	assert(pFont);
	pFont = mpFontManager->createAndManageFont(COUR_8_FONT_ID, path + "cour.ttf", 8);
	assert(pFont);
	pFont = mpFontManager->createAndManageFont(COUR_10_FONT_ID, path + "cour.ttf", 10);
	assert(pFont);
	pFont = mpFontManager->createAndManageFont(COUR_12_FONT_ID, path + "cour.ttf", 12);
	assert(pFont);
	pFont = mpFontManager->createAndManageFont(COUR_14_FONT_ID, path + "cour.ttf", 14);
	assert(pFont);
	pFont = mpFontManager->createAndManageFont(COUR_24_FONT_ID, path + "cour.ttf", 24);
	assert(pFont);
	pFont = mpFontManager->createAndManageFont(COUR_48_FONT_ID, path + "cour.ttf", 48);
	assert(pFont);

}

void Game::loadBuffers()
{
	std::string path = mpRepository->getEntry(DataKeyEnum::ASSET_PATH).getStringVal();
	GraphicsBuffer* pBuff;
	pBuff = mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, path + "wallpaper.bmp");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID, path + "arrow-right.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, path + "enemy-arrow-right.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mTargetBufferID, path + "target.png");
	assert(pBuff);
}

void Game::createSprites()
{
	GraphicsBuffer* pBuffer = mpGraphicsBufferManager->getBuffer(mBackgroundBufferID);
	if (pBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(BACKGROUND_SPRITE_ID, pBuffer, 0, 0, (float)pBuffer->getWidth(), (float)pBuffer->getHeight());
	}
	
	pBuffer = mpGraphicsBufferManager->getBuffer(mPlayerIconBufferID);
	if (pBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(PLAYER_ICON_SPRITE_ID, pBuffer, 0, 0, (float)pBuffer->getWidth(), (float)pBuffer->getHeight());
	}

	pBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	if (pBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pBuffer, 0, 0, (float)pBuffer->getWidth(), (float)pBuffer->getHeight());
	}

	pBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pBuffer, 0, 0, (float)pBuffer->getWidth(), (float)pBuffer->getHeight());
	}

}

void Game::drawDebugData() const
{
	InputSystem* pInputSystem = mpSystem->getInputSystem();

	Vector2D pos = pInputSystem->getCurrentMousePos();

	//create mouse text
	std::stringstream textStream;
	textStream << pos;

	//get the font
	Font* pFont = mpFontManager->getFont(COUR_24_FONT_ID);

	//write text at mouse position
	//GraphicsSystem::writeText(pos, *pFont, BLACK_COLOR, textStream.str(), Font::RIGHT);

	textStream.str("");
	textStream.clear();

	//write out current number of Units
	Uint32 numUnits = mpUnitManager->getNumUnits();
	textStream << "Units:" << numUnits;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth(), 0), *pFont, BLACK_COLOR, textStream.str(), Font::RIGHT);

	textStream.str("");
	textStream.clear();

	//write out current fps
	int fps = (int)((1000.0f / mTimeLastFrame) + .5f);//+.5f does rounding
	textStream << "FPS:" << fps;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 2, 0), *pFont, BLACK_COLOR, textStream.str(), Font::CENTER);

	textStream.str("");
	textStream.clear();

	//write out time multiple
	textStream << "time mult:" << mTimeMult;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 4, 0), *pFont, BLACK_COLOR, textStream.str(), Font::CENTER);
}

void Game::displayElapsedTime() const
{
	std::stringstream textStream;

	//get the font
	Font* pFont = mpFontManager->getFont(COUR_24_FONT_ID);

	//write out time left
	textStream << "time left:" << (int)((mGameTimeAllotment - mTotalGameTimeElapsed) + 0.5) << "\nScore:" << mScore;
	GraphicsSystem::writeText(Vector2D((GraphicsSystem::getDisplayWidth() / 4) * 3, 0), *pFont, BLACK_COLOR, textStream.str(), Font::RIGHT);

	if (getGameMode() == EDIT_GAME_MODE)
	{
		Font* pFont = mpFontManager->getFont(COUR_48_FONT_ID);
		GraphicsSystem::writeText(Vector2D((int)GraphicsSystem::getDisplayWidth() / 2, (int)GraphicsSystem::getDisplayHeight() / 2), *pFont, RED_COLOR, "EDIT_MODE", Font::CENTER);
	}

	textStream.str("");
	textStream.clear();


#ifdef VISUALIZE_PATH
	if (mpPathfinder->getPath().getNumNodes() > 0)
	{
		textStream << "Pathlength:" << mpPathfinder->getPath().getNumNodes();
	}

	textStream << "  Num Nodes Processed:" << mpPathfinder->getNumVisitedNodes();
#endif
	textStream << "  Elapsed Time:" << mpPathfinder->getElapsedTime();

	GraphicsSystem::writeText(ZERO_VECTOR2D, *pFont, BLACK_COLOR, textStream.str(), Font::LEFT);

}

void Game::displayStartGame() const
{
	std::stringstream textStream;

	//get the font
	Font* pFont = mpFontManager->getFont(COUR_48_FONT_ID);

	//write out total-time
	textStream << "Press Space to Start";
	Vector2D loc((int)(GraphicsSystem::getDisplayWidth() / 2), (int)(GraphicsSystem::getDisplayHeight() / 2));
	GraphicsSystem::writeText(loc, *pFont, BLACK_COLOR, textStream.str(), Font::CENTER);

}

void Game::displayPauseGame() const
{
	std::stringstream textStream;

	//get the font
	Font* pFont = mpFontManager->getFont(COUR_48_FONT_ID);

	//write out total-time
	textStream << "Game Paused\n" << "Press Space to Unpause\n" << "Press R to Restart";
	Vector2D loc((int)(GraphicsSystem::getDisplayWidth() / 2), (int)(GraphicsSystem::getDisplayHeight() / 2));
	GraphicsSystem::writeText(loc, *pFont, RED_COLOR, textStream.str(), Font::CENTER);

}

void Game::displayEndGame() const
{
	std::stringstream textStream;

	//get the font
	Font* pFont = mpFontManager->getFont(COUR_48_FONT_ID);

	//write out total-time
	textStream << "Game Over!\n" << "Press ESCAPE to Exit\n" << "Press R to Restart";
	Vector2D loc((int)(GraphicsSystem::getDisplayWidth() / 2), (int)(GraphicsSystem::getDisplayHeight() / 2));
	GraphicsSystem::writeText(loc, *pFont, RED_COLOR, textStream.str(), Font::CENTER);

}

void Game::saveGrid(std::ofstream& theStream)
{
	mpGrid->save(theStream);
}

void Game::loadGrid(std::ifstream& theStream)
{
	mpGrid->load(theStream);
}

void Game::setNewGameMode(GameMode newMode)
{
	switch (newMode)
	{
	case START_GAME_MODE:
		restartGame();
		mCurrentMode = START_GAME_MODE;
		break;
	case PLAY_GAME_MODE:
		mCurrentMode = PLAY_GAME_MODE;
		break;
	case PAUSE_GAME_MODE:
		mCurrentMode = PAUSE_GAME_MODE;
		break;
	case END_GAME_MODE:
		mCurrentMode = END_GAME_MODE;
		break;
	case EDIT_GAME_MODE:
		mCurrentMode = EDIT_GAME_MODE;
		break;

	}
}

void Game::restartGame()
{
	mCurrentMode = START_GAME_MODE;

	mpComponentManager->clear();
	mpUnitManager->clear();
	mpLocationManager->clear();
	mpMasterTimer->stop();

	//setup player unit
	Unit* pUnit = NULL;

	Sprite* pSprite = mpSpriteManager->getSprite(PLAYER_ICON_SPRITE_ID);
	if (pSprite)
	{
		Vector2D center((int)getGraphicsSystem()->getDisplayWidth() / 2, (int)getGraphicsSystem()->getDisplayHeight() / 2);
		pUnit = mpUnitManager->createPlayerUnit(*pSprite, false, PositionData(center, 0.0f));
		assert(pUnit);
		pUnit->setShowTarget(true);
	}

	mpMasterTimer->start();
	mTotalGameTimeElapsed = 0.0;
	mScore = 0;
	mGameTimeAllotment = mpRepository->getEntry(DataKeyEnum::GAME_TIME).getDoubleVal();
}

void Game::handleEvent(const Event& theEvent)
{

	switch (theEvent.getType())
	{
	case EXIT_GAME:
		mShouldExit = true;
		break;
	case CHANGE_GAME_MODE:
	{
		const ChangeGameModeEvent& gameEvent = static_cast<const ChangeGameModeEvent&>(theEvent);

		GameMode newMode = gameEvent.getNewMode();

		setNewGameMode(newMode);
		break;
	}
	case MOVE_TO:
	{
		const MoveToEvent& gameEvent = static_cast<const MoveToEvent&>(theEvent);
		const UnitID& id = gameEvent.getUnitID();
		const Vector2D& dest = gameEvent.getDestination();
		Unit* pUnit = getUnitManager()->getUnit(id);
		if (pUnit)
		{
			pUnit->setSteering(Steering::MOVE_TO, dest);
		}
		break;
	}
	case CREATE_ENEMY:
	{
		const CreateEnemyEvent& gameEvent = static_cast<const CreateEnemyEvent&>(theEvent);
		const Vector2D& pos = gameEvent.getPos();
		Unit* pUnit = nullptr;
		Sprite* pAISprite = mpSpriteManager->getSprite(AI_ICON_SPRITE_ID);
		if (pAISprite)
		{
			PhysicsData data;

			//move this to DataRepository!
			data.maxSpeed -= 50;//make sure enemies are slightly slower than the player

			pUnit = mpUnitManager->createUnit(*pAISprite, AIBrain::DEFAULT, true, PositionData(pos, 0.0f),data);
			//pUnit->setShowTarget(true);
			pUnit->setSteering(Steering::MOVE_TO, ZERO_VECTOR2D, PLAYER_UNIT_ID);
		}
		break;
	}
	case SCORE_POINTS:
	{
		const ScorePointsEvent& gameEvent = static_cast<const ScorePointsEvent&>(theEvent);

		int num = gameEvent.getNumPoints();

		mScore += num;
		break;
	}
	case EDIT_MOD_TERRAIN:
	{
		const ModTerrainEvent& gameEvent = static_cast<const ModTerrainEvent&>(theEvent);
		const Vector2D& loc = gameEvent.getPixelLocation();
		mpGrid->setValueAtPixelXY((int)loc.getX(), (int)loc.getY(), gameEvent.getTerrain());
		mpGridVisualizer->setModified();
		break;
	}
	case LOAD_SAVE_MAP:
	{
		const LoadSaveMapEvent& gameEvent = static_cast<const LoadSaveMapEvent&>(theEvent);
		if (gameEvent.shouldSave())
		{
			std::ofstream theStream(GRID_FILENAME);
			if (theStream.good())//file found
			{
				saveGrid(theStream);
			}
			theStream.close();
		}
		else
		{
			std::ifstream theStream(GRID_FILENAME);
			if (theStream.good())//file found
			{
				loadGrid(theStream);
			}
			theStream.close();
			mpGridVisualizer->setModified();
		}
		break;
	}
	case PATH_TO:
	{
		static Vector2D lastLoc;
		const PathToEvent& gameEvent = static_cast<const PathToEvent&>(theEvent);
		const Vector2D& loc = gameEvent.getPixelLocation();
		int fromIndex = mpGrid->getCellIndexFromPixelXY((int)lastLoc.getX(), (int)lastLoc.getY());
		int toIndex = mpGrid->getCellIndexFromPixelXY((int)loc.getX(), (int)loc.getY());
		Node* pFromNode = mpGridGraph->getNode(fromIndex);
		Node* pToNode = mpGridGraph->getNode(toIndex);
		mpPathfinder->findPath(pFromNode, pToNode);
		lastLoc = loc;
		break;
	}
	case CHANGE_PATHFINDING:
	{
		const ChangePathfindingEvent& gameEvent = static_cast<const ChangePathfindingEvent&>(theEvent);

		switch (gameEvent.getType())
		{
		case BF_PATHFINDING:
		{
			if (!dynamic_cast<BFPathfinding*>(mpPathfinder))
			{
				delete mpPathfinder;
				mpPathfinder = new BFPathfinding(mpGridGraph);
			}
			break;
		}
		case DF_PATHFINDING:
		{
			if (!dynamic_cast<DepthFirstPathfinder*>(mpPathfinder))
			{
				delete mpPathfinder;
				mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
			}
			break;
		}
		case DIJKSTRAS_PATHFINDING:
		{
			if (!dynamic_cast<DijkstrasPathfinding*>(mpPathfinder))
			{
				delete mpPathfinder;
				mpPathfinder = new DijkstrasPathfinding(mpGridGraph);
			}
			break;
		}
		case ASTAR_PATHFINDING:
		{
			if (!dynamic_cast<AStarPathfinding*>(mpPathfinder))
			{
				delete mpPathfinder;
				mpPathfinder = new AStarPathfinding(mpGridGraph);
			}
			break;
		}
		};
		break;
	}
	};
}

GraphicsSystem* Game::getGraphicsSystem() const
{
	return mpSystem->getGraphicsSystem();
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

