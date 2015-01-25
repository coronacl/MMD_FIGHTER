#include "stdafx.h"

static cPMDModel	g_clPMDModel;
static cPMDModel	g_clPMDModel2;
static cPMDModel	g_clPMDModel3;
static cVMDMotion	g_clVMDMotion;

static bool			g_bDispFPS;
static PlayerManage *rea;
static PlayerManage *field;
static PlayerManage *usa;

static void init( void );
static void display( void );
static void resize( int w, int h );
static void visibility( int visible );
static void idle( void );
static float getElapsedFrame( void );
static void keyboard( unsigned char key, int x, int y );
static void specialKeyboard( int key, int x, int y );
static void menu( int value );
static void openNewModel( void );
static void openNewModelAuto(int select_model );
static void openNewMotion( void );
static void openNewMotionAuto( int select_motion);
static void calcFps( void );
static void drawString( const char *szStr, int iPosX, int iPosY );
bool LoadGLTextures();

//テクスチャID
GLuint texture[2];
IplImage* image[2];

bool flag_move , flag_sphere = false;

//==========
// main関数
//==========
int main( int argc, char *argv[] )
{
	// GLUTの初期化
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );
	glutInitWindowSize( 1280, 960 );
	glutCreateWindow( "ARTK MMD" );


	glutDisplayFunc( display );
	glutReshapeFunc( resize );
	glutVisibilityFunc( visibility );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( specialKeyboard );

    glutCreateMenu( menu );
		glutAddMenuEntry( "Open Model(PMD)", 1 );
		glutAddMenuEntry( "Open Motion(VMD)", 2 );
		glutAddMenuEntry( "Stop Motion", 3 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	init();

	glutMainLoop();

	return 0;
}

//--------
// 初期化
//--------
void init( void )
{
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	glClearStencil( 0 );
	glEnable( GL_DEPTH_TEST );

	glEnable( GL_NORMALIZE );

	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GEQUAL, 0.05f );

	const float	fLightPos[] = { 15.0f, 15.0f , 10.0f, 0.0f };
	const float	fLightDif[] = { 0.9f, 0.9f, 0.9f, 1.0f },
				fLightAmb[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv( GL_LIGHT0, GL_POSITION, fLightPos );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, fLightDif );
	glLightfv( GL_LIGHT0, GL_AMBIENT, fLightAmb );
	glLightfv( GL_LIGHT0, GL_SPECULAR, fLightAmb );
	glEnable( GL_LIGHT0 );

	glEnable( GL_LIGHTING );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(60.0, (double)640 / (double)480, 1.0, 500.0);


	g_bDispFPS = true;

	//テクスチャの読み込み
	LoadGLTextures();

	//モデルの読み込み
	//openNewModelAuto(2);//フィールド
	//openNewModelAuto(1);
	//openNewModelAuto(3);
	rea =  new PlayerManage(-15,0,0,3);
	rea->setWay(Way::plusX);

	field = new PlayerManage(0,0,0,1);
	field->setWay(Way::minusX);

	usa = new PlayerManage(15,0,0,2);
	usa->setWay(Way::minusX);


	//モーションを読み込める状態にする
	//int flag_move = true;

}

void dispArrow()
{
	glDisable( GL_LIGHTING );

	glLineWidth(5.0);

	glColor3d(1,0,0);
	glBegin(GL_LINES);
	glVertex3d(0.0 , 1.0 , 0.0 );
	glVertex3d(15.0 , 1.0 , 0.0 );
	glEnd();

	glColor3d(0,1,0);
	glBegin(GL_LINES);
	glVertex3d(0.0 , 1.0 , 0.0 );
	glVertex3d(0.0 , 15.0 , 0.0 );
	glEnd();

	glColor3d(0,0,1);
	glBegin(GL_LINES);
	glVertex3d(0.0 , 1.0 , 0.0 );
	glVertex3d(0.0 , 1.0 , 15.0 );
	glEnd();

	glEnable( GL_LIGHTING );

}

