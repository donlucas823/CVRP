
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "gurobi_c++.h"
#include <string>
#include <map>
using namespace std; 

#define VehicleNum 2
#define TotalNum 12
#define VehicleCapacity 101

vector<vector<double>>DistanceMatrix(TotalNum, vector<double>(TotalNum));
GRBVar x[VehicleNum][TotalNum][TotalNum];
GRBVar y[VehicleNum][TotalNum];
GRBVar u[VehicleNum][TotalNum];
string varName = { 0 };
const double CustomCoordinate[][2] = {
{ 0 , 0	},{ 23, 95},{820,915},{470,302},{950,905},{868,744},
{891,987},{ 76,625},{  8,541},{219,988},{662,636},{  0,  0}
};

//��ӡ����
class myPrint {
public:
	myPrint() { cout << endl << "Demand_Num: "; }
	void operator()(int demand) {
		cout << setw(4) << demand << " ";	
	}
	void operator()(int index,bool on) {
		cout << endl << "Custom_Idx: ";
		for (size_t i = 1; i < index; i++)
			cout << setw(4) << i << " ";
		cout << endl;
	}

	~myPrint() { cout << endl; }
}myprint;

//�Ƚ�����
class myCompareInt{
public:
	bool operator() (int v1, int v2)const {
		return v1 > v2;
	}
};

vector<int>Demand(TotalNum);
map<int, int, myCompareInt> mapU[2];

//���������
class SetNumber {
public:
	SetNumber() { srand((unsigned)time(NULL)); }
	int operator()(int begin, int end) { //set Demand
		return (begin + rand() % (end - begin + 1));
	}
	double operator()(const double* city1, const double* city2) { //set Distance
		return sqrt(pow((city1[0] - city2[0]), 2) + pow((city1[1] - city2[1]), 2));
	}
}setNum;

//���ɾ��������������
void setDistanceAndDemand() {
	for (size_t i = 0; i < TotalNum; i++) {
		switch (i) {
			case 0:					Demand[i] = 1; break;
			case (TotalNum - 1):	Demand[i] = 0; break;
			default:				Demand[i] = setNum(10,20);
		}
		//Demand[i] = (i == 0|| i == TotalNum -1)? 1 : setNum(10, 20);
		//cout << Demand[i] << " ";
		for (size_t j = 0; j < TotalNum; j++) {
			if (i == j)DistanceMatrix[i][j] = 0.0;
			else DistanceMatrix[i][j] = setNum(CustomCoordinate[i], CustomCoordinate[j]);
			//cout << setw(7) <<setiosflags(ios::fixed) << setprecision(2) 
			//<< DistanceMatrix[i][j] << " ";
		}//cout << endl;	
	}//cout << endl;
}


//���ɱ�����
string getVarName(string head, string& varName, int k, int i, int j = -1)
{
	varName.erase();
	varName.append(head);
	varName.append("_");
	varName.append(to_string(k));
	varName.append("_");
	varName.append(to_string(i));
	if (j != -1)
	{
		varName.append("_");
		varName.append(to_string(j));
	}
	return varName;
}

