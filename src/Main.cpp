#include <iostream>
#include <TestClass.h>
//#include <SFML/Window.hpp>
#include <enet/enet.h>

using namespace std;

int main() {
    cout << "Hello world!" << endl;
    
    cout << enet_time_get() << endl;
    
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		cout << "A pressed" << endl;
	else
		cout << "A not pressed, obviously" << endl;
    */
    //cout << "Enet time: " << enet_time_get() << endl;
    
    /*TestClass* test = new TestClass();
    test->setName("Kitten");
    test->printName();
    
    Ogre::Root* mRoot;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    
    mResourcesCfg = Ogre::BLANKSTRING;
    mPluginsCfg = Ogre::BLANKSTRING;
    
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
    
    mRoot = new Ogre::Root(mPluginsCfg);
    
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
    
    Ogre::String name, locType;
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
    
    while (secIt.hasMoreElements()) {
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator it;
        for (it = settings->begin(); it != settings->end(); ++it) {
            locType = it->first;
            name = it->second;
        
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
        
        }     
    }
    

    Ogre::RenderWindow* mWindow;
    if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
        return false;
    
    mWindow = mRoot->initialise(true, "Tower Defense Render Window");
    
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    Ogre::SceneManager* mSceneMgr;
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    
    Ogre::Camera* mCamera;
    mCamera = mSceneMgr->createCamera("MainCam");
    mCamera->setPosition(0, 0, 80);
    mCamera->lookAt(0, 0, -300);
    mCamera->setNearClipDistance(5);
    
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    
    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
    
    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(ogreEntity);
    
    mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));
    
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);
    
    Ogre::Real sinVal = 0.0;
    
    while (true && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        Ogre::WindowEventUtilities::messagePump();
        
        if (mWindow->isClosed()) return false;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) 
            sinVal += 0.01;
        
        ogreNode->setOrientation(1, 0, 0, sin(sinVal)/4);
        
        if (!mRoot->renderOneFrame()) return false;
    }
    
    delete mRoot;*/
    return 0;
}

