void dispCarpet(void)
{
const GLfloat materialColBlue[] = {1.0 , 0.8 , 0.5 , 1};
const GLfloat materialColBlack[] = { 0.1 , 0.1 , 0.1 , 1};

	int i,j;
	for(i=-50;i<=50;i+=10){
		for(j=-50;j<=50;j+=10){
			if((i+j)/10%2){
				//glColor3d(0.1, 0.1, 0.1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColBlack);
				glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,  materialColBlack );
			}
			else{
				//glColor3d(0.7, 0.70, 0.70);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColBlue);
				glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,  materialColBlue );
			}
			glBegin(GL_POLYGON);
				glNormal3d(0.0,1.0,0.0);
				glVertex3d(i , 0.0 , j);
				glVertex3d(i+10 , 0.0 , j);
				glVertex3d(i+10 , 0.0 ,j+10);
				glVertex3d(i , 0.0 , j+10);
			glEnd();
		}
	}



}

//----------
// 表示関数
//----------
void display(void)
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(0.0, 20.0,50.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	dispCarpet();

	//フィールドの描画
	//dispArrow();
	field->render();
	//プレイヤー1の描画
	usa->render();
	//プレイヤー2の描画
	rea->render();
	//ビームの発射
	//if (flag_sphere == true){
	//	const GLfloat materialColBlue[] = { 1.0, 0.8, 0.5, 1 };
	//	static bool first_flag = true;
	//	static int pos_x, pos_z;
	//	static Way way_tmp;
	//	if (first_flag == true){
	//		pos_x = object_x;
	//		pos_z = object_z;
	//		way_tmp = rea->getWay();
	//		first_flag = false;
	//		openNewMotionAuto(5);

	//	}

	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColBlue);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialColBlue);
	//	glColor3d(1, 1, 0);
	//	GLUquadricObj* sphere = gluNewQuadric();
	//	static int distance = 0;
	//	distance++;
	//	glPushMatrix();	int height = 0.0;
	//		switch (way_tmp)
	//		{
	//		case Way::plusZ:
	//			glTranslated(object_x,height,object_z);
	//			break;
	//		case Way::minusZ:
	//			glTranslated(object_x,height,object_z);
	//			glRotated(180, 0.0, 1.0, 0.0);
	//			break;
	//		case Way::plusX:
	//			glTranslated(object_x,height,object_z);
	//			glRotated(90, 0.0, 1.0, 0.0);
	//			break;
	//		case Way::minusX:
	//			glTranslated(object_x,height,object_z);
	//			glRotated(-90, 0.0, 1.0, 0.0);
	//			break;
	//		default:
	//			break;
	//		}
	//		glRotated(-90, 0.0, 1.0, 0.0);
	//		glTranslated(distance, 15, 0);
	//		gluSphere(sphere, 5.0, 30, 30);
	//	glPopMatrix();

	//	if (distance > 50){
	//		distance = 0;
	//		flag_sphere = false;
	//		first_flag = true;
	//	}
	//}
	//----------------------------------------------------------
	//2D描画処理

	calcFps();

	glutSwapBuffers();
}

//--------------------------------
// ウインドウがリサイズされたとき
//--------------------------------
void resize( int w, int h )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport( 0, 0, w, h );
	
	//初期化処理
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(60.0, (double)w / (double)h, 1.0, 1300.0);

	//初期化処理
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(0.0, 20.0, 80.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);


}

//----------------------------------------
// ウインドウのビジビリティが変化したとき
//----------------------------------------
void visibility( int visible )
{
	if( visible == GLUT_VISIBLE )	glutIdleFunc( idle );
	else							glutIdleFunc( NULL );
}

