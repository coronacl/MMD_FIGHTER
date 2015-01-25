#include "PlayerManage.h"

PlayerManage::PlayerManage(int _object_x , int _object_y , int _object_z , int select_num)
{
	pos.x = _object_x;
	pos.y = _object_y;
	pos.z = _object_z;
	bf_x = _object_x;
	bf_z = _object_z;
	flag_move = false;
	flag_sphere = false;
	flag_motion = false;
	flag_stand = true;
	hp = 1.0;
	loadPMDModel(select_num);
}

PlayerManage::~PlayerManage()
{
}

void PlayerManage::loadPMDModel(int select_num)
{
	char	*szFileName;
	LPCTSTR	root;
	switch (select_num)
	{
	case 1://フィールド
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea\\tower.pmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea");
		SetCurrentDirectory(root);
		clPMDModel.load(szFileName);
		break;
	case 2://うさうさ
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\usa\\usa.pmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\usa");
		SetCurrentDirectory(root);
		clPMDModel.load(szFileName);
		break;
	case 3://レア
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea\\rea.pmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\model\\rea");
		SetCurrentDirectory(root);
		clPMDModel.load(szFileName);
		break;
	}
		

}
void PlayerManage::loadMotion(int select_num)
{
	char	*szFileName;
	LPCTSTR	root;
	switch (select_num)
	{
	case 1:
		//run
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\walk.vmd";
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
	case 6:
		//damage
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\damage.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	case 7:
		//down
		szFileName = "C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion\\down.vmd";
		root = _T("C:\\Users\\coronacl\\Documents\\Visual Studio 2013\\Projects\\MMD_PROJECT\\MMDビューアー\\ARTK_MMD\\MMD_DATA\\motion");
		break;
	}
	
	SetCurrentDirectory(root);

	clVMDMotion.load( szFileName );
	clPMDModel.setMotion( &clVMDMotion, false );

}

void PlayerManage::render()
{
	glPushMatrix();
			switch (direction)
			{
			case Way::plusZ:
				glTranslated(pos.x,pos.y,pos.z);
				break;
			case Way::minusZ:
				glTranslated(pos.x,pos.y,pos.z);
				glRotated(180, 0.0, 1.0, 0.0);
				break;
			case Way::plusX:
				glTranslated(pos.x,pos.y,pos.z);
				glRotated(90, 0.0, 1.0, 0.0);
				break;
			case Way::minusX:
				glTranslated(pos.x,pos.y,pos.z);
				glRotated(-90, 0.0, 1.0, 0.0);
				break;
			default:
				break;
			}
			
		glScalef(1.0f, 1.0f, -1.0f);	// 左手系 → 右手系
		clPMDModel.render();
	glPopMatrix();

}

void PlayerManage::upDateMotion(float _fElapsedFrame)
{
	clPMDModel.updateMotion( _fElapsedFrame );
	clPMDModel.updateSkinning();
}

void PlayerManage::setPos(int _x , int _y , int _z)
{
	pos.x += _x;
	pos.y += _y;
	pos.z += _z;
}

void PlayerManage::judgeRun()
{
	//static int bf_x=0, bf_z=0;

	if ((bf_x != pos.x || bf_z != pos.z) && flag_move == true){
		loadMotion(1);
		flag_move = false;
		flag_stand = true;
	}
	if (bf_x == pos.x && bf_z == pos.z && flag_stand == true){
		loadMotion(3);
		flag_stand = false;
		flag_move = true;
	}
	bf_x = pos.x;
	bf_z = pos.z;
		
}



