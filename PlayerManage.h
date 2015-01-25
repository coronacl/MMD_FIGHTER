#ifndef  GAME_MANAGE
#define  GAME_MANAGE

#include	<windows.h>
#include	"PMDModel.h"
#include	"VMDMotion.h"
#include	<tchar.h>
#include	<gl/glut.h>

enum Way
{
	plusX ,
	minusX,
	plusZ,
	minusZ,
};

struct Pos
{
	int x;
	int y;
	int z;
};

class PlayerManage
{
private:
	PlayerManage();
public:
	PlayerManage(int _object_x , int _object_y , int _object_z , int select_num);
	~PlayerManage();
	void loadPMDModel(int select_num);
	void loadMotion(int select_num);
	void render();
	void upDateMotion(float _fElapsedFrame);
	void setWay(Way _direction){direction = _direction;};
	Way getWay(){return(direction);};
	void setPos(int _x , int _y , int _z);
	Pos getPos(){ return(pos); };
	void judgeRun();
	void changeHp(double damage){hp += damage;};
	double getHp(){ return(hp); };


private:
	cPMDModel	clPMDModel;
	cVMDMotion	clVMDMotion;
	Pos pos;
	int bf_x,bf_z;
	bool flag_move , flag_sphere , flag_motion ,flag_stand;
	Way direction;
	double hp;


};



#endif