//------------
// アイドル時
//------------
void idle( void )
{
static bool flag = true;
	if (usa->getHp() <= 0 && flag){
		usa->loadMotion(7);
		flag = false;
	}
	if (rea->getHp() <= 0 && flag){
		rea->loadMotion(7);
		flag = false;
	}



		float	fElapsedFrame = getElapsedFrame();

		if( fElapsedFrame > 10.0f ){
			fElapsedFrame = 10.0f;
			}

		usa->upDateMotion(fElapsedFrame);
		field->upDateMotion(fElapsedFrame);
		rea->upDateMotion(fElapsedFrame);

		glutPostRedisplay();




}

//-----------------------------------------------
// 経過フレーム数を返す(1.0 = 1/30s = 33.333ms)
//-----------------------------------------------
float getElapsedFrame( void )
{
	static int counter = 0;
	counter ++;
	if (counter > 10){
		usa->judgeRun();
		rea->judgeRun();
		counter = 0;
	}


	static int	s_iPrevTime = 0;
	int			iTime = glutGet( GLUT_ELAPSED_TIME );
	float		fDiffTime;

	if( s_iPrevTime == 0 )	s_iPrevTime = iTime;
	fDiffTime = (float)(iTime - s_iPrevTime) * (30.0f / 1000.0f);

	s_iPrevTime = iTime;

	return fDiffTime;
}

//----------------------------
// キーボード入力があったとき
//----------------------------
void keyboard( unsigned char key, int x, int y )
{
	switch( key )
	{
		case 'o':	// モデル読込み
			openNewModel();
			break;

		case 'm':	// モーション読込み
		case 'M':
			openNewMotion();
			break;

		case 'l':	// カメラ目線モードOn/Off
		case 'L':
			g_clPMDModel.toggleLookAtFlag();
			break;

		case 'f':	// フレームレート表示On/Off
		case 'F':
			g_bDispFPS = !g_bDispFPS;
			break;
	}

	Pos posUsa,posRea;
	Way tmp_way;
	//プレイヤー1
	switch( key )
	{
		case 'a':	// パンチ
			usa->loadMotion(2);
			posRea = rea->getPos();
			posUsa = usa->getPos();
			if (abs(posRea.x - posUsa.x) < 10 && abs(posRea.z - posUsa.z) < 10){
				rea->loadMotion(6);
				rea->changeHp(-0.4);
			}
			
			break;
		case 's':	// キック
			usa->loadMotion(4);
			posRea = rea->getPos();
			posUsa = usa->getPos();
			if (abs(posRea.x - posUsa.x) < 15 && abs(posRea.z - posUsa.z) < 15){
				rea->loadMotion(6);
				rea->changeHp(-0.3);
			}
			break;
		case 'd':
			usa->loadMotion(7);
			break;
	}

	//プレイヤー2
	switch( key )
	{
		case'+':
			rea->loadMotion(2);
			posRea = rea->getPos();
			posUsa = usa->getPos();
			if (abs(posRea.x - posUsa.x) < 10 && abs(posRea.z - posUsa.z) < 10){
				usa->loadMotion(6);
				usa->changeHp(-0.4);
			}
			break;
		case'-':
			rea->loadMotion(4);
			posRea = rea->getPos();
			posUsa = usa->getPos();
			if (abs(posRea.x - posUsa.x) < 15 && abs(posRea.z - posUsa.z) < 15){
				usa->loadMotion(6);
				usa->changeHp(-0.3);
			}
			break;
		case '4':	//左移動
			//object_x2 --;
			rea->setPos(-1,0,0);
			rea->setWay(Way::minusX);
			break;
		case '6':	//右移動
			//object_x2 ++;
			rea->setPos(1,0,0);
			rea->setWay(Way::plusX);
			break;
		case '2':	//下移動
			//object_z2 ++;
			rea->setPos(0,0,1);
			rea->setWay(Way::plusZ);
			break;
		case '8':	//上移動
			//object_z2 --;
			rea->setPos(0,0,-1);
			rea->setWay(Way::minusZ);
			break;
	}
}
void specialKeyboard(int key, int x, int y)
{
	//static int before_x1=0,before_x2=0,before_x3=0;
	//static int before_y1=0,before_y2=0,before_y3=0;


	//if ((before_x1 != object_x || before_y1 != object_z) && flag_move == true){
	//	openNewMotionAuto(1);
	//	flag_move = false;
	//}

	//static int counter = 0;
	//counter ++;
	//if (counter > 30){
	//	before_x1 = object_x;
	//	before_y1 = object_z;
	//	counter = 0;
	//}

	switch( key )
	{
		case GLUT_KEY_LEFT:
			usa->setPos(-1,0,0);
			usa->setWay(Way::minusX);
			break;
		case GLUT_KEY_RIGHT:
			usa->setPos(1,0,0);
			usa->setWay(Way::plusX);
			break;
		case GLUT_KEY_DOWN:
			usa->setPos(0,0,1);
			usa->setWay(Way::plusZ);
			break;
		case GLUT_KEY_UP:
			usa->setPos(0,0,-1);
			usa->setWay(Way::minusZ);
			break;
	}

}