int main() {
	setDistanceAndDemand();

	try {
		GRBEnv env = GRBEnv();
		env.start();
		GRBModel model = GRBModel(env);


		//���߱��� 
		for (size_t k = 0; k < VehicleNum; k++) 
			for (size_t i = 0; i < TotalNum; i++) 
				for (size_t j = 0; j < TotalNum; j++) 
					if (i != j) {
						getVarName("x", varName, k, i, j);
						x[k][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
					}

		for (size_t k = 0; k < VehicleNum; k++) 
			for (size_t i = 0; i < TotalNum; i++) {
				getVarName("u", varName, k, i);
				u[k][i] = model.addVar(0.0, VehicleCapacity, 0.0, GRB_INTEGER, varName);
			}

		//Ŀ�꺯�� ·�����
		GRBLinExpr obj = 0;
		for (size_t k = 0; k < VehicleNum; k++) 
			for (size_t i = 0; i < TotalNum; i++) 
				for (size_t j = 0; j < TotalNum; j++) 
					if (i != j)
						obj += x[k][i][j] * DistanceMatrix[i][j];

		model.setObjective(obj, GRB_MINIMIZE);

		//Լ��1 ÿλ�˿Ͷ�������һ��
		for (size_t j = 1; j < TotalNum - 1; j++) {
			GRBLinExpr constr1 = 0.0;
			for (size_t k = 0; k < VehicleNum; k++) 
				for (size_t i = 0; i < TotalNum; i++)
					if (i != j)
						constr1 += x[k][i][j];
			model.addConstr(constr1, GRB_EQUAL, 1);
			constr1 = 0.0;
		}

		//Լ��2 �˿���ƽ��
		for (size_t k = 0; k < VehicleNum; k++) 
			for (size_t h = 1; h < TotalNum - 1; h++) {
				GRBLinExpr constr2_1 = 0.0;
				GRBLinExpr constr2_2 = 0.0;
				for (size_t i = 0; i < TotalNum; i++) 
					if (i != h)
						constr2_1 += x[k][i][h];
				for (size_t j = 0; j < TotalNum; j++) 
					if (j != h)
						constr2_2 += x[k][h][j];
				model.addConstr(constr2_1, GRB_EQUAL, constr2_2);
			}
		

		//Լ��3 �����뿪�������ģ����Ϳ�ʼ
		for (size_t k = 0; k < VehicleNum; k++) {
			GRBLinExpr constr3 = 0.0;
			for (size_t j = 1; j < TotalNum - 1; j++) 
				constr3 += x[k][0][j];
			model.addConstr(constr3, GRB_EQUAL, 1);
		}

		//Լ��4 ���������������ģ����ͽ���
		for (size_t k = 0; k < VehicleNum; k++) {
			GRBLinExpr constr4 = 0.0;
			for (size_t i = 1; i < TotalNum - 1; i++) 
				constr4 += x[k][i][TotalNum - 1];
			model.addConstr(constr4, GRB_EQUAL, 1);
		}

		//Լ��5 ����Լ��
		for (size_t k = 0; k < VehicleNum; k++) {
			GRBLinExpr constr5 = 0.0;
			for (size_t i = 0; i < TotalNum - 1; i++)
				for (size_t j = 1; j < TotalNum; j++)
					if (i != j)
						constr5 += x[k][i][j] * Demand[i];
			model.addConstr(constr5, GRB_LESS_EQUAL, VehicleCapacity);
		}

		//Լ��6 MTZԼ�������ӻ�·
		for (size_t i = 0; i < TotalNum; i++) 
			for (size_t j = 0; j < TotalNum; j++) 
				if (i != j) 
					for (size_t k = 0; k < VehicleNum; k++) {
						GRBLinExpr constr6_1 = 0;
						GRBLinExpr constr6_2 = 0;
						constr6_1 = u[k][j] - u[k][i] + x[k][i][j] * (VehicleCapacity);
						constr6_2 = (VehicleCapacity) - Demand[i];
						model.addConstr(constr6_1, GRB_LESS_EQUAL, constr6_2);
					}

		//�Ż�
		model.optimize();

		//����Ż����
		for (size_t k = 0; k < VehicleNum; k++) {
			for (size_t i = 0; i < TotalNum; i++) {
				for (size_t j = 0; j < TotalNum; j++) {
					if (i != j && x[k][i][j].get(GRB_DoubleAttr_X) == 1)
						cout << setw(10) << x[k][i][j].get(GRB_StringAttr_VarName) << " = "
						<< x[k][i][j].get(GRB_DoubleAttr_X) << "; ";
				}
			}cout << endl;
		}

		//for (size_t k = 0; k < VehicleNum; k++) {
		//	for (size_t i = 0; i < TotalNum; i++) {
		//		int useFlag = 0;
		//		for (size_t j = 0; j < TotalNum; j++)
		//			if(i!=j)
		//				useFlag += x[k][i][j].get(GRB_DoubleAttr_X);
		//		if (useFlag)
		//			cout << setw(10) << u[k][i].get(GRB_StringAttr_VarName) << " = "
		//			<< u[k][i].get(GRB_DoubleAttr_X) << "; ";
		//	}cout << endl;
		//}

		//��ӡ������ɵ�����
		myprint(TotalNum - 1, true);
		for_each(Demand.begin() + 1, Demand.end() - 1, myPrint());
		//��
		for (size_t k = 0; k < VehicleNum; k++){
			for (size_t i = 0; i < TotalNum; i++){
				int tmpU = u[k][i].get(GRB_DoubleAttr_X);
				for (size_t j = 0; j < TotalNum; j++)
					if (i != j && x[k][i][j].get(GRB_DoubleAttr_X) == 1)
						mapU[k].insert(make_pair(tmpU, i));
			}
			//cout << mapU[k].size() << endl;
			cout << "��" << k + 1 << "������·��Ϊ��";
			for (map<int, int, myCompareInt>::iterator it = mapU[k].begin(); it != mapU[k].end(); it++)
				cout << it->second << " - ";
			cout << 0 << endl;
			
			cout << "��" << k + 1 << "��������ʱ����Ϊ��";
			int tmpD = 0;
			for (map<int, int, myCompareInt>::iterator it = mapU[k].begin(); it != mapU[k].end(); it++)
				tmpD += Demand[it->second];
			cout<< tmpD - 1 << endl;

			double totalDist = 0;
			for (size_t i = 0; i < TotalNum; i++)
				for (size_t j = 0; j < TotalNum; j++) 
					if (i != j)
						totalDist += x[k][i][j].get(GRB_DoubleAttr_X) * DistanceMatrix[i][j];
				
			
			cout << "��" << k + 1 << "������·��Ϊ��" << totalDist << endl;
			cout << endl;
		}
		cout << "��·��Ϊ" << model.get(GRB_DoubleAttr_ObjVal) << endl;
	}
	catch (GRBException e) {
		cout << "Error Code :" << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...){
		cout << "Exception during optimization!" << endl;
	}

}
