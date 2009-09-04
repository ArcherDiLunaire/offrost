#ifndef CUSTOMGLVIEWDELEGATE_H
#define CUSTOMGLVIEWDELEGATE_H

/*

	You can either use this class as a bridge between the cocoa window and your
	custom c++ class or you can do whatever you want right here.

*/

#include <string>


class CustomGLViewDelegate
	{
	
	private:
	
		int			m_Width;		// dimentions of window
		int			m_Height;
		int			m_PositionX;	// position of window
		int			m_PositionY;
		bool		m_FullScreen;	// window mode
		std::string	m_Title;

		
		int			m_MouseX;		// save mouse position since mouse-up
		int			m_MouseY;		// doesn't supply any
		
		// slightly modified of function to intialize projection
		// with supplied width/height
		void setupOpenGLForOF( int w, int h );

		
	public:
		
		bool		m_NewSize;		// property flags
		bool		m_NewPosition;
		bool		m_NewMode;
		bool		m_NewTitle;
		
	//----------------------------------------------------
	
		CustomGLViewDelegate( ) : m_Width		( 800 ),	// default window params
								  m_Height		( 600 ),
								  m_PositionX	( 100 ),
								  m_PositionY	( 100 ),
								  m_FullScreen	( false ),
								  m_Title		( "Second OpenGL Window" ),
								  m_NewSize		( true ),
								  m_NewPosition	( true ),
								  m_NewMode		( false ),
								  m_NewTitle	( true ),
								  m_MouseX		( 0 ),
								  m_MouseY		( 0 ) { };

		~CustomGLViewDelegate( ){};
		
	//----------------------------------------------------
	// getters / setters
		
		int getWidth( );
		int getHeight( );
		int getPositionX( );
		int getPositionY( );
		bool isFullScreen( );
		std::string getTitle( );
		
		void setSize( int w, int h );
		void setPosition( int x, int y );
		void setFullScreen( bool f );
		void setTitle( std::string title );

	//----------------------------------------------------
	// event handlers
		
		void handleMouseMove	( int x, int y );
		void handleMouseDown	( int x, int y, int b );
		void handleMouseUp		( int b );
		void handleMouseDragged ( int x, int y, int b );
		
		void handleKeyDown		( int key );
		void handleKeyUp		( int key );
		
		void registerWithMainApplication( );
		void setup( );
		void update( );
		void render( int w, int h );
	};


#endif