//--------------------------------
// メニューの項目が選択されたとき
//--------------------------------
void menu( int value )
{
	switch( value )
	{
		case 1 :	// モデル読込み
			openNewModel();
			break;

		case 2 :	// モーション読込み
			openNewMotion();
			break;

		case 3 :
			g_clVMDMotion.release();
			break;
	}
}

//----------------------
// モデルファイルを開く
//----------------------
void openNewModel( void )
{
#ifdef	_WIN32
	OPENFILENAME	ofn;
	wchar_t			wszFileName[256];
	wchar_t			wszFile[64];
	char			szFileName[256] ;

	//初期化処理(adress,size)
	ZeroMemory( &wszFileName, 256 );
	ZeroMemory( &wszFile,      64 );
	ZeroMemory( &ofn, sizeof(OPENFILENAME) );

	//構造体のサイズ
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ダイアログボックスを所有するウィンドウハンドル
	ofn.hwndOwner = NULL;
	//？
	ofn.lpstrFilter = L"PMD File(*.pmd)\0*.pmd\0\0";
	//ファイル名（フルパス）を保存するバッファ
	ofn.lpstrFile = wszFileName;
	//ファイル名と拡張子のバッファへのポインタ
	ofn.lpstrFileTitle = wszFile;
	//lpstrFileのバッファサイズ
	ofn.nMaxFile = MAX_PATH;
	//lpstrFileTitleのバッファサイズ
	ofn.nMaxFileTitle = sizeof(wszFile);
	//ダイアログボックスの初期化用フラグ
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//拡張子の指定
	ofn.lpstrDefExt = L"pmd";
	//タイトル
	ofn.lpstrTitle = L"OpenPMD";


	if( GetOpenFileName( &ofn ) )//ダイアログの表示
	{
		//カレントディレクトリをwszFileNameにset
		SetCurrentDirectory( wszFileName );
		//文字化け対策？
		setlocale( LC_ALL, "Japanese_Japan.932" );
		//ワイド文字列からマルチバイト文字列に変換
	    wcstombs( szFileName, wszFileName, 255 );

		g_clPMDModel.load( szFileName );
	}

#endif
}

//----------------------
// モデルファイルを開く
//----------------------
void openNewModelAuto( int select_model )
{
#ifdef	_WIN32
	char	*szFileName;
	LPCTSTR	root;
	switch (select_model)
	{
	case 1:
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea\\tower.pmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea");
		SetCurrentDirectory(root);
		g_clPMDModel2.load(szFileName);
		break;
	case 2:
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\usa\\usa.pmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\usa");
		SetCurrentDirectory(root);
		g_clPMDModel.load(szFileName);
		break;
	case 3:
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea\\rea.pmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea");
		SetCurrentDirectory(root);
		g_clPMDModel3.load(szFileName);
		break;
		
	}

#endif
}

//--------------------------
// モーションファイルを開く
//--------------------------
void openNewMotion( void )
{
#ifdef	_WIN32
	OPENFILENAME	ofn;
	wchar_t			wszFileName[256],
					wszFile[64];
	char			szFileName[256];

	ZeroMemory( &wszFileName, 256 );
	ZeroMemory( &wszFile,      64 );
	ZeroMemory( &ofn, sizeof(OPENFILENAME) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"VMD File(*.vmd)\0*.vmd\0\0";
	ofn.lpstrFile = wszFileName;
	ofn.lpstrFileTitle = wszFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = sizeof(wszFile);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"vmd";
	ofn.lpstrTitle = L"Open";

	if( GetOpenFileName( &ofn ) )
	{
		SetCurrentDirectory( wszFileName );

		setlocale( LC_ALL, "Japanese_Japan.932" );
	    wcstombs( szFileName, wszFileName, 255 );

		g_clVMDMotion.load( szFileName );

		g_clPMDModel.setMotion( &g_clVMDMotion, false );
	}
#endif
}

//----------------------
// モーションファイルを開く
//----------------------
void openNewMotionAuto( int select_motion )
{
#ifdef	_WIN32
	char	*szFileName;
	LPCTSTR	root;
	switch (select_motion)
	{
	case 1:
		//run
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\run.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	case 2:
		//panchi
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\panchi.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	case 3:
		//stand
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\stand.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	case 4:
		//kick
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\kick.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	case 5:
		//bi-mu
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\atack.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	}
	
	SetCurrentDirectory(root);

	g_clVMDMotion.load( szFileName );
	g_clPMDModel.setMotion( &g_clVMDMotion, false );
#endif
}

//-------------------
// FPSの計測(精度低)
//-------------------
void calcFps( void )
{
	static int		iT1 = 0,
					iT2 = 0,
					iCnt = 0;
	static float	fFps = 0.0f;

	iT1 = glutGet( GLUT_ELAPSED_TIME );
	if( iT1 - iT2 >= 1000 )
	{
		fFps = (1000.0f * iCnt) / (float)(iT1 - iT2);
		iT2 = iT1;
		iCnt = 0;
	}
	iCnt++; 

	if( g_bDispFPS )
	{
		char	szStr[16];
		sprintf( szStr, "%6.2f", fFps );
		drawString( szStr, 8, 8 );
	}
}

//------------
// 文字列描画
//------------
void drawString( const char *szStr, int iPosX, int iPosY )
{
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( 0.0f, (float)glutGet( GLUT_WINDOW_WIDTH ), 0.0f, (float)glutGet( GLUT_WINDOW_HEIGHT ) );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity();

			//glRasterPos2i( iPosX, glutGet( GLUT_WINDOW_HEIGHT ) - (iPosY + 15) );

			//while( *szStr )
			//{
			//	glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *szStr );
			//	szStr++;
 		//	}
			int w = (int)glutGet( GLUT_WINDOW_WIDTH );
			int h = (int)glutGet( GLUT_WINDOW_HEIGHT );
			
			int st_x1 = (int)w * 0.1 , st_y1 = (int)h * 0.95;
			int hpW = (int)w * 0.35 , hpH = (int)h * 0.05 ;
			int st_x2 = (int)w * 0.55 , st_y2 = (int)h * 0.95;
			
			//テクスチャの処理
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[0]);//(テクスチャの次元,テクスチャの名前)
			glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0);
				glVertex2d(w * 0.04, st_y1);

				glTexCoord2d(1.0, 1.0);
				glVertex2d(w * 0.1, st_y1);

				glTexCoord2d(1.0, 0.0);
				glVertex2d(w * 0.1, st_y1-hpH);

				glTexCoord2d(0.0, 0.0);
				glVertex2d(w * 0.04, st_y1-hpH);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, texture[1]);//(テクスチャの次元,テクスチャの名前)
			glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0);
				glVertex2d(w * 0.9, st_y1);

				glTexCoord2d(1.0, 1.0);
				glVertex2d(w * 0.95, st_y1);

				glTexCoord2d(1.0, 0.0);
				glVertex2d(w * 0.95, st_y1-hpH);

				glTexCoord2d(0.0, 0.0);
				glVertex2d(w * 0.9, st_y1-hpH);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			
			//glTranslated(w*0.1,h*0.9,0.0);
			//glutWireTeapot(15);
			glBegin(GL_QUADS);
			glColor4f( 1.0f, 1.0f, 0.0f, 0.6f );
			glVertex2f( st_x1 , st_y1);
			glVertex2f( st_x1 + hpW* rea->getHp() , st_y1);
			glVertex2f( st_x1 + hpW* rea->getHp() , st_y1 - hpH);
			glVertex2f( st_x1 , st_y1 - hpH);
			glEnd();

			glBegin(GL_LINE_LOOP);
			glColor4f( 1.0f, 0.5f, 0.0f, 1.0f );
			glVertex2f( st_x1 , st_y1);
			glVertex2f( st_x1 + hpW , st_y1);
			glVertex2f( st_x1 + hpW , st_y1 - hpH);
			glVertex2f( st_x1 , st_y1 - hpH);
			glEnd();

			glBegin(GL_QUADS);
			glColor4f( 1.0f, 1.0f, 0.0f, 0.6f );
			glVertex2f( st_x2 + hpW* (1-usa->getHp()), st_y2);
			glVertex2f( st_x2 + hpW, st_y2);
			glVertex2f( st_x2 + hpW, st_y2 - hpH);
			glVertex2f( st_x2 + hpW* (1-usa->getHp()), st_y2 - hpH);
			glEnd();

			glBegin(GL_LINE_LOOP);
			glColor4f( 1.0f, 0.5f, 0.0f, 1.0f );
			glVertex2f( st_x2 , st_y2);
			glVertex2f( st_x2 + hpW , st_y2);
			glVertex2f( st_x2 + hpW , st_y2 - hpH);
			glVertex2f( st_x2 , st_y2 - hpH);
			glEnd();


			glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		

		glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
}

//---------- 画像を読み込んでコンバート ----------------//
bool LoadGLTextures()
{

	image[0] = cvLoadImage("rea.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if (image[0] == NULL)
	{
		printf("ファイル読み込みに失敗しました\n");
		return false;
	}
	image[1] = cvLoadImage("usa.jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if (image[1] == NULL)
	{
		printf("ファイル読み込みに失敗しました\n");
		return false;
	}


	//BGRからRGBへ変更
	cvCvtColor(image[0], image[0], CV_BGR2RGB);
	cvCvtColor(image[1], image[1], CV_BGR2RGB);

	//二次元配列の反転
	//when 0 : x軸周りで反転（上下反転)
	//when + : ｙ軸周りで反転（左右反転）
	//when - ; 両軸周りで反転（上下左右反転）
	cvFlip(image[0], NULL, 0);
	cvFlip(image[1], NULL, 0);

	/******* テクスチャ作成 ****************/
	//テクスチャオブジェクトの作成
	glGenTextures(2, &texture[0]);//(作成するテクスチャの数,その名前)

	for (int i = 0; i < 2; i++){
		//テクスチャの有効化
		glBindTexture(GL_TEXTURE_2D, texture[i]);//(テクスチャの次元,テクスチャの名前)
		//拡大縮小の設定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//テクスチャの作成パターン
		//(~,色の数,画像の幅,画像の高さ,カラー情報,カラー配列の型,カラー配列)
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
						  image[i]->width, image[i]->height,
						  GL_RGB, GL_UNSIGNED_BYTE, image[i]->imageData);

	}
	//cvReleaseImage(&image1);


	return true;
};
